#ifndef QRCODE_WIDGET_H
#define QRCODE_WIDGET_H

#include <QWidget>
#include <QImage>

#include "ClickedLabel.h"


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class declared ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace Ui {
	class QRcodeWidget;
}	// end namespace Ui




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/**********************************************************
	QRcodeWidget
***********************************************************/
class QRcodeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QRcodeWidget(QWidget *parent = 0);
    ~QRcodeWidget();

	int		get_qrcode_width();	
	int		get_qrcode_height();	

	void	set_qrimg( QImage qr_img );

protected:
	void 	showEvent ( QShowEvent * event );

private:
    Ui::QRcodeWidget	*ui;

	ClickedLabel	*cancel_Label;

};

#endif // QRCODE_WIDGET_H
