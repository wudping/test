#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "ThreadManager.h"

#include <QMessageBox>
#include <QTimer>
#include <QRCodeGenerator.h>
#include <QTcpServer>

#include <curl/curl.h>


#ifndef Q_MOC_RUN
#include <boost/thread.hpp>
#endif


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ extern global point ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern "C" {
	extern	Uncord::UncordObject	*p_uncord_obj;				// 為了方便function取用資料 使用上請小心. 原則上只讓mainwindow來設定這些指標 不讓外部取用.
	extern	Uncord::DataManager		*p_data_manager;

	QObject		*p_main_window		=	NULL;
	QObject		*p_setting_widget	=	NULL;
}	// end extern "C"




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/**********************************************************
	constructor
***********************************************************/
MainWindow::MainWindow( QWidget *parent ) 
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, p_thread_manager(NULL)
{
	curl_global_cleanup();		// for test
	curl_global_init(CURL_GLOBAL_ALL);

	p_main_window	=	this;		// global 變數

	status_label	=	NULL;
	setting_label	=	NULL;

	status_widget	=	NULL;
	setting_widget	=	NULL;
	qrcode_widget	=	NULL;

	title_bar		=	NULL;

	//
	p_thread_manager	=	new ThreadManager();

	set_port();		// need before init.  因為會在init的時候同時啟動server服務.  (除非將這件事情往後延)

	//
    setWindowFlags(Qt::FramelessWindowHint);
    setMouseTracking(true);

	ui->setupUi(this);

	//
	init_ui_obj();

	set_instance();		
	init();
	set_connect();
}




/**********************************************************
	destructor
***********************************************************/
MainWindow::~MainWindow()
{
	curl_global_cleanup();

	delete status_label;
	delete setting_label;

    delete ui;
}






/**********************************************************
	set_port
***********************************************************/
void	MainWindow::set_port()
{
	// 掃描可用port. 要注意, range如果有重複會造成bug.
	int		port;
	// sftp server
	port	=	search_port( 9022, 9123 );
	uncord_obj.set_port( port, Uncord::SFTP_SERVER );
	// async server
	port	=	search_port( 10080, 10181 );
	uncord_obj.set_port( port, Uncord::ASYNC_HTTP_SERVER );
	// http server
	port	=	search_port( 11080, 11181 );
	uncord_obj.set_port( port, Uncord::HTTP_SERVER );
}




/**********************************************************
	search_port
	這邊有一個缺點
	假如在掃描port到server啟動之前,有其他程式占用了port
	則會造成bug.
	機率低,暫不處理
***********************************************************/
int		MainWindow::search_port( int start, int end )
{
	// 用tcp server檢查port
	int				port;
	QTcpServer		tcp_server;
	QHostAddress	qaddress( QString("127.0.0.1") );	// 因為tunnel綁 127.0.0.1

	for( port = start; port <= end; port++ )
	{
		if( tcp_server.listen( qaddress, port ) == true )
			break;
	}

	tcp_server.close();
	return	port;
}



/**********************************************************
	init_ui_obj
***********************************************************/
void	MainWindow::init_ui_obj()
{
	// add title bar
	title_bar	=	new TitleBar(this);

	// add status widget
	status_widget	=	new StatusWidget( ui->main_Widget );
	status_widget->move( 0, 50 );
	
	// add setting widget
	setting_widget	=	new	SettingWidget( ui->main_Widget );
	setting_widget->move( 0, 50 );
	setting_widget->setVisible(false);
	p_setting_widget	=	setting_widget;

	// add qrcode widget
	qrcode_widget	=	new QRcodeWidget( ui->main_Widget );
	qrcode_widget->move( 0, 50 );
	qrcode_widget->setVisible(false);

	// add device rename widget
	device_rename_widget	=	new DeviceRenameWidget( ui->main_Widget );
	device_rename_widget->move( 0, 50 );
	device_rename_widget->setVisible(false);

	// add status label
	status_label	=	new ClickedLabel( "Status", this );
	status_label->setStyleSheet(	"QLabel {" \
									"color: #A95AE5;" \
									"background-image: url(:/mainwindow/bar_status.png);" \
									"font: 75 13pt \"Lucida Grande\"; " \
									" } "  );
	status_label->setGeometry( 0, 118, 75, 23 );	// 未來看要不要改成動態設置
	status_label->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );

	setting_label	=	new ClickedLabel( "Setting", this );
	setting_label->setStyleSheet(	"QLabel {" \
									"color: #FFFFFF;" \
									"background-image: url(:/mainwindow/bar_setting.png);" \
									"font: 75 13pt \"Lucida Grande\" ; " \
									" } " );
	setting_label->setGeometry( 75, 118, 75, 23 ); 
	setting_label->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
}

