#include "OCR_Recognition.h"



 OCR_INIT_TYPE		init_type;



/******************************************************************************
	OCR ��l��
*******************************************************************************/
void	OCR_init( OCR_INIT_TYPE _init_type )
{
	init_type	=	_init_type;

	OCR_init_template( );				// ��l�Ƽ˪O
	
	////Ū��SVM_MODLE�̭������׻P�S�x�ƶq***********************************************************
	nr_class	=	0;
	nr_feature	=	0;
	O_label		=	NULL;
	Omega		=	NULL;

	OCR_pre_read_SVM_model( init_type );			// Ū�� nr_class, nr_feature, O_label
	OCR_read_SVM_model( init_type );				// Ū�� Omega
	////********************************************************************************************
	OCR_read_SCALE_TABLE();
}



/******************************************************************************
	OCR ��l�Ƽ˪O
*******************************************************************************/
void	OCR_init_template(  )
{
	FILE	*p;
	int		index;
	int		i,	j; //,	tmp;
	int		block, v_shift, h_shift;
	char	filename[100];

	BYTE	tmp;

	for( index = 1; index <= 36; index++ )
	{
		if( init_type == OCR_CNY_20 )
			sprintf( filename, "%s\\%02d.bin", _TEMPLATE_PATH_CNY_20, index );		
		else if( init_type == OCR_CNY_100 )
			sprintf( filename, "%s\\%02d.bin", _TEMPLATE_PATH_CNY_100, index );		
		else
			printf("error.......\n");

		p	=	fopen( filename, "rb" );

		for( j = 0; j < Sub_height; j++ )
			for( i = 0; i < Sub_width; i++ )
			{
				//fscanf( p, "%d", &tmp );
				fread( &tmp, sizeof(BYTE), 1, p );

#ifdef _OCR_USE_FLOAT_
				template_original[ index*Sub_width*Sub_height + j*Sub_width + i ]	=	(float)(tmp);
#else
				template_original[ index*Sub_width*Sub_height + j*Sub_width + i ]	=	(short)tmp;
#endif

			}
		fclose(p);


		for(block = 0; block<4; block++)
		{
			for(v_shift = 0; v_shift<3; v_shift++)
				for(h_shift = 0; h_shift<4; h_shift++)
				{
					Pre_Load_Shift_Template(index, block, v_shift, h_shift);
				}
		}


	}
}




/******************************************************************************
	OCR ����Ϲ�����
*******************************************************************************/
int		OCR_exec( BYTE *img, BYTE *bin )
{	
	int		i, j;
	int		output;

	BYTE	Segmental_image_Binary[ 22*32 ];
	BYTE	Segmental_image[ 22*32 ];


	////************************************************************
	for( i = 0; i < 22*32; i++ )
		Segmental_image[i]		=	img[i];

	// william ��bin
	LWO_binary( Segmental_image, Segmental_image_Binary );

	////OCR��r����*********************************************************************************
	output	=	OCR_Recognize(Segmental_image_Binary, Segmental_image );
	////********************************************************************************************

	return output;
}





