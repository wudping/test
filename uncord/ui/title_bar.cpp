#include "title_bar.h"
#include "ui_title_bar.h"

#include "UncordObject.h"
#include <QLibrary>

#include <cassert>


/**********************************************************
	Constructor
***********************************************************/
TitleBar::TitleBar(QWidget *parent) 
	: QFrame(parent)
	, ui( new Ui::TitleBar )
{
    ui->setupUi(this);

	bool	check	=	true;
	check	&=	static_cast<bool>(		connect(  ui->closeButton,		SIGNAL(clicked()),		parent,		SLOT(close())	)	);

	//connect( ui->comboBox, SIGNAL(currentIndexChanged(int)), Uncord::get_main_window_instance(), SLOT(disk_change_slot(int)) );
	connect( ui->comboBox, SIGNAL(currentIndexChanged(QString)), Uncord::get_qt_obj_instance(Uncord::MAIN_WINDOW), SLOT(disk_change_slot(QString)) );
	
	assert(check);

	QLibrary	sftp_server_dll("sftp_lite_api");
	dll_list_disk_part_t	get_disk_partition	=	(dll_list_disk_part_t)sftp_server_dll.resolve("get_disk_partition");

	list<string>	disk_part_list;

	if( get_disk_partition )
	{
		get_disk_partition( disk_part_list );

		list<string>::iterator	itr;
		//ui->comboBox->clear();
		for( itr = disk_part_list.begin(); itr != disk_part_list.end(); ++itr )
		{
			if( *itr == "C:\\" )
			{}
			else
				ui->comboBox->addItem( QString( itr->c_str() ) );
		}
		ui->comboBox->setCurrentIndex(0);

	}
	else
		BOOST_ASSERT(false);

}



/**********************************************************
	Desturctor
***********************************************************/
TitleBar::~TitleBar()
{
    delete ui;
}


/**********************************************************
	mousePressEvent
***********************************************************/
void	TitleBar::mousePressEvent( QMouseEvent *e )
{
    startPos	=	e->globalPos();   
	clickPos	=	mapToParent( e->pos() );
}



/**********************************************************
	mousePressEvent
***********************************************************/
void	TitleBar::mouseMoveEvent( QMouseEvent *e )
{
	// 注意移出螢幕解析度外的問題!!
	parentWidget()->move( e->globalPos() - clickPos );
}