//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ThEmuAction.h"
#include "dmUMS.h"
#include "APUtil.h"
#include <oledberr.h>
#include "comobj.hpp"

#include <StrUtils.hpp>
#include <DateUtils.hpp>

#pragma package(smart_init)

#define MAXPROCESSTIME  8000

// =========================================================================
// Thread 物件
// =========================================================================

//---------------------------------------------------------------------------
//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//

//---------------------------------------------------------------------------
//  建構及解構 Function
//---------------------------------------------------------------------------

__fastcall ThEmulatorAction::ThEmulatorAction( EMiddlewareController *EMC , TListConsoleItem * ConsoleItem , AnsiString sClassName , AnsiString sResName , AnsiString UserID , AnsiString Password )
    : ETaskWorkingThread( 30000 , "ThEmulatorAction" )
{
    try {
        mEMC = EMC ;
        m_ConsoleItem   = ConsoleItem ;
        m_ResName       = sResName ;
        m_ClassName     = sClassName ;
        m_UserID        = UserID ;
        m_Password      = Password ;
        m_TempFile      = ExtractFilePath( Application->ExeName ) + "_tmp" + m_ResName ;
        m_TempStream    = new TMemoryStream() ;
        FreeOnTerminate = true ;
        m_TimeoutInterval = 0 ;
        m_PrevTimeoutTick = 0 ;
        m_ResItem       = EMC->AppendResource( m_ClassName.c_str() , m_ResName.c_str() , NULL , this , m_ConsoleItem ) ;
        m_ResItem->Suspend() ;
        m_IsConnect     = false ;
        m_ConfigIni = new TIniFile( ExtractFilePath( Application->ExeName ) + "Configure.Ini" );
        m_TimeoutInterval = m_ConfigIni->ReadInteger( "UMSInfo" , "TimeoutInterval" , 0 ) ;
        m_ChangeSessionOwner =  m_ConfigIni->ReadInteger( "UMSInfo" , "ChangeSessionOwner" , 0 ) == 1 ;
        UpdateMessage( "Waitting ..." ) ;
        Resume() ;
    } // try
    catch( Exception & E ) {
        g_CommLog->WriteLog( "[ThEmulatorAction::ThEmulatorAction] ResName=[%s] Exception=[%s]" , m_ResName.c_str() , E.Message.c_str() );
    } // catch
}

__fastcall ThEmulatorAction::~ThEmulatorAction()
{
    delete m_TempStream ;
}


//---------------------------------------------------------------------------
//  Log 狀態訊息處理 Function
//---------------------------------------------------------------------------


// 更新畫面上的狀態
void __fastcall ThEmulatorAction::UpdateMsg( )
{
    m_ConsoleItem->ShowStatus( m_Message.c_str() );

}


// 顯示訊息至畫面上
void ThEmulatorAction::UpdateMessage( AnsiString sMsg , bool bLog )
{
    m_Message = sMsg ;
    Synchronize( UpdateMsg );
    if( bLog )
        g_CommLog->WriteLog( "[ThEmulatorAction::UpdateMessage] ResName=[%s] Msg=[%s]" , m_ResName.c_str() , m_Message.c_str() );
}

//---------------------------------------------------------------------------
//  Message 相關 Function
//---------------------------------------------------------------------------

bool ThEmulatorAction::OnInit()
{
    CoInitialize( NULL );
    try {

    } // try
    catch( Exception &E ) {
        g_CommLog->WriteLog( "[ThEmulatorAction::OnInit] ResName=[%s] Exception=[%s]" , m_ResName.c_str() , E.Message.c_str() );
        return false ;
    } // catch

    return true ;
}

bool ThEmulatorAction::OnExit()
{
    try {

    } // try
    catch( Exception &E ) {
        g_CommLog->WriteLog( "[ThEmulatorAction::OnExit] ResName=[%s] Exception=[%s]" , m_ResName.c_str() , E.Message.c_str() );
    } // catch
    CoUninitialize( ) ;
    return true ;
}

