#ifndef STATUS_WIDGET_H
#define STATUS_WIDGET_H

#include <QWidget>
#include <qlabel.h>



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ namespace ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace Ui {
	class StatusWidget;
}	// end namespace StatusWidget




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/**********************************************************
	StatusWidget
***********************************************************/
class	StatusWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatusWidget( QWidget *parent = 0 );
    ~StatusWidget();

	QLabel	*download_speed_Label;		// ¥ý¶Ã¶ë...
	QLabel	*upload_speed_Label;

public slots:
	void	refresh_device_list_slot();

private:
    Ui::StatusWidget	*ui;
};

#endif // STATUS_WIDGET_H
