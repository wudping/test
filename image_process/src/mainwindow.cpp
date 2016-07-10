/*=====================================================================================================
	�D����
/*=====================================================================================================*/

#include <QtGui>
#include <QVector>
#include <QPointF>
#include <cmath>
#include <iostream>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "settings.h"
#include "importimages.h"

#include "pointlist.h"
#include "vectorlist.h"
#include "imagepaint.h"



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);
    initialize();							// ��l��
    setupWindows();
    setupConnections();
    initializeProcessTab();
    initializeFilterTab();
    setDisplayTab();
    setColourButtons();
    setDoingPIV();
    initializeDataQualityTab();
    initializeViewTab();
}

MainWindow::~MainWindow()
{
    delete importImages;
    delete filedata;
    delete process;
}


/*****************************************************************************************************************
	��l��pivdisplay
******************************************************************************************************************/
void	MainWindow::displayInitialize()
{
	display[0]	=	pivDisplay;
	display[1]	=	pivDisplay_2;
	display[2]	=	pivDisplay_3;
	display[3]	=	pivDisplay_4;
	position[0]	=	positionLabel;
	position[1]	=	positionLabel_2;
	position[2]	=	positionLabel_3;
	position[3]	=	positionLabel_4;
	rgbLabel[0]	=	RGBLabel;
	rgbLabel[1]	=	RGBLabel_2;
	rgbLabel[2]	=	RGBLabel_3;
	rgbLabel[3]	=	RGBLabel_4;

	for( int i = 0; i < displaySize; i++ )
	{
		display[i]->setSettings( settings );
		display[i]->setData( filedata );
		display[i]->setPositionLabel( position[i] );
		display[i]->setRGBLabel( rgbLabel[i] );
	}
}


/*****************************************************************************************************************
	��l��point list
******************************************************************************************************************/
void	MainWindow::pointlistInitialize()
{
	pointList[0]	=	aListWidget;
	pointList[1]	=	bListWidget;
}


/*****************************************************************************************************************
	��l��
******************************************************************************************************************/
void MainWindow::initialize()
{
	progress		=	new Progress;						// �]�w progress  �o�䶶�Ǥ����H�K���
    settings		=	new Settings( progress );
    filedata		=	new DataContainer( settings );

	pointlistInitialize();

    vectorListWidget->setData(filedata);
    process		=	new Processing(settings,filedata);

	displayInitialize();

}

void MainWindow::setupWindows()
{
    importImages = new ImportImages;
	importImages->setProgress(progress);
    importImages->setData(filedata);
}

