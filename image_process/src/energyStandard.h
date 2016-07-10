#ifndef _ENERGY_STANDARD_H
#define _ENERGY_STANDARD_H

#include "mrfEnergyAbstract.h"
#include "moment.h"
#include <QImage>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	�з� Energy  (�ѦҰ�¦��MRF���k)
	neighbor �ĥ� +-Betta
	single   �ĥ� Gaussian
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

class	EnergyStandard : public mrfEnergyAbstract
{
	private:

	public:
		EnergyStandard( mrfFeatureAbstract &ft, QImage img );				// �غc
		EnergyStandard( const EnergyStandard &e );							// �ƻs�غc
		~EnergyStandard();													// �Ѻc

		// virtual ��
		double				E_data( int pix, int lab );								// 1-clique
		double				E_smooth( int pix1, int pix2, int lab1, int lab2 );		// 2-clique
		double				Prob( int pix, int lab );								// �^�Ǿ��v�K�ר�ƭ�
		MRF_ENERGY_TYPE		type();													// �^�Ǧۤv�O��������

		// �D virtual ��
		void	operator = ( const EnergyStandard &e );

};



#endif