#include "colorcode.h"




#ifdef _OPENPIV_
/*=======================================================================================================
	�غc
	OPENPIV ���U�ϥ�
/*=======================================================================================================*/
ColorCode::ColorCode(Settings *settingsPass, DataContainer  *filedataPass)	
	: AndreaEngine()
{
	progress	=	settingsPass->getProgress();	
	settings	=	settingsPass;
	filedata	=	filedataPass;

    mutex.lock();
    mutex.unlock();
}
#else
/*=======================================================================================================
	�غc
	�L�ǤJ��
/*=======================================================================================================*/
ColorCode::ColorCode()	
{
	image.load("test.bmp");
	final.load("test.bmp");
}
#endif


/*=================================================================================
	�w�]function
===================================================================================*/
ColorCode::~ColorCode()
{}



#ifdef _OPENPIV_
/*=================================================================================
	���Q�γo��funtion�Ӱ��Ʊ�. ���N�쥻��main
===================================================================================*/
void	ColorCode::dosomething()
{
	progress->debuger("Start color code detector...");

	image	=	filedata->get_image();

	// ��watershed 4.8 ���Ф��}��
	Watershed	watershed( settings, filedata );
	label	=	watershed.divide( image );
	label.save("label.bmp");

	// ����gray tone����
	/*Channel		channel( settings, filedata );
	graytone	=	channel.gray_tone( image, label );
	graytone.save("graytone.bmp");*/


	SUBPIX_BLOB		blob( settings, filedata );
	//blob.dosomething3( graytone );



	// ��X��D�e��
	//emit( filedata->output(graytone) );
	//progress->outputImage( 0, graytone );
	progress->debuger( "finish color code detector...", QColor(255,0,0) );
}
#endif