void MainWindow::setupConnections()
{
	int		i;

	/*
		Andrea�ק�
	*/
	//connect( openButton,		  SIGNAL(clicked()), this, SLOT( launchBMPImport() ) );  ���ե� 

    connect( openBMP,			 	SIGNAL(clicked()),						this,       			SLOT(launchBMPImport()) );						// ���Uopen BMP  �i�HŪ������  Andrea
    connect( openBMP_2,			 	SIGNAL(clicked()),						this,       			SLOT(launchBMPImport()) );						// ���Uopen BMP  �i�HŪ������  Andrea    2���Φbmeasure
	connect( progress,				SIGNAL(signalImage(int,QImage)),		this,					SLOT(displayImage(int,QImage)) );				// ��X�e������ܵ���

	connect( filedata,			 	SIGNAL(svgChanged(QString)),			pivDisplay_3,			SLOT(displaySVG(QString)) );					// ��svg���覡��ܹϤ� (svg�Ȧs�b�w��)   �������T�h�����

	connect( ScaleW,			 	SIGNAL(valueChanged(int)),				settings,    			SLOT(set_scale_w(int)) );						// �]�w Gauss filter scale w
	connect( ScaleL,			 	SIGNAL(valueChanged(int)),				settings,    			SLOT(set_scale_l(int)) );						// �]�w Gauss filter scale l
	connect( measureX,			 	SIGNAL(valueChanged(int)),				settings,    			SLOT(set_measure_X(int)) );						// �]�w Measure���y��X
	connect( measureY,			 	SIGNAL(valueChanged(int)),				settings,    			SLOT(set_measure_Y(int)) );						// �]�w Measure���y��Y
	connect( this,               	SIGNAL(doprocess()),                	process,				SLOT(processBatch()) );							// �i��process���ʧ@
	connect( DoFilter,			 	SIGNAL(clicked()),						this,					SLOT(blob()) );									// ���U���s �}�l����blob detector  (�]�tsubblob  watershed)
	connect( ChannelButton,			SIGNAL(clicked()),						this,					SLOT(channel()) );								// ������w�C�⪺�q�D 
	connect( FFTJunctionButton,   	SIGNAL(clicked()),						this,					SLOT(FFTJunctionProcess()) );					// ���U���s �}�l���� FFT Junction Detector
	connect( SteerJunctionButton, 	SIGNAL(clicked()),						this,					SLOT(SteerJunctionProcess()) );					// ���U���s �}�l���� Steer Junction Detector
	connect( EdgeButton,		  	SIGNAL(clicked()),						this,					SLOT(EdgeProcess()) );							// ���U���s �}�l���� Subpixel Edge
	connect( CornerButton,		  	SIGNAL(clicked()),						this,					SLOT(CornerProcess()) );						// ���UCorner�����s �i��subpixel��Corner�p��
	
	//connect( process,			  	SIGNAL(imageThrow(QImage)),				pivDisplay,				SLOT(displayBMP(QImage)) );						// �p��n Gauss Filter �N��ƥᵹ���
	connect( process,			  	SIGNAL(signalsGress(int)),				filterProgress,			SLOT(setValue(int)) );							// �]�w�i�ױ�
	connect( process,			  	SIGNAL(signalsGress(int)),				measureProgress,		SLOT(setValue(int)) );							// �]�w�i�ױ�
	connect( process,			  	SIGNAL(signalTabText(int,QString)),		this,					SLOT(setTabText(int,QString)) );				// �]�w�i�ױ�

	connect( blobCombo,			  	SIGNAL(currentIndexChanged(int)),		this,					SLOT(setBlobmethod(int)) );						// �]�w�{�b���檺�O���@��blob detector
	connect( waterCheck,		  	SIGNAL(stateChanged(int)),				this,					SLOT(setHighlight(int)) );						// �]�w�O�_�n�ϥ�
	connect( waterCombo,		  	SIGNAL(currentIndexChanged(int)),		this,					SLOT(setWatershed(int)) );						// �]�w����watershed��k  4.8 4.1 mser


	// ��T���L�찣���T�����a��   append  or   setText
	connect( filedata,   			SIGNAL(debugSignal(QString)),     		text_debug,				SLOT(append(QString)) );
	connect( process,    			SIGNAL(debugSignal(QString)),     		text_debug,				SLOT(append(QString)) );
	connect( progress,	 			SIGNAL(debugSignal(QString)),	   		text_debug,				SLOT(append(QString)) );

	// �o�����ܵe�� pointlist����
	for( i = 0; i < 2; i++ )
		connect( display[i],			SIGNAL(mousePressed(PIXEL)),			this,					SLOT(addPixel(PIXEL)) );					// �I���e�� ��s���
	connect( progress,				SIGNAL(updateWidgeB()),					pointList[1],			SLOT(updatePointList()) );						// �N�����ܨ� point list (output)
	//connect( pointList[0],			SIGNAL(cellClicked(int,int)),			this,					SLOT(removePointList(int,int)) );				// �I�� point list (input) �R���I���
	// ���v�˴��� �令�I���쪺�ɭ���ܸӰϰ����ܰ�
	connect( pointList[0],			SIGNAL(cellClicked(int,int)),			this,					SLOT(view_square(int,int)) );
	connect( removeButton,			SIGNAL(clicked()),						this,					SLOT(remove_square()) );						// �����Y��square
	connect( clearButton,			SIGNAL(clicked()),						this,					SLOT(clearData()) );							// ���Uclear���s  �M�Ÿ��
	connect( inhenceCheck,			SIGNAL(stateChanged(int)),				settings,				SLOT(setInhence(int)) );						// �]�winhencement  �P�_�O����I���v �٬O��q�����v (���Inoise�ܱj ��noise���j��case)
	connect( saveCheck,				SIGNAL(stateChanged(int)),				settings,				SLOT(setSaved(int))	);							// �]�w�O�_�n�N���G��X���ɮ� (image����)
	connect( batchCheck,			SIGNAL(stateChanged(int)),				settings,				SLOT(setBatch(int)) );							// �]�w�O�_���妸�@�~
	
    // Importing
   // connect(importButton,			SIGNAL(clicked()),						this,					SLOT(launchImageImport()));
   // connect(filedata,				SIGNAL(imagesImported()),				this,					SLOT(notifyFolderChange()));

    connect(vectorListWidget, SIGNAL(fileClicked(int)), this, SLOT(vectorClicked(int)));

    // The following should be moved into settings
    //
    connect(colourButtonFiltered, SIGNAL(clicked()), this, SLOT(chooseFilteredColour()));
    connect(colourButtonUnfiltered, SIGNAL(clicked()), this, SLOT(chooseUnfilteredColour()));
    connect(scaleSpin, SIGNAL(valueChanged(double)), settings, SLOT(setVectorScale(double)));
    connect(subSpin, SIGNAL(valueChanged(double)), settings, SLOT(setVectorSub(double)));

    // Process tab
    connect(hSizeCombo, SIGNAL(activated(int)), settings, SLOT(setIntLengthX(int)));
    connect(vSizeCombo, SIGNAL(activated(int)), settings, SLOT(setIntLengthY(int)));
    connect(hSpaceSpin, SIGNAL(valueChanged(int)), settings, SLOT(setDeltaX(int)));
    connect(vSpaceSpin, SIGNAL(valueChanged(int)), settings, SLOT(setDeltaY(int)));


    // DoingPIV
    connect(outputFolderEdit, SIGNAL(textEdited(QString)), this, SLOT(setOutput()));
    connect(outputFolderButton, SIGNAL(clicked()), this, SLOT(chooseOutputFolder()));
    connect(outputFormatCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(setOutput()));


    connect(batchButton, SIGNAL(clicked()), process, SLOT(launchBatchWindow()));
    connect(process, SIGNAL(batchProcessed()), this, SLOT(batchDone()));


	//for( i = 0; i < displaySize; i++ )
	//	connect(display[i],   	SIGNAL(mousePressed(PIXEL)),			this,			SLOT(updatePositionPixel(PIXEL)));				// Andrea�ק�  �I�@�U�ƹ�,��RGB��T��J�i�h.


	/*connect(pivDisplay,   	SIGNAL(mousePressed(QPointF)),			this,			SLOT(uptdaePositionRGB(QPointF)));				// Andrea�ק�  �I�@�U�ƹ�,��RGB��T��J�i�h.
	connect(pivDisplay_2,  	SIGNAL(mousePressed(QPointF)),			this,			SLOT(uptdaePositionRGB(QPointF)));				// Andrea�ק�  �I�@�U�ƹ�,��RGB��T��J�i�h.   �Φb����2
	connect(pivDisplay,   	SIGNAL(mousePressed(QPointF)),			this,			SLOT(updatePositionXY(QPointF)));				// Andrea�ק�  �I�@�U�ƹ�,��y�п�J�i�h.
	connect(pivDisplay_2,  	SIGNAL(mousePressed(QPointF)),			this,			SLOT(updatePositionXY(QPointF)));				// Andrea�ק�  �I�@�U�ƹ�,��y�п�J�i�h.   �Φb����2*/



	//connect(pivDisplay,   	SIGNAL(mousePressed(QPointF)),			pivDisplay,		SLOT(mark_XY(QPointF)));						// Andrea�ק�  �I�@�U�ƹ�, �b�W���аO�I
	//connect(pivDisplay_2,  	SIGNAL(mousePressed(QPointF)),			pivDisplay_2,	SLOT(mark_XY(QPointF)));						// Andrea�ק�  �I�@�U�ƹ�, �b�W���аO�I    �Φb����2



	connect(this,			SIGNAL(set_measure_X(int)),				measureX,		SLOT(setValue(int) ) );							// �]�w X �y�Ъ�spinbox
	connect(this,			SIGNAL(set_measure_Y(int)),				measureY,		SLOT(setValue(int) ) );							// �]�w Y �y�Ъ�spinbox
	

}

