#include "mrf.h"

#include <iostream>
#include <iomanip>

using namespace std;


/********************************************************************
	�غc
/*********************************************************************/
MarkovRandomField::MarkovRandomField()
{
	P_lambda	=	NULL;
	K_lambda	=	NULL;
}



/********************************************************************
	�غc
/*********************************************************************/
MarkovRandomField::MarkovRandomField( mrfEnergyAbstract &e )
{
	P_lambda	=	NULL;
	K_lambda	=	NULL;

	// ��l�Ư�q
	switch( e.type() )
	{
		//case MRF_ENERGY_TEST:
		//	vpEnergy.push_back( new EnergyTest((EnergyTest&)e) );
		//	break;

		case MRF_ENERGY_STANDARD:
			vpEnergy.push_back( new EnergyStandard((EnergyStandard&)e) );
			break;
		default:
			assert(false);
	}

	const int	width	=	vpEnergy[0]->get_img_width();		// �]���O�غc��� �u�|���@�Ӫ���
	const int	height	=	vpEnergy[0]->get_img_height();

	// ��l�� label_map
	label_map.resize( width, height );
}


/********************************************************************
	�Ѻc
/*********************************************************************/
MarkovRandomField::~MarkovRandomField()
{
	delete [] P_lambda;		P_lambda	=	NULL;
	delete [] K_lambda;		K_lambda	=	NULL;
}


/********************************************************************
	�p�����������V�� 2-clique
/*********************************************************************/
void	MarkovRandomField::cal_2_clique_hv( int i, int j )
{
	const int	width		=	image.width();
	const int	height		=	image.height();
	const int	MAX_LABEL	=	vpEnergy.size();				// �̤jlabel�Ӽ�

	int		label;
	int		pix1,	pix2,	lab2;

	// �W -------------------------------------------------------------------------------------------------------
	if( j-1 >= 0 )
	{
		pix1			=	ij2pix( i, j );
		pix2			=	ij2pix( i, j-1 );
		lab2			=	label_map( i, j-1 );

		for( label = 0; label < MAX_LABEL; label++ )
			D[label](i,j)	+=	vpEnergy[label]->E_smooth( pix1, pix2, label, lab2 );
	}
	// �U -------------------------------------------------------------------------------------------------------
	if( j+1 < height )
	{
		pix1			=	ij2pix( i, j );
		pix2			=	ij2pix( i, j+1 );
		lab2			=	label_map( i, j+1 );

		for( label = 0; label < MAX_LABEL; label++ )
			D[label](i,j)	+=	vpEnergy[label]->E_smooth( pix1, pix2, label, lab2 );
	}
	// �� -------------------------------------------------------------------------------------------------------
	if( i-1 >= 0 )
	{
		pix1			=	ij2pix( i, j );
		pix2			=	ij2pix( i-1, j );
		lab2			=	label_map( i-1, j );

		for( label = 0; label < MAX_LABEL; label++ )
			D[label](i,j)	+=	vpEnergy[label]->E_smooth( pix1, pix2, label, lab2 );
	}
	// �k -------------------------------------------------------------------------------------------------------
	if( i+1 < width )
	{
		pix1			=	ij2pix( i, j );
		pix2			=	ij2pix( i+1, j );
		lab2			=	label_map( i+1, j );

		for( label = 0; label < MAX_LABEL; label++ )
			D[label](i,j)	+=	vpEnergy[label]->E_smooth( pix1, pix2, label, lab2 );
	}
}

/********************************************************************
	�j�M�̤p�� 
	��s label map
/*********************************************************************/
void	MarkovRandomField::update_label_map( int i, int j )
{
	const int	MAX_LABEL	=	vpEnergy.size();		// �̤jlabel�Ӽ�

	int			min_id;
	int			label;
	double		min;

	// �j�M min
	min_id	=	0;
	min		=	D[0](i,j);    //D(i,j).data[0];
	for( label = 1; label < MAX_LABEL; label++ )
	{
		if( min > D[label](i,j) )
		{
			min_id	=	label;
			min		=	D[label](i,j);
		}
	}
	label_map(i,j)	=	min_id;
}



/********************************************************************
	��l�� map D  
	(�T�תŶ���map ��O�Ȧs�ʽ� �O�֤�)
/*********************************************************************/
void	MarkovRandomField::init_D()
{
	const int	width		=	image.width();
	const int	height		=	image.height();
	const int	MAX_LABEL	=	vpEnergy.size();				// �̤jlabel�Ӽ�

	int		i,	j;
	int		label;
	int		pix;

	for( i = 0; i < width; i++ )
	{
		for( j = 0; j < height; j++ )
		{
			pix	=	ij2pix( i, j );

			for( label = 0; label < MAX_LABEL; label++ )
				D[label](i,j)	=	vpEnergy[label]->E_data( pix, label );	// �L�t�� �]����ۤv�Z���ܪ� value�|�ܧC  (�ഫ��Probability�~�ݭn�[�W�t��)
		}
	}
}



/********************************************************************
	���� ICM mode
/*********************************************************************/
void	MarkovRandomField::ICM_exec( QImage img )
{
	image	=	img;

	int		i,	j,	n;
	int		label;

	// ���s�]�w�@���Ϥ�
	for( i = 0; i < vpEnergy.size(); i++ )
		vpEnergy[i]->set_image(image);

	const int	MAX_ITER	=	1000;
	const int	width		=	vpEnergy[0]->get_img_width();
	const int	height		=	vpEnergy[0]->get_img_height();
	const int	MAX_LABEL	=	vpEnergy.size();				// �̤jlabel�Ӽ�

	label_map.resize( width, height );
	label_map.clear();

	bMatrixInt	old_map		=	label_map;		// �ª� label map, �P�_���N����ϥ�

#ifdef _DEBUG		// �ˬd�Ϥ��O�_�ۦP
	for( label = 0; label < MAX_LABEL; label++ )
	{
		assert( vpEnergy[label]->get_image() == image );
		assert( vpEnergy[label]->check_image() );
	}
#endif

	D.resize(MAX_LABEL);							// �`�@���X��label
	for( i = 0; i < MAX_LABEL; i++ )				// ��l��  ���N���ɭ����ӭn����l��
		D[i].resize( width, height );

	for( n = 0; n < MAX_ITER; n++ )
	{	
		cout << "itr = " << setw(3) << n << " energy = " << dataEnergy() + smoothEnergy() << endl;

		// �����;�� 3D map����� �C�����ݭn���s����
		init_D();

		// �w��2-clique(�|�V)�h�Denergy
		for( i = 0; i < width; i++ )
		{
			for( j = 0; j < height; j++ )
			{
				cal_2_clique_hv( i, j );		// �p����� ������ 2-clique
				update_label_map( i, j );		// ��s label map
			}
		}

		// �Q��label map�M�w�O�_�n����N
		if( old_map == label_map )		break;
		old_map		=	label_map;
	
	} // end MAX_ITR

	cout << "end energy = " << dataEnergy() + smoothEnergy() << endl;

	// ��X���G
	QImage	img( width, height, QImage::Format_RGB888 );
	for( i = 0; i < width; i++ )
		for( j = 0; j < height; j++ )
		{
			label	=	label_map(i,j);
			img.setPixel( i, j, (label*761838257287)%193707721 );
		}

	char	filename[100];
	sprintf( filename, "%s/output.bmp", TMP_PATH );
	img.save(QString(filename));
	cout << "output finish.\n";
}



