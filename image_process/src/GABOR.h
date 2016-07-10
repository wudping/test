#ifndef _GABOR_H
#define _GABOR_H




/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	texture
	Gabor
	�Q��sin, cos, exp����filter
	�z�L��filter�ӵѨ�feature
	����feature���]�p�٦��Q�תŶ�
	(���ժ����S����feature���p��h���վ�  �u����B�F�ѯS�� �t�׹L�C)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/



#include <QImage>
#include "../tools.h"
#include "mrfFeatureAbstract.h"
#include <iostream>
#include <fstream>
#include <assert.h>
#include <boost/numeric/ublas/matrix.hpp>





// --------------------------------------------------------------- Gabor ���� -----------------------------------------------------------------------------
class	Gabor : public mrfFeatureAbstract
{
	private:
		static const int	DIMENSION	=	1;

		static const int	win_size	=	10;								// sliding windows size
		double				sigma;											// Gauss function �Ѽ�									
		double				wave;											// �ݭnwave, theta�o��ӰѼ�  (�D�@�����зǮt�̤p���զX)		
		double				theta;				

		bMatrixDouble		map;											// �N�g�LGabor filter����Ʀs�b�o��
		bMatrixDouble		map_tmp;										// �Ȧs�� map
		double				mean_tmp;										// �Ȧs�� mean
		double				devia_tmp;										// �Ȧs�� devia
		double				max_tmp;										// �Ȧs�γ̤j��

		// virtual ��
		void				cal_mean();										// �p�⥭���� �o��ӥ�����feature�өI�s
		void				cal_devia();									// �p��зǮt

	public:
		Gabor();															// �غc �L�ǤJ��
		Gabor( QImage imagePass );
		Gabor( const Gabor &gb );											// copy construct
		~Gabor();															// �Ѻc

		// virtual ��
		void				segmentation( QImage img );						// ���դ���
		void				cal_feature( QImage img );						// �p��feature
		void				output_txt();									// ��X�ɮ�
		double				get_norm( int pix );							// �p�� |u|
		double				get_norm( int pix1, int pix2 );					// �p����pixel��norm |u-v|
		MRF_FEATURE_TYPE	type();											// �^�ǥثe��feature type
		int					dim();											// �^�Ǻ���
		double				get_norm_squ( int pix );						// �^�� norm ������
		double				get_devia_det();								// �^��deviation��determine  (�]����Ӫ��פ�O�[�W |Sigma| ���T�w�Onorm�٬Odet)
		void				get_feature( int pix, double *_f );				// �^�Ǹ�pixel��feature
		bool				check_dim();									// �ˬd����
		void				cal_sample_feature( QImage img );				// �p��˱i ��cal_feature���e�|�ܹ� �t�b�o��|�⥭���зǮt

		// �Dvirtual ��
		bMatrixDouble		RealPart( double wave, double theta );			// ����real part��filter
		bMatrixDouble		ImagePart( double wave, double theta );			// ����image part��filter
		void				cal_wh( QImage img );							// �p�� wave, theta ��ӰѼ�  (�b�o��|�P�ɭp��X�зǮt)
		void				cal_feature2( double _wave, double _theta );	// ���w wave, theta �h�p�� feature   �^�Ǫ��O�зǮt
		void				cal_max();										// �p��̤j�� (�����)

		// operator = 
		void	operator	=	( const Gabor &gb );

};



#endif