/**************************************************************************************************************************

	��output��������b�o��

/***************************************************************************************************************************/

#ifndef OUTPUT_H
#define OUTPUT_H

#include <QImage>
#include <QPainter>
#include <QSvgGenerator>
#include <QSvgRenderer>


class	Output
{
	private:


	public:
		Output();
		~Output();

		void	outputSVG( QImage imagePass, QString filename );				// ���Ϳ�X��SVG�ɮ�

};




#endif // OUTPUT_H