/********************************************************************
	�p��צV��2-clique
/*********************************************************************/
void	MarkovRandomField::cal_2_clique_oblique( int i, int j )
{
	const int		width		=	image.width();
	const int		height		=	image.height();
	const int		MAX_LABEL	=	vpEnergy.size();				// �̤jlabel�Ӽ�
	const double	weight		=	1.0 / sqrt(2.0);				

	int		label;
	int		pix1,	pix2,	lab2;

	// ���W
	if( i-1 >= 0 && j-1 >= 0 )
	{
		pix1			=	ij2pix( i, j );
		pix2			=	ij2pix( i-1, j-1 );
		lab2			=	label_map( i-1, j-1 );

		for( label = 0; label < MAX_LABEL; label++ )
			D[label](i,j)	+=	weight * vpEnergy[label]->E_smooth( pix1, pix2, label, lab2 );
	}
	// ���U
	if( i-1 >= 0 && j+1 < height )
	{
		pix1			=	ij2pix( i, j );
		pix2			=	ij2pix( i-1, j+1 );
		lab2			=	label_map( i-1, j+1 );

		for( label = 0; label < MAX_LABEL; label++ )
			D[label](i,j)	+=	weight * vpEnergy[label]->E_smooth( pix1, pix2, label, lab2 );
	}
	// �k�W
	if( i+1 < width && j-1 >= 0 )
	{
		pix1			=	ij2pix( i, j );
		pix2			=	ij2pix( i+1, j-1 );
		lab2			=	label_map( i+1, j-1 );

		for( label = 0; label < MAX_LABEL; label++ )
			D[label](i,j)	+=	weight * vpEnergy[label]->E_smooth( pix1, pix2, label, lab2 );
	}
	// �k�U
	if( i+1 < width && j+1 < height )
	{
		pix1			=	ij2pix( i, j );
		pix2			=	ij2pix( i+1, j+1 );
		lab2			=	label_map( i+1, j+1 );

		for( label = 0; label < MAX_LABEL; label++ )
			D[label](i,j)	+=	weight * vpEnergy[label]->E_smooth( pix1, pix2, label, lab2 );
	}
}




/********************************************************************
	���� ICM mode
	�Ҽ{ 8-neighbor
/*********************************************************************/
void	MarkovRandomField::ICM8_exec( QImage img )
{
	image	=	img;

	int			i,	j,	n;
	int			label;
	double		min;

	// ���s�]�w�@���Ϥ�
	for( i = 0; i < vpEnergy.size(); i++ )
		vpEnergy[i]->set_image(image);

	const int	MAX_ITER	=	1000;
	const int	width		=	image.width();					// vpEnergy[0]->get_img_width();
	const int	height		=	image.height();					//vpEnergy[0]->get_img_height();
	const int	MAX_LABEL	=	vpEnergy.size();				// �̤jlabel�Ӽ�

	label_map.resize( width, height );
	label_map.clear();

	bMatrixInt	old_map		=	label_map;		// �W�@�Ӫ��A��label map  �P�_���N�O�_����ϥ�

#ifdef _DEBUG		// �ˬd�Ϥ��O�_�ۦP
	for( label = 0; label < MAX_LABEL; label++ )
	{
		assert( vpEnergy[label]->get_image() == image );
		assert( vpEnergy[label]->check_image() );
	}
#endif

	D.resize(MAX_LABEL);							// �`�@���X��label
	for( i = 0; i < MAX_LABEL; i++ )				// ��l��  ���N���ɭ����ӭn����l��
		D[i].resize( width, height );

	for( n = 0; n < MAX_ITER; n++ )
	{	
		cout << "itr = " << setw(3) << n << endl;//" energy = " << dataEnergy() + smoothEnergy() + smoothEnergyOblique() << endl;

		// �����;�� 3D map����� �C�����ݭn���s����
		init_D();

		// �w��2-clique(�|�V)�h�Denergy
		for( i = 0; i < width; i++ )
		{
			for( j = 0; j < height; j++ )
			{
				cal_2_clique_hv( i, j );		// �p����� ������ 2-clique
				cal_2_clique_oblique( i, j );	// �p��צV�� 2-clique
				update_label_map( i, j );		// ��s label map
			}
		}

		// �Q��label map�M�w�O�_�n����N
		if( old_map == label_map )		break;
		old_map		=	label_map;

	} // end MAX_ITR

	cout << "end energy = " << dataEnergy() + smoothEnergy() + smoothEnergyOblique() << endl;

	// ��X���G
	QImage	img( width, height, QImage::Format_RGB888 );
	for( i = 0; i < width; i++ )
		for( j = 0; j < height; j++ )
		{
			label	=	label_map(i,j);
			img.setPixel( i, j, (label*761838257287)%193707721 );
			//img.gray_channel( i, j, label*30 );
		}

	char	filename[100];
	sprintf( filename, "%s/output.bmp", TMP_PATH );
	img.save(QString(filename));
	cout << "output finish.\n";
}




