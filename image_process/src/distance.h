#ifndef _DISTANCE_H
#define _DISTANCE_H



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	texture
	distance
	�ϥ�image value�@���S�x 
	��²�檺�S�x
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/



#include "mrfFeatureAbstract.h"

#include "../tools.h"
#include <assert.h>
#include <iostream>
#include <cmath>
#include <stdio.h>



// --------------------------------------------------------------- distance ���� -----------------------------------------------------------------------------
class	Distance : public mrfFeatureAbstract
{
	private:	
		static const int	DIMENSION	=	1;
		int					anchor_x,									// ���˽d��
							anchor_y,
							anchor_width,
							anchor_height;

		// virtual ��
		void				cal_mean();									// �p�⥭���� �o��ӥ�����cal_feature�өI�s
		void				cal_devia();								// �p��зǮt

	public:
		Distance();
		Distance( QImage imagePass );
		Distance( const Distance &dt );									// copy construct
		~Distance();

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
		void				get_feature( int pix, double *_f );			// �^�Ǹ�pixel��feature
		bool				check_dim();								// �ˬd����
		void				cal_sample_feature( QImage img );			// �p��˱i ��cal_feature���e�|�ܹ� �t�b�o��|�⥭���зǮt


		// �Dvirtual ��
		void				set_anchor( int x, int y, int w, int h );	// �]�w���˽d��

		// operator = 
		void	operator	=	( const Distance &dt );
};




#endif



