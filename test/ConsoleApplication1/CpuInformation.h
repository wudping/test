#ifndef FDR_CPU_INFORMATION_H
#define FDR_CPU_INFORMATION_H

#include <string>

using namespace std;

string	cpu_get_vender();		// 取得製造商
string	cpu_get_serial();		// 取得序號


/*
int i = system("wmic CPU get ProcessorID");    win7取得 CPU ProcessorID的作法

參考資料
http://home.educities.edu.tw/wanker742126/win32asm/w32asm_ap01.html   CPU ID 基本觀念 
http://en.wikipedia.org/wiki/CPUID#EAX.3D80000002h.2C80000003h.2C80000004h:_Processor_Brand_String  wiki CPU ID

http://stackoverflow.com/questions/14266772/how-do-i-call-cpuid-in-linux   未使用
http://zhidao.baidu.com/question/192966322.html  未確認  linux底下取得CPU ProcesserID的作法
http://www.linuxidc.com/Linux/2012-09/69817.htm 同上 
http://www.51testing.com/html/38/225738-236367.html 同上
http://stackoverflow.com/questions/1666093/cpuid-implementations-in-c  windows取得製造商的做法 
http://my.oschina.net/dream0303/blog/192375  asm 參考資料  未看完
http://blog.csdn.net/HackerJLY/article/details/4095373  windows取得CPU資料 目前是參考這篇   尚未釐清 03h的作用
http://zhidao.baidu.com/question/33366901.html  會連到其他資料  取得一些硬體資訊
http://msdn.microsoft.com/en-us/library/hskdteyh.aspx   MicroSoft的CPUID  但是code有編譯錯誤 去他的
*/



#endif