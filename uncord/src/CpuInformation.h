#ifndef CPU_INFORMATION_H
#define CPU_INFORMATION_H

#include <string>

using namespace std;

string	cpu_get_vender();		// ���o�s�y��
string	cpu_get_serial();		// ���o�Ǹ�


/*
int i = system("wmic CPU get ProcessorID");    win7���o CPU ProcessorID���@�k

ubuntu command
CPU ID
sudo dmidecode -t 4 | grep ID
MB
sudo dmidecode -t 2 | grep Serial
MAC
sudo lshw -c network | grep serial | head -n 1

�ѦҸ��
http://home.educities.edu.tw/wanker742126/win32asm/w32asm_ap01.html   CPU ID ���[�� 
http://en.wikipedia.org/wiki/CPUID#EAX.3D80000002h.2C80000003h.2C80000004h:_Processor_Brand_String  wiki CPU ID

http://stackoverflow.com/questions/14266772/how-do-i-call-cpuid-in-linux   ���ϥ�
http://zhidao.baidu.com/question/192966322.html  ���T�{  linux���U���oCPU ProcesserID���@�k
http://www.linuxidc.com/Linux/2012-09/69817.htm �P�W 
http://www.51testing.com/html/38/225738-236367.html �P�W
http://stackoverflow.com/questions/1666093/cpuid-implementations-in-c  windows���o�s�y�Ӫ����k 
http://my.oschina.net/dream0303/blog/192375  asm �ѦҸ��  ���ݧ�
http://blog.csdn.net/HackerJLY/article/details/4095373  windows���oCPU��� �ثe�O�Ѧҳo�g   �|����M 03h���@��
http://zhidao.baidu.com/question/33366901.html  �|�s���L���  ���o�@�ǵw���T
http://msdn.microsoft.com/en-us/library/hskdteyh.aspx   MicroSoft��CPUID  ���Ocode���sĶ���~ �h�L��
http://www.linuxidc.com/Linux/2012-09/69817.htm  ubuntu command
http://www.codeproject.com/Questions/437627/Processor-serial-number
http://www.codeproject.com/Questions/437379/Cplusplus-source-code-to-get-CPU-number-motherboar   use this to modify....
*/



#endif
