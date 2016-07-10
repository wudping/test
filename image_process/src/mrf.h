#ifndef	_MRF_H
#define _MRF_H

//#include "Energy_test.h"
#include "energyStandard.h"
#include "gaussfilter.h"
#include "../tools.h"
#include <vector>
#include <ctime>
#include <iomanip>
#include <QImage>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_01.hpp>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Markov Random Field ����
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


class	MarkovRandomField
{
	private:


		std::vector<mrfEnergyAbstract*>		vpEnergy;				// Energy pointer vector
		std::vector<bMatrixDouble>			D;						// ���� width * height * label �� �T�תŶ� map        �ݩ�Ȧs�ʽ� �O�֤�
																	// ���vector - matrix �[�c  (�쥻�]�p�|�b�Ѻc���a��]�ܺC)  
																	// E-M �@�˨ϥΦ� D

		double				*P_lambda;								// E-M �ϥ� P(lambda)  �פ夤������weight
		double				*K_lambda;								// E-M �ϥ� K_lambda   


		bMatrixInt			label_map;								// �s��label��T
		QImage				image;									// ���FE-M method, �٬O�[�i�ӤF

	public:
		MarkovRandomField();										// �غc
		MarkovRandomField( mrfEnergyAbstract &e );					// �غc
		~MarkovRandomField();										// �Ѻc

		void				ICM_exec( QImage img );					// ���� ICM mode
		void				ICM8_exec( QImage img);					// �Ҽ{ 8-neighbor
		void				SimuAnneal_exec( QImage img );			// ��������h���k
		void				SimuAnneal8_exec( QImage img );			// �Ҽ{ 8-neighbor

		void				Gibbs_exec( QImage img );				// ���� gibbs sampler  (��EM�ĪG����)
		void				initial_state( int MAX_LABEL, int *counts, double *sum, double *sumsq );				// gibbs ��l��
		void				remove_observation( double data, int label, int *counts, double *sum, double *sumsq );	// gibbs �����I
		void				add_observation( double data, int label, int *counts, double *sum, double *sumsq );		// gibbs �W�[�I
		void				update_estimates( int MAX_LABEL, double *prob, double *mean, double *sigma, int *counts, double *sum , double *sumsq );		// ��s���p���G
		double				dnorm( double x, double mean, double sigma );												// �D���v (�W�٥�norm�O�]���פ�g��)
		double				dmix( int MAX_LABEL, double x, double *probs, double *mean, double *sigma );				// �D���v�M (����normalized��Z)
		int					sample_group( int MAX_LABEL, double data, double *probs, double *mean, double *sigma );		// ����

		double				totalEnergy( bMatrixInt label );		// �ھ�label���G�ӭp���q
		double				singleEnergy( int pix, int lab );		// �p���@�I��energy
		double				dataEnergy();							// �p���q (E_data)
		double				smoothEnergy();							// �p���q (E_smooth)
		double				smoothEnergyOblique();					// �p���q (�צV)

		int					ij2pix( int i, int j );					// �N (i,j) �y���ন pixel
		void				push_back( mrfEnergyAbstract &e );		// �W�[�s��energy

		void				EM_exec( QImage img );					// ���� E-M ���p�Ѽ�
		void				EM_init();								// ��l�� (�M�w�X��label, mean �� sigma ����l��
		void				EM_init_distance();						// �ھڶZ���Ӱ���l

		void				cal_2_clique_hv( int i, int j );		// �p�����������V�� 2-clique
		void				cal_2_clique_oblique( int i, int j );	// �p��צV��2-clique
		void				update_label_map( int i, int j );		// �j�M�̤p�� ��s label map
		void				init_D();								// ��l�� map D  (�T�תŶ���map ��O�Ȧs�ʽ� �O�֤�)

		void				E_Step();								// E-M �� Expectation ���q
		void				M_Step();								// E-M �� Maximization ���q

		void				cal_K_lambda();							// E-M �p�� K_(lambda)
		void				cal_P_lambda();							// E-M �p�� P(lambda)
		void				cal_EM_mean();							// E-M �p�� mean
		void				cal_EM_devia();							// E-M �p��зǮt

};


#endif