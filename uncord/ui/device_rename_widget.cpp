#include "device_rename_widget.h"
#include "ui_device_rename_widget.h"

#include "UncordObject.h"


/**********************************************************
	constructor
***********************************************************/
DeviceRenameWidget::DeviceRenameWidget(QWidget *parent) 
	: QWidget(parent)
	, ui( new Ui::DeviceRenameWidget )
{
    ui->setupUi(this);

	cancel_Label	=	new ClickedLabel("Cancel", this );
	cancel_Label->setStyleSheet(	"QLabel { " \
									"color: #FFFFFF;" \
									"background-color: #00000000;" \
									"font: 75 16pt \"Lucida Grande\"; " \
									" } " );
	cancel_Label->setGeometry( 80, 200, 70, 25 ); 
	cancel_Label->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );

	confirm_Label	=	new ClickedLabel("Confirm", this );
	confirm_Label->setStyleSheet(	"QLabel {" \
									"color: #A95AE5;" \
									"background-color: #00000000;" \
									"font: 75 16pt \"Lucida Grande\"; " \
									" } " );
	confirm_Label->setGeometry( 200, 200, 90, 25 ); 
	confirm_Label->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );


	connect( cancel_Label, SIGNAL( clicked() ), Uncord::get_qt_obj_instance(Uncord::MAIN_WINDOW), SLOT( view_setting_slot() ) );
	connect( confirm_Label, SIGNAL( clicked() ), this, SLOT( confirm_name_slot() ) );

}


/**********************************************************
	destructor
***********************************************************/
DeviceRenameWidget::~DeviceRenameWidget()
{
    delete ui;
}



/**********************************************************
	confirm_name_slot
***********************************************************/
void	DeviceRenameWidget::confirm_name_slot()
{
	QString		qstr	=	ui->rename_TextEdit->toPlainText(); ;
	emit( change_name_sig(qstr) );
}




/**********************************************************
	showEvent
***********************************************************/
void 	DeviceRenameWidget::showEvent ( QShowEvent * e )
{
	ui->rename_TextEdit->clear();
}