/*-------- Setting the tabs -----------------*/

void MainWindow::setDoingPIV()
{
    outputFolderEdit->setText(settings->outputFolder());
    outputFormatCombo->insertItem(OpenPIV::Text,"Text file");
    outputFormatCombo->insertItem(OpenPIV::HDF5,"HDF5");
}

void MainWindow::initializeProcessTab()
{
    int i, index;
    double power2;

    for (i = 0; i < 4; i++)
    {
        power2 = pow(2.0,double(4+i));
        hSizeCombo->insertItem(i,QString("%1").arg(power2));
        vSizeCombo->insertItem(i,QString("%1").arg(power2));
    }

    index = int(log10(double(settings->intLengthX())) / log10(2.0)) - 4;
    index = int(log10(double(settings->intLengthY())) / log10(2.0)) - 4;

    hSpaceSpin->setMinimum(0);
    vSpaceSpin->setMinimum(0);
    hSpaceSpin->setMaximum(100);
    vSpaceSpin->setMaximum(100);
    double value;
    value = double(settings->deltaX()) / double(settings->intLengthX()) * 100;
    hSpaceSpin->setValue(int(value));
    value = double(settings->deltaY()) / double(settings->intLengthY()) * 100;
    vSpaceSpin->setValue(int(value));

    interpolateCombo->insertItem(OpenPIV::GaussianSubPixel,"Three-point Gaussian");
}

