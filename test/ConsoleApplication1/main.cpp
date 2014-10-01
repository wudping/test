// ConsoleApplication1.cpp : 定義主控台應用程式的進入點。
//
// BFEBFBFF000306A9    
//#include "stdafx.h"





#include <string>
#include <stdio.h>

using namespace std;



void getPSN(char *PSN)
{
int varEAX, varEBX, varECX, varEDX;
char str[9];
//%eax=1 gives most significant 32 bits in eax
__asm__ __volatile__ ("cpuid": "=a" (varEAX), "=b" (varEBX), "=c" (varECX), "=d" (varEDX) : "a" (1));

sprintf(str, "%08X", varEDX); //i.e. XXXX-XXXX-xxxx-xxxx-xxxx-xxxx
sprintf(PSN, "%C%C%C%C-%C%C%C%C", str[0], str[1], str[2], str[3], str[4], str[5], str[6], str[7]);

sprintf(str, "%08X", varEAX); //i.e. XXXX-XXXX-xxxx-xxxx-xxxx-xxxx
sprintf(PSN, "%s-%C%C%C%C-%C%C%C%C",PSN,  str[0], str[1], str[2], str[3], str[4], str[5], str[6], str[7]);


//%eax=3 gives least significant 64 bits in edx and ecx [if PN is enabled]
__asm__ __volatile__ ("cpuid": "=a" (varEAX), "=b" (varEBX), "=c" (varECX), "=d" (varEDX) : "a" (1));

sprintf(str, "%08X", varEDX); //i.e. xxxx-xxxx-XXXX-XXXX-xxxx-xxxx

sprintf(PSN, "%s-%C%C%C%C-%C%C%C%C", PSN, str[0], str[1], str[2], str[3], str[4], str[5], str[6], str[7]);

sprintf(str, "%08X", varEBX); //i.e. xxxx-xxxx-xxxx-xxxx-XXXX-XXXX

sprintf(PSN, "%s-%C%C%C%C-%C%C%C%C", PSN, str[0], str[1], str[2], str[3], str[4], str[5], str[6], str[7]);

}


    string GetCPUPhysicalSerialNo()  
    {  
        unsigned int ulS1,ulS2;  
        //string strCPUID,strCPUID1,strCPUID2;  
        char strCPUID1[40],strCPUID2[40];  
		string strCPUID;
		

    

      /* sprintf( strCPUID1, "%08X%08X", ulS1,ulS2 );
       __asm__  __volatile__ (
	 "mov eax,00h   \n\t"
	 "xor ecx,ecx   \n\t"
	 "xor edx,edx   \n\t"
	 "cpuid   \n\t"
	 "mov ulS1,edx   \n\t"
	 "mov ulS2,ecx   \n\t"
      );
       sprintf( strCPUID2, "%08X%08X",ulS1,ulS2);*/
		
#if 0
        __asm  
        {  
            mov eax ,01h  
            xor edx,edx  
            cpuid  
            mov ulS1,edx  
            mov ulS2,eax  
        }  
        //strCPUID1.Format("%08X%08X",ulS1,ulS2);  
		sprintf( strCPUID1, "%08x%08x", ulS1, ulS2 );
        __asm  
        {  
            mov eax, 00h  
            xor ecx,ecx  
            xor edx,edx  
            cpuid  
            mov ulS1,edx  
            mov ulS2,ecx  
        }  
#endif
        //strCPUID2.Format("%08X%08X",ulS1,ulS2);  
		sprintf( strCPUID2, "%08x%08x", ulS1, ulS2 );

      
        strCPUID = string(strCPUID1) + string(strCPUID2);  
      
        return strCPUID;  
      
    }  
    
    
 //  http://clang.llvm.org/doxygen/cpuid_8h.html
 // http://clang.llvm.org/doxygen/cpuid_8h_source.html
 // http://www.cs.usfca.edu/~cruse/cs686s07/cpuid.cpp
 // http://en.wikipedia.org/wiki/CPUID
    
    
#include <cpuid.h>
#include <CpuInformation.h>

#include <cstdlib>
#if 0
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char* argv[])
#endif
{
  
  printf( "%s\n", cpu_get_serial().c_str() );
  
  //char PSN[30]; //24 Hex digits, 5 '-' separators, and a '\0'
//getPSN(PSN);
//printf("%s\n", PSN); //compare with: lshw | grep serial:
  
  
//GetCPUPhysicalSerialNo();
  
  
    //printf( "%s\n", cpu_get_vender().c_str()  );
  
  
	//cout << GetCPUPhysicalSerialNo() << "\n";
  
  //__get_cpuid (unsigned int __level, unsigned int *__eax, unsigned int *__ebx, unsigned int *__ecx, unsigned int *__edx)
  
    /*unsigned int eax, ebx, ecx, edx;
    __get_cpuid( 0, &eax, &ebx, &ecx, &edx );

    char buf[17];
    ((unsigned int*)buf)[0] = ebx;
    ((unsigned int*)buf)[1] = edx;
    ((unsigned int*)buf)[2] = ecx;        
    ((unsigned int*)buf)[3] = eax;
    printf("%s\n",buf);
    
    unsigned int sig ;
     __get_cpuid_max( 0,& sig );
    printf("%x",sig);*/
     
	//int i = system("wmic CPU get ProcessorID"); 

	//system("PAUSE");
	return 0;
}