/********************************************************************
	�p���q (E_data)
/*********************************************************************/
double	MarkovRandomField::dataEnergy()
{
	const int	width	=	vpEnergy[0]->get_img_width();
	const int	height	=	vpEnergy[0]->get_img_height();

	int		i,		j;
	int		pix,	lab;
	double	sum		=	0;

	for( i = 0; i < width; i++ )
		for( j = 0; j < height; j++ )
		{
			pix		=	ij2pix( i, j );
			lab		=	label_map( i, j );

			sum		+=	vpEnergy[0]->E_data( pix, lab ); 
		}

	return	sum;
}


/********************************************************************
	�p���q (�צV)
/*********************************************************************/
double	MarkovRandomField::smoothEnergyOblique()
{
	const int		width	=	image.width();		//vpEnergy[0]->get_img_width();
	const int		height	=	image.height();		//vpEnergy[0]->get_img_height();
	const double	weight	=	1.0 / sqrt(2.0);	// ���M���v�T���G ���O�n��cal���䰵�P�B

	int		i,		j;
	int		pix1,	lab1,	pix2,	lab2;
	double	sum		=	0;

	for( i = 0; i < width; i++ )
		for( j = 0; j < height; j++ )
		{
			pix1	=	ij2pix( i, j );
			lab1	=	label_map( i, j );

			// ���W
			if( i-1 >= 0 && j-1 >= 0 )
			{
				pix2	=	ij2pix( i-1, j-1 );
				lab2	=	label_map( i-1, j-1 );
				sum		+=	weight * vpEnergy[0]->E_smooth( pix1, pix2, lab1, lab2 );
			}

			// ���U
			if( i-1 >= 0 && j+1 < height )
			{
				pix2	=	ij2pix( i-1, j+1 );
				lab2	=	label_map( i-1, j+1 );
				sum		+=	weight * vpEnergy[0]->E_smooth( pix1, pix2, lab1, lab2 );
			}

			// �k�W
			if( i+1 < width && j-1 >= 0 )
			{
				pix2	=	ij2pix( i+1, j-1 );
				lab2	=	label_map( i+1, j-1 );
				sum		+=	weight * vpEnergy[0]->E_smooth( pix1, pix2, lab1, lab2 );
			}

			// �k�U
			if( i+1 < width && j+1 < height)
			{
				pix2	=	ij2pix( i+1, j+1 );
				lab2	=	label_map( i+1, j+1 );
				sum		+=	weight * vpEnergy[0]->E_smooth( pix1, pix2, lab1, lab2 );
			}
		}

	return	sum;
}


/********************************************************************
	�p���q (E_smooth)
/*********************************************************************/
double	MarkovRandomField::smoothEnergy()
{
	const int	width	=	image.width();		//vpEnergy[0]->get_img_width();
	const int	height	=	image.height();		//vpEnergy[0]->get_img_height();

	int		i,		j;
	int		pix1,	lab1,	pix2,	lab2;
	double	sum		=	0;

	for( i = 0; i < width; i++ )
		for( j = 0; j < height; j++ )
		{
			pix1	=	ij2pix( i, j );
			lab1	=	label_map( i, j );

			// �W
			if( j-1 >= 0 )
			{
				pix2	=	ij2pix( i, j-1 );
				lab2	=	label_map( i, j-1 );
				sum		+=	vpEnergy[0]->E_smooth( pix1, pix2, lab1, lab2 );
			}

			// �U
			if( j+1 < height )
			{
				pix2	=	ij2pix( i, j+1 );
				lab2	=	label_map( i, j+1 );
				sum		+=	vpEnergy[0]->E_smooth( pix1, pix2, lab1, lab2 );
			}

			// ��
			if( i-1 >= 0 )
			{
				pix2	=	ij2pix( i-1, j );
				lab2	=	label_map( i-1, j );
				sum		+=	vpEnergy[0]->E_smooth( pix1, pix2, lab1, lab2 );
			}

			// �k
			if( i+1 < width )
			{
				pix2	=	ij2pix( i+1, j );
				lab2	=	label_map( i+1, j );
				sum		+=	vpEnergy[0]->E_smooth( pix1, pix2, lab1, lab2 );
			}

		}

	return	sum;
}



/********************************************************************
	�N (i,j) �y���ন pixel
/*********************************************************************/
int		MarkovRandomField::ij2pix( int i, int j )
{
	const int	width	=	image.width();		//vpEnergy[0]->get_img_width();
	const int	height	=	image.height();		//vpEnergy[0]->get_img_height();

	int		pixel;

	pixel	=	i * height + j ;

	return	pixel;
}



/********************************************************************
	�W�[�s��energy
/*********************************************************************/
void	MarkovRandomField::push_back( mrfEnergyAbstract &e )
{
	// ��l�Ư�q
	switch( e.type() )
	{
		//case MRF_ENERGY_TEST:
		//	vpEnergy.push_back( new EnergyTest((EnergyTest&)e) );
		//	break;

		case MRF_ENERGY_STANDARD:
			vpEnergy.push_back( new EnergyStandard((EnergyStandard&)e) );
			break;

		default:
			assert(false);
	}
}






/********************************************************************
	E-M �� Expectation ���q
/*********************************************************************/
void	MarkovRandomField::E_Step()
{
	const int	MAX_LABEL	=	vpEnergy.size();
	const int	width		=	image.width();
	const int	height		=	image.height();

	int		i,	j,	pix,	label;
	double	normal;			// ���� normalized (�ন���v? 0~1 �Ӥ��O�쥻�|�W�L1��pdf)

	// Estimation
	for( i = 0; i < width; i++ )
		for( j = 0; j < height; j++ )
		{
			pix		=	i * height + j;

			// �p�����
			normal	=	0;
			for( label = 0; label < MAX_LABEL; label++ )
				normal	+=	P_lambda[label] * vpEnergy[label]->Prob(pix,label);

			for( label = 0; label < MAX_LABEL; label++ )
				D[label](i,j)	=	P_lambda[label] * vpEnergy[label]->Prob(pix,label) / normal;
		}
}




/********************************************************************
	E-M �p�� K_(lambdas)
/*********************************************************************/
void	MarkovRandomField::cal_K_lambda()
{
	const int	MAX_LABEL	=	vpEnergy.size();
	const int	width		=	image.width();
	const int	height		=	image.height();

	int		label,	i,	j;

	// �p�� K_lambda
	// ��l��
	for( label = 0; label < MAX_LABEL; label++ )
		K_lambda[label]	=	0;

	// �p��sum   (�Q���ٱ��@���ܼ�sum)
	for( i = 0; i < width; i++ )
		for( j = 0; j < height; j++ )
		{
			for( label = 0; label < MAX_LABEL; label++ )	
				K_lambda[label]	+=	D[label](i,j);
		}
}




