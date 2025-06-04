//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DBUnit.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

TADOConnection * g_dbComm ;
TADOQuery  * g_qryComm ;

// 開啟資料庫
bool DBOpen( AnsiString sConnStr )
{
    bool rc = false ;
    try {
        // 建立 DB 相關物件
        g_dbComm    = new TADOConnection( Application );
        g_qryComm   = new TADOQuery( Application );

        // 建立 DB 相關物件
        g_dbComm->ConnectionString    = sConnStr ;
        g_dbComm->LoginPrompt         = false ;
        g_dbComm->KeepConnection      = true ;
        g_dbComm->Connected           = true ;
        g_dbComm->Open() ;

        rc = true ;
    } // try
    catch( Exception & E ) {
        g_CommLog->WriteLog( "FunName=[DBOpen] , Exception=[%s]" , E.Message.c_str() );
    } // catch

    return rc ;
}

// 關閉資料庫
bool DBClose( )
{
    bool rc = false ;

    try {
        // 關閉 DB 相關物件
        g_qryComm->Close() ;
        g_dbComm->Close() ;

        // 刪除 DB 相關物件
        delete g_qryComm ;
        delete g_dbComm ;

        rc = true ;
    } // try
    catch( Exception & E ) {
        g_CommLog->WriteLog( "FunName=[DBClose] , Exception=[%s]" , E.Message.c_str() );
    } // catch

    return rc ;
}

// 執行 SQL 查詢指令
bool DBOpenSQL( const char * sSQL  , TADOQuery * ActionQuery  , bool bWriteLog )
{
    bool rc = false ;
    TADOQuery * bQuery ;

    try {
        if( bWriteLog ) g_CommLog->WriteLog( "FunName=[DBOpenSQL] , SQL String=[%s]" , sSQL );
        if( ActionQuery ) {
            bQuery = ActionQuery ;
        } // if
        else {
            bQuery = g_qryComm ;
        } // else

        // 設定 Query 物件
        bQuery->Connection         = g_dbComm ;
        bQuery->LockType           = ltBatchOptimistic ;
        bQuery->CursorLocation     = clUseClient ;
        bQuery->CursorType         = ctStatic ;

        bQuery->Close() ;
        bQuery->SQL->Text = sSQL ;
        bQuery->Open() ;

        rc = true ;
    } // try
    catch( Exception & E ) {
        g_CommLog->WriteLog( "FunName=[DBOpenSQL] , Exception=[%s]" , E.Message.c_str() );
    } // catch

    return rc ;
}


// 執行 SQL 新增、修改、刪除 指令
bool DBExecSQL( const char * sSQL , bool bWriteLog )
{
    bool rc = false ;

    try {
        if( bWriteLog ) g_CommLog->WriteLog( "FunName=[DBExecSQL] , SQL String=[%s]" , sSQL );

        // 設定 Query 物件
        g_qryComm->Connection         = g_dbComm ;
        g_qryComm->LockType           = ltBatchOptimistic ;
        g_qryComm->CursorLocation     = clUseClient ;
        g_qryComm->CursorType         = ctStatic ;

        g_qryComm->Close() ;
        g_qryComm->SQL->Text = sSQL ;
        g_qryComm->ExecSQL() ;

        rc = true ;
    } // try
    catch( Exception & E ) {
        g_CommLog->WriteLog( "FunName=[DBExecSQL] , Exception=[%s]" , E.Message.c_str() );
    } // catch

    return rc ;
}