bool ThEmulatorAction::OnResourceOpen()
{
    bool rc = false ;
    AnsiString sSQL ;

    try {
        g_CommLog->WriteLog( "[ThEmulatorAction::OnResourceOpen] ResName=[%s] " , m_ResName.c_str()  );

        // m_TempDBFile.sprintf( "temp-%0X.mdb" , this ) ;
        // CopyFile( "ScreenLayoutInfo.mdb" , m_TempDBFile.c_str() , false ) ;
        m_TempDBFile = "ScreenLayoutInfo.mdb" ;
        m_ADOConn = new TADOConnection( NULL ) ;
        m_ADOConn->ConnectionString = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + m_TempDBFile + ";Mode=Read;Persist Security Info=False" ;
        // m_ADOConn->ConnectionString = "Provider=SQLOLEDB.1;Password=ab1234..;Persist Security Info=True;User ID=sa;Initial Catalog=EmuScript;Data Source=CASSELL";
        m_ADOConn->LoginPrompt = false ;
        m_ADOConn->Open();
        m_testini = new TIniFile( "./TEST.ini" ) ;
        m_pcom = new CEmuService( m_ADOConn );
        m_ds = new TADODataSet( NULL ) ;
        m_pcom->SetLoginInfo( m_UserID , m_Password ) ;
        m_ConsoleItem->ShowDescription( m_UserID.c_str() ) ;
        //m_pcom->OpenLink( m_ResName ) ;
        rc = true ;
        m_ResItem->Resume();

    } // try
    catch( Exception & E ) {
        g_CommLog->WriteLog( "[ThEmulatorAction::OnResourceOpen] ResName=[%s] Exception=[%s]" , m_ResName.c_str() , E.Message.c_str() );
    } // catch

    return rc ;
}

bool ThEmulatorAction::OnResourceClose()
{
    UpdateMessage( "已斷線！" ) ;
    delete m_pcom ;
    return true ;
}


bool ThEmulatorAction::OnTimeOut()
{
    return true ;
    /*switch( m_ResItem->GetStatus() ) {
        case rsInactive:    UpdateMessage( "Inactive"       , false ) ;   break;
        case rsIdle:        UpdateMessage( "等待資料(Idle)" , false ) ;   break;
        case rsSuspend:     UpdateMessage( "Suspend"        , false ) ;   break;
        case rsBusy:        UpdateMessage( "Busy"           , false ) ;   break;
        case rsBooking:     UpdateMessage( "Booking"        , false ) ;   break;
        case rsDestroy:     UpdateMessage( "Destroy"        , false ) ;   break;
        default:            UpdateMessage( "末知狀態"       , false ) ;   break;
    } // switch

    if( ( !m_IsConnect ) || ( m_ResItem->GetStatus() == rsSuspend ) ) {

    } // if

    CheckTimeoutProcess();

    return true ; */
}

bool CheckDataExist( AnsiString sSerialNo , AnsiString Item )
{

}

bool SetData( AnsiString sSerialNo , AnsiString Item , PElement * pe )
{

}

bool GetData( AnsiString sSerialNo , AnsiString Item , PElement * pe )
{

}

