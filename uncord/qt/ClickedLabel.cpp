#include "ClickedLabel.h"



/**********************************************************
	constructor
***********************************************************/
ClickedLabel::ClickedLabel( const QString& text, QWidget * parent ) 
	: QLabel(parent)
{
	this->setText(text);
}
 

/**********************************************************
	destructor
***********************************************************/
ClickedLabel::~ClickedLabel()
{}
 

/**********************************************************
	mousePressEvent
***********************************************************/
void ClickedLabel::mousePressEvent ( QMouseEvent * event ) 
{
	emit clicked();
}