void MainWindow::initializeFilterTab()
{
    // Global
    minUedit->setValidator(new QDoubleValidator(-256.0, 256.0, 3, minUedit));
    maxUedit->setValidator(new QDoubleValidator(-256.0, 256.0, 3, maxUedit));
    minVedit->setValidator(new QDoubleValidator(-256.0, 256.0, 3, minVedit));
    maxVedit->setValidator(new QDoubleValidator(-256.0, 256.0, 3, maxVedit));

    // Local
    QString label;
    int i, index;

	// Andrea�s�W blobCombo �Ψӿ�ܲ{�b�n�έ��@�Ӥ�k
	blobCombo->addItem("up");
	blobCombo->addItem("down");
	blobCombo->addItem("left");
	blobCombo->addItem("right");
	blobCombo->addItem("left_up");
	blobCombo->addItem("left_down");
	blobCombo->addItem("right_up");
	blobCombo->addItem("right_down");
	blobCombo->addItem("all_direction");
	blobCombo->addItem("up_down");
	blobCombo->addItem("left_right");
	blobCombo->addItem("left up <-> right down");
	blobCombo->addItem("left down <-> right up");

	waterCombo->addItem( "4.8" );
	waterCombo->addItem( "4.1" );
	waterCombo->addItem( "MSER" );

	//blobCombo->setCurrentIndex( OpenPIV::SubPixBlob );				// andrea �w�]��

	
	// ���v
	ChannelBox->setValue(4);
	saveCheck->setCheckState( Qt::Checked );
	ScaleL->setMaximum(300);



    for (i = 0; i < 5; i++)
    {
        index = (i+1)*2 + 1;
        label = QString("%1x%2").arg(index).arg(index);
        localNxNCombo->insertItem(i,label);
    }
    localUedit->setValidator(new QDoubleValidator(0.0, 256.0, 3, localUedit));
    localVedit->setValidator(new QDoubleValidator(0.0, 256.0, 3, localVedit));

    // Interpolate
//    interpolateMethodCombo->insertItem(OpenPIV::InterpolateMean,"Mean");
    for (i = 0; i < 5; i++)
    {
        index = (i+1)*2 + 1;
        label = QString("%1x%2").arg(index).arg(index);
        interpolateNxNCombo->insertItem(i,label);
    }

    // Smoothing
    for (i = 0; i < 5; i++)
    {
        index = (i+1)*2 + 1;
        label = QString("%1x%2").arg(index).arg(index);
        smoothNxNCombo->insertItem(i,label);
    }
    smoothRadiusEdit->setValidator(new QDoubleValidator(0.0, 1000.0, 3, smoothRadiusEdit));
}