/********************************************************************
	E-M �p�� P(lambda)
/*********************************************************************/
void	MarkovRandomField::cal_P_lambda()
{
	const int	MAX_LABEL	=	vpEnergy.size();
	const int	width		=	image.width();
	const int	height		=	image.height();
	const int	size		=	width * height;

	int		label;

	// �p��P_lambda
	for( label = 0; label < MAX_LABEL; label++ )
		P_lambda[label]		=	 K_lambda[label] / size;		// size = �פ媺 D	

}








/********************************************************************
	E-M �p�� mean
/*********************************************************************/
void	MarkovRandomField::cal_EM_mean()
{
	const int	MAX_LABEL	=	vpEnergy.size();
	const int	width		=	image.width();
	const int	height		=	image.height();
	const int	dim			=	vpEnergy[0]->dim();

	int		label,	i,	j,	k,	pix;

#ifdef _DEBUG
	for( label = 0; label < MAX_LABEL - 1; label++ )
		assert( vpEnergy[label]->dim() == vpEnergy[label+1]->dim() );
#endif

	//sVectorDouble	vec(dim);
	//sVectorDouble	mean(dim);
	double	*mean	=	new double [dim];
	double	*vec	=	new double [dim];


	// �p��mean
	for( label = 0; label < MAX_LABEL; label++ )
	{
		// �k�s �o�䥲���w�qvector��operator = (int)
		for( k = 0; k < dim; k++ )
			mean[k]	=	0;
		//set_vector( mean, 0.0 );

		for( i = 0; i < width; i++ )
			for( j = 0; j < height; j++ )
			{
				pix		=	ij2pix( i, j );
				//mean	+=	D[label](i,j) * vpEnergy[label]->get_feature(pix);
				vpEnergy[label]->get_feature( pix, vec );
				for( k = 0; k < dim; k++ )
					mean[k]	+=	D[label](i,j) * vec[k];
			}

		for( k = 0; k < dim; k++ )
			mean[k]	/=	K_lambda[label];
		//mean	/=	K_lambda[label];

		vpEnergy[label]->set_mean(mean);	

#ifdef _DEBUG
		vpEnergy[label]->check_dim();		// �]��dimension�����D���[�J��H���� �ҥH�u�n�g�@��function�S�O���ˬd
#endif
	}

	delete	[] mean;	mean	=	NULL;
	delete	[] vec;		vec		=	NULL;
}







/********************************************************************
	E-M �p��зǮt
/*********************************************************************/
void	MarkovRandomField::cal_EM_devia()
{
	const int	MAX_LABEL	=	vpEnergy.size();
	const int	width		=	image.width();
	const int	height		=	image.height();
	const int	dim			=	vpEnergy[0]->dim();

	int		i,	j,	k,	label,	pix;

#ifdef _DEBUG
	for( label = 0; label < MAX_LABEL - 1; label++ )
		assert( vpEnergy[label]->dim() == vpEnergy[label+1]->dim() );
#endif

	//sVectorDouble	devia(dim);
	//sVectorDouble	value(dim);
	//sVectorDouble	v_tmp(dim);
	double	*devia		=	new double [dim];
	double	*feature	=	new double [dim];
	double	*v_tmp		=	new double [dim];
	double	*mean		=	new double [dim];

	// �p��зǮt
	for( label = 0; label < MAX_LABEL; label++ )
	{
		//set_vector( devia, 0.0 );
		for( k = 0; k < dim; k++ )
			devia[k]	=	0;


		for( i = 0; i < width ; i++ )
			for( j = 0 ; j < height; j++ )
			{
				pix		=	ij2pix( i, j );
				vpEnergy[label]->get_feature( pix, feature );
				vpEnergy[label]->get_mean( mean );
		
				for( k = 0; k < dim; k++ )
				{
					v_tmp[k]	=	feature[k] - mean[k];
					devia[k]	+=	D[label](i,j) * pow( v_tmp[k], 2 );
				}

				//v_tmp	=	vpEnergy[label]->get_feature(pix) - vpEnergy[label]->get_mean();
				//devia	+=	D[label](i,j) * ( v_tmp*v_tmp );		// �o�䪺���k�O�I��!!
			}

		//devia	/=	K_lambda[label];
		//devia	=	sqrt(devia);		// �פ�W�Υ���᪺�F�� �ܲ���  �ۤv�令�зǮt �ҥH�n�[ sqrt !!

		for( k = 0; k < dim; k++ )
		{
			devia[k]	/=	K_lambda[label];
			devia[k]	=	sqrt( devia[k] );
		}


		vpEnergy[label]->set_devia(devia);

#ifdef _DEBUG
		vpEnergy[label]->check_dim();		// �]��dimension�����D���[�J��H���� �ҥH�u�n�g�@��function�S�O���ˬd
#endif
	}
	
	delete	[] devia;		devia	=	0;
	delete	[] feature;		feature	=	0;
	delete	[] v_tmp;		v_tmp	=	0;
	delete	[] mean;		mean	=	0;
}



/********************************************************************
	E-M �� Maximization ���q
/*********************************************************************/
void	MarkovRandomField::M_Step()
{
	// �p�� K_lambda
	cal_K_lambda();

	// �p�� P(lambda)
	cal_P_lambda();

	// �p�� mean (�令vector�Φ�)
	cal_EM_mean();

	// �p��зǮt
	cal_EM_devia();
}





