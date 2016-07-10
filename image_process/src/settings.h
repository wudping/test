#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QColor>
#include <QSize>
#include <QList>
#include <QPoint>
#include <QRect>

#include "progress.h"

namespace OpenPIV
{

	// ���v
	const int	up			=	0;
	const int	down		=	1;
	const int	left		=	2;
	const int	right		=	3;
	const int	left_up		=	4;
	const int	left_down	=	5;
	const int	right_up	=	6;
	const int	right_down	=	7;

	// Processors
	const int SubPixBlob	=	0;
	const int Watershed		=	1;
	const int FFT			=	2;
	const int Steer			=	3;
	const int Edge			=	4;
	const int Corner		=	5;
	const int Channel		=	6;
	const int ColorCode		=	7;

    // Detectors
    const int GaussianSubPixel = 0;

    // Output
    const int Text = 0;
    const int HDF5 = 1;

	// watershed
	const int i4_8	=	0;
	const int i4_1	=	1;
	const int MSER	=	2;
};

class Settings : public QObject
{
#ifndef _GTEST_
    Q_OBJECT
#endif
    protected:
        void changeDeltaX();
        void changeDeltaY();

    private:
        // Display
        QSize _imageSize;

        // Masking
        bool _isMask;
        QRect _roi;
        bool roiSet;

        // Processing
        int _deltaX, _deltaY;
        int xSpin, ySpin;
        int _intLengthX, _intLengthY;
        int _processor;
        int _detector;

        // Batch processing
        bool _batchFilter;
        bool _batchMask;
        bool _batchShowImage;
        bool _batchShowVectors;
        bool _batchThreading;

        // Vectors
        QColor _vectorColourFiltered;
        QColor _vectorColourUnfiltered;
        double _vectorScale;
        double _vectorSub;


        // Output
        QString _outputFolder;
        QString _expName;
        int _outputFormat;

		// Blob detector�\��
		int		scale_w,	scale_l;		// �ث׳]�w

		// measure �ϥΪ�
		int		measure_x,	measure_y;		// x, y �y��
		QRgb	rgb;						// �C���T

		// �t�d�^���i�ץΪ�
		Progress	*progress;


    public:
        Settings( Progress *_progress );
        virtual ~Settings();

        // Processing
        int deltaX();       
        int deltaY();
        int intLengthX();
        int intLengthY();
        int processor();
        int detector();
        QSize imageSize();
        void setImageSize(QSize imageSizePass);
        bool isMask();
        QRect roi();

        // Batch processing
        bool batchFilter();
        bool batchMask();
        bool batchShowImage();
        bool batchShowVectors();
        bool batchThreading();

        // Vectors
        QColor vectorColourFiltered();
        QColor vectorColourUnfiltered();
        double vectorScale();
        double vectorSub();


        // Output
        QString expName();
        QString outputFolder();
        int outputFormat();

		// sub pixel blob detector�Ϊ�
		int			get_scale_w();
		int			get_scale_l();

		// measure�Ϊ�
		int			get_measure_X();
		int			get_measure_Y();

		// �]�wprogress
		Progress*	getProgress();

		// Watershed�Ϊ�
		bool		isHighlight;			// �O�_�n�ϥ�
		int			watershed;				// �M�w�n��watershed�����@�Ӥ�k

		// ��L
		void		setRGB( QRgb _rgb );	// �]�w�C��
		QRgb		getRGB();				// �^���C��

		// ���v�˴���
		int			shadow_dir;				// ���v��V
		int			square_size;			// ����Τj�p
		bool		set_square;				// �O�_�i�J�]�w����Ϊ����q
		int			shadow_depth_threshold;	// �`�ת�threshold
		bool		inhence_mode;			// �P�_�O�_��inhencemode  �M�w�n����I���v �٬O��q�����v ���Inoise�ܱj�����p
		int			file_size;				// �ɮ׭Ӽ�
		bool		save_check;				// �O�_�C�L���G(image)���ɮ�
		bool		batch_check;			// �O�_�妸�@�~
		int			dark_light;				// ��G�v�Ϊ̳��v
		int			id;						// �ɮ� id

#ifndef _GTEST_
    public slots:
#endif
        
        // Processing
        void setDeltaX(int deltaXPass);
        void setDeltaY(int deltaYPass);
        void setIntLengthX(int intLengthXPass);
        void setIntLengthY(int intLengthYPass);
        void setProcessor(int processPass);
        void setDetector(int detectPass);
        void setIsMask(bool isMaskPass);
        void setRoi(QRect roiPass);

        // Batch processing
        void setBatchFilter(bool batchFilterPass);
        void setBatchMask(bool batchMaskPass);
        void setBatchShowImage(bool batchShowImagePass);
        void setBatchShowVectors(bool batchShowVectorsPass);
        void setBatchThreading(bool batchThreadingPass);

        // Vectors
        void setVectorColourFiltered(QColor vectorColourPass);
        void setVectorColourUnfiltered(QColor vectorColourPass);
        void setVectorScale(double vectorScalePass);
        void setVectorSub(double vectorSubPass);


        // Output
        void setExpName(QString expNamePass);
        void setOutputFolder(QString outputFolderPass);
        void setOutputFormat(int outputFormatPass);

		// blob detector
		void	set_scale( int w, int l );
		void	set_scale_w( int w );
		void	set_scale_l( int l );

		// measure�ϥΪ�
		void	set_measure_X( int x );
		void	set_measure_Y( int y );

		// ���v�ϥ�
		void	setInhence( int state );		// �]�winhence mode
		void	setSaved( int state );			// �]�w�O�_�n��X���G���ɮ� (image����)
		void	setBatch( int state );			// �]�w�O�_���妸�@�~



#ifndef _GTEST_
    signals:
        void imageSizeChanged();				// Display
        void processSettingsChanged();			// Processing
        void vectorSettingChanged();			// Vectors
#endif


};

#endif // SETTINGS_H
