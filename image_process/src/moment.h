#ifndef _MOMENT_H
#define _MOMENT_H



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	texture
	moment
	�ثe�ϥ� m00 m01 m10 m02 m11 m20   �����V�q
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/




#include "mrfFeatureAbstract.h"

#include "../tools.h"
#include <assert.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <stdio.h>


// --------------------------------------------------------------- moment ���� -----------------------------------------------------------------------------
class	Moment : public mrfFeatureAbstract
{
	private:	
		static const int			DIMENSION	=	6;					// m00 m01 m10 m02 m11 m20 �@����
		static const int			win_size	=	25;					// sliding window size

		std::vector<bMatrixDouble>	moment;								// �s��moment���  (vector��e�� �קK�į���D)  

		// virtual ��
		void				cal_mean();									// �p�⥭���� �o��ӥ�����feature�өI�s
		void				cal_devia();								// �p��зǮt

	public:
		Moment();														// �غc
		Moment( QImage imagePass );										// �غc
		Moment( const Moment &mt );										// copy construct
		~Moment();

		// virtual ��
		void				segmentation( QImage img );					// ���դ���
		void				cal_feature( QImage img );					// �p��feature �P�ɷ|�⥭���� �зǮt
		void				output_txt();								// ��X�ɮ�
		double				get_norm( int pix );						// �p�� |u|
		double				get_norm( int pix1, int pix2 );				// �p����pixel��norm |u-v|
		MRF_FEATURE_TYPE	type();										// �^�ǥثe��feature type
		int					dim();										// �^�Ǻ���
		double				get_norm_squ( int pix );					// �^�� norm ������
		double				get_devia_det();							// �^��deviation��determine  (�]����Ӫ��פ�O�[�W |Sigma| ���T�w�Onorm�٬Odet)
		void				set_mean( sVectorDouble _m );				// �]�w������
		sVectorDouble		get_mean();									// �^�ǥ�����
		void				set_devia( sVectorDouble _d );				// �]�w�зǮt
		sVectorDouble		get_devia();								// �^�ǼзǮt
		void				get_feature( int pix, double *_f );			// �^�Ǹ�pixel��feature
		bool				check_dim();								// �ˬd����
		void				cal_sample_feature( QImage img );			// �p��˱i ��cal_feature���e�|�ܹ� �t�b�o��|�⥭���зǮt

		// �Dvirtual ��
		void				cal_moment( int p, int q );					// �p�� moment p q
		int					MomentStep( int p, int q );					// �N p, q �ন Moment[i] ���ഫ����

		double				get_data( int i, int j );					// ���զ^�Ǹ��

		// operator = 
		void	operator	=	( const Moment &mt );
		
};




#endif