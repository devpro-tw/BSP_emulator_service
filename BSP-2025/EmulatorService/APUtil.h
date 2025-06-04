//---------------------------------------------------------------------------
#ifndef APUtilH
#define APUtilH
//---------------------------------------------------------------------------

#include <ADODB.hpp>
#include <DBGrids.hpp>
#include <syncobjs.hpp>
#include <stdio.h>
enum EM_ENTRY_MODE { emOpen = 0, emEdit = 1 , emNew = 2 , emBrowse = 3 , emLocate = 4 , emLookup = 5 , emImport = 6 } ;

// �O�_�� Debug Mode
extern bool gIsDebugMode  ;

extern AnsiString FillZero(AnsiString sSrc, int DescLen);
extern AnsiString ChangeDateFmt(AnsiString sDate);
extern AnsiString GetDateStyle(AnsiString sDay);
extern AnsiString GetTimeStyle(AnsiString sTime);
extern AnsiString CheckFileNameNum(AnsiString sFileName);
extern int LoadFromUniCodeFile(TStrings* ss, const String& fname);
extern void SaveToUniCodeFile(TStrings* ss, const String& fname);

// �N���r����
char * QuickReplaceStr( char * Dest , char * sSource , char * sSearch , char * sReplaceWith ) ;
// �B�z SQL �r�ꤺ�e
AnsiString ParamStr( AnsiString sParam ) ;
// �B�z SQL �ƭȤ��e
AnsiString ParamInt( AnsiString sParam ) ;

// �զX�ɮ׸��|
AnsiString CheckFilePath( char * path , char *filename ) ;
// �B�z Excel �r�ꤺ�e
AnsiString ExcelStr( AnsiString sStr ) ;

AnsiString GetBookmarkStringsByField( TDBGrid * dbGrid , TADOQuery * adoQuery , AnsiString sFieldName ) ;
bool OpenSQL( AnsiString sSQL , TADOQuery * qry );

AnsiString GetVersionStr( void );
// =========================================================================
// CommLog Class
// =========================================================================

// Comm Log ����
class ECommLog
{

private:
        int m_servicecount ;
		// �O�� Log �ؿ�
		char m_LogPath[_MAX_PATH] ;
		// �إߥؿ�
		bool CreateDir( char * sFilePath ) ;
        AnsiString m_TransactionID ;
        AnsiString m_System ;

public:
        bool m_WriteToFile ;
		// �غc
		ECommLog( AnsiString sSystem , char * sLogPath = NULL ) ;
		// �Ѻc
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
                    // �g�J�ɶ� & Log
                    fprintf( elog_fp , "%s\t%08d\t%04d\t%s" , Now().FormatString("yyyy/mm/dd hh:nn:ss").c_str() , GetTickCount() , GetCurrentThreadId() , sLog.c_str() ) ;
                    // �[�J����r��
                    fprintf( elog_fp , "\n") ;
                    fclose(elog_fp);
                } // if
            }
            return ;
        }

        // �g�J Log
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
                    // �g�J�ɶ� & Log
                    fprintf( elog_fp , "%s\t%04d\t%d\t%s" , Now().FormatString("yyyy/mm/dd hh:nn:ss").c_str() , GetCurrentThreadId() , GetTickCount() , sLog.c_str() ) ;
                    // �[�J����r��
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

// �ŧi Global Comm Log ����
extern ECommLog * g_CommLog ;

#endif

