#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#define CURL_STATICLIB
#include <direct.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
// curl-7.40.0-devel-mingw32
#include <curl/curl.h>

#define POST_SIZE 2048

int mkdir(const char *);
void member_check(void *, size_t , size_t , void *);
size_t callback_file(void *, size_t , size_t , FILE *);
char lis[256]="https://120.125.80.141/opgs/agent/agent.php";
char filename[FILENAME_MAX] = "C:\\tmp\\agent_info.exe";


int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    FILE *fp;
    CURL *curl;
    //CURLcode response;
    char *url = "https://120.125.80.141/opgs/agent/agent_check.php";
    char agent[] = "opgs/1.1";
    char postdata[POST_SIZE];
    char *callback;
    char cmd[256];

    _mkdir("C:\\tmp");

///start - get install id and check member information
    char install_id[128];
    char line[1024];
    sprintf(cmd,"%s %s > C:\\tmp\\a.txt",
            "%windir%\\Sysnative\\cscript %windir%\\System32\\slmgr.vbs /dlv",
            "| %windir%\\Sysnative\\findstr \"¦w¸ËÃÑ§O½X\"");
    system(cmd);
    fp = fopen("C:\\tmp\\a.txt", "r");
    fgets(line,1024,fp);
    sscanf(line,"%*s %s",install_id);
    printf("install id = %s\n",install_id);
    sprintf(postdata,"install_id=%s",install_id);
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, agent);
    curl_easy_setopt(curl, CURLOPT_HEADER, 0L);
    curl_easy_setopt(curl, CURLOPT_POST,1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS,postdata);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, member_check);
    curl_easy_perform(curl);
///end - get install id and check member information


///start - update agent information and download listen
    url = "https://120.125.80.141/opgs/agent/agent_info.exe";
    fp = fopen(filename,"wb");
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback_file);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_perform(curl);
    fclose(fp);
    system("C:\\tmp\\agent_info.exe");
///end - update agent information and download listen


    fclose(fp);

    curl_easy_cleanup(curl);



    return 0;
}

void member_check(void *ptr, size_t size, size_t nmemb, void *stream)
{

    if ( strcmp((char *)ptr,"incorrent") == 0 ) {
        printf("%s\n","Register your install id first!!");
        system("PAUSE");
        exit(1);
    }
    else printf("success!\n");
}

size_t callback_file(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}