#if 0
/******************************************************************************
	OCR ��r���� ���鳡��
*******************************************************************************/
int		OCR_Recognize( BYTE *Segmental_image_Binary, BYTE *Segmental_image )
{

	int		BZ_h_shift = 0, BZ_v_shift = 0;
	short	Sample_Binary[256]	=	{0};
	int		m,	q;


#ifdef	_OCR_USE_FLOAT_
	float	act_out[300]	=	{0};
#else
	short	act_out_carry_positive[35]		=	{0};		// �i�쳡�� ��
	int		act_out_remain_positive[35]		=	{0};		// �ݾl��
	short	act_out_carry_negative[35]		=	{0};		// �i�쳡�� �t
	int		act_out_remain_negative[35]		=	{0};		// �ݾl��
	short	act_out_carry[35]				=	{0};		// �̫ᵲ�G
	int		act_out_remain[35]				=	{0};		

	short	act_out_max_carry;		// �Ƨǥθ��
	int		act_out_max_remain;
	int		act_out_max_id;

#endif


	int		i,	j,	k,	tmp;
	short	stmp;
	int		attribute;

#ifdef	_OCR_USE_FLOAT_
	float	reorder_output[300]	=	{0};
#else
	short	reorder_output[35]	=	{0};		// ���ӥi�H��signed char  ���O����short  �u�s  -1 0 1 �T�ص��G
#endif

	int		reorder_label[35]	=	{0};
	float	_t_output[35]	=	{0};

	float	temp_out;
	int		temp_label_1;			

	int		h_shift, v_shift;

	float	order_t_output;
	int		flag_t_output;

	int		classifier_output;

#ifdef _OCR_USE_FLOAT_
	float	Feature[300]	=	{0};
#else
	int		Feature[129]	=	{0};
#endif

	short	Sample[256]	=	{0};

#ifdef _OCR_USE_FLOAT_
	float	BZ_Feature[300]	=	{0};
	float	F_DWT_image[300]	=	{0},	S_DWT_image[300]	=	{0};
	float	F_DWT_LL[300]	=	{0},	F_DWT_LH[300]	=	{0},	F_DWT_HL[300]	=	{0},	F_DWT_HH[300]	=	{0};
	float	S_DWT_LL[300]	=	{0},	S_DWT_LH[300]	=	{0},	S_DWT_HL[300]	=	{0},	S_DWT_HH[300]	=	{0};
	float	HOG_Feature[300]	=	{0},	S_LSF_Feature[300]	=	{0};
#else
	int		BZ_Feature[29]		=	{0};
	int		F_DWT_image[256]	=	{0},	S_DWT_image[64]		=	{0};
	int		F_DWT_LL[64]		=	{0},	F_DWT_LH[64]		=	{0},	F_DWT_HL[64]	=	{0},	F_DWT_HH[64]	=	{0};
	int		S_DWT_LL[16]		=	{0},	S_DWT_LH[16]		=	{0},	S_DWT_HL[16]	=	{0},	S_DWT_HH[16]	=	{0};
	int		HOG_Feature[9]		=	{0},	S_LSF_Feature[16]	=	{0};
#endif


	short	source_matching[1024]	=	{0};


	////�p��BZF����pixel��**************************************************
	BZ_h_shift = (int)(( Sub_width - OCR_MASK_WIDTH )/(BZ_H_Sample_Num-1));
	BZ_v_shift = (int)(( Sub_height - OCR_MASK_HEIGHT )/(BZ_V_Sample_Num-1));
	////********************************************************************

	
	////���Ψ�BZF�S�x*******************************************************
	for( i=0; i<BZ_H_Sample_Num; i++)
		for( k=0; k<BZ_V_Sample_Num; k++)
		{
			////Get Binary Sample
			OCR_Sampling(Segmental_image_Binary, Sample_Binary, BZ_h_shift, BZ_v_shift, i, k,  Sub_width, Sub_height );		
			// �N Sample_Binary ��buffer  �]���o��i�H����

			////Binary zone feature
			OCR_Pre_BZF(Sample_Binary, BZ_Feature, OCR_MASK_WIDTH, k*BZ_H_Sample_Num + i );
			// BZ_Feature ��Buffer 2  (�n���@���u�g�J�@�Ӧ�m)
		}
	////calculate BZF feature
	OCR_BZF(BZ_Feature, BZ_H_Sample_Num , BZ_V_Sample_Num);
	////*********************************************************************

	////�p��HOG-W�PLSF����pixel��********************************************
	//int h_shift, v_shift = 0;
	h_shift = (int)((Sub_width- OCR_MASK_WIDTH )/(H_Sample_Num-1));
	v_shift = (int)((Sub_height- OCR_MASK_HEIGHT )/(V_Sample_Num-1));
	////*********************************************************************

	////���Ψ�HOG-W�PLSF�S�x�æs�JFeature�x�}********************************
	//int attribute =0;
	attribute	=	0;
	for( i=0; i<H_Sample_Num; i++)
	{
		for( k=0; k<V_Sample_Num; k++)
		{
			////Get Sample
			OCR_Sampling( Segmental_image, Sample, h_shift, v_shift, i, k, Sub_width, Sub_height );
			// Sample �i�H��buffer (�򤧫e��Sample_Binary�@��?)

			////FIRST ORDER DWT
			OCR_Haar_DWT(Sample, F_DWT_image, OCR_MASK_WIDTH, OCR_MASK_HEIGHT);

			OCR_Calcu_DWT_L_H(F_DWT_image, F_DWT_LL, F_DWT_LH, F_DWT_HL, F_DWT_HH, OCR_MASK_WIDTH, OCR_MASK_HEIGHT);

			////HOG feature
			OCR_HOG(F_DWT_HL, F_DWT_LH, HOG_Feature, OCR_MASK_WIDTH/2, OCR_MASK_HEIGHT/2);

			////SECOND ORDER DWT
			OCR_Haar_DWT_2( F_DWT_LL, S_DWT_image, OCR_MASK_WIDTH/2, OCR_MASK_HEIGHT/2);

			OCR_Calcu_DWT_L_H(S_DWT_image, S_DWT_LL, S_DWT_HL, S_DWT_LH, S_DWT_HH, OCR_MASK_WIDTH/2, OCR_MASK_HEIGHT/2);

			////LSF feature
			OCR_LSF(S_DWT_HL, S_DWT_LH, S_DWT_HH, S_LSF_Feature, OCR_MASK_WIDTH/4, OCR_MASK_HEIGHT/4);
				
			for( q=0; q<OCR_MASK_WIDTH*OCR_MASK_HEIGHT; q++)
				source_matching[((attribute)*OCR_MASK_WIDTH*OCR_MASK_HEIGHT) + q]	=	Sample[q];			

			////Test Text Feature
			for( m=0; m<9; m++)
				Feature[attribute*25+m] = HOG_Feature[m];

			for( m=0; m<OCR_MASK_WIDTH; m++)
				Feature[9+attribute*25+m] = S_LSF_Feature[m];

			attribute++;
		}
	}//16*16 sampling end
	

	////�NBZF�S�x�s�JFeature�x�}��*******************************************
	for( m = 0; m<BZ_V_Sample_Num*BZ_H_Sample_Num+BZ_V_Sample_Num+BZ_H_Sample_Num; m++)
		Feature[25*(V_Sample_Num+H_Sample_Num)+m] = BZ_Feature[m];
		// �i�H�h��e�� �M������BZ_Feature
	////*********************************************************************
	
	for( i=0; i<nr_class; i++)
	{
		for( j=0; j<nr_feature; j++)
		{
#ifdef  _OCR_USE_FLOAT_
			act_out[i] +=   *(Omega + nr_feature*i + j ) * Feature[j]; //Omega[i][j]*Feature[j];
#else	
			tmp		=		*(Omega + nr_feature*i + j ) * Feature[j];

			if( tmp >= 0 )
			{
				if( tmp > OCR_2_30 )
				{
					tmp		-=	OCR_2_30;
					act_out_carry_positive[i]++;
				}

				act_out_remain_positive[i]	+=	tmp;

				if( act_out_remain_positive[i] > OCR_2_30 )
				{
					act_out_remain_positive[i]	-=	OCR_2_30;
					act_out_carry_positive[i]++;
				}
			}
			else if( tmp < 0 )
			{
				tmp		=	-tmp;		// ���Υ��ƥh�B�z
				if( tmp > OCR_2_30 )
				{
					tmp		-=	OCR_2_30;
					act_out_carry_negative[i]++;
				}

				act_out_remain_negative[i]	+=	tmp;

				if( act_out_remain_negative[i] > OCR_2_30 )
				{
					act_out_remain_negative[i]	-=	OCR_2_30;
					act_out_carry_negative[i]++;
				}	
			}

#endif
		}
	}

#ifndef   _OCR_USE_FLOAT_
	// ���t�X��
	for( i = 0; i < nr_class; i++ )
	{
		// �`�N��ƪ�ܪk  �o�� carry remain �����O���� (�̫��ഫ���ɭԦA�令�t��)
		// ����carry
		if( act_out_carry_positive[i] > act_out_carry_negative[i] )
		{
			if( act_out_remain_positive[i] >= act_out_remain_negative[i] )		// �S���i����D
			{
				act_out_remain[i]	=	act_out_remain_positive[i] - act_out_remain_negative[i];
				act_out_carry[i]	=	act_out_carry_positive[i] - act_out_carry_negative[i];
			}
			else		// ���i����D
			{
				act_out_remain[i]	=	OCR_2_30 + act_out_remain_positive[i] - act_out_remain_negative[i];
				act_out_carry[i]	=	act_out_carry_positive[i] - act_out_carry_negative[i] - 1;		// �����ɦ�
			}
		}
		else if( act_out_carry_positive[i] < act_out_carry_negative[i] )
		{
			if( act_out_remain_negative[i] >= act_out_remain_positive[i] )		// �S���i����D
			{
				act_out_remain[i]	=	act_out_remain_negative[i] - act_out_remain_positive[i];
				act_out_carry[i]	=	act_out_carry_negative[i] - act_out_carry_positive[i];
			}
			else		// ���i����D
			{
				act_out_remain[i]	=	OCR_2_30 + act_out_remain_negative[i] - act_out_carry_positive[i];
				act_out_carry[i]	=	act_out_carry_negative[i] - act_out_carry_positive[i] - 1;		// �����ɦ�
			}
			// �����ܦ��t��
			act_out_remain[i]	=	-act_out_remain[i];
			act_out_carry[i]	=	-act_out_carry[i];
		}
		else
		{
			act_out_carry[i]	=	0;
			act_out_remain[i]	=	act_out_remain_positive[i] - act_out_remain_negative[i];
		}
	}
#endif



#ifdef _OCR_USE_FLOAT_
	for( i = 0; i < nr_class; i++ )
	{
		reorder_output[i]	=	act_out[i];
		reorder_label[i]	=	O_label[i];
	}
	//find largest three output
	for( i = 0; i < nr_class; i++ )			//�Ƨ�
		for( m = 0; m < (nr_class-i-1); m++ )
		{
			if( reorder_output[m] < reorder_output[m+1] )
			{
				float temp1			=	reorder_output[m];
				reorder_output[m]	=	reorder_output[m+1];
				reorder_output[m+1]	=	temp1;
						
				//int temp_label1=reorder_label[m];
				temp_label_1	=	reorder_label[m];

				reorder_label[m]	=	reorder_label[m+1];
				reorder_label[m+1]	=	temp_label_1;
			}
		}	
#else
	// �j�M�e n �� �̤j��
	for( i = 0; i < nr_class; i++ )
		reorder_label[i]	=	O_label[i];
	            /* 5 �i�H��define �o��ثe�u�ݭn��X�e���j�N�n */
	for( i = 0; i < 5; i++ )
	{
		act_out_max_carry	=	act_out_carry[ i ];
		act_out_max_remain	=	act_out_remain[ i ];
		act_out_max_id		=	i;

		for( j = 1 + i; j < nr_class; j++ )		// ��Ʀn����ƥ��e i �Ӧ�m  �q 1 �}�l�O�]�� 0+i����b max
		{
			if( act_out_carry[j] > act_out_max_carry )
			{
				act_out_max_carry	=	act_out_carry[ j ];
				act_out_max_remain	=	act_out_remain[ j ];
				act_out_max_id		=	j;
			}
			else if( act_out_carry[j] == act_out_max_carry && act_out_remain[j] > act_out_max_remain )
			{
				act_out_max_carry	=	act_out_carry[ j ];
				act_out_max_remain	=	act_out_remain[ j ];
				act_out_max_id		=	j;
			}
		}

		// �O���� reorder_output, reorder_label  (�o��]�|�N��ƥ洫)
		tmp									=	reorder_label[ i ];
		reorder_label[i]					=	reorder_label[ act_out_max_id ];
		reorder_label[ act_out_max_id ]		=	tmp;

		// �� 0 -1 1  (�쥻�O�θ�ƪ�����i�h ���O�o���ƥi��|overflow, �ҥH��γo�ا@�k)
		if( act_out_carry[ act_out_max_id ] < 0 || act_out_remain[ act_out_max_id ] < 0 )
			reorder_output[i]	=	-1;
		else if( act_out_carry[ act_out_max_id ] > 0 || act_out_remain[ act_out_max_id ] > 0 )
			reorder_output[i]	=	1;
		else
			reorder_output[i]	=	0;

		// �N��ƥ洫
		tmp		=	act_out_remain[ act_out_max_id ];
		stmp	=	act_out_carry[ act_out_max_id ];

		act_out_remain[ act_out_max_id ]	=	act_out_remain[ i ];
		act_out_carry[ act_out_max_id ]		=	act_out_carry[ i ];
		
		act_out_remain[ i ]		=	tmp;
		act_out_carry[ i ]		=	stmp;

	}

#endif

	for( i = 0; i < nr_class; i++ )
		_t_output[i]	=	0;

	if(reorder_output[0]<0)////svm��X�Ҥp��0
		{	
			for( i=0; i<5; i++)
			{
				_t_output[i]	=	OCR_Correlation(source_matching, template_original, reorder_label[0], reorder_label[i], V_Sample_Num*H_Sample_Num, OCR_MASK_WIDTH*OCR_MASK_HEIGHT);
			}
				
			order_t_output	=	_t_output[0];
			flag_t_output	=	0;
			
			for( i=0; i<5; i++)
			{
				if(order_t_output< _t_output[i])
				{
					order_t_output = _t_output[i];
					flag_t_output = i;
				}
			}	
			reorder_label[0]=reorder_label[flag_t_output];
		}
		else if(reorder_output[0]>0 && reorder_output[1]>0 && reorder_output[2]<0)		////svm��ӿ�X�j��0
		{
			for( i=0; i<2; i++)
			{
				_t_output[i]	=	OCR_Correlation(source_matching, template_original, reorder_label[0], reorder_label[i], V_Sample_Num*H_Sample_Num, OCR_MASK_WIDTH*OCR_MASK_HEIGHT);
			}
			
			order_t_output	=	_t_output[0];
			flag_t_output	=	0;
			for( i=0; i<2; i++)
			{
				if(order_t_output< _t_output[i])
				{
					order_t_output = _t_output[i];
					flag_t_output = i;
				}
			}
					
			reorder_label[0]=reorder_label[flag_t_output];
		}
		else if(reorder_output[0]>0 && reorder_output[1]>0 && reorder_output[2]>0)		////�e�T��svm��X�Ҥj��0
		{
			for( i=0; i<3; i++)
			{
				_t_output[i]	=	OCR_Correlation(source_matching, template_original, reorder_label[0], reorder_label[i], V_Sample_Num*H_Sample_Num, OCR_MASK_WIDTH*OCR_MASK_HEIGHT);
			}
				
			order_t_output	=	_t_output[0];
			flag_t_output	=	0;
			for( i=0; i<3; i++)
			{
				if(order_t_output< _t_output[i])
				{
					order_t_output = _t_output[i];
					flag_t_output = i;
				}
			}
				
			reorder_label[0]=reorder_label[flag_t_output];
		}

	// ���o���G
	classifier_output	=	reorder_label[0];
	
	return classifier_output;
}
#endif