void MainWindow::setDisplayTab()
{
    vectorToggle->setChecked(true);
}

void MainWindow::initializeDataQualityTab()
{
    snrEdit->setValidator(new QDoubleValidator(0.0, 1000.0, 3, snrEdit));
    intensityEdit->setValidator(new QDoubleValidator(0.0, 5000.0, 3, intensityEdit));
}



void MainWindow::initializeViewTab()
{
    scaleSpin->setValue(settings->vectorScale());
    subSpin->setValue(settings->vectorSub());
}


void MainWindow::notifyFolderChange()
{
    QString directory = settings->outputFolder();
    QString boxText = QString(tr("The output folder has been set to:\n %1")).arg(directory);
    boxText.append(tr("\n This folder can be changed on the \"Doing PIV\" tab.\n"));
    QMessageBox::information(this,QString(tr("Output Folder Set")),boxText);
//    pivAclicked(0);
}




void MainWindow::chooseFilteredColour()
{
    QColor colour;
    colour = QColorDialog::getColor(settings->vectorColourFiltered(),this);
    settings->setVectorColourFiltered(colour);
    setColourButtons();
}

void MainWindow::chooseUnfilteredColour()
{
    QColor colour;
    colour = QColorDialog::getColor(settings->vectorColourUnfiltered(),this);
    settings->setVectorColourUnfiltered(colour);
    setColourButtons();
}

void MainWindow::setColourButtons()
{
    QColor colour;
    QString styleItem;
    colour = settings->vectorColourUnfiltered();
    styleItem = QString("background-color: rgb(%1,%2,%3);").arg(colour.red()).arg(colour.green()).arg(colour.blue());
    colourButtonUnfiltered->setStyleSheet(styleItem);
    colour = settings->vectorColourFiltered();
    styleItem = QString("background-color: rgb(%1,%2,%3);").arg(colour.red()).arg(colour.green()).arg(colour.blue());
    colourButtonFiltered->setStyleSheet(styleItem);
}


/* ----------- Output ----------------*/

void MainWindow::setOutput()
{
    settings->setOutputFolder(outputFolderEdit->text());
    settings->setOutputFormat(outputFormatCombo->currentIndex());
}

void MainWindow::chooseOutputFolder()
{
    QString directory = QFileDialog::getExistingDirectory(this);
    outputFolderEdit->setText(directory);
    setOutput();
}




/*============================================================================
	�]�w�n�έ���blob����k
==============================================================================*/
void MainWindow::setBlobmethod( int	method )
{
	settings->shadow_dir	=	method;
}



/*============================================================================
	�]�w�n��watershed���Ӥ�k
	�ثe��
	4.8
	4.1
	MSER
==============================================================================*/
void MainWindow::setWatershed( int	method )
{
	settings->watershed	=	method;
}


// Andrea �s�W
/*-----------------------------�i��measure-------------------------------*/
void	MainWindow::measure( int method )
{
	settings->setProcessor( method );
	emit( doprocess() );
}

/*-----------------------------�i��FFT��Junction Detector------------------------------*/
void	MainWindow::FFTJunctionProcess()
{
	measure(OpenPIV::FFT);
}

/*-----------------------------�i��Steer��Junction Detector------------------------------*/
void	MainWindow::SteerJunctionProcess()
{

	measure(OpenPIV::Steer);
}

/*-----------------------------�i��subpixel Edge------------------------------*/
void	MainWindow::EdgeProcess()
{

	measure(OpenPIV::Edge);
}

/*-----------------------------�i��subpixel Corner------------------------------*/
void	MainWindow::CornerProcess()
{

	measure(OpenPIV::Corner);
}


/*-----------------------------�i��blob detector------------------------------*/
void	MainWindow::blob()
{
	// ���v�˴� (����bchannel���U)
	settings->setProcessor( OpenPIV::Channel );
	emit( doprocess() );
}

