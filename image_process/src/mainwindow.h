/*===================================================================================================================================================

	�D�n����
	�D�n����X�G���w�q�b�o��
	����P���󤧶������q�Ʊ�z�Lmainwindow   ���� <-> mainwindow <-> ����  �Ӥ��O����P���󪽱����q  ���� <-> ����
	�n�`�Nthread���ϥ�  �קK��ܸ�p��ΦP�@��thread  �y���t�ש쫱

	�ثe�[�c�W�j����

	thread   ����B��
	display  �������
	data     �U�ظ��
	setting  �U�س]�w
	output   �t�d��X���ɮ�

	progress �O�t�dthread����~�������q

/*===================================================================================================================================================*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "pivdisplay.h"
#include "vectorlist.h"
#include "progress.h"

#include "importimages.h"
#include "batchwindow.h"

#include "processing.h"
#include "datacontainer.h"
#include "output.h"

#include "ui_mainwindow.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

	private:

        Settings			*settings;

        ImportImages		*importImages;

        Processing			*process;
        DataContainer		*filedata;

		Progress			*progress;									// �t�d�^���i�׵���

		static const int	displaySize		=	4;	
		PivDisplay			*display [ displaySize ];					// �D�n��ܵ���
		QLabel				*position[ displaySize ];					// ��ܦ�m��
		QLabel				*rgbLabel[ displaySize ];					// ����C�⪺

		static const int	pointListSize	=	2;						
		PointList			*pointList[pointListSize];					// �D�e�����U�� point list


    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();

    protected:
        // Initialization
        void	setupWindows();
        void	initialize();
		void	displayInitialize();								// ��l��pivdisplay
		void	pointlistInitialize();								// ��l��point list
        void	setupConnections();

        void	initializeProcessTab();
        void	initializeFilterTab();
        void	initializeDataQualityTab();
        void	initializeViewTab();

		void	measure( int method );									// �i��measure

	signals:
		void	doprocess();											// �}�l�i��process���ʧ@
		void	set_measure_X( int );									// �]�wmeasure���y�� X Y �� spinbox
		void	set_measure_Y( int );

    protected slots:

		void 	launchBMPImport();										// Andrea�s�W Ū��BMP�ɮ�
		void 	setBlobmethod( int method );							// Andrea�s�W �]�wblob�n�έ��Ӥ�k
		void 	setHighlight( int is );									// Andrea�s�W �O�_�n�ϥ�
		void 	setWatershed( int method );								// Andrea�s�W ��ܭn�έ���watershed����k  �ثe�� 4.8 4.1 MSER
		void 	blob();													// �i��blob
		void	channel();												// �i��extract channel
		void 	FFTJunctionProcess();									// �i��FFT Junction 
		void 	SteerJunctionProcess();									// �i��Steer Junctioni
		void 	EdgeProcess();											// �i��subpixel Edge 
		void 	CornerProcess();										// �i��subpixel Edge 

		void	setTabText( int index, QString str );					// �]�w�D�e�����U��tab��r����

        void 	notifyFolderChange();
        void 	chooseFilteredColour();
        void 	chooseUnfilteredColour();
        void 	setColourButtons();

        void 	setDisplayTab();
		void	clearData();											// �M�Ÿ�� pointlist

        void 	setDoingPIV();
        void 	setOutput();
        void 	chooseOutputFolder();

		void	addPixel(PIXEL pixel);									// �I���e�� �W�[�I�M��
		void	removePointList( int row, int col );					// �I�� point list (input) �R���I���
		void	view_square( int row, int col );						// ���v��  �|�b�e���W�����ϼаO�X��
		void	remove_square();										// �����Y��square
		void	displayImage( int index, QImage image );				// ��X�e������ܵ���

};

#endif // MAINWINDOW_H
