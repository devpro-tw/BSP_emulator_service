//---------------------------------------------------------------------------


#include <StrUtils.hpp>
#pragma hdrstop

#include "UMSAPIS.h"
#include "APUtil.h"
#include "DBUnit.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TStrings * gUmsParams ;

TCriticalSection * g_csUMS ;
TCriticalSection * g_csUMS_NextKey ;

//---------------------------------------------------------------------------
// Parameter Function
//---------------------------------------------------------------------------
void LoadUmsParams( )
{
    AnsiString sSQL ;

    sSQL = "Select * From tblUMSParam Order By ParamName" ;

    if( DBOpenSQL( sSQL.c_str() ) ) {
        while( !g_qryComm->Eof ) {
            gUmsParams->Values[g_qryComm->FieldByName("ParamName")->AsString.Trim()]= g_qryComm->FieldByName("ParamValue")->AsString.Trim();
            g_qryComm->Next();
        } // while
    } // if
}


//---------------------------------------------------------------------------
// SQL Funstion
//---------------------------------------------------------------------------
bool EXECSQL( TADOQuery * adoQuery , char * szSQL )
{
    bool rc = false ;
    try {
        adoQuery->Close() ;
        adoQuery->SQL->Text = szSQL ;
        adoQuery->ExecSQL();
        rc = true ;
    } // try
    catch( Exception & E ) {
        g_CommLog->WriteLog( "EXECSQL Exception=[%s] " , E.Message.c_str() );
    } // catch

    return rc ;
}

bool RUNSQL( TADOQuery * adoQuery , char * szSQL )
{
    bool rc = false ;
    try {
        adoQuery->Close() ;
        adoQuery->SQL->Text = szSQL ;
        adoQuery->Open();
        rc = true ;
    } // try
    catch( Exception & E ) {
        g_CommLog->WriteLog( "RUNSQL Exception=[%s] " , E.Message.c_str() );
    } // catch

    return rc ;
}

//---------------------------------------------------------------------------
// Serial Funstion
//---------------------------------------------------------------------------


bool IncreaseSerial( TADOQuery * adoQuery , char * MainCategory , char * SubCategory , char * LockID , unsigned int & MaxSerial )
{
    char szSQL[1024] ;
    bool brc = false ;
    bool rc ;
    try {
        MaxSerial = -1 ;
        // Try Lock by LockID
        sprintf( szSQL , "Update tblSerialInfo Set LockID='%s' , LockTime=GetDate(), LockEntity='%s' , MaxSerialNo=MaxSerialNo+1 \
                          Where MainCategory='%s' and SubCategory='%s' and ( (LockID='<FREE>') Or ( DATEADD(s,30,LockTime) <= GetDate() ) ) " ,
                          LockID , gEntityName.c_str(), MainCategory , SubCategory ) ;
        rc = EXECSQL( adoQuery , szSQL );

        // Find Category
        if ( rc ) {
            sprintf( szSQL , "Select * From tblSerialInfo where MainCategory='%s' and SubCategory='%s'" , MainCategory , SubCategory ) ;
            rc = RUNSQL( adoQuery , szSQL );
            if ( rc ) {
                if ( adoQuery->Eof ) { // Category not exists , then create it !
                    sprintf( szSQL , "Insert Into tblSerialInfo(MainCategory,SubCategory,MaxSerialNo,LockID) Values('%s','%s',0,'<FREE>') " , MainCategory , SubCategory ) ;
                    EXECSQL( adoQuery , szSQL ) ;
                }
                else {
                    // Check LockID is match , means lock success or fail
                    if ( !stricmp( adoQuery->FieldByName("LockID")->AsString.c_str() , LockID ) ) {
                        MaxSerial = adoQuery->FieldByName("MaxSerialNo")->AsInteger ;
                        brc = true ;

                        // UnLockID
                        sprintf( szSQL , "Update tblSerialInfo Set LockID='<FREE>', LockTime=NULL, LockEntity='' Where MainCategory='%s' and SubCategory='%s' and LockID='%s' " ,
                            MainCategory , SubCategory , LockID ) ;
                        EXECSQL( adoQuery , szSQL ) ;
                    } // if
                } // else
            } // if
        } // if
    } // try
    catch ( Exception &E ) {
        g_CommLog->WriteLog( "IncreaseSerial Exception=[%s] " , E.Message.c_str() );
    } // catch
    return brc ;
}

