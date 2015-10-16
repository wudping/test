#include <QRCodeGenerator.h>


#include <QPainter>
#include <QDebug>    
#include <qrencode.h>
#include <cassert>


/**********************************************************
	constructor
	Note: The encoding fails with empty string so I just default to something else. Use the setQRData() call to change this.
***********************************************************/
QRCodeGenerator::QRCodeGenerator( QObject *parent ) 
    : QObject( parent )
    , qr_str("no uid")
	, qr_img()
{
	qr_img	=	QImage( default_width, default_height, QImage::Format_RGB888 );
}


/**********************************************************
	destructor
***********************************************************/
QRCodeGenerator::~QRCodeGenerator()
{}



/**********************************************************
	set_QR_str
***********************************************************/
void	QRCodeGenerator::set_QR_str( QString qstr )
{
	qr_str	=	qstr;
}




/**********************************************************
	set_QR_str
***********************************************************/
void	QRCodeGenerator::generate()
{																						  /* 0 會讓內容變成大寫 */
    QRcode	*qr	=	QRcode_encodeString( qr_str.toStdString().c_str(), 1, QR_ECLEVEL_M, QR_MODE_8, 1 );

    if( qr != NULL )
	{
		QImage		img( default_width, default_height, QImage::Format_RGB888 );
		QPainter	painter(&img);

        QColor		foreground("black");
        QColor		background( 177, 248, 88 );

        painter.setBrush( background );
        painter.setPen( Qt::NoPen );
		painter.drawRect( 0, 0, img.width(), img.height() );
        painter.setBrush( foreground );

        const int		s	=	qr->width > 0 ? qr->width : 1;
        const double	w	=	img.width();
        const double	h	=	img.height();
        const double	aspect	=	w / h;
        const double	scale	=	( (aspect > 1.0) ? h : w) / s;

		// 
        for( int y = 0; y < s; y++ )
		{
            const int	yy	=	y * s;	// temp
            
			for( int x = 0; x < s; x++ )
			{
                const int	xx	=	yy + x;
                const unsigned char	b	=	qr->data[xx];
                if( b & 0x01 )
				{
                    const double	rx1	=	x * scale, 
									ry1	=	y * scale;
                    QRectF	r( rx1, ry1, scale, scale );
                    painter.drawRects( &r, 1 );
                }
            }
        }

#ifdef _DEBUG
		img.save("QRcode.bmp");
#endif

		qr_img	=	img;
		
        QRcode_free(qr);
		qr	=	NULL;
    }
    else
	{
		assert(false);
    }

}