/********************************************************************
	���� E-M ���p�Ѽ�
/*********************************************************************/
void	MarkovRandomField::EM_exec( QImage img )
{
	image	=	img;

	const int		MAX_ITR	=	1000;
	const int		width	=	image.width();
	const int		height	=	image.height();
	const int		size	=	width * height;
	const int		DIM		=	vpEnergy[0]->dim();
	const double	end_thr	=	0.00001;

	EM_init();	// init

	// �}�l�i�� E-M �L�{
	const int	MAX_LABEL	=	vpEnergy.size();

	int		i,	j,	n;
	int		label;
	double	sum;

	//std::vector<std::vector<double>>	mean(MAX_LABEL),		devia(MAX_LABEL);
	//std::vector<std::vector<double>>	old_mean(MAX_LABEL),	old_devia(MAX_LABEL);

	double	**mean		=	new double* [MAX_LABEL];	// ���F�[�t��Ϋ��� �p��index����y�������~
	double	**devia		=	new double* [MAX_LABEL];
	double	**old_mean	=	new double* [MAX_LABEL];
	double	**old_devia	=	new double* [MAX_LABEL];

	for( i = 0; i < MAX_LABEL; i++ )
	{
		mean[i]			=	new double[DIM];
		devia[i]		=	new double[DIM];
		old_mean[i]		=	new double[DIM];
		old_devia[i]	=	new double[DIM];
	}


#ifdef _DEBUG		// �ˬd�Ϥ��O�_�ۦP
	for( label = 0; label < MAX_LABEL; label++ )
	{
		assert( vpEnergy[label]->get_image() == image );
		assert( vpEnergy[label]->check_image() );
	}
#endif

	D.resize(MAX_LABEL);
	for( i = 0; i < MAX_LABEL; i++ )
		D[i].resize( width, height );

	label_map.resize(width,height);		// �쥻���ݭn ���F��ܶi�פ~�[�i��

	P_lambda	=	new double[MAX_LABEL];
	K_lambda	=	new double[MAX_LABEL];	// �b�p�⪺�L�{���|����l��
	for( i = 0; i < MAX_LABEL; i++ )
		P_lambda[i]	=	1.0 / MAX_LABEL;	// ��l�� P_lambda  �Χ������覡 (weight sum = 1)

	cout << "initial...\n";

	// �]�w�ª�mean, devia
	for( i = 0; i < MAX_LABEL; i++ )
	{
		vpEnergy[i]->get_mean( old_mean[i] );
		vpEnergy[i]->get_devia( old_devia[i] );
		//old_mean[d]		=	vpEnergy[d]->get_mean();
		//old_devia[d]	=	vpEnergy[d]->get_devia();
	}
	// �}�l�i�J���N
	for( n = 0; n < MAX_ITR; n++ )
	{
		cout << "the " << n << " iteration...\n";

		// Estimation
		E_Step();

		// �j�M�̤p��  (�i�H�ٲ� ���F�ݨ�L�{�~�[�i��
		double	max;
		int		max_id;
		for( i = 0; i < width; i++ )
			for( j = 0; j < height; j++ )
			{
				max		=	D[0](i,j);
				max_id	=	0;
				for( label = 1; label < MAX_LABEL; label++ )
				{
					if( max < D[label](i,j) )
					{
						max		=	D[label](i,j);
						max_id	=	label;
					}
				}
				label_map( i, j )	=	max_id;
			}
		// ��ܷ�e�i�׹�
		for( i = 0; i < width; i++ )
			for( j = 0; j < height; j++ )
			{
				img.setPixel( i, j, (label_map(i,j)*761838257287)%193707721 );
				//img.gray_channel( i, j, label_map(i,j)*30 );
			}
		img.save("../TEMP/output.bmp");



#ifdef _DEBUG	// �ˬd���v�X�O�_���� 1
		for( i = 0; i < width; i++ )
			for( j = 0; j < height; j++ )
			{
				double	tmp_sum	=	0;
				for( label = 0; label < MAX_LABEL; label++ )
					tmp_sum	+=	D[label](i,j);
				assert( fabs(tmp_sum-1.0) < 0.0001 );
			}
#endif

		// Maximization
		M_Step();

		// �ˬd�O�_�n�����j��
		for( i = 0; i < MAX_LABEL; i++ )
		{
			vpEnergy[i]->get_mean( mean[i] );
			vpEnergy[i]->get_devia( devia[i] );
			//mean[d]		=	vpEnergy[d]->get_mean();
			//devia[d]	=	vpEnergy[d]->get_devia();
		}
		sum		=	0;
		for( i = 0; i < MAX_LABEL; i++ )
			for( j = 0; j < DIM; j++ )
				sum		+=	fabs(mean[i][j] - old_mean[i][j]) + fabs(devia[i][j] - old_devia[i][j] );
		if( sum < end_thr )		break;

		// ��X���G
		cout << "mean = " << endl;
		for( i = 0; i < MAX_LABEL; i++ )
			for( j = 0; j < DIM; j++ )
				cout << mean[i][j] << " ";
		cout << "\ndevia = " << endl;
		for( i = 0; i < MAX_LABEL; i++ )
			for( j = 0; j < DIM; j++ )
				cout << devia[i][j] << " ";
		cout << endl;

		// ��s
		for( i = 0; i < MAX_LABEL; i++ )
			for( j = 0; j < DIM; j++ )
			{
				old_mean[i][j]		=	mean[i][j];
				old_devia[i][j]		=	devia[i][j];
			}
		//old_mean	=	mean;
		//old_devia	=	devia;

	}

	// ��X���G
	cout << "end ...\n";
	cout << "mean = " << endl;
	for( i = 0; i < MAX_LABEL; i++ )
		for( j = 0; j < DIM; j++ )
			cout << mean[i][j] << " ";
	cout << "\ndevia = " << endl;
	for( i = 0; i < MAX_LABEL; i++ )
		for( j = 0; j < DIM; j++ )
			cout << devia[i][j] << " ";
	cout << endl;

	delete	[] P_lambda;	P_lambda	=	NULL;
	delete	[] K_lambda;	K_lambda	=	NULL;

//	for( label = 0; label < MAX_LABEL; label++ )
//		cout << ((Distance*)vpEnergy[label]->feature)->get_mean() << " " << ((Distance*)vpEnergy[label]->feature)->get_devia() << endl;

	for( i = 0; i < MAX_LABEL; i++ )
	{
		delete	[] mean[i];			mean[i]			=	0;
		delete	[] devia[i];		devia[i]		=	0;
		delete	[] old_mean[i];		old_mean[i]		=	0;
		delete	[] old_devia[i];	old_devia[i]	=	0;
	}
	delete	[]	mean;
	delete	[]	devia;
	delete	[]	old_mean;
	delete	[]	old_devia;

}


/********************************************************************
	��l�� (�M�w�X��label, mean �� sigma ����l��
/*********************************************************************/
void	MarkovRandomField::EM_init()
{
	int		i;

	switch( vpEnergy[0]->get_feature_type() )
	{
		case	MRF_FEATURE_MOMENT:
			assert(false);	// �|�����g
			break;

		case	MRF_FEATURE_DISTANCE:
			EM_init_distance();
			break;

		default:
			assert(false);
	}

	// ���s�]�w�@���Ϥ� ��b�o��O�]����o�̤~�T�wvpEnergy���ƶq
	for( i = 0; i < vpEnergy.size(); i++ )
		vpEnergy[i]->set_image(image);
}