/******************************************************************************
	����O����
*******************************************************************************/
void	OCR_free()
{
	free(Omega);		Omega	=	NULL;
	free(O_label);		O_label	=	NULL;
}




// �p�� binary�Ϫ�feature
void	OCR_bin_feature( BYTE *Segmental_image_Binary, short *Sample, int *BZ_Feature )
{
	int		BZ_h_shift,	BZ_v_shift;
	int		i,	k;

	////�p��BZF����pixel��**************************************************
	BZ_h_shift	=	(int)(( Sub_width - OCR_MASK_WIDTH )/(BZ_H_Sample_Num-1));
	BZ_v_shift	=	(int)(( Sub_height - OCR_MASK_HEIGHT )/(BZ_V_Sample_Num-1));
	////********************************************************************

	
	////���Ψ�BZF�S�x*******************************************************
	for( i = 0; i < BZ_H_Sample_Num; i++)
		for( k = 0; k < BZ_V_Sample_Num; k++)
		{
			////Get Binary Sample
			OCR_Sampling( Segmental_image_Binary, Sample, BZ_h_shift, BZ_v_shift, i, k,  Sub_width, Sub_height );		
			// �N Sample_Binary ��buffer  �]���o��i�H����

			////Binary zone feature
			OCR_Pre_BZF(Sample, BZ_Feature, OCR_MASK_WIDTH, k*BZ_H_Sample_Num + i );
			// BZ_Feature ��Buffer 2  (�n���@���u�g�J�@�Ӧ�m)
		}
	////calculate BZF feature
	OCR_BZF(BZ_Feature, BZ_H_Sample_Num , BZ_V_Sample_Num);
	////*********************************************************************
}




