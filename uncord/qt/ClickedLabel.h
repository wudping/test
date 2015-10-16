#ifndef CLICKED_LABEL_H
#define CLICKED_LABEL_H

#include <QLabel>



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************
	ClickedLabel
***********************************************************/
class ClickedLabel : public QLabel
{
Q_OBJECT
 
public:
    explicit ClickedLabel( const QString& text ="", QWidget * parent = 0 );
    ~ClickedLabel();
 
signals:
    void	clicked();
 
protected:
    void	mousePressEvent ( QMouseEvent * event );
};


#endif