#ifndef QR_CODE_GENERATOR_H
#define QR_CODE_GENERATOR_H

#include <QObject>
#include <QString>
#include <QImage>


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/**********************************************************
	QRCodeGenerator
***********************************************************/
class QRCodeGenerator : public QObject
{
    Q_OBJECT

public:

    QRCodeGenerator( QObject *parent = 0 );
	~QRCodeGenerator();

    void		set_QR_str( QString qstr );
	void		generate();
	QImage		get_img() { return qr_img; }

private:
    QString		qr_str;
	QImage		qr_img;

	const static int	default_width	=	300;
	const static int	default_height	=	300;
};




#endif