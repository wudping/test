#include "status_widget.h"
#include "ui_status_widget.h"

#include "DataManager.h"


/**********************************************************
	constructor
***********************************************************/
StatusWidget::StatusWidget(QWidget *parent) 
	: QWidget(parent)
	, ui( new Ui::StatusWidget )
{
    ui->setupUi(this);

	download_speed_Label	=	ui->download_speed_Label;
	upload_speed_Label		=	ui->upload_speed_Label;

}


/**********************************************************
	destructor
***********************************************************/
StatusWidget::~StatusWidget()
{
    delete ui;
}




/**********************************************************
	refresh_device_list_slot
	目前寫法效能較差 之後修改
***********************************************************/
void	StatusWidget::refresh_device_list_slot()
{
	Uncord::DataManager*		p_data_manager	=	static_cast<Uncord::DataManager*>( Uncord::get_instance(Uncord::DATA_MANAGER) );
	deque<Uncord::DeviceInfo>	device_info		=	p_data_manager->get_device_info();

	int		i;

	ui->device_ComboBox->clear();
	for( i = 0; /*ui->device_ComboBox->count()*/ i < device_info.size(); i++ )
	{
		if( i >= 20 )
		{
			LOG_MESSAGE( 3, "list full." );
			break;
		} 

		QString		qstr( device_info[i].device_name.c_str() );
		ui->device_ComboBox->addItem(qstr);
	}
}
