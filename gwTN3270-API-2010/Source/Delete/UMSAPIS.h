//---------------------------------------------------------------------------

#ifndef UMSAPISH
#define UMSAPISH
#include <ADODB.hpp>
#include <SyncObjs.hpp>

extern TStrings * gUmsParams ;
extern TCriticalSection * g_csUMS ;
extern TCriticalSection * g_csUMS_NextKey ;

//---------------------------------------------------------------------------
// Parameter Function
//---------------------------------------------------------------------------
void LoadUmsParams( ) ;

//---------------------------------------------------------------------------
// SQL Funstion
//---------------------------------------------------------------------------
bool EXECSQL( TADOQuery * adoQuery , char * szSQL ) ;
bool RUNSQL( TADOQuery * adoQuery , char * szSQL );

//---------------------------------------------------------------------------
// Serial Funstion
//---------------------------------------------------------------------------
long GetNextKeyValue( TADOQuery * adoQuery , char * lpTableName , char * lpKeyName ) ;

//---------------------------------------------------------------------------
// Lock Funstion
//---------------------------------------------------------------------------
bool umsDoLock( TADOQuery * adoQuery , char * sLockName , char * sLockID ) ;
bool umsUnLock( TADOQuery * adoQuery , char * sLockName , char * sLockID ) ;

//---------------------------------------------------------------------------
// UMS Functions
//---------------------------------------------------------------------------
AnsiString umsUserVirtualPath( int nUserId );
AnsiString umsUserCurrentPath( int nUserId );
// �ˬd�O�_���¦W��
bool umsCheckIsBlackTelNo( TADOQuery * adoQuery , AnsiString sCCode , AnsiString sACode , AnsiString sDialNo ) ;
// �o�e�l��q���ǯu�~�����G
void umsSendEMail( TADOQuery * adoQuery, int nMsgId, AnsiString FunctionName, AnsiString ResName );

//---------------------------------------------------------------------------
#endif
