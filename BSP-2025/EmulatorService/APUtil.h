//---------------------------------------------------------------------------
#ifndef APUtilH
#define APUtilH
//---------------------------------------------------------------------------

#include <ADODB.hpp>
#include <DBGrids.hpp>
#include <syncobjs.hpp>
#include <stdio.h>
enum EM_ENTRY_MODE { emOpen = 0, emEdit = 1 , emNew = 2 , emBrowse = 3 , emLocate = 4 , emLookup = 5 , emImport = 6 } ;

// 是否為 Debug Mode
extern bool gIsDebugMode  ;

extern AnsiString FillZero(AnsiString sSrc, int DescLen);
extern AnsiString ChangeDateFmt(AnsiString sDate);
extern AnsiString GetDateStyle(AnsiString sDay);
extern AnsiString GetTimeStyle(AnsiString sTime);
extern AnsiString CheckFileNameNum(AnsiString sFileName);
extern int LoadFromUniCodeFile(TStrings* ss, const String& fname);
extern void SaveToUniCodeFile(TStrings* ss, const String& fname);

// 代換字串函數
char * QuickReplaceStr( char * Dest , char * sSource , char * sSearch , char * sReplaceWith ) ;
// 處理 SQL 字串內容
AnsiString ParamStr( AnsiString sParam ) ;
// 處理 SQL 數值內容
AnsiString ParamInt( AnsiString sParam ) ;

// 組合檔案路徑
AnsiString CheckFilePath( char * path , char *filename ) ;
// 處理 Excel 字串內容
AnsiString ExcelStr( AnsiString sStr ) ;

AnsiString GetBookmarkStringsByField( TDBGrid * dbGrid , TADOQuery * adoQuery , AnsiString sFieldName ) ;
bool OpenSQL( AnsiString sSQL , TADOQuery * qry );

AnsiString GetVersionStr( void );
// =========================================================================
// CommLog Class
// =========================================================================

// Comm Log 物件
class ECommLog
{

private:
        int m_servicecount ;
		// 記錄 Log 目錄
		char m_LogPath[_MAX_PATH] ;
		// 建立目錄
		bool CreateDir( char * sFilePath ) ;
        AnsiString m_TransactionID ;
        AnsiString m_System ;

public:
        bool m_WriteToFile ;
		// 建構
		ECommLog( AnsiString sSystem , char * sLogPath = NULL ) ;
		// 解構
		~ECommLog() ;

        void StartService( AnsiString Service , AnsiString TermID ) {
            m_TransactionID.sprintf( "./log/%s-%s-%s-%s-%04Xd-%02d.log" , m_System.c_str() , Service.c_str() , Now().FormatString("yyyymmddhhnnss").c_str() , TermID.c_str() , GetCurrentThreadId() , m_servicecount++ ) ;
        }

        AnsiString GetFileName() {
            return m_TransactionID ;
        }

        static void sWriteLog( char * filename , char * fmt , ... )
        {
            va_list    parg;
            AnsiString sLog ;
            va_start( parg, fmt);
            sLog.vprintf( fmt , parg ) ;
            va_end(parg);
            OutputDebugString( sLog.c_str() );
            bool bLogToFile = true ;
            FILE * elog_fp ;
            if ( bLogToFile ) {
                if( (elog_fp=fopen( filename ,"at")) != NULL ) {
                    // 寫入時間 & Log
                    fprintf( elog_fp , "%s\t%08d\t%04d\t%s" , Now().FormatString("yyyy/mm/dd hh:nn:ss").c_str() , GetTickCount() , GetCurrentThreadId() , sLog.c_str() ) ;
                    // 加入換行字元
                    fprintf( elog_fp , "\n") ;
                    fclose(elog_fp);
                } // if
            }
            return ;
        }

        // 寫入 Log
        void WriteLog( char * fmt , ... )
        {
            if ( !m_WriteToFile ) return ;
            va_list    parg;
            AnsiString sLog ;
            va_start( parg, fmt);
            sLog.vprintf( fmt , parg ) ;
            va_end(parg);
            OutputDebugString( sLog.c_str() );
            bool bLogToFile = true ;
            //AnsiString sFileName ;
            FILE * elog_fp ;
            //sFileName.sprintf( "./log/DebugString%s-%s.log" , m_TransactionID.c_str() , Now().FormatString("yyyymmdd").c_str() ) ;
            if ( bLogToFile ) {
                if( (elog_fp=fopen( GetFileName().c_str() ,"at")) != NULL ) {
                    // 寫入時間 & Log
                    fprintf( elog_fp , "%s\t%04d\t%d\t%s" , Now().FormatString("yyyy/mm/dd hh:nn:ss").c_str() , GetCurrentThreadId() , GetTickCount() , sLog.c_str() ) ;
                    // 加入換行字元
                    fprintf( elog_fp , "\n") ;
                    fclose(elog_fp);
                } // if
            }
            return ;
        }
} ;

    bool IsAutoLogin( AnsiString s )
    {
        s = s.UpperCase() ;
        if ( s == "SIGN-ON" || s== "RELOGIN" || s == "LOGOFF" )
        {
             return true ;
        }
        else
            return false ;
    }

// 宣告 Global Comm Log 物件
extern ECommLog * g_CommLog ;

#endif

