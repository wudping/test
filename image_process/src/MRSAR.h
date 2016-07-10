#ifndef _MRSAR_H
#define _MRSAR_H


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
#include <cmath>
#include <vector>
#include "mrfFeatureAbstract.h"
#include <assert.h>
#include "../MRSAR/mr_sar.h"


// --------------------------------------------------------------- co-occurence ���� -----------------------------------------------------------------------------
class	MRSAR : public mrfFeatureAbstract
{
	private:
		static const int	DIMENSION	=	15;										
   
		std::vector<bMatrixDouble>		map;													// �s��p��n���S�x

		// virtual ��
		void				cal_mean();												// �p�⥭���� �o��ӥ�����cal_feature�өI�s
		void				cal_devia();											// �p��зǮt

	public:
		MRSAR();
		MRSAR( QImage imagePass );
		MRSAR( const MRSAR &mr );													// copy construct
		~MRSAR();

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

		// operator = 
		void	operator	=	( const MRSAR &mr );

		void	output();
};



#endif