/**********************************************************
	view_qr_code_slot
***********************************************************/
void	MainWindow::view_qr_code_slot()
{
	status_widget->setVisible(false);
	setting_widget->setVisible(false);
	qrcode_widget->setVisible(true);
	device_rename_widget->setVisible(false);
}



/**********************************************************
	view_device_rename_slot
***********************************************************/
void	MainWindow::view_device_rename_slot()
{
	status_widget->setVisible(false);
	setting_widget->setVisible(false);
	qrcode_widget->setVisible(false);
	device_rename_widget->setVisible(true);
}



/**********************************************************
	view_setting_slot
	note: 物件一多  改用map
***********************************************************/
void	MainWindow::view_setting_slot()
{
	status_widget->setVisible(false);
	setting_widget->setVisible(true);
	qrcode_widget->setVisible(false);
	device_rename_widget->setVisible(false);
}

/**********************************************************
	view_setting_slot
***********************************************************/
void	MainWindow::view_status_slot()
{
	status_widget->setVisible(true);
	setting_widget->setVisible(false);
	qrcode_widget->setVisible(false);
	device_rename_widget->setVisible(false);
}


/**********************************************************
	status_clicked_slot
***********************************************************/
void	MainWindow::status_clicked_slot()
{
	status_label->setStyleSheet(	"QLabel {" \
									"color: #A95AE5;" \
									"background-image: url(:/mainwindow/bar_status.png);" \
									"font: 75 13pt \"Lucida Grande\"; " \
									" } " );
	setting_label->setStyleSheet(	"QLabel {" \
									"color: #FFFFFF;" \
									"background-image: url(:/mainwindow/bar_setting.png);" \
									"font: 75 13pt \"Lucida Grande\" ;" \
									" } " );
	// 
	view_status_slot();
}




/**********************************************************
	status_clicked_slot
***********************************************************/
void	MainWindow::setting_clicked_slot()
{
	status_label->setStyleSheet(	"QLabel {" \
									"color: #FFFFFF;" \
									"background-image: url(:/mainwindow/bar_status.png);" \
									"font: 75 13pt \"Lucida Grande\" ; " \
									" } " );
	setting_label->setStyleSheet(	"QLabel { " \
									"color: #A95AE5;" \
									"background-image: url(:/mainwindow/bar_setting.png);" \
									"font: 75 13pt \"Lucida Grande\" ; " \
									" } " );
	//
	view_setting_slot();
}




/**********************************************************
	init
***********************************************************/
void	MainWindow::init()
{
	if( p_thread_manager == NULL )
		BOOST_ASSERT(false);

	uncord_obj.set_thread_manager( p_thread_manager );
	uncord_obj.init();	

	data_manager.init_account();		// 測試用的  create帳號  必須放在init instance之後.
	uncord_obj.get_UID();				// 必須在創建帳號後

	// set timer, 定期去更新上傳/下載速度.
	QTimer	*network_speed_timer	=	new QTimer(this);
    connect( network_speed_timer, SIGNAL(timeout()), this, SLOT(update_network_speed_slot()));
    network_speed_timer->start(1000);
}


/**********************************************************
	set_instance
***********************************************************/
void	MainWindow::set_instance()
{
	p_uncord_obj	=	&uncord_obj;
	p_data_manager	=	&data_manager;
}


