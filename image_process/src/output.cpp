#include "output.h"



/********************************************************************************************************************
	�غc���
*********************************************************************************************************************/
Output::Output()
{}


/********************************************************************************************************************
	�Ѻc���
*********************************************************************************************************************/
Output::~Output()
{}


/********************************************************************************************************************
	���Ϳ�X��SVG�ɮ�
	���w�ɦW
*********************************************************************************************************************/
void	Output::outputSVG( QImage imagePass, QString filename = "test.svg" )
{
	//QString path("test.svg");
	//QString		path	=	filedata->filename;
	//path.replace( ".bmp", ".svg" );


	// �Nbmp��svg �d��

//![configure SVG generator]
 
    QSvgGenerator	generator;

    //generator.setFileName(path);
	generator.setFileName( filename );

	generator.setSize( QSize(imagePass.width(), imagePass.height()) );

	generator.setViewBox( QRect(0, 0, imagePass.width(), imagePass.height()) );
    generator.setTitle("SVG Generator Example Drawing");
    generator.setDescription("An SVG drawing created by the SVG Generator "
                                "Example provided with Qt.");
//![configure SVG generator]
//![begin painting]
	//QImage	img	=	imagePass;


    QPainter painter(&generator);

	painter.drawImage( QPoint(0,0), imagePass  );
	//painter.drawEllipse( 100, 100, 20, 50 );


    painter.begin(&generator);
	//displayWidget->paint(&img);
	//displayWidget->paint(painter);
    painter.end();
//![end painting]
}