// �@�몺�S�x
void	OCR_feature( BYTE *Segmental_image, short *Sample, short *source_matching, int *Feature, int  *BZ_Feature  )
{


	int		F_DWT_image[256]	=	{0},	S_DWT_image[64]		=	{0};
	int		F_DWT_LL[64]		=	{0},	F_DWT_LH[64]		=	{0},	F_DWT_HL[64]	=	{0},	F_DWT_HH[64]	=	{0};
	int		S_DWT_LL[16]		=	{0},	S_DWT_LH[16]		=	{0},	S_DWT_HL[16]	=	{0},	S_DWT_HH[16]	=	{0};
	int		HOG_Feature[9]		=	{0},	S_LSF_Feature[16]	=	{0};

	int		h_shift,	v_shift;												  
	int		attribute;					
	int		i,	k,	q,	m;
																				  
	////�p��HOG-W�PLSF����pixel��********************************************
	//int h_shift, v_shift = 0;
	h_shift	=	(int)((Sub_width- OCR_MASK_WIDTH )/(H_Sample_Num-1));
	v_shift	=	(int)((Sub_height- OCR_MASK_HEIGHT )/(V_Sample_Num-1));
	////*********************************************************************

	////���Ψ�HOG-W�PLSF�S�x�æs�JFeature�x�}********************************
	//int attribute =0;
	attribute	=	0;
	for( i=0; i<H_Sample_Num; i++)
	{
		for( k=0; k<V_Sample_Num; k++)
		{
			////Get Sample
			OCR_Sampling( Segmental_image, Sample, h_shift, v_shift, i, k, Sub_width, Sub_height );
			// Sample �i�H��buffer (�򤧫e��Sample_Binary�@��?)

			////FIRST ORDER DWT
			OCR_Haar_DWT(Sample, F_DWT_image, OCR_MASK_WIDTH, OCR_MASK_HEIGHT);

			OCR_Calcu_DWT_L_H(F_DWT_image, F_DWT_LL, F_DWT_LH, F_DWT_HL, F_DWT_HH, OCR_MASK_WIDTH, OCR_MASK_HEIGHT);

			////HOG feature
			OCR_HOG(F_DWT_HL, F_DWT_LH, HOG_Feature, OCR_MASK_WIDTH/2, OCR_MASK_HEIGHT/2);

			////SECOND ORDER DWT
			OCR_Haar_DWT_2( F_DWT_LL, S_DWT_image, OCR_MASK_WIDTH/2, OCR_MASK_HEIGHT/2);

			OCR_Calcu_DWT_L_H(S_DWT_image, S_DWT_LL, S_DWT_HL, S_DWT_LH, S_DWT_HH, OCR_MASK_WIDTH/2, OCR_MASK_HEIGHT/2);

			////LSF feature
			OCR_LSF(S_DWT_HL, S_DWT_LH, S_DWT_HH, S_LSF_Feature, OCR_MASK_WIDTH/4, OCR_MASK_HEIGHT/4);
				
			for( q=0; q<OCR_MASK_WIDTH*OCR_MASK_HEIGHT; q++)
				source_matching[((attribute)*OCR_MASK_WIDTH*OCR_MASK_HEIGHT) + q]	=	Sample[q];			

			////Test Text Feature
			for( m=0; m<9; m++)
				Feature[attribute*25+m] = HOG_Feature[m];

			for( m=0; m<OCR_MASK_WIDTH; m++)
				Feature[9+attribute*25+m] = S_LSF_Feature[m];

			attribute++;
		}
	}//16*16 sampling end

	
	////�NBZF�S�x�s�JFeature�x�}��*******************************************
	for( m = 0; m<BZ_V_Sample_Num*BZ_H_Sample_Num+BZ_V_Sample_Num+BZ_H_Sample_Num; m++)
		Feature[25*(V_Sample_Num+H_Sample_Num)+m] = BZ_Feature[m];
		// �i�H�h��e�� �M������BZ_Feature
	////*********************************************************************


}


