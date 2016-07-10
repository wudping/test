#ifndef _MRF_ENERGY_ABSTRACT_H
#define _MRF_ENERGY_ABSTRACT_H

#include "mrfEnergyAbstract.h"
#include "mrfFeatureAbstract.h"
#include "moment.h"
#include "distance.h"
#include "Gabor.h"
#include "COCCM.h"
#include "MRSAR.h"
#include <QImage>
#include <assert.h>



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	��HEnergy���O
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/



enum	MRF_ENERGY_TYPE
{
	MRF_ENERGY_TEST,
	MRF_ENERGY_STANDARD,
};



class	mrfEnergyAbstract
{
	private:

	protected:
		mrfFeatureAbstract			*feature;
		QImage						image;

	public:
		mrfEnergyAbstract();																		// �غc
		mrfEnergyAbstract( mrfFeatureAbstract &ft, QImage img );									// �غc
		mrfEnergyAbstract( const mrfEnergyAbstract &e );											// copy construct
		virtual		~mrfEnergyAbstract();															// �Ѻc

		virtual double				E_data( int pix, int lab )							=	0;		// data Energy
		virtual double				E_smooth( int pix1, int pix2, int lab1, int lab2 )	=	0;		// smooth Energy 
		virtual double				Prob( int pix, int lab )							=	0;		// �^�Ǿ��v�K�ר�ƭ�
		virtual MRF_ENERGY_TYPE		type()												=	0;		// �^�Ǧۤv�O��������

		int							get_img_width();												// �^�ǹϤ��e
		int							get_img_height();												// �^�ǹϤ���
		void						set_image( QImage img );										// �]�w�Ϥ�
		MRF_FEATURE_TYPE			get_feature_type();												// �^�� feature type
		QImage						get_image();													// �^��image
		bool						check_image();													// �ˬd��feature��image�O�_�ۦP
		bool						is_feature();													// �ˬdfeature�O�_��l��
		int							dim();															// �^��feature��dim
		void						get_feature( int pix, double *_f );								// ���o��pixel��feature
		void						set_mean( double *_m );											// �]�w���� (��feature)
		void						get_mean( double *_m );											// �^��feature����
		void						set_devia( double *_d );										// �]�w�зǮt				
		void						get_devia( double *_d );										// �^�ǼзǮt
		bool						check_dim();													// �ˬd dimension
		void						cal_feature();													// �p�� feature

		void						output();														// ��X��� (���ե�)

		void	operator = ( const mrfEnergyAbstract &e );


};





#endif