bool GetNextSerialNo( TADOQuery * adoQuery , char * MainCategory , char * SubCategory , char * LockID , unsigned int & MaxSerialNo )
{
    char szTempLockID[129] ;
    bool brc = false ;
    GUID guid ;

    if ( LockID == NULL ) {
        HRESULT hrc = CoCreateGuid(&guid) ;
        if ( hrc != 0 )
            return false ;
        strcpy( szTempLockID , GUIDToString(guid).c_str() ) ;
    }

    MaxSerialNo = 0 ;

    for ( int n = 0 ; (n < 300 ) && ! brc ; n ++ ) {
        brc = IncreaseSerial( adoQuery , MainCategory , SubCategory , szTempLockID , MaxSerialNo ) ;
        Sleep( 100 ) ;
    }
    return brc ;
}

long GetNextKeyValue( TADOQuery * adoQuery , char * lpTableName , char * lpKeyName )
{
    char szSQL[1024] ;
    long Result = -1 ;
    unsigned int NextSerialNo ;

    g_csUMS_NextKey->Enter() ;
    try {
        if ( GetNextSerialNo( adoQuery , lpTableName , lpKeyName , NULL , NextSerialNo ) )
            Result = NextSerialNo ;

        adoQuery->Close();
    }
    catch ( Exception &E ) {
        g_CommLog->WriteLog( "GetNextKeyValue Exception=[%s] " , E.Message.c_str() );
    }

    g_csUMS_NextKey->Leave() ;
    return Result ;
}

//---------------------------------------------------------------------------
// Lock Funstion
//---------------------------------------------------------------------------

bool umsCheckLockName( TADOQuery * adoQuery , char * LockName, char * LockID )
{
    char szSQL[1024] ;
    bool brc = false ;
    bool rc ;
    try {
        // Try Lock by LockID
        sprintf( szSQL , "Update tblLockInfo Set LockID='%s', LockTime=GetDate(), LockEntity='%s' \
                          Where LockName='%s' And ( (LockID='<FREE>') Or ( DATEADD(s,30,LockTime) <= GetDate() ) ) " ,
                          LockID , gEntityName.c_str() , LockName ) ;
        rc = EXECSQL( adoQuery , szSQL );

        if ( rc ) {
            sprintf( szSQL , "Select * From tblLockInfo where LockName='%s' " , LockName ) ;
            rc = RUNSQL( adoQuery , szSQL );
            if ( rc ) {
                if ( adoQuery->Eof ) { // LockName not exists , then create it !
                    sprintf( szSQL , "Insert Into tblLockInfo (LockName,LockID) Values('%s','<FREE>') " , LockName ) ;
                    EXECSQL( adoQuery , szSQL ) ;
                } // if
                else {
                    // Check LockID is match , means lock success or fail
                    if ( !strcmp( adoQuery->FieldByName("LockID")->AsString.Trim().c_str() , LockID ) ) {
                        brc = true ;
                    } // if
                } // else
            } // if
        } // if
    } // try
    catch ( Exception &E ) {
        g_CommLog->WriteLog( "CheckUMSLockName Exception=[%s] " , E.Message.c_str() );
    } // catch
    return brc ;
}


bool umsDoLock( TADOQuery * adoQuery , char * sLockName , char * sLockID )
{

    g_csUMS->Enter() ;

    bool brc = false ;
    GUID guid ;

    HRESULT hrc = CoCreateGuid(&guid) ;
    if ( hrc != 0 )
        return false ;
    strcpy( sLockID , GUIDToString(guid).c_str() ) ;

    for ( int n = 0 ; (n < 300) && ! brc ; n ++ ) {
        brc = umsCheckLockName( adoQuery , sLockName , sLockID ) ;
        Sleep( 100 ) ;
    } // for

    return brc ;
}

