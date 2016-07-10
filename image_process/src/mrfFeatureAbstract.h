#ifndef _MRF_FEATURE_ABSTRACT_H
#define _MRF_FEATURE_ABSTRACT_H

#include "../tools.h"
#include <QImage>
#include <boost/numeric/ublas/matrix.hpp>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	��HFeature���O
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


enum	MRF_FEATURE_TYPE
{
	MRF_FEATURE_MOMENT,
	MRF_FEATURE_DISTANCE,
	MRF_FEATURE_GABOR,
	MRF_FEATURE_COCCM,
	MRF_FEATURE_MRSAR,
};



class	mrfFeatureAbstract
{
	private:

	protected:
		QImage						sample;											// �˱i
		QImage						image;											// ��e�p��feature���Ϥ�

		int							dimension;										// ���� (�ѭl�ͪ���ǤJ)
		double						*mean;											// ������ (��pointer ���F�[�t)
		double						*devia;

		// �µ������
		virtual void				cal_mean()							=	0;		// �p�⥭���� �o��ӥ�����cal_feature�өI�s  �]���p�G�S��feature,�S��k�p�⥭���ȸ�зǮt
		virtual void				cal_devia()							=	0;		// �p��зǮt

	public:
		mrfFeatureAbstract( int dim );
		mrfFeatureAbstract( QImage imagePass, int dim );							// �غc
		mrfFeatureAbstract( const mrfFeatureAbstract &ft );							// copy construct
		virtual		~mrfFeatureAbstract();											// �Ѻc

		// �µ������
		virtual void				segmentation( QImage img )			=	0;		// ��H���� �ΨӴ���feature��Ų�O��
		virtual	void				cal_feature( QImage img )			=	0;		// �p��feature �P�ɷ|�⥭���� �зǮt
		virtual	void				output_txt()						=	0;		// �Nfeature��X���ɮ� (��txt�榡)
		virtual double				get_norm( int pix )					=	0;		// �p��ۤv�� norm |u|
		virtual double				get_norm( int pix1, int pix2 )		=	0;		// �p����pixel��norm |u-v|
		virtual	MRF_FEATURE_TYPE	type()								=	0;		// �^�ǥثe��feature type
		virtual int					dim()								=	0;		// �^�Ǻ���
		virtual double				get_norm_squ( int pix )				=	0;		// �^�� norm ������
		virtual double				get_devia_det()						=	0;		// �^��deviation��determine  (�]����Ӫ��פ�O�[�W |Sigma| ���T�w�Onorm�٬Odet)  �ݭn�Ψ�dimension,�L�k�{�b�M�w
		virtual void				get_feature( int pix, double *_f )	=	0;		// �^�Ǹ�pixel��feature
		virtual bool				check_dim()							=	0;		// �ˬd����
		virtual	void				cal_sample_feature( QImage img )	=	0;		// �p��˱i ��cal_feature���e�|�ܹ� �t�b�o��|�⥭���зǮt

		// �D�µ������
		void						set_image( QImage img );						// �]�w�Ϥ�
		QImage						get_image();									// �^�ǹϤ�
		void						set_mean( double *_m );							// �]�w������
		void						get_mean( double *_m );							// �^�ǥ�����
		void						set_devia( double *_d );						// �]�w�зǮt
		void						get_devia( double *_d );						// �^�ǼзǮt

		void	operator	=	( const mrfFeatureAbstract &ft );					// = operator

};





#endif