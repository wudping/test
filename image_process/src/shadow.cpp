#include "shadow.h"

//using namespace std;


//test
namespace	emboss{



/************************************************************************************
	�غc
/************************************************************************************/
Shadow::Shadow()
{
	size		=	0;
	sub_inx		=	0;
	link_inx	=	0;
}


/************************************************************************************
	�Ѻc
/************************************************************************************/
Shadow::~Shadow()
{
	size	=	0;
	delete [] sub_inx;
	delete [] link_inx;
}


/************************************************************************************
	�]�wsize
	�o�䪺_size����qsettingŪ�� 
	�]���O�ɭȫ᪺size
/************************************************************************************/
void	Shadow::set( int _size, const  ShadowSettings &setting )
{
	size		=	_size;				// �`�N�o�䪺size�O�ɭȫ᪺size,���O�쥻��size

	assert( size > 0 );

	sub_inx		=	new int[size];
	link_inx	=	new int[size];

	threshold	=	setting.threshold;
	enhance		=	setting.enhance;
	bump		=	setting.bump;

	dark_light	=	setting.dark_light;
}



/************************************************************************************
	�M���դl�ǦC
	�^�Ǥ@��vector, value = 1 ��ܸӤ����bsub seq��
	                value = 0 ��ܤ��bsub seq��.
/************************************************************************************/
void	Shadow::sub_mono_seq( andrea::vector<unsigned char> &seq, andrea::vector<unsigned char> &is_choose )
{
	// �]���O�䧹�~�ɭ� �ҥH�i�H�[�W�o�ӧP�_.
	assert( seq.size() == size && is_choose.size() == size );

	int		i,	j,	max;
	int		sub_find;
	int		find_inx;
	int		start_inx;		

	// ��l�Ƹ��  �o�䴿�g�]��size���~�y��bug
	memset( sub_inx  , 0, (size)*sizeof(int) );
	memset( link_inx , 0, (size)*sizeof(int) );

	// �M��̪�mono sub seq 
	// �t�ץi�H�i�@�B�i�B�� n*log(n) (�ثe = n^2)
	//max			=	0;
	//start_inx	=	0;
	for( i = size-1; i >= 0; i-- )
	{
		sub_find	=	0;
		find_inx	=	0; 
		
		// ���ۤ�V�� �t�׷|����� (���U>=���P�_�|�ܤ�)
		for( j = i+1;  j < size  ; j++ )
		{					
			/* �o�䪺>=�|�M�w���W����*/  
			if( sub_inx[j] >= sub_find  &&  seq[i] <= seq[j] )  
			{
				if( sub_inx[j] == sub_find  && seq[j] > seq[find_inx] )
				{
					sub_find	=	sub_inx[j];
					find_inx	=	j;					
				}
				else if( sub_inx[j] > sub_find   )
				{
					sub_find	=	sub_inx[j];
					find_inx	=	j;
				}
			}
			else if( sub_find > size-j )		// ��ܤw�g���i��������seq,�i�H���X.  �p�߳o�䲣�ͪ�bug
				break;
		}

		sub_inx[i]	=	sub_find + 1;
		link_inx[i]	=	find_inx;			// �N��Ʀ�_�� �إ�sub seq�ϥ�

		// �M��̤j��  �o��i�i�@�B�[�t (�]��max��᭱�Ѿl������)
		// �D�n�ت��O��X mono sub seq���_�I
		/*if( sub_inx[i] > max )
		{
			max			=	sub_inx[i];
			start_inx	=	i;
		}*/
	}

	// �M��̤j��  �o��i�i�@�B�[�t (�]��max��᭱�Ѿl������)
	// �D�n�ت��O��X mono sub seq���_�I
	max			=	0;
	start_inx	=	0;
	for( i = 0; i < size; i++ )
	{
		if( sub_inx[i] > max )
		{
			max			=	sub_inx[i];
			start_inx	=	i;
		}	
		else if( max > size-i )		// ��ܤw�g���i��������seq,�i�H���X.  �p�߳o�䲣�ͪ�bug
			break;
	}


	// �إ�list  �إ�mono sub seq����
	is_choose.clear();
	i	=	start_inx;
	do{
		assert( i >= 0 && i < size );
		is_choose(i)	=	1;
		i				=	link_inx[i];		// next step
	}while( i > 0 );	//  0 ��ܨS���U�@��

}



/************************************************************************************
	�M���դl�ǦC
	�^�Ǥ@��vector, value = 1 ��ܸӤ����bsub seq��
	                value = 0 ��ܤ��bsub seq��.

	�[�t�� �קK���ƨϥΰO����

	��ڤW�O�p�󵥩� ��W�r���P
/************************************************************************************/
void	Shadow::sub_mono_seq_less( bVectorByte &seq, bVectorByte &is_choose )
{
	assert( is_choose.size() == size && seq.size() == size);

	int				i,	j,	max;
	int				sub_find;
	int				find_inx;
	int				start_inx;	

	memset( sub_inx  , 0, (size)*sizeof(int) );
	memset( link_inx , 0, (size)*sizeof(int) );

	// �M��̪�mono sub seq 
	// �t�ץi�H�i�@�B�i�B�� n*log(n)
	max			=	0;
	start_inx	=	0;
	for( i = size-1; i >= 0; i-- )
	{
		sub_find	=	0;
		find_inx	=	0; 
		//for( j = size-1; j > i; j-- )
		for( j = i+1;  j < size  ; j++ )
		{					
			// �M��l�ǦC  �걵���ʧ@�b���U
			if( sub_inx[j] >= sub_find && seq[i] <= seq[j] )
			{
				if( sub_inx[j] == sub_find && seq(j) > seq(find_inx) )
				{
					sub_find	=	sub_inx[j];
					find_inx	=	j;					
				}
				else if( sub_inx[j] > sub_find )
				{
					sub_find	=	sub_inx[j];
					find_inx	=	j;
				}
			}
			else if( sub_find > size-j )		// ��ܤw�g���i��������seq,�i�H���X.  �p�߳o�䲣�ͪ�bug
				break;
		}

		sub_inx[i]	=	sub_find + 1;
		link_inx[i]	=	find_inx;			// �N��Ʀ�_�� �إ�sub seq�ϥ�

		// �M��̤j��  �o��i�i�@�B�[�t (�]��max��᭱�Ѿl������)
		// �D�n�ت��O��X mono sub seq���_�I
		if( sub_inx[i] > max )
		{
			max			=	sub_inx[i];
			start_inx	=	i;
		}
	}

	// �إ�list  �إ�mono sub seq����
	is_choose.clear();
	i	=	start_inx;
	do{
		assert( i < size );
		is_choose(i)	=	SHADOW_VALUE;
		i				=	link_inx[i];		// next step
	}while( i > 0 );	//  0 ��ܨS���U�@��
}





/************************************************************************************
	�M���դl�ǦC
	�^�Ǥ@��vector, value = 1 ��ܸӤ����bsub seq��
	                value = 0 ��ܤ��bsub seq��.

	�[�t�� �קK���ƨϥΰO����

	��ڤW�O�j�󵥩� ��W�r���P
/************************************************************************************/
void	Shadow::sub_mono_seq_greater( bVectorByte &seq, bVectorByte &is_choose )
{
	assert( is_choose.size() == size && seq.size() == size );

	int				i,	j,	max;
	int				sub_find;
	int				find_inx;
	int				start_inx;	

	memset( sub_inx  , 0, (size)*sizeof(int) );
	memset( link_inx , 0, (size)*sizeof(int) );

	// �M��̪�mono sub seq 
	// �t�ץi�H�i�@�B�i�B�� n*log(n)
	max			=	0;
	start_inx	=	0;
	for( i = size-1; i >= 0; i-- )
	{
		sub_find	=	0;
		find_inx	=	0; 
		//for( j = size-1; j > i; j-- )
		for( j = i+1;  j < size  ; j++ )
		{					
			// �M��l�ǦC  �걵���ʧ@�b���U  ��less�u�t�b�o��
			if( sub_inx[j] >= sub_find && seq[i] >= seq[j] ) 
			{
				if( sub_inx[j] == sub_find && seq(j) > seq(find_inx) )
				{
					sub_find	=	sub_inx[j];
					find_inx	=	j;					
				}
				else if( sub_inx[j] > sub_find )
				{
					sub_find	=	sub_inx[j];
					find_inx	=	j;
				}
			}
			else if( sub_find > size-j )		// ��ܤw�g���i��������seq,�i�H���X.  �p�߳o�䲣�ͪ�bug
				break;
		}

		sub_inx[i]	=	sub_find + 1;
		link_inx[i]	=	find_inx;			// �N��Ʀ�_�� �إ�sub seq�ϥ�

		// �M��̤j��  �o��i�i�@�B�[�t (�]��max��᭱�Ѿl������)
		// �D�n�ت��O��X mono sub seq���_�I
		if( sub_inx[i] > max )
		{
			max			=	sub_inx[i];
			start_inx	=	i;
		}
	}

	// �إ�list  �إ�mono sub seq����
	is_choose.clear();
	i	=	start_inx;
	do{
		is_choose(i)	=	SHADOW_VALUE;
		i				=	link_inx[i];		// next step
	}while( i > 0 );	//  0 ��ܨS���U�@��
}




/************************************************************************************
	�����˴�
	seq:        ���
	settings:   �U�ذѼƳ]�w
	�o��Q�ΦP�ɰ����W ����l�ǦC �A���涰

	�N���p�Ϥ����|��

	1. �G�v �W    ���涰  ����
	2. �t�v �Y    ���涰  ���W
	3. �G�v �W    ���p��  ����
	4. �t�v �W    ���p��  ����

	�G�t�M�w���W�λ���
	�o��u�����V �ѤW�@�h�h�P�_�H�W�|�I

	�ɭȤ]�浹�W�@�h�P�_

/************************************************************************************/
void	Shadow::screen_detector( bVectorByte &seq,  ShadowReturnData &return_data, int th_id )
{

	int					size		=	seq.size();
	int					i,	j,	i1,	i2;
	double				mag;

	bVectorByte			&is_choose		=	return_data.mono_sub_seq;
	bVectorByte			&shadow_list	=	return_data.shadow_seq;

	assert( is_choose.size() == size && shadow_list.size() == size );

	if( bump == PROTRUDING )		// �Y
		sub_mono_seq_less( seq, is_choose );
		//sub_mono_seq<unsigned char>( seq, less_than_or_equal<unsigned char>, is_choose, sub_inx, link_inx );
	else if( bump == CONCAVE )		// �W
		sub_mono_seq_greater( seq, is_choose );
		//sub_mono_seq<unsigned char>( seq, greater_than_or_equal<unsigned char>, is_choose, sub_inx, link_inx );
	else
		assert(false);

	// ���p�̫�@�Ӹ�ƨS�Q�襤,�h�ݭn�ɭ�  
	// ���ӷQ�ٲ� �̫�o�{����...orz
	if( is_choose(size-1) == 0 )
	{
		// �j�M�̫�@�ӳQ��쪺���
		for( i = size-1; i >= 0; i-- )
		{
			if( is_choose(i) != 0 )
				break;
		}
		seq.add_value( seq(i) );
		is_choose.add_value(1);
		size	=	seq.size();	// size = size + 1
	}	


	// �}�l�j�M�G�t�v
	shadow_list.clear();
	i1	=	1;
	i2	=	2;
	while(1)
	{
		// �s�W���������
		if( i1 >= size )
			break;

		find_non_sub_seq_interval( i1, i2, is_choose );		// �j�M�X�@�q���b  sub mono seq ���Ϭq 

		//if( i2 + 1 >= size )  // �פ����
		if( i2 >= size )		// �o�䦳�ק�L,�p��bug
			break;

		for( j = i1; j < i2 && j < size;  j++  )
		{
			if( dark_light == EMBOSS_DARK )					// �쳱�v
				mag		=	seq(i1-1) - seq(j);
			else if( dark_light == EMBOSS_LIGHT )			// ��G�v
				mag		=	seq(j) - seq(i1-1);
			else
				assert(false);

			// �W�Lthreshold����O���v
			// inhence = false, ��ܧ��q�����v
			if( mag > threshold )
				shadow_list(j)	=	SHADOW_VALUE;		
		}
		i1	=	i2 + 1;
	}	// end while

}




/************************************************************************************
	���v����
	seqo :      ��l�����
	seq:        �[�Wgauss blur�����
	settings:   �U�ذѼƳ]�w
/************************************************************************************/
void	Shadow::shadow_detector( bVectorByte &seq,  ShadowReturnData &return_data )
{
	int		size	=	seq.size();

	bVectorByte	 &is_choose		=	return_data.mono_sub_seq;		// ���W sub seq �� list.   1 ��ܦ��b sub seq ��  0 ��ܨS��
	bVectorByte	 &shadow_list	=	return_data.shadow_seq;			// ��X�Ӫ����v�϶����
	bVectorByte	 &depth			=	return_data.shadow_depth;

	assert( is_choose.size() == size && shadow_list.size() == size && depth.size() == size );

	// �i�h�仼�W�l�ǦC
	sub_mono_seq( seq, is_choose );

	int			i,		j;
	int			i1,		i2;
	int			max,	max_inx;
	double		mag;

	// ���p�̫�@�Ӹ�ƨS�Q�襤,�h�ݭn�ɭ�
	if( is_choose(size-1) == 0 )
	{
		// �j�M�̫�@�ӳQ��쪺���
		for( i = size-1; i >= 0; i-- )
		{
			if( is_choose(i) != 0 )
				break;
		}

		seq.add_value( seq(i) );
		is_choose.add_value( 1 );
		size	=	seq.size();	
	}	

	// ��threshold�C������O���v
	// �j�M�X�@��interval,  �åB���O�S�Q��쪺
	i1		=	1;		// ���]���v���|�X�{�b�Ĥ@��
	i2		=	2;
	max		=	0;
	max_inx	=	0;

	shadow_list.clear();							// ���]depth�S�ɭȫo�o��index�s�����~ ��ܦ��t��k�W�����D  �n�`�N   (�]�����ɭȪ���,�̫�@���I�@�w���|���i�h��)
	depth.clear();

	while(1)
	{
		// �s�W���������
		if( i1 >= size )
			break;

		find_non_sub_seq_interval( i1, i2, is_choose );		// �j�M�X�@�q���b  sub mono seq ���Ϭq 

		//if( i2 + 1 >= size )  // �פ����
		if( i2 >= size )		// �o�䦳�ק�L,�p��bug
			break;

		for( j = i1; j < i2 && j < size;  j++  )
		{
			//if( !settings.dark_light )			// �쳱�v
			if( dark_light == EMBOSS_DARK )			// �쳱�v
				mag		=	seq(i1-1) - seq(j);
			//else									// ��G�v
			else if( dark_light == EMBOSS_LIGHT )	// ��G�v
				mag		=	seq(j) - seq(i1-1);
			else
				assert(false);

			depth(j)	=	(int)mag;					// ���ޫ�� ���O���Ӭq�� sub mono seq ���Z��
	
			// �W�Lthreshold����O���v
			// inhence = false, ��ܧ��q�����v
			if( mag > threshold && !enhance )
				shadow_list(j)	=	SHADOW_VALUE;		

			// �����̲`����m
			if( mag > max )
			{
				max		=	(int)mag;
				max_inx	=	j;
			}
		}

		i1	=	i2 + 1;
	}

	// inhence = true, ��̲`�����v. �D�n�Φbnoise�񳱼v�ٱj�����p.
	if( enhance )
		shadow_list( max_inx )	=	SHADOW_VALUE;		
}






/************************************************************************************
	�D�X���Asub seq���϶�
/************************************************************************************/
void	Shadow::find_non_sub_seq_interval( int &i1, int &i2, bVectorByte &is_choose )
{
	const int	size	=	is_choose.size();		// �]���|�ɭ� ������~��.

	// ���Ҽ{�̫�@��
	while(  i1 < size-1  &&  is_choose(i1) == 1  )	i1++;
	
	i2	=	i1;
	while(  i2 < size && is_choose(i2) == 0  )		i2++;

	// ��ܨS�j�M��
	if( is_choose(i1) == 1 )
		i2	=	size-1;

	assert( i1 < size && i2 < size );
}






} // end namespace emboss