: visual studio½sÄ¶¨Ï¥ÎªºbatÀÉ
if not exist ..\release\ (
	mkdir ..\release\
)
if not exist ..\release\*.dll (
	copy ..\lib\win32\libcurl\lib\*.dll 			..\release
	copy ..\lib\win32\sftp_lite\release\*.dll 		..\release
	copy ..\lib\win32\network_speed\release\*.dll 	..\release
	copy ..\TUTK\win32\*.dll 						..\release
)
qmake ../client.pro -r -spec win32-msvc2012
nmake release
