#ifndef DEVICE_RENAME_WIDGET_H
#define DEVICE_RENAME_WIDGET_H

#include <QWidget>
#include <ClickedLabel.h>



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class declared ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace Ui {
	class DeviceRenameWidget;
}	// end namespace Ui



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/**********************************************************
	DeviceRenameWidget
***********************************************************/
class DeviceRenameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceRenameWidget(QWidget *parent = 0);
    ~DeviceRenameWidget();

public slots:
	void	confirm_name_slot();

signals:
	void	change_name_sig(QString);

private:
    Ui::DeviceRenameWidget *ui;

	// events
	void 	showEvent ( QShowEvent * e );

	ClickedLabel	*cancel_Label;
	ClickedLabel	*confirm_Label;
};

#endif // DEVICE_RENAME_WIDGET_H



#if 0

/**********************************************************
	SettingWidget
***********************************************************/
void	SettingWidget::confirm_name()
{
	//QString		str		=	ui->textEdit_2->toPlainText();
	QString		str		=	ui->textEdit_2->toPlainText();     //device_name_Label->text();
	//std::string		strr	=	str.toStdString();

	printf( "%s", str.toStdString().c_str() );

	bool	is	=	false;
	//QString:iterator	itr;
	//for( itr = str.begin(); itr != str.end(); ++itr )
	for( int i = 0; i < str.length(); i++ )
	{
		//if( *itr != std::string(" ") && *itr != std::string("¡@") )
		if( str.at(i) != QString(" ") && str.at(i) != QString("¡@") )
		{
			is	=	true;
			break;
		}
	}

	if( is )
	{
		//<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd">\n<html><head><meta name="qrichtext" content="1" /><style type="text/css">\np, li { white-space: pre-wrap; }\n</style></head><body style=" font-family:'Lucida Grande'; font-size:9pt; font-weight:72; font-style:normal;">\n			
			//<p style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;">
			//<span style=" font-size:14pt; font-weight:600; color:#ffffff;">Nexus 5</span>    </p></body></html>

		//	status_label	=	new MyLabel( "Status", this );
	//status_label->setStyleSheet( "QLabel { color: #A95AE5; background-image: url(:/bar_1.png); font: 75 13pt \"Lucida Grande\" }" );
	//status_label->setGeometry( 0, 120, 75, 23 ); 
	//status_label->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );

		device_name_Label->setText(str);

//		ui->textEdit->setPlainText( str );
	//	ui->textEdit->setStyleSheet( "QLabel { color: #A95AE5; background-color: #00000000; font: 75 14pt \"Lucida Grande\"; }" );
	}
	view_setting();
}
#endif