/********************************************************************
	��l�� (�M�w�X��label, mean �� sigma ����l��
/*********************************************************************/
void	MarkovRandomField::EM_init_distance()
{
	GaussFilter_2D	gf2(7,7);		// ���� gauss filter �|��histogram��local maximal����ܦh

	QImage	img	=	gf2.do_filter(image);


	//img.save("../TEMP/output.bmp");

	const int	width	=	img.width();
	const int	height	=	img.height();
	const int	THR		=	width*height/100;	// �ʦ�

	// �έp histogram
	int				i,	j;
	int				a[256]	=	{0};
	int				value;
	//sVectorDouble	vec(1);
	double			vec[1];


	for( i = 0; i < image.width(); i++ )
		for( j = 0; j < image.height(); j++ )
		{
			value	=	img.gray_channel( i, j );
			a[value]++;
		}

	// �j�Mlocal max
	int		map[256];	//  0 or 1

	// ��l�� map �p��thr���屼
	for( i = 0; i < 256; i++ )
	{
		if( a[i] < THR )	map[i]	=	0;
		else				map[i]	=	1;
	}

	// ��P��� �T�w�Olocal max
	for( i = 0; i < 256; i++ )
	{
		if( map[i] == 0 )
			continue;

		for( j = i-3; j <= i+3; j++ )
		{
			if( j >= 0 && j < 256 && j!=i )
			{
				if( a[j] > a[i] )
					map[i]	=	0;
			}
		}
	}

	vpEnergy.clear();	// �N�¸�ƧR��

	// �Q�ε��G�h���� feature (��Jvector)
	for( i = 0; i < 256; i++ )
	{
		if( map[i] == 1 )
		{
			Distance	dt;
			//assert( vec.size() == dt.dim() );
				
			vec[0]	=	i;					// ���F�Τ@�������vector
			dt.set_mean(vec);				// histogram���ͪ�mean  (�̤j�Ȫ�index)

			vec[0]	=	2.0;
			dt.set_devia(vec);				// ��l�ƪ�deviation (�T�w��5.0 �H��ݭn���n�qhistogram�p��)

			EnergyStandard	et( dt , image );	// ���ݭn set_anchor
			push_back(et);
		}
	}

	cout << "#Label = " << vpEnergy.size() << endl;

}


/********************************************************************
	��������h���k
/*********************************************************************/
void	MarkovRandomField::SimuAnneal_exec( QImage img )
{
	image	=	img;

	int		i,	j,	n;
	int		label;

	// ���s�]�w�@���Ϥ�
	for( i = 0; i < vpEnergy.size(); i++ )
		vpEnergy[i]->set_image(image);

	const int	width		=	vpEnergy[0]->get_img_width();
	const int	height		=	vpEnergy[0]->get_img_height();
	const int	MAX_ITER	=	1999999999;//width*height*500;
	const int	MAX_LABEL	=	vpEnergy.size();				// �̤jlabel�Ӽ�

	double		T;				// �ū�
	double		xi;				// �s��ūפ�����ü� (���v)
	int			value_tmp;		// �Ȧs��
	int			pix;
	bMatrixInt	label_tmp;		// �Ȧs�Ϊ�label map

	double		energy1,	energy2;		// �Ȧs��q��  1�O�쥻��  2�O�ק諸

	boost::random::mt19937						gen(time(0));
	boost::random::uniform_int_distribution<>	dist( 0, MAX_LABEL-1 );		// �üƲ��;�
	boost::random::uniform_int_distribution<>	dist_x( 0, width-1 );
	boost::random::uniform_int_distribution<>	dist_y( 0, height-1 );
	boost::random::uniform_01<>					dist_01;


	//bMatrixInt	old_map		=	label_map;		// �ª� label map, �P�_���N����ϥ�

#ifdef _DEBUG		// �ˬd�Ϥ��O�_�ۦP
	for( label = 0; label < MAX_LABEL; label++ )
	{
		assert( vpEnergy[label]->get_image() == image );
		assert( vpEnergy[label]->check_image() );
	}
#endif

	D.resize(MAX_LABEL);							// �`�@���X��label
	for( i = 0; i < MAX_LABEL; i++ )				// ��l��  ���N���ɭ����ӭn����l��
		D[i].resize( width, height );

	// �ζüƪ��覡���� label map
	label_map.resize( width, height );
	for( i = 0; i < width; i++ )
		for( j = 0; j < height; j++ )
			label_map( i, j )	=	dist(gen);

	T	=	100 + 1;//MAX_ITER + 1;
	energy1		=	totalEnergy( label_map );
	label_tmp	=	label_map;	// ���F�[�t
	for( n = 0; n < MAX_ITER; n++ )
	{	
		//cout << "itr = " << setw(3) << n << " energy = " << dataEnergy() + smoothEnergy() << endl;

		//label_tmp	=	label_map;
		i			=	dist_x(gen);
		j			=	dist_y(gen);	// �üƲ��ͭn���ܪ��I

		while(1)
		{
			value_tmp	=	dist(gen);		// �üƲ��ͭn���ܪ���
			if( value_tmp != label_map(i,j) )
				break;
		}

		xi				=	dist_01(gen);
		label_tmp(i,j)	=	value_tmp;
		pix				=	i * height + j;

		//energy1			=	totalEnergy( label_map );
		//energy2			=	totalEnergy( label_tmp );
		energy2		=	energy1 - singleEnergy( pix, label_map(i,j) ) + singleEnergy( pix, label_tmp(i,j) );
		                                     /* �p�� label_map                              label _tmp */
		
		if( energy1 >= energy2 ) // �h�����\
		{
			//cout << "*";
			//label_map	=	label_tmp;
			label_map(i,j)	=	value_tmp;		// ��ֹB��q
			energy1			=	energy2;
		}
		else if( energy1 < energy2 && xi < exp( -(energy2-energy1)/T ) )
		{
			//cout << "*";
			//label_map	=	label_tmp;
			label_map(i,j)	=	value_tmp;
			energy1			=	energy2;
		}
		else
		{
			//cout << "-";
			// ����
			label_tmp(i,j)	=	label_map(i,j);		// ���F�[�t
		}

		//T	-=	(100.0/MAX_ITER);
		if( T > 0.1 )	T -= 0.1;

		if( n%10000 == 0 )
		{
			cout << "*";
			// ��X���G
			QImage	img( width, height, QImage::Format_RGB888 );
			for( i = 0; i < width; i++ )
				for( j = 0; j < height; j++ )
				{
					label	=	label_map(i,j);
					img.setPixel( i, j, (label*761838257287)%193707721 );
				}

			char	filename[100];
			sprintf( filename, "%s/output.bmp", TMP_PATH );
			img.save(QString(filename));
		}


		// �Q��label map�M�w�O�_�n����N
		//if( old_map == label_map )		break;
		//old_map		=	label_map;
	
	} // end MAX_ITR

	cout << "end energy = " << dataEnergy() + smoothEnergy() << endl;

	// ��X���G
	QImage	img( width, height, QImage::Format_RGB888 );
	for( i = 0; i < width; i++ )
		for( j = 0; j < height; j++ )
		{
			label	=	label_map(i,j);
			img.setPixel( i, j, (label*761838257287)%193707721 );
		}

	char	filename[100];
	sprintf( filename, "%s/output.bmp", TMP_PATH );
	img.save(QString(filename));
	cout << "output finish.\n";
}


