#ifndef SETTING_WIDGET_H
#define SETTING_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include "ClickedLabel.h"
#include "deviceinfo_frame.h"


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ namespace ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace Ui {
	class SettingWidget;
}	// end namespace SettingWidget



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/**********************************************************
	SettingWidget
***********************************************************/
class	SettingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingWidget(QWidget *parent = 0);
    ~SettingWidget();
	
	QLabel	*download_speed_Label;		// 先亂塞...
	QLabel	*upload_speed_Label;

public slots:
	void	device_rename_slot( QString qstr );
	void	refresh_device_list_slot();
	void	set_rename_index_slot( int d );

signals:
	void	view_setting_sig();

private:
    Ui::SettingWidget	*ui;

	int		rename_index;

	QLabel	*QRcode_Label;

	ClickedLabel		*addDevice_Label;
	//ClickedLabel		*device_name_Label;

	QScrollArea			*deviceSArea;
	QWidget				*deviceWidget;
	DeviceInfoFrame		*device_list[20];		// 上限先設20


};

#endif // SETTING_WIDGET_H