// �B�zact_out  (�ݭn�B�zoverflow ��j�p)
void	OCR_act_out( int *Feature, int *reorder_label, short *reorder_output )
{
	int		i,	j,	tmp;
	short	stmp;

	short	act_out_carry_positive[35]		=	{0};		// �i�쳡�� ��
	int		act_out_remain_positive[35]		=	{0};		// �ݾl��
	short	act_out_carry_negative[35]		=	{0};		// �i�쳡�� �t
	int		act_out_remain_negative[35]		=	{0};		// �ݾl��
	short	act_out_carry[35]				=	{0};		// �̫ᵲ�G
	int		act_out_remain[35]				=	{0};		

	short	act_out_max_carry;		// �Ƨǥθ��
	int		act_out_max_remain;
	int		act_out_max_id;

	// �p�� act_out   �n�Ҽ{overflow���D
	for( i = 0; i < nr_class; i++ )
	{
		for( j = 0; j < nr_feature; j++)
		{
			tmp		=		*(Omega + nr_feature*i + j ) * Feature[j];

			if( tmp >= 0 )
			{
				if( tmp > OCR_2_30 )
				{
					tmp		-=	OCR_2_30;
					act_out_carry_positive[i]++;
				}

				act_out_remain_positive[i]	+=	tmp;

				if( act_out_remain_positive[i] > OCR_2_30 )
				{
					act_out_remain_positive[i]	-=	OCR_2_30;
					act_out_carry_positive[i]++;
				}
			}
			else if( tmp < 0 )
			{
				tmp		=	-tmp;		// ���Υ��ƥh�B�z
				if( tmp > OCR_2_30 )
				{
					tmp		-=	OCR_2_30;
					act_out_carry_negative[i]++;
				}

				act_out_remain_negative[i]	+=	tmp;

				if( act_out_remain_negative[i] > OCR_2_30 )
				{
					act_out_remain_negative[i]	-=	OCR_2_30;
					act_out_carry_negative[i]++;
				}	
			}
		}
	}

	// ���t�X��
	for( i = 0; i < nr_class; i++ )
	{
		// �`�N��ƪ�ܪk  �o�� carry remain �����O���� (�̫��ഫ���ɭԦA�令�t��)
		// ����carry
		if( act_out_carry_positive[i] > act_out_carry_negative[i] )
		{
			if( act_out_remain_positive[i] >= act_out_remain_negative[i] )		// �S���i����D
			{
				act_out_remain[i]	=	act_out_remain_positive[i] - act_out_remain_negative[i];
				act_out_carry[i]	=	act_out_carry_positive[i] - act_out_carry_negative[i];
			}
			else		// ���i����D
			{
				act_out_remain[i]	=	OCR_2_30 + act_out_remain_positive[i] - act_out_remain_negative[i];
				act_out_carry[i]	=	act_out_carry_positive[i] - act_out_carry_negative[i] - 1;		// �����ɦ�
			}
		}
		else if( act_out_carry_positive[i] < act_out_carry_negative[i] )
		{
			if( act_out_remain_negative[i] >= act_out_remain_positive[i] )		// �S���i����D
			{
				act_out_remain[i]	=	act_out_remain_negative[i] - act_out_remain_positive[i];
				act_out_carry[i]	=	act_out_carry_negative[i] - act_out_carry_positive[i];
			}
			else		// ���i����D
			{
				act_out_remain[i]	=	OCR_2_30 + act_out_remain_negative[i] - act_out_carry_positive[i];
				act_out_carry[i]	=	act_out_carry_negative[i] - act_out_carry_positive[i] - 1;		// �����ɦ�
			}
			// �����ܦ��t��
			act_out_remain[i]	=	-act_out_remain[i];
			act_out_carry[i]	=	-act_out_carry[i];
		}
		else
		{
			act_out_carry[i]	=	0;
			act_out_remain[i]	=	act_out_remain_positive[i] - act_out_remain_negative[i];
		}
	}

	// �j�M�e n �� �̤j��
	for( i = 0; i < nr_class; i++ )
		reorder_label[i]	=	O_label[i];
	            /* 5 �i�H��define �o��ثe�u�ݭn��X�e���j�N�n */
	for( i = 0; i < 5; i++ )
	{
		act_out_max_carry	=	act_out_carry[ i ];
		act_out_max_remain	=	act_out_remain[ i ];
		act_out_max_id		=	i;

		for( j = 1 + i; j < nr_class; j++ )		// ��Ʀn����ƥ��e i �Ӧ�m  �q 1 �}�l�O�]�� 0+i����b max
		{
			if( act_out_carry[j] > act_out_max_carry )
			{
				act_out_max_carry	=	act_out_carry[ j ];
				act_out_max_remain	=	act_out_remain[ j ];
				act_out_max_id		=	j;
			}
			else if( act_out_carry[j] == act_out_max_carry && act_out_remain[j] > act_out_max_remain )
			{
				act_out_max_carry	=	act_out_carry[ j ];
				act_out_max_remain	=	act_out_remain[ j ];
				act_out_max_id		=	j;
			}
		}

		// �O���� reorder_output, reorder_label  (�o��]�|�N��ƥ洫)
		tmp									=	reorder_label[ i ];
		reorder_label[i]					=	reorder_label[ act_out_max_id ];
		reorder_label[ act_out_max_id ]		=	tmp;

		// �� 0 -1 1  (�쥻�O�θ�ƪ�����i�h ���O�o���ƥi��|overflow, �ҥH��γo�ا@�k)
		if( act_out_carry[ act_out_max_id ] < 0 || act_out_remain[ act_out_max_id ] < 0 )
			reorder_output[i]	=	-1;
		else if( act_out_carry[ act_out_max_id ] > 0 || act_out_remain[ act_out_max_id ] > 0 )
			reorder_output[i]	=	1;
		else
			reorder_output[i]	=	0;

		// �N��ƥ洫
		tmp		=	act_out_remain[ act_out_max_id ];
		stmp	=	act_out_carry[ act_out_max_id ];

		act_out_remain[ act_out_max_id ]	=	act_out_remain[ i ];
		act_out_carry[ act_out_max_id ]		=	act_out_carry[ i ];
		
		act_out_remain[ i ]		=	tmp;
		act_out_carry[ i ]		=	stmp;

	}

}