bool umsUnLock( TADOQuery * adoQuery , char * sLockName , char * sLockID )
{
    char szSQL[1024] ;
    bool brc;
    
    sprintf( szSQL , "Update tblLockInfo Set LockID='<FREE>', LockTime=NULL, LockEntity='' Where LockName='%s' and LockID='%s' " , sLockName , sLockID ) ;
    brc = EXECSQL( adoQuery , szSQL ) ;

    g_csUMS->Leave() ;

    return brc ;
}

//---------------------------------------------------------------------------
// UMS Functions
//---------------------------------------------------------------------------
AnsiString umsUserVirtualPath( int nUserId )
{
    AnsiString sVirtualPath ;
    sVirtualPath = "<%ROOT%>/" + IntToStr(nUserId) + "/" ;
    return sVirtualPath ;
}


AnsiString umsUserCurrentPath( int nUserId )
{
    AnsiString sCurrentPath ;

    sCurrentPath = CheckFilePath( gUmsParams->Values["RootPath"].c_str() , IntToStr(nUserId).c_str() ) + "/" ;

    return sCurrentPath ;
}

// 檢查是否為黑名單
bool umsCheckIsBlackTelNo( TADOQuery * adoQuery , AnsiString sCCode , AnsiString sACode , AnsiString sDialNo )
{
    bool rc = false ;

    AnsiString sSQL ;

    sSQL.sprintf( "Select * From tblBlackTelList Where CCode='%s' And ACode='%s' And DialNo='%s'" , sCCode.c_str() , sACode.c_str() , sDialNo.c_str() );

    if( RUNSQL( adoQuery , sSQL.c_str() ) ) {
        if( !adoQuery->Eof ) {
            rc = true ;
        } // if
    } // if

    return rc ;
}

// 發送郵件通知傳真外撥結果
void umsSendEMail( TADOQuery * adoQuery, int nMsgId, AnsiString FunctionName, AnsiString ResName )
{
	AnsiString		sSQL, sMailApp, sMailParam;
    unsigned long 	nEMailResult = 0 ;

    sSQL = "SELECT EMAIL,CCODE,ACODE,DIALNO FROM tblOutMsgFax F LEFT JOIN tblUserList U ON F.UserId=U.UserId WHERE U.EMAIL<>'' AND MSGID=" + IntToStr( nMsgId );
    if( RUNSQL( adoQuery , sSQL.c_str() ) ) {
        if( !adoQuery->Eof ) {
            // 設定 EMail 資訊
            sMailApp    = g_tmSetup->ReadString( "OutBoxInfo" , "MailApp" , "" ).Trim();
            sMailParam  = g_tmSetup->ReadString( "OutBoxInfo" , "FailMailParam", "" ).Trim();

            sMailApp = ExtractFilePath( Application->ExeName ) + sMailApp ;
            sMailParam = AnsiReplaceStr( sMailParam , "<%MAILTO%>"  , adoQuery->FieldByName("EMAIL")->AsString.Trim() );
            sMailParam = AnsiReplaceStr( sMailParam , "<%CCODE%>"   , adoQuery->FieldByName("CCODE")->AsString.Trim() );
            sMailParam = AnsiReplaceStr( sMailParam , "<%ACODE%>"   , adoQuery->FieldByName("ACODE")->AsString.Trim() );
            sMailParam = AnsiReplaceStr( sMailParam , "<%DIALNO%>"  , adoQuery->FieldByName("DIALNO")->AsString.Trim() );

            g_CommLog->WriteLog( "[ThUMSAction::%s] ResName=[%s] Send Mail=[%s]" , FunctionName.c_str(), ResName.c_str() , ( sMailApp + " " + sMailParam ).c_str() );

            if( ES_ShellExecute( Application->Handle , "open" , sMailApp.c_str() , sMailParam.c_str() , NULL , SW_SHOWDEFAULT, &nEMailResult ) == false )
                nEMailResult = 99;
//            sSQL.sprintf( "Update tblOutMsgFax Set EMailResult=%d Where MSGID=%d", nEMailResult, nMsgId );
//            EXECSQL( adoQuery , sSQL.c_str() ) ;
        } // if
    } // if
}