bool ThEmulatorAction::RunService( EMsgInfo *msginfo )
{
    bool brc = true ;
    long nMsgId ;
    try {
        // 設定傳回值

        m_ResultParams.reset() ;
        m_ResultCode = -1 ;
        m_ResultMsg = "" ;
        bool bChangeOwner = m_ChangeSessionOwner ;
        g_CommLog->WriteLog( "[ThEmulatorAction::OnGetRequest] ResName=[%s] MsgID=[%d] MsgType=[%d] Action=[%s]" , m_ResName.c_str() , msginfo->m_msgid , msginfo->m_mwtype , msginfo->m_Action.c_str() );
        DWORD tick = GetTickCount();
        switch( msginfo->m_mwtype ) {
            case mwApplication :
                            {
                            UpdateMessage( msginfo->m_Action.Trim().UpperCase() + " 處理中！" ) ;
                            int rc = -100 ; // Initial Value
                            PElement *pe = msginfo->m_Params["ParamStr"];
                            PElement *peguid = msginfo->m_Params["GUID"];
                            PElement *peuserid = msginfo->m_Params["UserID"];
                            PElement *pereqno = msginfo->m_Params["REQNO"];
                            PElement *pereqtime = msginfo->m_Params["REQTIME"];
                            PElement peitem ;
                            std::string ParamStr = pe ? *pe : PElement("");
                            PEStringMap dataset;
                            AnsiString ResultRecordList ;
                            AnsiString ServiceName = msginfo->m_Action ;
                            AnsiString ReceiveTime = Now().FormatString( "yyyy/mm/dd hh:nn:ss") ;
                            string  guid = peguid ? *peguid : PElement("") ;
                            string  userid = peuserid ? *peuserid : PElement("") ;
                            string  reqno =  pereqno ? *pereqno : PElement("") ;
                            string  reqtime =  pereqtime ? *pereqtime : PElement("2000/1/1 00:00:00") ;
                            AnsiString itemkey = ServiceName + "." + ParamStr.c_str() ;

                            int iDebug = m_testini->ReadInteger( "Common" , "Debug" , 0 ) ;
                            int nSleep = 0 ;
                            bool bKeepLog = (ServiceName.UpperCase() != "PINGALIVE") ;
                            if ( ServiceName.UpperCase() == "PINGALIVE" ) {
                               bChangeOwner == false ;
                            }

                            if ( bChangeOwner ) {
                                m_pcom->SetLoginInfo( userid.c_str() , "A1234" ) ;
                                m_ConsoleItem->ShowDescription( userid.c_str() ) ;
                            }

                            switch ( iDebug ) {
                                case 0 : // Normal
                                case 1 : // For Step Debug
                                {
                                    if ( true || peguid == NULL || dmgwUMS->GetData( guid.c_str() , itemkey , &peitem) == false ) {
                                        bool btemp = (iDebug == 1) ;  // for Step Debug ...
                                        if ( bChangeOwner ) {
                                            rc = m_pcom->ExecService( "LOGOUT" , "" , m_ResName , false , btemp , NULL ) ;
                                        }
                                        rc = m_pcom->ExecService( ServiceName.c_str() , ParamStr.c_str() , m_ResName , true , btemp , NULL ) ;
                                        m_pcom->m_DataSets->Save( dataset ) ;
                                        ResultRecordList = m_pcom->m_DataSets->GetRecordsetNameList() ;
                                        msginfo->m_Params.set("DataSet", new PElement(dataset));
                                        msginfo->m_Params.set("RecordList", new PElement(ResultRecordList.c_str()));
                                        msginfo->m_Params.set("ResultCode", new PElement(rc));
                                        if ( rc == 0 ) {
                                            PElement * pe_param = new PElement(msginfo->m_Params) ;
                                            dmgwUMS->SetData( guid.c_str() , itemkey , pe_param );
                                            delete pe_param ;
                                        }
                                        /*
                                        else if ( ServiceName.UpperCase() != "PINGALIVE" ) { // Exec Fail , check is session lose
                                            rc = m_pcom->ExecService( "PINGALIVE" , "" , m_ResName , false , btemp , NULL ) ;
                                            if ( rc != 0 )
                                                m_ResItem->Suspend();
                                        }
                                        */
                                        if ( bChangeOwner ) {
                                            rc = m_pcom->ExecService( "LOGOUT" , "" , m_ResName , false , btemp , NULL ) ;
                                            m_pcom->SetLoginInfo( m_UserID , m_Password ) ;
                                            m_ConsoleItem->ShowDescription( m_UserID.c_str() ) ;
                                        }
                                    }
                                    else {
                                        msginfo->m_Params = *(PEStringMap*)peitem ;
                                        rc = 0 ;
                                    }
                                }
                                break ;
                                case 2 :
                                    // Sleep
                                    nSleep = m_testini->ReadInteger( "Common" , "Sleep" , 3000 ) ;
                                    Sleep (nSleep);
                                    //Application->ProcessMessages();
                                    break ;
                                case 3 : // Do Not thing
                                    break ;
                            } // switch

                            int processtime = GetTickCount() - tick ;
                            AnsiString sAction = msginfo->m_Action ;
                            AnsiString RespTime = Now().FormatString( "yyyy/mm/dd hh:nn:ss") ;
                            msginfo->m_Params.set("ReceiveTime", new PElement(ReceiveTime.c_str()));
                            msginfo->m_Params.set("RespTime", new PElement(RespTime.c_str()));
                            msginfo->m_Params.set("ProcessTime", new PElement( processtime ));
                            msginfo->m_Params.set("EntityName", new PElement( mEMC->EntityName.c_str() ));
                            mEMC->SetTaskResult((unsigned long)msginfo, msginfo->m_Action + ".Result", rc , msginfo->m_Params, mEMC->EntityName) ;

                            AnsiString sResult ;
                            sResult.sprintf( "ReceiveTime : %s , RespTime : %s , Action %s , ResultCode = %d , ProcessTime : %d " , ReceiveTime.c_str() , RespTime.c_str() , sAction + ".Result" , rc , processtime  ) ;
                            UpdateMessage( sResult ) ;

                            // WriteBackToDB
                            if ( peuserid || bKeepLog || rc != 0 || processtime >= MAXPROCESSTIME )
                                dmgwUMS->CardlinkLog( reqno.c_str() , reqtime.c_str() , ReceiveTime.c_str() , IntToStr(msginfo->m_gwmsgid) , userid.c_str() , m_UserID.c_str() , ServiceName.c_str() , ParamStr.c_str() , IntToStr(rc) , sResult ,  m_pcom->m_logger->GetFileName() ) ;

                            try { // Clear Log
                                AnsiString slogfile = m_pcom->m_logger->GetFileName() ;
                                if ( FileExists( slogfile ) ) {
                                    AnsiString sWinCmd = m_ConfigIni->ReadString( "AfterServiceExecute" , "WINCMD" , "" ) ;
                                    if ( sWinCmd.Length() ) {
                                        sWinCmd = AnsiReplaceStr( sWinCmd , "$(LogFileName)" , m_pcom->m_logger->GetFileName() ) ;
                                        sWinCmd = AnsiReplaceStr( sWinCmd , "$(Password)" , "sinopac" ) ;
                                        int nHide = m_ConfigIni->ReadInteger( "AfterServiceExecute" , "Hide" , 0 ) ;
                                        int showmode = SW_SHOW ;
                                        if ( nHide != 0 )
                                            showmode = SW_HIDE ;
                                        WinExec(sWinCmd.c_str(),showmode);
                                    }
                                    if ( rc == 0 && !bKeepLog )
                                        DeleteFile( slogfile ) ;
                                }
                                else {
                                    g_CommLog->WriteLog( "Log File not exists=[%s]" , slogfile.c_str() );
                                }
                            }
                            catch ( Exception &e ) {
                                g_CommLog->WriteLog( "Clear Log Exception ResName=[%s] Exception=[%s]" , m_ResName.c_str() , e.Message.c_str() );
                            }

                            //pcom->ExecService( "InitEnter" , "" , lbleTopic->Text , true , cbStepDebug->Checked , &m_Terminated ) ;
                            //nMsgId = mEMC->SetTaskResult( (unsigned long)&msginfo , msginfo.m_Action , 0 , msginfo.m_Params , m_ResName );
                            }
                            break ;
            case mwAdministration :
            case mwAlarm :
            default :
                            // 不支援此動作
                            //g_CommLog->WriteLog( "[ThEmulatorAction::OnGetRequest] ResName=[%s] MsgType Not Support=[%s]" , m_ResName.c_str() , msginfo.m_Action.c_str() );
                            nMsgId = mEMC->SetTaskResult( (unsigned long)&msginfo , msginfo->m_Action , -99 , msginfo->m_Params , m_ResName );
                            //g_CommLog->WriteLog( "[ThEmulatorAction::OnGetRequest] ResName=[%s] SetTaskResult MsgID=[%d] " , m_ResName.c_str() , nMsgId );
                            break;
        } // switch
    } // try
    catch( Exception & E ) {
        g_CommLog->WriteLog( "[ThEmulatorAction::OnGetRequest] ResName=[%s] Exception=[%s]" , m_ResName.c_str() , E.Message.c_str() );
        nMsgId = mEMC->SetTaskResult( (unsigned long)msginfo , msginfo->m_Action , -99 , msginfo->m_Params , m_ResName );
        g_CommLog->WriteLog( "[ThEmulatorAction::OnGetRequest] ResName=[%s] SetTaskResult MsgID=[%d] " , m_ResName.c_str() , nMsgId );
    } // catch
    return brc ;
}

