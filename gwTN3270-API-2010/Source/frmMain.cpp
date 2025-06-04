//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "frmMain.h"
#include "ThDB.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "EMiddlewareController"
#pragma resource "*.dfm"

using namespace message_gateway ;
using namespace _com_util ;
DECLARE_ENVIRONMENT;
DECLARE_THREAD_CONTROLLER;

TfrmDB * frmDB;

//---------------------------------------------------------------------------
__fastcall TfrmDB::TfrmDB(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfrmDB::FormCreate(TObject *Sender)
{
    TMemIniFile * tmSetup ;
    AnsiString sSection , sKey , sValue , sResName ;
    int i , nDBCount ;
    TListItem * lsItem ;
    ThDBAction * DBThread ;

    try {
        g_csLog = new TCriticalSection() ;
        m_ThreadList = new TList() ;

        // 抓取執行後面的參數 (判斷是否進行 Debug Mode )
        gIsDebugMode    = false ;
        gIsVirtualMode  = false ;
        for( i = 1 ; i <= ParamCount() ; i++ ) {
            if( ParamStr(i).UpperCase() == "-D" )   gIsDebugMode    = true ;
            if( ParamStr(i).UpperCase() == "-V" )   gIsVirtualMode  = true ;
        } // for

        // 建立 Log 物件 - g_CommLog
        g_CommLog = new ECommLog( (ExtractFilePath( Application->ExeName ) + "Log").c_str() );

        g_CommLog->WriteLog( "=====================================================" );
        g_CommLog->WriteLog( "Application Start" );

        // 讀入相關設定值
        tmSetup = new TMemIniFile( ExtractFilePath( Application->ExeName ) + "gwdb.Ini" );

        sSection = "DBInfo" ;
        m_ClassName                     = tmSetup->ReadString( sSection , "ClassName" , "" ) ;
        EMC->ApplicationName            = tmSetup->ReadString( sSection , "ApplicationName" , "" ) ;
        EMC->EntityName                 = tmSetup->ReadString( sSection , "EntityName" , "" ) ;
        EMC->ProfileName                = tmSetup->ReadString( sSection , "ProfileName" , "" ) ;
        EMC->EngsoundGatewayEnabled     = tmSetup->ReadInteger( sSection , "IsGateway" , 0 ) ;
        gConnStr                        = tmSetup->ReadString( sSection , "ConnStr" , "" ) ;


        // 啟動 DB 執行緒
        lvShowState->Items->Clear() ;
        nDBCount = tmSetup->ReadInteger( sSection , "ResCount" , 0 );
        for( i = 1 ; i <= nDBCount ; i++ ) {
            sKey = "ResName" + IntToStr( i ) ;
            sResName = tmSetup->ReadString( sSection , sKey , "" ) ;

            // 加入畫面上的 ListView
            lsItem = lvShowState->Items->Add();

            DBThread = new ThDBAction( EMC , lsItem , sResName );
            m_ThreadList->Add( DBThread );
            EResItem * resitem = EMC->AppendResource( m_ClassName.c_str() , sResName.c_str() , NULL , DBThread , NULL) ;
        } // for
        
        EMC->Open() ;

    } // try
    catch( Exception & E ) {
        g_CommLog->WriteLog( "[Form Create Exception]: %s" , E.Message.c_str() );
    } // catch

    if( tmSetup ) delete tmSetup ;

}
//---------------------------------------------------------------------------

void __fastcall TfrmDB::FormDestroy(TObject *Sender)
{

    g_CommLog->WriteLog( "Application End" );

    // 刪除 Log 物件 - g_CommLog
    delete g_CommLog ;
    delete g_csLog ;
    delete m_ThreadList ;
}
//---------------------------------------------------------------------------


void __fastcall TfrmDB::FormClose(TObject *Sender, TCloseAction &Action)
{
    int nIndex , nCnt = 0 ;
    ThDBAction * DBThread ;
    TListItem * lsItem ;
    int hResult ;
    HANDLE hThread[255] ;

    for( nIndex = 0 ; nIndex < lvShowState->Items->Count ; nIndex++ ) {
        lsItem = lvShowState->Items->Item[nIndex] ;
        if( lsItem->SubItems->Count <= 0 ) lsItem->SubItems->Add("") ;
        lsItem->SubItems->Strings[0] = lsItem->SubItems->Strings[0] + " - 等待結束" ;
    } // if

    // 等待停止所有的執行緒
    for( nIndex = 0 ; nIndex < m_ThreadList->Count ; nIndex++ ) {
        DBThread = (ThDBAction *)( m_ThreadList->Items[nIndex] ) ;
        DBThread->Terminate() ;
        hThread[nIndex] = (HANDLE)DBThread->Handle ;
    } // for

    while( ++nCnt <= 60000) {
        hResult = WaitForMultipleObjects( m_ThreadList->Count , hThread , true , 1 );
        if( hResult != WAIT_TIMEOUT	 ) {
            break;
        } // if
        Application->ProcessMessages() ;
    } // while

}
//---------------------------------------------------------------------------

void __fastcall TfrmDB::bbQueryTestClick(TObject *Sender)
{
    PEStringMap   Params ;
    string sSQL ;
    sSQL = "Select * From BBPFXUS" ;
    Params.set("SQL", new PElement(sSQL));
    EMC->AppendTask(mwApplication , "" , "Query" , &Params , "TaskOwner" , ctLocalProcess ) ;
}
//---------------------------------------------------------------------------

void __fastcall TfrmDB::bbExecuteTestClick(TObject *Sender)
{
    PEStringMap   Params ;
    string sSQL ;
    sSQL = "Insert Into CIF (CustomerID,Name,DOB,Gender) Values( 'B987654321' , 'abc' , '901231' , '0' )" ;
    Params.set("SQL", new PElement(sSQL));
    EMC->AppendTask(mwApplication , "" , "Execute" , &Params , "TaskOwner" ,ctLocalProcess ) ;
}
//---------------------------------------------------------------------------

void __fastcall TfrmDB::bbStoreTestClick(TObject *Sender)
{
    PEStringMap   Params ;
    PEStringMap   * IParams ;

    IParams = new PEStringMap() ;
    string sProcName ;
    sProcName = "spSetCIF" ;
    Params.set("ProcName", new PElement(sProcName));

    IParams->set("@CustomerId", new PElement("F123456789")) ;
    IParams->set("@Name", new PElement("test")) ;
    IParams->set("@DOB", new PElement("920101")) ;
    IParams->set("@Gender" , new PElement("1")) ;

    Params.set("IParams", new PElement( IParams ));
    EMC->AppendTask(mwApplication , "" , "StoreProc" , &Params , "TaskOwner" , ctLocalProcess ) ;
}
//---------------------------------------------------------------------------