/**********************************************************
	disk_change_slot
***********************************************************/
void	MainWindow::disk_change_slot( QString qstr )
{
	uncord_obj.set_root( qstr.toStdString() );
}




/**********************************************************
	update_network_speed_slot
***********************************************************/
void	MainWindow::update_network_speed_slot()
{
	float	download_value	=	uncord_obj.get_download_value();
	float	upload_value	=	uncord_obj.get_upload_value();

	// download value
	if( download_value < 0 )
		status_widget->download_speed_Label->setText( QString("-- KBs") );
	else
	{
		download_value	/=	1024;
		if( download_value > 1024 )
		{
			download_value /= 1024;
			download_value = (int)download_value;
			status_widget->download_speed_Label->setText( QString("%1 MBs").arg(download_value) );
		}
		else
		{
			download_value = (int)download_value;
			status_widget->download_speed_Label->setText( QString("%1 KBs").arg(download_value) );
		}

	}
	// upload value
	if( upload_value < 0 )
		status_widget->upload_speed_Label->setText( QString("-- KBs") );
	else
	{
		upload_value	/=	1024;
		if( upload_value > 1024 )
		{
			upload_value /= 1024;
			upload_value = (int)upload_value;
			status_widget->upload_speed_Label->setText( QString("%1 MBs").arg(upload_value) );
		}
		else
		{
			upload_value = (int)upload_value;
			status_widget->upload_speed_Label->setText( QString("%1 KBs").arg(upload_value) );
		}
	}
}




/**********************************************************
	set_connect
***********************************************************/
void	MainWindow::set_connect()
{
	bool	check	=	true;
	check	&=	(bool)(		connect(	&uncord_obj,			SIGNAL( get_uid_sig() ),				this,				SLOT( get_uid_slot() )					)	);
	check	&=	(bool)(		connect(	status_label,			SIGNAL( clicked() ),					this,				SLOT( status_clicked_slot() )			)	);
	check	&=	(bool)(		connect(	setting_label,			SIGNAL( clicked() ),					this,				SLOT( setting_clicked_slot() )			)	);
	check	&=	(bool)(		connect(	device_rename_widget,	SIGNAL( change_name_sig(QString) ),		setting_widget,		SLOT( device_rename_slot(QString) )		)	);
	check	&=	(bool)(		connect(	setting_widget,			SIGNAL( view_setting_sig() ),			this,				SLOT( view_setting_slot() )				)	);
	check	&=	(bool)(		connect(	&uncord_obj,			SIGNAL( refresh_device_list_sig() ),	setting_widget,		SLOT( refresh_device_list_slot() )		)	);
	check	&=	(bool)(		connect(	&uncord_obj,			SIGNAL( refresh_device_list_sig() ),	status_widget,		SLOT( refresh_device_list_slot() )		)	);
	check	&=	(bool)(		connect(	&uncord_obj,			SIGNAL( refresh_device_list_sig() ),	this,				SLOT( refresh_device_list_slot() )		)	);

	assert(check);
}



/**********************************************************
	refresh_device_list_slot
***********************************************************/
void	MainWindow::refresh_device_list_slot()
{
	view_status_slot();
}




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MainWindow slots ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************
	set_connect
***********************************************************/
void	MainWindow::get_uid_slot()
{
	// 產生 QR code
	QRCodeGenerator		qr_generator;
	QString				qr_str	=	QString::fromStdString( data_manager.get_UID() );
	
	LOG_MESSAGE( 3, qr_str.toStdString().c_str() );

	qr_generator.set_QR_str( qr_str );
	qr_generator.generate();
																
	int		w		=	qrcode_widget->get_qrcode_width();		
	int		h		=	qrcode_widget->get_qrcode_height();		
	QImage	qr_img	=	qr_generator.get_img().scaled( w, h );

	Uncord::UncordObject	*p_uncord_obj	=	static_cast<Uncord::UncordObject*>( Uncord::get_instance(Uncord::UNCORD_OBJ) );
	p_uncord_obj->start_tutk_tunnel();

	// 將qr img丟入UI
	qrcode_widget->set_qrimg( qr_img );
}