bool ThEmulatorAction::OnGetRequest( EMsgInfo &msginforef )
{
    EMsgInfo *msginfo = &msginforef ;
    return RunService( msginfo ) ;
}

void ThEmulatorAction::LoadLoginInfo()
{
    TMemIniFile * tmSetup = new TMemIniFile( ExtractFilePath( Application->ExeName ) + "Configure.Ini" );
    m_UserID = tmSetup->ReadString( m_ResName , "Userid" , "" ) ;
    m_Password = tmSetup->ReadString( m_ResName , "Password" , "PHILIP" ) ;
    delete tmSetup ;
}

bool ThEmulatorAction::OnGetInfo(EMsgInfo &msginfo)
{
    UpdateMessage( Now().FormatString("hh:nn:ss").c_str() ) ;
    bool btemp = false ;
    AnsiString ServiceName ;
    string ParamStr ;

    LoadLoginInfo();

    int iDebug = m_testini->ReadInteger( "Common" , "Debug" , 0 ) ;
    ServiceName = m_testini->ReadString( msginfo.m_Action , "Service" , "test" ) ;
    ParamStr = m_testini->ReadString( msginfo.m_Action , "ParamStr" , "params").c_str() ;
    DWORD tick = GetTickCount();
    int rc = 0 ;

    if ( iDebug < 2 )
        rc = m_pcom->ExecService( ServiceName.c_str() , ParamStr.c_str() , m_ResName , true , btemp , NULL ) ;

    AnsiString sResult ;
    sResult.sprintf( "Action : %s , ResultCode = %d , SpendTime : %d " ,  "LoclTest", rc , GetTickCount() - tick ) ;
    UpdateMessage( sResult ) ;
}

