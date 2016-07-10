#include "MRSAR.h"

using namespace std;


/**********************************************************************
	�غc
/***********************************************************************/
MRSAR::MRSAR( QImage imagePass )
	:	mrfFeatureAbstract( imagePass, dim() )
{}


/**********************************************************************
	�غc
/***********************************************************************/
MRSAR::MRSAR()
	:	mrfFeatureAbstract( dim() )
{}


/**********************************************************************
	copy construct
/***********************************************************************/
MRSAR::MRSAR( const MRSAR &mr )
	:	mrfFeatureAbstract(mr)
{
	*this	=	mr;
}


/**********************************************************************
	�Ѻc
/***********************************************************************/
MRSAR::~MRSAR()
{}



/**********************************************************************
	����
/***********************************************************************/
void	MRSAR::segmentation( QImage img )
{
	assert(false);
}


/**********************************************************************
	�p��˱i 
	��cal_feature���e�|�ܹ� 
	�t�b�o��|�⥭���зǮt
/***********************************************************************/
void	MRSAR::cal_sample_feature( QImage img )
{
	cal_feature( img );		// �i�h�����S�x

	cal_mean();				// �p�⥭��
	cal_devia();			// �p��зǮt
}




/**********************************************************************
	�p��feature
/***********************************************************************/
void	MRSAR::cal_feature( QImage img )
{
	mr_sar_cal( img, map );

	// �]��mr_sar�����k�|������� �ҥH�o��n�A���^�ഫ

	int		i,	j,	k;

	std::vector<bMatrixDouble>	map_tmp(DIMENSION);//( img.width(), img.height() );
	//map_tmp.clear();
	for( i = 0; i < DIMENSION; i++ )
	{
		map_tmp[i].resize( img.width(), img.height() );
		map_tmp[i].clear();
	}

	for( i = 0; i < map[0].size1(); i++ )
		for( j = 0; j < map[0].size2(); j++ )
			for( k = 0; k < DIMENSION; k++ )
				map_tmp[k]( i+10, j+10 )	=	map[k]( i, j );		// 10�O�g�����Ʀr �����bmr_sar = 21

	map		=	map_tmp;


	for( i = 0; i < DIMENSION; i++ )
	{
		char	filename[100];
		sprintf( filename, "../TEMP/%d.bmp", i );

		img		=	Matrix2Image( map[i], M2I::normalized );
		img.save(filename);
	}

}



/**********************************************************************
	���տ�X
/***********************************************************************/
void	MRSAR::output_txt()
{
	assert(0);
}



/**********************************************************************
	�p�� norm
	�o��|�}�ڸ�
/***********************************************************************/
double	MRSAR::get_norm( int pix )
{
	double	value	=	get_norm_squ(pix);

	return	sqrt(value);
}


/**********************************************************************
	�p�� |u-v|
/***********************************************************************/
double	MRSAR::get_norm( int pix1, int pix2 )
{
	assert(0);
	return	0;
}

/**********************************************************************
	�^�ǫ��O
/***********************************************************************/
MRF_FEATURE_TYPE	MRSAR::type()
{
	return	MRF_FEATURE_MRSAR;
}


/**********************************************************************
	����B��
/***********************************************************************/
void	MRSAR::operator	=	( const MRSAR &mr )
{
	mrfFeatureAbstract::operator = (mr);

	map	=	mr.map;
}


/**********************************************************************
	�^�Ǻ���
/***********************************************************************/
int		MRSAR::dim()
{
	return	DIMENSION;
}


/**********************************************************************
	�p�⥭����
/***********************************************************************/
void	MRSAR::cal_mean()
{
	const int	width	=	image.width();
	const int	height	=	image.height();
	const int	size	=	(width-2*10) * (height-2*10);		// �o�� 10���α`��

	double	sum	=	0;

	cout << endl;
	for( int k = 0; k < DIMENSION; k++ )
	{
		sum	=	0;
		for( int i = 10; i < width - 10; i++ )
			for( int j = 10; j < height - 10; j++ )
				sum	+=	map[k](i,j);
		mean[k]		=	sum / size;
		cout << "mean[" << k << "] = " << mean[k] << endl;
	}

	//mean[0]		=	sum / size;					// �]���ثe���� = 1
	//cout << "mean = " << mean[0] << endl;
}


/**********************************************************************
	�p��зǮt
/***********************************************************************/
void	MRSAR::cal_devia()
{
	const int	width	=	image.width();
	const int	height	=	image.height();
	const int	size	=	(width-2*10) * (height-2*10);

	double	sum	=	0;

	for( int k = 0; k < DIMENSION; k++ )
	{
		sum	=	0;
		for( int i = 10; i < width - 10; i++ )
			for( int j = 10; j < height - 10; j++ )
				sum	+=	(map[k](i,j) - mean[k]) * (map[k](i,j) - mean[k]);
		devia[k]	=	sqrt( sum/size );
		cout << "devia[" << k << "] = " << devia[k] << endl;
	}

	//devia[0]	=	sqrt( sum / size );			// �]������ = 1
	//cout << "devia = " << devia[0] << endl;
}


/**********************************************************************
	�^�� norm^2
/***********************************************************************/
double		MRSAR::get_norm_squ( int pix )
{
	const int	width	=	image.width();
	const int	height	=	image.height();

	assert( width > 0 && height > 0 );

	int		i,	j;
	double	value;
	//double	r;
	int		nth;
	double	v1,	v2;

	i	=	pix / height;
	j	=	pix % height;

	assert( i >= 0 && i < width && j >= 0 && j < height );

	// ��ڤW�����Wdeviation!!
	//value	=	map( i, j ) - mean[0];					// �]������ = 1
	//r		=	pow(value,2) / (2.0*pow(devia[0],2));	// �]������ = 1

	double	sum;

	sum		=	0;
	for( nth = 0; nth < DIMENSION; nth++ )
	{
		v1		=	map[nth](i,j) - mean[nth];
		v2		=	pow( v1, 2 ) / 2.0 / pow( devia[nth], 2 );
		sum		+=	v2;
	}
	//sum		*=	0.5;

	return	sum;
}


/**********************************************************************
	�^�� devia^2
/***********************************************************************/
double		MRSAR::get_devia_det()
{
	double	mul	=	1;

	for( int i = 0; i < DIMENSION; i++ )
		mul	*=	devia[i]*devia[i];

	//return	devia[0] * devia[0];
	return	mul;
}


/**********************************************************************
	���o�S�x
/***********************************************************************/
void	MRSAR::get_feature( int pix, double *_f )
{
	const int	width	=	image.width();
	const int	height	=	image.height();

	int		i	=	pix / height;
	int		j	=	pix % height;
	int		k;

	assert( i >= 0 && i < width && j >= 0 && j < height );

	//_f[0]	=	map( i, j );			// �]��DIMENSION = 1


	for( k = 0; k < DIMENSION; k++ )
		_f[k]	=	map[k](i,j);

}

/**********************************************************************
	�ˬd����
/***********************************************************************/
bool	MRSAR::check_dim()
{
	return	true;
}




/**********************************************************************
	�ˬd����
/***********************************************************************/
void	MRSAR::output()
{
	const int	width	=	image.width();
	const int	height	=	image.height();

	


}