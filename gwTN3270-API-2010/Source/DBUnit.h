//---------------------------------------------------------------------------
#ifndef DBUnitH
#define DBUnitH
#include <DBTables.hpp>
#include "APUtil.h"
#include <ADODB.hpp>
//---------------------------------------------------------------------------
// �}�Ҹ�Ʈw
extern bool DBOpen( AnsiString sConnStr ) ;
// ������Ʈw
extern bool DBClose( ) ;
// ���� SQL �d�߫��O
extern bool DBOpenSQL( const char * sSQL , TADOQuery * ActionQuery = NULL , bool bWriteLog = true ) ;
// ���� SQL �s�W�B�ק�B�R�� ���O
extern bool DBExecSQL( const char * sSQL , bool bWriteLog = true ) ;

// Query ����
extern TADOQuery * g_qryComm ;

//---------------------------------------------------------------------------
#endif