/********************************************************************
	�Ҽ{ 8-neighbor
/*********************************************************************/
void	MarkovRandomField::SimuAnneal8_exec( QImage img )
{
	// �٨S�g ���ݭn�A�g.
	assert(false);
}



/********************************************************************
	�p���@�I��energy
/*********************************************************************/
double	MarkovRandomField::singleEnergy( int pix, int lab )
{
	const int	width	=	image.width();
	const int	height	=	image.height();

	double		sum		=	0;
	int			i,		j;
	int			lab1,	lab2,	pix1,	pix2;

	i		=	pix / height;
	j		=	pix % height;


	lab1	=	lab;
	pix1	=	ij2pix(i,j);

	// 1 clique
	sum		+=	vpEnergy[lab]->E_data( pix, lab );

	// 2 clique
	// �W
	if( j-1 >= 0 )
	{
		pix2	=	ij2pix(i,j-1);
		lab2	=	label_map(i,j-1);
		sum		+=	vpEnergy[lab]->E_smooth( pix1, pix2, lab1, lab2 );
	}

	// �U
	if( j+1 < height )
	{
		pix2	=	ij2pix(i,j+1);
		lab2	=	label_map(i,j+1);
		sum		+=	vpEnergy[lab]->E_smooth( pix1, pix2, lab1, lab2 );
	}

	// ��
	if( i-1 >= 0 )
	{
		pix2	=	ij2pix(i-1,j);
		lab2	=	label_map(i-1,j);
		sum		+=	vpEnergy[lab]->E_smooth( pix1, pix2, lab1, lab2 );
	}

	// �k
	if( i+1 < width )
	{
		pix2	=	ij2pix(i+1,j);
		lab2	=	label_map(i+1,j);
		sum		+=	vpEnergy[lab]->E_smooth( pix1, pix2, lab1, lab2 );
	}


	return	sum;
}



/********************************************************************
	�ھ�label���G�ӭp���q
/*********************************************************************/
double	MarkovRandomField::totalEnergy( bMatrixInt label )
{
	const int	width	=	image.width();
	const int	height	=	image.height();

	double		sum		=	0;
	int			i,		j;
	int			lab,	pix;
	int			lab1,	lab2,	pix1,	pix2;

	for( i = 0; i < width; i++ )
		for( j = 0; j < height; j++ )
		{
			lab		=	label(i,j);
			lab1	=	label(i,j);
			pix		=	ij2pix(i,j);
			pix1	=	ij2pix(i,j);

			// 1 clique
			sum		+=	vpEnergy[lab]->E_data( pix, lab );

			// 2 clique
			// �W
			if( j-1 >= 0 )
			{
				pix2	=	ij2pix(i,j-1);
				lab2	=	label(i,j-1);
				sum		+=	vpEnergy[lab]->E_smooth( pix1, pix2, lab1, lab2 );
			}

			// �U
			if( j+1 < height )
			{
				pix2	=	ij2pix(i,j+1);
				lab2	=	label(i,j+1);
				sum		+=	vpEnergy[lab]->E_smooth( pix1, pix2, lab1, lab2 );
			}

			// ��
			if( i-1 >= 0 )
			{
				pix2	=	ij2pix(i-1,j);
				lab2	=	label(i-1,j);
				sum		+=	vpEnergy[lab]->E_smooth( pix1, pix2, lab1, lab2 );
			}

			// �k
			if( i+1 < width )
			{
				pix2	=	ij2pix(i+1,j);
				lab2	=	label(i+1,j);
				sum		+=	vpEnergy[lab]->E_smooth( pix1, pix2, lab1, lab2 );
			}
		}

	return	sum;
}



/********************************************************************
	gibbs ��l��
/*********************************************************************/
void	MarkovRandomField::initial_state( int MAX_LABEL, int *counts, double *sum, double *sumsq )
{
	int		width	=	image.width();
	int		height	=	image.height();
	int		i,	j;
	int		label;
	double	data;

	boost::mt19937								gen(time(0));	
	boost::random::uniform_int_distribution<>	dist( 0, MAX_LABEL-1 );

	// ��l��label map (�ζü�)
	label_map.resize( width, height );
	for( i = 0; i < width; i++ )
		for( j = 0; j < height; j++ )
			label_map( i, j )	=	dist(gen);

	for( i = 0; i < MAX_LABEL; i++ ) 
	{
		counts[i]	=	0;
		sum[i]		=	0;
		sumsq[i]	=	0;
	}

	for( i = 0; i < width; i++ )
		for( j = 0; j < height; j++ )
		{
			label	=	label_map( i, j );
			data	=	image.gray_channel(i,j);

			counts[label]++;
			sum[label]		+=	data;
			sumsq[label]	+=	pow(data,2);
		}
}


/********************************************************************
	gibbs �����I
/*********************************************************************/
void	MarkovRandomField::remove_observation( double data, int label, int *counts, double *sum, double *sumsq )
{
	counts[label]--;
	sum[label]		-=	data;
	sumsq[label]	-=	pow( data, 2 );
}




