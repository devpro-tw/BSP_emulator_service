//---------------------------------------------------------------------------
#ifndef DBUnitH
#define DBUnitH
#include <DBTables.hpp>
#include "APUtil.h"
#include <ADODB.hpp>
//---------------------------------------------------------------------------
// 開啟資料庫
extern bool DBOpen( AnsiString sConnStr ) ;
// 關閉資料庫
extern bool DBClose( ) ;
// 執行 SQL 查詢指令
extern bool DBOpenSQL( const char * sSQL , TADOQuery * ActionQuery = NULL , bool bWriteLog = true ) ;
// 執行 SQL 新增、修改、刪除 指令
extern bool DBExecSQL( const char * sSQL , bool bWriteLog = true ) ;

// Query 物件
extern TADOQuery * g_qryComm ;

//---------------------------------------------------------------------------
#endif

