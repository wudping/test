#ifndef _ENERGY_TEST_H
#define _ENERGY_TEST_H

#include "mrfEnergyAbstract.h"
#include "moment.h"
#include <QImage>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	���ե�energy function
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

class	EnergyTest : public mrfEnergyAbstract
{
	private:

	public:
		EnergyTest( mrfFeatureAbstract &ft, QImage img );				// �غc
		EnergyTest( const EnergyTest &e );								// �ƻs�غc
		~EnergyTest();													// �Ѻc

		// virtual ��
		double				E_data( int pix, int lab );								// 1-clique
		double				E_smooth( int pix1, int pix2, int lab1, int lab2 );		// 2-clique
		double				Prob( int pix, int lab );								// �^�Ǿ��v�K�ר�ƭ�
		MRF_ENERGY_TYPE		type();													// �^�Ǧۤv�O��������

		// �D virtual ��
		void	operator = ( const EnergyTest &e );


};



#endif