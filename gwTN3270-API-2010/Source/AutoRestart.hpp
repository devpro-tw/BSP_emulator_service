//---------------------------------------------------------------------------

#ifndef AutoRestartH
#define AutoRestartH

#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <dir.h>

// log function
void toLog(char *title, const char *str, ... )
{
	va_list arglist;
	char text[ 4096 ];		// °T®§¦s©ñ

	va_start( arglist, str );
	_vsnprintf( text, sizeof( text ), str, arglist );
	va_end( arglist );

    char FileName[256];
    char DTBuf[30];
    
    time_t		ltime;
    struct tm	*dt;

    time( &ltime );
    dt = localtime( &ltime );

    char path[256] = {0};
    int len = GetModuleFileName(0, path, sizeof(path));
    while (len > 0 && path[len - 1] != '/' && path[len - 1] != '\\') path[--len] = 0;

//    sprintf( FileName, "%s%04d%02d%02d.log",
//                        title, dt->tm_year + 1900, dt->tm_mon + 1, dt->tm_mday);

    sprintf( FileName, "%s/%s.LOG", path, title);

    FILE *fp;
    fp = fopen( FileName, "a+" );

    if( fp == NULL ) return;

    sprintf(DTBuf, "%04d/%02d/%02d %02d:%02d:%02d",
                    dt->tm_year + 1900, dt->tm_mon + 1, dt->tm_mday,
                    dt->tm_hour, dt->tm_min, dt->tm_sec);

    fprintf( fp, "%s\t%d\t", DTBuf, ::GetCurrentProcessId() );
    fprintf( fp, text );
    fprintf( fp, "\n" );

    fclose( fp );
}

char *FmtErrMsg(DWORD dwCode, char *szBuf)
{
    LPVOID lpMsgBuf;

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        dwCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    strcpy(szBuf, (char *)lpMsgBuf);

    LocalFree(lpMsgBuf);

    return szBuf;
}

class TAutoRestart {
public:
    int WaitFor(char *EntityName, int Timeout = 30000) {
        int rc = 0;
        char szEnv[256] = "\0";
        char *szProcId = ::getenv(EntityName);
        toLog("RESTART", "[WaitFor.Entry]\tProcessId=%s EntityName=%s", szProcId, EntityName);
        if ( szProcId ) {
            HANDLE hProc = ::OpenProcess(PROCESS_ALL_ACCESS, false, atoi(szProcId));
            toLog("RESTART", "[WaitFor.OpenProcess]\tProcessId=%s ProcessHwnd=0x%08X", szProcId, (long)hProc);
            if (WAIT_TIMEOUT == ::WaitForSingleObject(hProc, Timeout)) {
                toLog("RESTART", "[WaitFor.Timeout]\tProcessId=%s Timeout=%d", szProcId, Timeout);
                if (!::TerminateProcess(hProc, 99)) {
                    char TmpBuf[4096];
                    toLog("RESTART", "[WaitFor.TerminateProcess Fail]\tProcessId=%s ErrMsg=%s", szProcId, FmtErrMsg(GetLastError(), TmpBuf));
                }
                else
                    toLog("RESTART", "[WaitFor.TerminateProcess Success]\tProcessId=%s", szProcId);
                rc = 99;
            }
            else
                rc = 1;
        }
        ::sprintf(szEnv, "%s=%d", EntityName, ::GetCurrentProcessId());
        ::putenv(szEnv);
        toLog("RESTART", "[WaitFor.Leave]\tEnv=%s RC=%d", szEnv, rc);
        return rc;
    }

    bool CreateProcess() {
        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ::ZeroMemory( &si, sizeof(si) );
        si.cb = sizeof(si);
        ::ZeroMemory( &pi, sizeof(pi) );
        
        char szCmdLine[255] = "\0";
        ::strcpy(szCmdLine, GetCommandLine());

        toLog("RESTART", "[CreateProcess]\tCmdLine=%s", szCmdLine);

        return ::CreateProcess( NULL, szCmdLine, NULL, NULL, false, 0,
                                NULL, //(LPVOID)GetEnvironmentStrings(),
                                NULL, &si, &pi);
    }
};

#endif
