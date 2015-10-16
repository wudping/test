#ifndef TITLE_BAR_H
#define TITLE_BAR_H

#include <QFrame>
#include <QMouseEvent>
#include <stdio.h>

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ namespace ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

namespace Ui {
	class TitleBar;
}	// end namespace Ui



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ typedef ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef void (*dll_list_disk_part_t)( std::list<std::string>& );




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************
	TitleBar
***********************************************************/
class TitleBar : public QFrame
{
    Q_OBJECT

public:
    explicit TitleBar(QWidget *parent = 0);
    ~TitleBar();

protected:
    void	mousePressEvent( QMouseEvent *e );
    void	mouseMoveEvent( QMouseEvent *e );

private:
    Ui::TitleBar *ui;

	QPoint	startPos;
    QPoint	clickPos;

};

#endif // TITLE_BAR_H