/*-----------------------------�i��extract channel------------------------------*/
void	MainWindow::channel()
{
	//settings->setProcessor( OpenPIV::Channel );
	settings->square_size	=	ChannelBox->value()+1;
	settings->set_square	^=	true;
	//emit( doprocess() );
}




/*==================================================================================================================
	�]�w�D�e�����U��tab��r����
/*==================================================================================================================*/
void	MainWindow::setTabText( int index, QString str )
{
	pivTab->setTabText( index, str );
}

/*----------Launching BMP  windows---------------------------------*/
void MainWindow::setHighlight( int is )
{
	settings->isHighlight	=	(is == 0) ? false : true;
	settings->dark_light	=	is ? true : false;
}



/*==================================================================================================================
	��s�y�� measure�M��  
	�I�@�U�ƹ��N��]�wXY�y��
/*==================================================================================================================*/
/*void	MainWindow::updatePositionXY( QPointF XY )
{
	int		x	=	floor( XY.x() );
	int		y	=	floor( XY.y() );

	settings->set_measure_X( x );
	settings->set_measure_Y( y );

	emit( set_measure_X(x) );
	emit( set_measure_Y(y) );
}*/

/*==================================================================================================================
	���o���I�C���T  
	�I�@�U�ƹ�
/*==================================================================================================================*/
/*void	MainWindow::uptdaePositionRGB( QPointF XY )
{
	int		x	=	floor( XY.x() );
	int		y	=	floor( XY.y() );

	QRgb	rgb	=	pivDisplay_2->getRGB( x, y );		// ���ޫ�˳��q�G���������� 

	settings->setRGB( rgb );

}*/



/*==================================================================================================================
	�I���e�� 
	�W�[�I�M��
/*==================================================================================================================*/
void	MainWindow::addPixel( PIXEL pixel )
{
	bool	is_add;

	pixel.direction		=	blobCombo->currentIndex();

	// ��s�� filedata �^�ǭȪ�ܬO�_���s�W���\
	is_add	=	filedata->addPointList( pixel );

	// ��s�� point list (input )  ������I��������filedata����
	if( is_add )
		pointList[0]->updatePointList( pixel );

}


/*==================================================================================================================
	�M�Ÿ�� 
	pointlist
/*==================================================================================================================*/
void	MainWindow::clearData()
{
	filedata->clearPixelList();

	pointList[0]->clearData();
	pointList[1]->clearData();
}



/*==================================================================================================================
	�I�� point list (input) 
	�R���I���
/*==================================================================================================================*/
void	MainWindow::removePointList( int row, int col )
{
	filedata->removePointList( row, col );
	pointList[0]->removePointList( row, col );
}

/*==================================================================================================================
	�|�b�e���W�����ϼаO�X��
/*==================================================================================================================*/
void	MainWindow::view_square( int row, int col )
{
	//filedata->removePointList( row, col );
	//pointList[0]->removePointList( row, col );
	PIXEL	pixel	=	filedata->get_square( row, col );
	for( int i = 0; i < 2; i++ )
		display[i]->mark_square( pixel );
}



/*==================================================================================================================
	��X�e������ܵ���
/*==================================================================================================================*/
void	MainWindow::displayImage( int index, QImage image  )
{
	display[index]->displayImage( image );
}




/*==================================================================================================================
	Andrea �s�W
	Ū���@��bmp�ɮ�
/*==================================================================================================================*/
void	MainWindow::launchBMPImport()
{
    QString		filename	=	QFileDialog::getOpenFileName( this , tr("Open File"), "./BMP/test", tr("Images (*.bmp)"));
	QImage		image;
	bool		bsucess;

    if( !filename.isEmpty() )
    {
		bsucess		=	image.load( filename, "BMP" );
		if( bsucess )
		{
			filedata->loadBMP( image );
			display[0]->displayImage( image );
			display[1]->displayImage( image );
		}
	}
}



/*==================================================================================================================
	�����Y��square
/*==================================================================================================================*/
void	MainWindow::remove_square()
{
	int		index	=	removeBox->value();

	filedata->removePointList( index, 0 );
	pointList[0]->removePointList( index, 0 );
}
