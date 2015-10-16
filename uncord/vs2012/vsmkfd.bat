: visual studio½sÄ¶¨Ï¥ÎªºbatÀÉ
if not exist ..\debug\ (
	mkdir ..\debug\
)
if not exist ..\debug\*.dll (
	copy ..\lib\win32\libcurl\lib\*.dll 		..\debug
	copy ..\lib\win32\sftp_lite\debug\*.dll 	..\debug
	copy ..\lib\win32\network_speed\debug\*.dll ..\debug
	copy ..\TUTK\win32\*.dll 					..\debug
)
qmake ../client.pro -r -spec win32-msvc2012
nmake debug