// �˪O��� 
void	OCR_template( int *reorder_label, short *reorder_output, short *source_matching )
{
	int		i;

	float	_t_output[35]	=	{0};
	float	order_t_output;
	int		flag_t_output;

	


	for( i = 0; i < nr_class; i++ )
		_t_output[i]	=	0;

	////��CNY_20 MODE�� ���� T ��TEMPLATE MATCHING*****************************************
	if( ((init_type == OCR_CNY_20) && (reorder_label[0] != 20)) || (init_type == OCR_CNY_100))
	{
		if(reorder_output[0]<0)////svm��X�Ҥp��0
			{	
				for( i=0; i<5; i++)
				{
					_t_output[i]	=	OCR_Correlation(source_matching, template_original, reorder_label[0], reorder_label[i], V_Sample_Num*H_Sample_Num, OCR_MASK_WIDTH*OCR_MASK_HEIGHT);
				}
					
				order_t_output	=	_t_output[0];
				flag_t_output	=	0;
				
				for( i=0; i<5; i++)
				{
					if(order_t_output< _t_output[i])
					{
						order_t_output = _t_output[i];
						flag_t_output = i;
					}
				}	
				reorder_label[0]=reorder_label[flag_t_output];
			}
			else if(reorder_output[0]>0 && reorder_output[1]>0 && reorder_output[2]<0)		////svm��ӿ�X�j��0
			{
				for( i=0; i<2; i++)
				{
					_t_output[i]	=	OCR_Correlation(source_matching, template_original, reorder_label[0], reorder_label[i], V_Sample_Num*H_Sample_Num, OCR_MASK_WIDTH*OCR_MASK_HEIGHT);
				}
				
				order_t_output	=	_t_output[0];
				flag_t_output	=	0;
				for( i=0; i<2; i++)
				{
					if(order_t_output< _t_output[i])
					{
						order_t_output = _t_output[i];
						flag_t_output = i;
					}
				}
						
				reorder_label[0]=reorder_label[flag_t_output];
			}
			else if(reorder_output[0]>0 && reorder_output[1]>0 && reorder_output[2]>0)		////�e�T��svm��X�Ҥj��0
			{
				for( i=0; i<3; i++)
				{
					_t_output[i]	=	OCR_Correlation(source_matching, template_original, reorder_label[0], reorder_label[i], V_Sample_Num*H_Sample_Num, OCR_MASK_WIDTH*OCR_MASK_HEIGHT);
				}
					
				order_t_output	=	_t_output[0];
				flag_t_output	=	0;
				for( i=0; i<3; i++)
				{
					if(order_t_output< _t_output[i])
					{
						order_t_output = _t_output[i];
						flag_t_output = i;
					}
				}
					
				reorder_label[0]=reorder_label[flag_t_output];
			}
	}
}



#if 1
/******************************************************************************
	OCR ��r���� ���鳡��
*******************************************************************************/
int		OCR_Recognize( BYTE *Segmental_image_Binary, BYTE *Segmental_image )
{
	short	Sample[256]	=	{0};

	short	reorder_output[35]	=	{0};		// ���ӥi�H��signed char  ���O����short  �u�s  -1 0 1 �T�ص��G
	int		reorder_label[35]	=	{0};

	int		classifier_output;

	int		Feature[129]	=	{0};
	int		BZ_Feature[29]		=	{0};
	short	source_matching[1024]	=	{0};

	// binary �S�x
	OCR_bin_feature( Segmental_image_Binary, Sample, BZ_Feature );

	// ���q�S�x 
	OCR_feature( Segmental_image, Sample, source_matching,Feature ,BZ_Feature );
	
	// �p�� act_out
	OCR_act_out( Feature, reorder_label, reorder_output );

	// �˪O���
	OCR_template( reorder_label, reorder_output, source_matching );

	// ���o���G
	classifier_output	=	reorder_label[0];
	
	return classifier_output;
}
#endif
