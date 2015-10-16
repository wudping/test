#include "qrcode_widget.h"
#include "ui_qrcode_widget.h"

#include <cassert>
#include "UncordObject.h"
#include "DataManager.h"


/**********************************************************
	constructor
***********************************************************/
QRcodeWidget::QRcodeWidget(QWidget *parent) 
	: QWidget(parent)
	, ui( new Ui::QRcodeWidget )
{
    ui->setupUi(this);

	cancel_Label	=	new ClickedLabel("Cancel", this );
	cancel_Label->setStyleSheet(	"QLabel {" \
									"color: #FFFFFF;" \
									"background-color: #00000000;" \
									"font: 75 16pt \"Lucida Grande\"; " \
									" } " );
	cancel_Label->setGeometry( 145, 300, 70, 25 ); 
	cancel_Label->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );

	bool	check	=	true;
	check	&=	static_cast<bool>(	connect(	cancel_Label,	SIGNAL( clicked() ),	Uncord::get_qt_obj_instance(Uncord::MAIN_WINDOW),	 SLOT( view_setting_slot() )	) );
	assert(check);
}



/**********************************************************
	destructor
***********************************************************/
QRcodeWidget::~QRcodeWidget()
{
    delete ui;
}




/**********************************************************
	showEvent
***********************************************************/
void 	QRcodeWidget::showEvent ( QShowEvent * event )
{
	Uncord::DataManager*	p_data_manager	=	static_cast<Uncord::DataManager*>( Uncord::get_instance(Uncord::DATA_MANAGER) );

	string	QR_pw	=	p_data_manager->generate_QR_pw();

	ui->password_Label->setText( QString("PASSWORD: %1").arg(QR_pw.c_str())  );
}



/**********************************************************
	destructor
***********************************************************/
void	QRcodeWidget::set_qrimg( QImage qr_img )
{
	ui->QRcode_Label->setPixmap(QPixmap::fromImage(qr_img));
}


/**********************************************************
	get_qrcode_width
***********************************************************/
int		QRcodeWidget::get_qrcode_width()	
{ 
	return ui->QRcode_Label->width(); 
}



/**********************************************************
	get_qrcode_height
***********************************************************/
int		QRcodeWidget::get_qrcode_height()	
{ 
	return ui->QRcode_Label->height(); 
}