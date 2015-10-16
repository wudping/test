#ifndef DEVICEINFO_FRAME_H
#define DEVICEINFO_FRAME_H

#include <QFrame>


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ namespace ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace Ui {
	class DeviceInfoFrame;
}	// end namespace Ui



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/**********************************************************
	DeviceInfoFrame
***********************************************************/
class DeviceInfoFrame : public QFrame
{
    Q_OBJECT

public:
    explicit DeviceInfoFrame( int _index, QString _device_name, QWidget *parent = 0 );
    ~DeviceInfoFrame();

	void	set_device_name( QString qstr );
	//void	set_text( QString qstr );

public slots:
	void	rename_slot();

signals:
	void	rename_sig(int);

private:
    Ui::DeviceInfoFrame		*ui;

	int			index;
	QString		device_name;

};

#endif // DEVICEINFO_FRAME_H
