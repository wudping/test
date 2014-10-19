// ftp_client_test.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include <stdio.h>
#include <curl/curl.h>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;


struct FtpFile {
  const char *filename;
  FILE *stream;
};
 
size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {

	char buf[1000];
	strcpy( buf, (char*)ptr );

	printf("%s",buf);


    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

   void downloadFile(const char* url, const char* ofname)
   {
    CURL *curl;
        FILE *fp;
        CURLcode res;        
        curl = curl_easy_init();
        if (curl){
            fp = fopen(ofname,"wb");
            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_USERPWD, "user:pass");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            fclose(fp);
        }
     }




  void downloadAllFiles()
{
    FILE *ftpFile = NULL;
    string strFileName = "";
	string strFtpListFile = "ftp-list";
    ifstream infile(strFtpListFile.c_str());

	string strSaveDestFolder = "./";

    if(!infile.is_open())
    {
        cerr << "can not open ftpList.txt" << endl;
        return ;
    }

    /* ftpList.txt get data, line by line, processing */
    while(getline(infile, strFileName))
    {
        CURL *curl;
        FILE *destFilePath;
        CURLcode res;        

		cout << strFileName << endl;

        curl = curl_easy_init();

        if (curl)
        {
            destFilePath = fopen((strSaveDestFolder+strFileName).c_str(),"wb");
            //curl_easy_setopt(curl, CURLOPT_URL, (strHost+strUri+strFileName).c_str());
            //curl_easy_setopt(curl, CURLOPT_USERPWD, (strUser+":"+strPassWord).c_str());
            //curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            //curl_easy_setopt(curl, CURLOPT_WRITEDATA, destFilePath);
            //res = curl_easy_perform(curl);

            //curl_easy_cleanup(curl);

            //fclose(destFilePath);
        }
    }// end while
}





int _tmain(int argc, _TCHAR* argv[])
{

  CURL *curl;
  CURLcode res;

  FILE *ftplister = fopen("ftp-list", "wb"); /* b is binary, needed on win32 */

 
  curl_global_init(CURL_GLOBAL_DEFAULT);
 
  curl = curl_easy_init();
  if(curl) {
    /*
     * You better replace the URL with one that works!
     */ 
    curl_easy_setopt(curl, CURLOPT_URL,
                     //"ftp://Foldrs:hahahaha@192.168.1.2:8021");
                     "ftp://hidog:yas12129@hidog.no-ip.org:252");
    /* Define our callback to get called when there's data to be written */ 
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    /* Set a pointer to our struct to pass to the callback */ 
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, ftplister);
 
    /* Switch on full protocol/debug output */ 
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

	curl_easy_setopt( curl,  CURLOPT_FTPLISTONLY, true );


    res = curl_easy_perform(curl);
 
    /* always cleanup */ 
    curl_easy_cleanup(curl);
 
    if(CURLE_OK != res) {
      /* we failed */ 
      fprintf(stderr, "curl told us %d\n", res);
    }
  }
 
    fclose(ftplister); /* close the local file */ 
 
  curl_global_cleanup();

  downloadAllFiles();
 
	system("PAUSE");
	return 0;
}

