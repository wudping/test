CONFIG	+=	QWT


# QWT source code

INCLUDEPATH	+=	QWT\include

Debug:LIBS	+=	QWT\lib\qwtd.lib \						# debug�ϥΪ�QWT lib
				QWT\lib\qwtmathmld.lib \

Release:LIBS	+=	QWT\lib\qwt.lib \					# release�ϥΪ�QWT lib
					QWT\lib\qwtmathml.lib 
					
LIBS	+=	QWT\lib\qwt.lib \
			QWT\lib\qwtd.lib \
			QWT\lib\qwtmathml.lib \
			QWT\lib\qwtmathmld.lib \
			
HEADERS	+=	QWT\include\*.h \
			#QWT\src\*.h
			
#SOURCES	+=	QWT\src\*.cpp


# �ۦ漶�g���u��

HEADERS	+=	QWT\curvdemo2.h \
			QWT\spectrogram.h \
			QWT\plot.h

SOURCES	+=	QWT\curvdemo2.cpp \
			QWT\plot.cpp \