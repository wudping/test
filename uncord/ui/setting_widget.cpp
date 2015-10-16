#include "setting_widget.h"
#include "ui_setting_widget.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QStyleFactory>
#include <QScrollbar>

#include "DataManager.h"
#include "UncordObject.h"


/**********************************************************
	SettingWidget
***********************************************************/
SettingWidget::SettingWidget( QWidget *parent ) 
	: QWidget(parent)
	, ui( new Ui::SettingWidget )
	, rename_index(-1)
{
	int		i;

	for( i = 0; i < 20; i++ )
		device_list[i]	=	NULL;

	QRcode_Label	=	NULL;

    ui->setupUi(this);

	addDevice_Label	=	new ClickedLabel( "" , ui->mainWidget );
	QPixmap pixmap	=	QPixmap (":/mainwindow/edit_adddevice.png");
	addDevice_Label->setPixmap( pixmap );
	addDevice_Label->setGeometry( 130, 250, 100, 100 ); 

	// 似乎只能按照這樣的順序 不然scrollbar出不來@@
	deviceWidget	=	new QWidget;
	deviceWidget->setGeometry( 0, 0, 320, 140 );

	deviceSArea	=	new QScrollArea( ui->mainWidget );
	//deviceSArea->setBackgroundRole(QPalette::Dark);
	deviceSArea->setGeometry( 10, 70, 340, 160 );
	deviceSArea->setFrameShape( QFrame::NoFrame );
	//QApplication::setStyle(QStyleFactory::create("Fusion"));
	//deviceSArea->setStyle( QStyleFactory::create("macintosh") );
	//deviceSArea->setStyle( QStyleFactory::create("transparent") );

	//deviceSArea->setWidget(w);
	deviceSArea->setVerticalScrollBarPolicy( Qt::ScrollBarPolicy::ScrollBarAsNeeded );
	deviceSArea->setWidget( deviceWidget );

	connect( addDevice_Label, SIGNAL( clicked() ), Uncord::get_qt_obj_instance(Uncord::MAIN_WINDOW), SLOT( view_qr_code_slot() ) );

}




/**********************************************************
	SettingWidget
***********************************************************/
SettingWidget::~SettingWidget()
{
    delete ui;
}




/**********************************************************
	device_rename_slot
***********************************************************/
void	SettingWidget::device_rename_slot( QString qstr )
{
	std::string	str	=	qstr.toStdString();		// 未來再做全形字的支援

	bool	flag	=	false;;
	std::string::iterator	itr;
	for( itr = str.begin(); itr != str.end(); ++itr )
	{
		if( *itr != ' ' )
		{
			flag	=	true;
			break;
		}
	}

	if( flag && rename_index >= 0 )
	{
		assert( rename_index < 20 );
		//device_name_Label->setText( qstr );
		//device_list[rename_index]->set_device_name( qstr );
		
		// 將資料寫入DataManager的device_info
		Uncord::DataManager		*p_data_manager		=	(Uncord::DataManager*)Uncord::get_instance( Uncord::DATA_MANAGER );
		p_data_manager->set_device_name( rename_index, qstr.toStdString() );

		// refresh
		Uncord::UncordObject*		p_uncord_obj	=	static_cast<Uncord::UncordObject*>( Uncord::get_instance(Uncord::UNCORD_OBJ) );
		p_uncord_obj->refresh_device_list();
	}

	//emit( view_setting_sig() );
}



/**********************************************************
	set_rename_index_slot
***********************************************************/
void	SettingWidget::set_rename_index_slot( int d )
{
	rename_index	=	d;
}




/**********************************************************
	refresh_device_list_slot
	這邊效能不好 未來再修改
	
	//device_list[0]->deleteLater();	// 移除的方法
	//delete device_list[0];
***********************************************************/
void	SettingWidget::refresh_device_list_slot()
{
	bool	need_set_visible	=	false;
	if( this->isVisible() )
	{
		this->setVisible(false);	// 目前不這樣做就不能同步更新畫面@@  未來找其他方式
		need_set_visible	=	true;
	}

	Uncord::DataManager*		p_data_manager	=	static_cast<Uncord::DataManager*>( Uncord::get_instance(Uncord::DATA_MANAGER) );
	deque<Uncord::DeviceInfo>	device_info		=	p_data_manager->get_device_info();

	int		i;

	//list<Uncord::DeviceInfo>::iterator	itr;
	//for( i = 0, itr = device_info.begin(); itr != device_info.end(); ++itr, i++ );
	for( i = 0; i < device_info.size(); i++ )
	{
		if( i >= 20 )
		{
			LOG_MESSAGE( 3, "list full." );
			break;
		}

		if( device_list[i] != NULL )
		{
			device_list[i]->set_device_name( QString(device_info[i].device_name.c_str()) );
			continue;	// 已經建立的 跳過.  
		}

		QString		qstr( device_info[i].device_name.c_str() );
		device_list[i]	=	new DeviceInfoFrame( i, qstr, deviceWidget );
		device_list[i]->move( 30, 10 + i*40 );
	}

	if( i*40 + 10 > deviceWidget->height() )
		deviceWidget->resize( 320, i*40 + 10 );

	if( need_set_visible ) 
		this->setVisible(true);
}
