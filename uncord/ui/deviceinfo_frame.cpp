#include "deviceinfo_frame.h"
#include "ui_deviceinfo_frame.h"

#include "UncordObject.h"


/**********************************************************
	constructor
***********************************************************/
DeviceInfoFrame::DeviceInfoFrame( int _index, QString _device_name, QWidget *parent ) 
    : QFrame(parent)
	, ui( new Ui::DeviceInfoFrame )
	, index(_index)
	, device_name(_device_name)
{
    ui->setupUi(this);

	ui->deviceName_Label->setText( device_name );

	connect( ui->renameButton, SIGNAL( clicked() ), this, SLOT( rename_slot() ) );
	connect( this, SIGNAL( rename_sig(int) ), Uncord::get_qt_obj_instance(Uncord::MAIN_WINDOW), SLOT( view_device_rename_slot() ) );
	connect( this, SIGNAL( rename_sig(int) ), Uncord::get_qt_obj_instance(Uncord::SETTING_WIDGET), SLOT( set_rename_index_slot(int) ) );


}


/**********************************************************
	destructor
***********************************************************/
DeviceInfoFrame::~DeviceInfoFrame()
{
    delete ui;
}


/**********************************************************
	rename_slot
***********************************************************/
void	DeviceInfoFrame::rename_slot()
{
	emit( rename_sig(index) );
}




/**********************************************************
	set_device_name
***********************************************************/
void	DeviceInfoFrame::set_device_name( QString qstr )
{
	ui->deviceName_Label->setText( qstr );
}