/********************************************************************
	gibbs �W�[�I
/*********************************************************************/
void	MarkovRandomField::add_observation( double data, int label, int *counts, double *sum, double *sumsq )
{
	counts[label]++;
	sum[label]		+=	data;
	sumsq[label]	+=	pow( data, 2 );
}



/********************************************************************
	gibbs ��s���p��
/*********************************************************************/
void	MarkovRandomField::update_estimates( int MAX_LABEL, double *prob, double *mean, double *sigma, int *counts, double *sum , double *sumsq )
{
	const int	width	=	image.width();
	const int	height	=	image.height();
	const int	n		=	width * height - 1;		// �]��remove���@���I �ҥH�Ӽƥ��� -1

	int		i;

	for( i = 0; i < MAX_LABEL; i++ )
	{
		prob[i]		=	1.0 * counts[i] / n;
		mean[i]		=	sum[i] / counts[i];
		sigma[i]	=	sqrt( (sumsq[i] - mean[i]*mean[i]*counts[i]) / (counts[i]) );
	}
}



/********************************************************************
	gibbs �D���v
	(�W�٥�norm�O�]���פ�g��)
/*********************************************************************/
double	MarkovRandomField::dnorm( double x, double mean, double sigma )
{
	const double	T	=	1;	// �ū� �ثe�w�] = 1

	if( sigma < 0.001 )		// �зǮt�L�p��case  (�i�হlabel�L�Ĥ�����)
		return	0;
	else
		return	exp( -pow( x-mean, 2 ) / ( 2.0*pow(sigma,2) ) / T )  / sqrt( 2.0*3.1415926) / (sigma)  ;
}



/********************************************************************
	gibbs �D���v�M
	(����normalized���Ѽ�Z)
/*********************************************************************/
double	MarkovRandomField::dmix( int MAX_LABEL, double x, double *probs, double *mean, double *sigma )
{
	int		i;
	double	sum	=	0;

	for( i = 0; i < MAX_LABEL; i++ )
		sum		+=	probs[i] * dnorm( x, mean[i], sigma[i] );

	return	sum;
}




/********************************************************************
	gibbs �h�f�����v����
/*********************************************************************/
int		MarkovRandomField::sample_group( int MAX_LABEL, double data, double *probs, double *mean, double *sigma )
{
	static boost::mt19937						mrf_gen(time(0));		// �n�`�N������J�ɶ���ؤl
	static boost::random::uniform_01<>			dist_01;				// 0~1�����ä���

	int			label;
	double		p	=	dist_01(mrf_gen);		// �H�����;��v
	double		Z	=	dmix( MAX_LABEL, data, probs, mean, sigma );

	for( label = 0; label < MAX_LABEL - 1; label++ )	// -1 �]���̫�@�ӴN���ΦA��F
	{
		double	p_label	=	probs[label] * dnorm( data, mean[label], sigma[label] ) / Z;		// �D�X���v����

		if( p < p_label )
			return	label;	// ���b�Ӱ϶� �N�^��

		p	-=	p_label;		// �Q���O�֭p���v���� �� pdf���ഫ
	}

	return	MAX_LABEL - 1;
}







/********************************************************************
	���� gibbs sampler  (��EM�ĪG����)
/*********************************************************************/
void	MarkovRandomField::Gibbs_exec( QImage img )
{
	image	=	img;

	const int		width	=	image.width();
	const int		height	=	image.height();

	EM_init_distance();	// init  (�o���٬O��EM���{���X�h��l�� �H��n��A��) 

	// �i�Jgibbs sampler
	const int		MAX_LABEL	=	vpEnergy.size();
	const int		MIN_GROUP	=	100;			// ��ܰϰ��I�ƹL�p�N���L (�Y�ǮɭԬY�ǰ϶��|�Q�Y��)

	int				label;
	int				i,	j,	n;
	int				x,	y;

	int				*counts		=	new int[MAX_LABEL];
	double			*sum		=	new double[MAX_LABEL];
	double			*sumsq		=	new double[MAX_LABEL];
	double			*probs		=	new double[MAX_LABEL];
	double			*mean		=	new double[MAX_LABEL];
	double			*sigma		=	new double[MAX_LABEL];

	boost::mt19937								gen(time(0));	
	boost::random::uniform_int_distribution<>	dist_x( 0, width-1 );
	boost::random::uniform_int_distribution<>	dist_y( 0, height-1 );
	boost::random::uniform_int_distribution<>	dist_label( 0, MAX_LABEL-1 );

	// ��l�Ƹ��
	initial_state( MAX_LABEL, counts, sum, sumsq );
	
	for( n = 0; n < 1999999999; n++ )		// 1999999999 ���N���j���Ʀr
	{
		x	=	dist_x(gen);
		y	=	dist_y(gen);

		if( counts[ label_map(x,y) ] < MIN_GROUP )		// �϶��ֳQ�Y���N���L
			continue;

		remove_observation( image.gray_channel( x, y ) , label_map(x,y) , counts, sum , sumsq );	
		update_estimates( MAX_LABEL, probs, mean, sigma, counts, sum, sumsq );
	
		label_map(x,y)	=	sample_group( MAX_LABEL, image.gray_channel(x,y), probs, mean, sigma );

		add_observation( image.gray_channel( x, y), label_map(x,y) , counts, sum, sumsq );

		// �@�q�ɶ��L�X��T
		if( n % 1000000 == 0 )
		{
			for( i = 0; i < MAX_LABEL; i++ )
				cout << setw(7) << setprecision(5) << mean[i] << " ";
			cout << endl;
			for( i = 0; i < MAX_LABEL; i++ )
				cout << setw(7) << setprecision(5) << sigma[i] << " ";
			cout << endl << endl;

			for( i = 0; i < width; i++ )
				for( int j = 0; j < height; j++ )
				{
					label	=	label_map(i,j);
					img.setPixel( i, j, (label*761838257287)%193707721 );
					//img.gray_channel( ii, jj, label_map(ii,jj) * 30 );
				}
			img.save("../TEMP/output.bmp");

		}
	}

	// �R�����
	delete [] counts;	counts	=	0;
	delete [] sum;		sum		=	0;
	delete [] sumsq;	sumsq	=	0;
	delete [] probs;	probs	=	0;
	delete [] mean;		mean	=	0;
	delete [] sigma;	sigma	=	0;
}