#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "UncordObject.h"
#include "DataManager.h"

#include "ClickedLabel.h"
#include "status_widget.h"
#include "setting_widget.h"
#include "title_bar.h"
#include "qrcode_widget.h"
#include "device_rename_widget.h"


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class declared ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace Ui 
{
	class MainWindow;
} // end namespace Ui

class	ThreadManager;




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/**********************************************************
	MainWindow
***********************************************************/
class MainWindow : public QMainWindow 
{
    Q_OBJECT

public slots:
    void	get_uid_slot();
	void	update_network_speed_slot();
	void	refresh_device_list_slot();

	void	status_clicked_slot();
	void	setting_clicked_slot();
	void	disk_change_slot(QString);

	void	view_status_slot();
	void	view_qr_code_slot();
	void	view_setting_slot();
	void	view_device_rename_slot();

signals:

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

	// 
	void	init();
	void	init_ui_obj();

	int		search_port( int start, int end );

private:
    Ui::MainWindow *ui;

	// obj  
	Uncord::UncordObject	uncord_obj;
	Uncord::DataManager		data_manager;

	ThreadManager			*p_thread_manager;

	// function
	void	set_instance();
	void	set_connect();
	void	set_port();

	// 自訂義的介面 物件等等
	ClickedLabel		*status_label;
	ClickedLabel		*setting_label;

	StatusWidget		*status_widget;
	SettingWidget		*setting_widget;
	QRcodeWidget		*qrcode_widget;
	DeviceRenameWidget	*device_rename_widget;

	TitleBar			*title_bar;

};





#endif // MAINWINDOW_H
