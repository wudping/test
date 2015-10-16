#-------------------------------------------------
#
# Project created by QtCreator 2014-09-30T15:51:16
#
#-------------------------------------------------

# http://qt-project.org/doc/qt-4.8/qmake-advanced-usage.html
#C:\Ruby21-x64\bin\;D:\code\QT5\qtbase\bin\;D:\code\zlib\zlib_bin\Debug\;C:\OpenSSL_Win32\;D:\code\libcurl\build\Win32\VC11\Debug\;D:\code\MPFDParser\mpfd_bin\Debug\;D:\code\libcurl\build\Win32\VC11\Debug\;D:\code\Uncord_client\client\TUTK\win32\;D:\code\libssh\libssh_bin\src\Debug\;D:\code\sftp_lite\Sample\bin\Debug\

win32{ message("this is win32 test...") }
win64{ message("this is win64 test...") }
macos{ message("this is macos test...") }
unix:!macos{ message("this is unix test...") }

QT	+=	core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32:Debug:DESTDIR		=	../debug/

win32:Release:DESTDIR	=	../release/

TARGET	 = Uncord

# use for command line version.
#QT -= gui
#CONFIG += console
#CONFIG -= app_bundle

TEMPLATE = app

win32 {
	DEFINES	+=	_CRT_SECURE_NO_WARNINGS \
				_SCL_SECURE_NO_WARNINGS \
				_WIN32_WINNT=0x0501 \
				_CONSOLE \
				WIN32
}

win32:Release:DEFINES	+=	NDEBUG 

unix:!macos {
	DEFINES 	+=	UNIX 
}

macos {
	DEFINES 	+=	MACOS
}

DEFINES		+=	QT_DLL USE_QT

#DEPENDPATH	+=	. debug src ui share\icons share\images release BLOB MEASURE QWT

INCLUDEPATH +=	./src/ \
                ./qt/ \
				./ui/

win32 {
	INCLUDEPATH	+=	..\lib\win32\libcurl\include \
					..\lib\win32\qrencode\include \
					..\lib\win32\netlib\include \
					..\TUTK \
					..\src 
					C:\Boost\include\boost-1_56 
}

#INCLUDEPATH   +=  /Users/hidog/code/boost

win32:Debug:LIBPATH	+=	..\lib\win32\libcurl\lib \
						..\lib\win32\qrencode\lib\debug \
						..\lib\win32\netlib\lib\debug \
						..\TUTK\win32 \
						C:\Boost\lib \
						C:\OpenSSL_Win32\lib\VC

win32:Release:LIBPATH	+=	..\lib\win32\libcurl\lib \
							..\lib\win32\qrencode\lib\release \
							..\lib\win32\netlib\lib\release \
							..\TUTK\win32 \
							C:\Boost\lib \
							C:\OpenSSL_Win32\lib\VC

#LIBPATH       +=  /Users/hidog/code/boost/stage/lib


win32:Debug:LIBS	+=	libcurld.lib \
						qrenc.lib \
						cppnetlib-client-connections.lib \
						cppnetlib-server-parsers.lib \
						cppnetlib-uri.lib \
						IOTCAPIs.lib \
						RDTAPIs.lib \
						P2PTunnelAPIs.lib \
						libeay32MDd.lib \
						ssleay32MDd.lib


win32:Release:LIBS	+=	libcurl.lib \
						qrenc.lib \
						cppnetlib-client-connections.lib \
						cppnetlib-server-parsers.lib \
						cppnetlib-uri.lib \
						IOTCAPIs.lib \
						RDTAPIs.lib \
						P2PTunnelAPIs.lib \
						libeay32MDd.lib \
						ssleay32MDd.lib

SOURCES	+=	src/UncordObject.cpp \
			src/UncordBase.cpp \
			src/NetBase.cpp \
			src/CpuInformation.cpp \
			src/DataManager.cpp \
			src/TUTK_P2PTunnel.cpp \
			src/NetBase.cpp \
			src/NetWorkSpeed.cpp \
			src/client/NetClientBase.cpp \
			src/client/HttpClient.cpp \
			src/server/HttpServer.cpp \
			src/server/HttpServer_priv.cpp \
			src/server/NetServerBase.cpp \
			src/server/SFtpServer.cpp \
			src/server/AsyncHttpServer.cpp \
			src/server/AsyncHttpServer_priv.cpp \
			src/server/mmap_windows.cpp \
			qt/ThreadManager.cpp \
			qt/Thread.cpp \
			qt/QRCodeGEnerator.cpp \
			qt/ClickedLabel.cpp \
			ui/title_bar.cpp \
			ui/status_widget.cpp \
			ui/setting_widget.cpp \
			ui/qrcode_widget.cpp \
			ui/device_rename_widget.cpp \
			ui/deviceinfo_frame.cpp \
			ui/mainwindow.cpp \
			src/main.cpp 

			
HEADERS +=	src/UncordObject.h \
			src/UncordBase.h \
			src/NetBase.h \
			src/CpuInformation.h \
			src/DataManager.h \
			src/TUTK_P2PTunnel.h \
			src/NetBase.h \
			src/NetWorkSpeed.h \
			src/client/NetClientBase.h \
			src/client/HttpClient.h \
			src/server/HttpServer.h \
			src/server/HttpServer_priv.h \
			src/server/NetServerBase.h \
			src/server/SFtpServer.h \
			src/server/AsyncHttpServer.h \
			src/server/AsyncHttpServer_priv.h \
			src/server/mmap_windows.h \
			qt/ThreadManager.h \
			qt/Thread.h \
			qt/QRCodeGEnerator.h \
			qt/ClickedLabel.h \ 
			ui/title_bar.h \
			ui/status_widget.h \
			ui/setting_widget.h \
			ui/qrcode_widget.h \
			ui/device_rename_widget.h \
			ui/deviceinfo_frame.h \
			ui/mainwindow.h \
			src/common.h 

			
FORMS   += 	ui/mainwindow.ui \ 
			ui/title_bar.ui \
			ui/status_widget.ui \
			ui/setting_widget.ui \
			ui/qrcode_widget.ui \
			ui/device_rename_widget.ui \
			ui/deviceinfo_frame.ui 

RESOURCES +=	resource/title_bar.qrc \
				resource/mainwindow.qrc

OTHER_FILES += icon.rc
RC_FILE += icon.rc		# program icon.  mac不知道是否通用.  目前聽說要用不同的方式   see http://qt-project.org/doc/qt-4.8/appicon.html   http://213style.blogspot.tw/2013/03/qt-icon-change.html