bool ThEmulatorAction::CheckTimeoutProcess()
{
    if ( m_TimeoutInterval ) {
        if ( m_PrevTimeoutTick == 0 )
            m_PrevTimeoutTick = GetTickCount() ;
        else {
            DWORD tick = GetTickCount() ;
            if ( (tick - m_PrevTimeoutTick) > m_TimeoutInterval ) {
                m_PrevTimeoutTick = tick ;
                UpdateMessage( "DoTimeoutService" ) ;
                DoTimeoutService();
            }
        }
    }
    m_TimeoutInterval = m_ConfigIni->ReadInteger( "UMSInfo" , "TimeoutInterval" , 0 ) ;
}

bool ThEmulatorAction::DoTimeoutService()
{
    AnsiString ServiceName = m_ConfigIni->ReadString( "UMSInfo" , "Service" , "test" ) ;
    AnsiString ParamStr = m_ConfigIni->ReadString( "UMSInfo" , "ParamStr" , "params").c_str() ;
    // rc = m_pcom->ExecService( ServiceName.c_str() , ParamStr.c_str() , m_ResName , true , btemp , NULL ) ;
    g_CommLog->WriteLog( "[ThEmulatorAction::DoTimeoutService] " );
}

bool ThEmulatorAction::OnGetMessage(EMsgInfo &msginfo, bool &Handled)
{
    return true ;
}




