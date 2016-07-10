#ifndef _COCCM_H
#define _COCCM_H

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	texture
	co-occurence matrix
	C  OCC       M
	�ΨӰ��έp ����
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include <QImage>
#include <boost/numeric/ublas/matrix.hpp>
#include "../tools.h"
#include <fstream>
#include <omp.h>
#include <cmath>
#include <vector>
#include "mrfFeatureAbstract.h"
#include <assert.h>



// --------------------------------------------------------------- co-occurence ���� -----------------------------------------------------------------------------
class	COCCM : public mrfFeatureAbstract
{
	private:

		
		//omp_lock_t		writelock;

		static const int	DIMENSION	=	1;
		static const int	vec_size	=	10;										// �V�q�q (0,0) ~ (10,10)
		static const int	sl_size		=	15;										// sliding windows size  (�]���o��|����� windows size)

		std::vector<PIXEL>	*A;														// �Ȧs�� (���F�[�t �NPd����ư��ഫ��b�o��)  ��Ƶ��c��PIXEL�O���F��K  �W�٤W�N�q����   
		bMatrixDouble		map;													// �s��p��n���S�x

		// virtual ��
		void				cal_mean();												// �p�⥭���� �o��ӥ�����cal_feature�өI�s
		void				cal_devia();											// �p��зǮt

	public:
		COCCM();
		COCCM( QImage imagePass );
		COCCM( const COCCM &cc );													// copy construct
		~COCCM();

		// virtual ��
		void				segmentation( QImage img );								// ���դ���
		void				cal_feature( QImage img );								// �p��feature
		void				output_txt();											// ��X�ɮ�
		double				get_norm( int pix );									// �p�� |u|
		double				get_norm( int pix1, int pix2 );							// �p����pixel��norm |u-v|
		MRF_FEATURE_TYPE	type();													// �^�ǥثe��feature type
		int					dim();													// �^�Ǻ���
		double				get_norm_squ( int pix );								// �^�� norm ������
		double				get_devia_det();										// �^��deviation��determine  (�]����Ӫ��פ�O�[�W |Sigma| ���T�w�Onorm�٬Odet)
		void				get_feature( int pix, double *_f );						// �^�Ǹ�pixel��feature
		bool				check_dim();											// �ˬd����
		void				cal_sample_feature( QImage img );						// �p��˱i ��cal_feature���e�|�ܹ� �t�b�o��|�⥭���зǮt



		void			cal_Pd( PIXEL d_vector, int x, int y, bMatrixInt &Pd );		// �p��Pd�x�}
		double				Energy( bMatrixInt Pd );				// �p���q
		double				Entropy( bMatrixInt Pd );								// �p���i
		double				Contrast( bMatrixInt Pd );								// �p���� 
		double				Homogeneity( bMatrixInt Pd );							// �p�⧡�ë�
		//double			Correlation( bMatrixInt Pd )


		void				output();												// ��X���ɮ�
		double				get_feature_pix( int x, int y );						// �p����I�� feature
		void				pre_Pd_data( int vec );									// �w�����ͦV�q��� 
		void				cal_feature_abc( QImage img );

		void				test();


		// operator = 
		void	operator	=	( const COCCM &cc );

};


#endif