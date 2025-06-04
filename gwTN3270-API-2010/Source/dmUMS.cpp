//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "dmUMS.h"
#include "Global.h"
#include "Console.h"
#include <inifiles.hpp>
#include "ThEmuAction.h"
#include "APUtil.h"
#include "DBUnit.h"
#include <StrUtils.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "EMiddlewareController"
#pragma resource "*.dfm"

using namespace message_gateway ;

TdmgwUMS *dmgwUMS;
TCriticalSection *g_csLog;
TMemIniFile *g_tmSetup ;
const char * tempfile = "c:\\temp.bin" ;

bool TdmgwUMS::CheckDataExist( AnsiString sSerialNo , AnsiString Item , bool Booking )
{
    try {

    }
    catch ( ... ){

    }
}

bool TdmgwUMS::CardlinkLog( AnsiString REQNO , AnsiString REQTIME , AnsiString RECEIVETIME , AnsiString MsgId , AnsiString UserID , AnsiString AgentID , AnsiString ServiceName , AnsiString ParamStr , AnsiString ResultCode , AnsiString Desc , AnsiString LogFile )
{
    bool bOk = false ;
    if ( !m_LogDBEnabled ) return bOk;
    AnsiString sSQL ;
    try {
        g_csLog->Enter();
        sSQL.sprintf( "Insert Into tblCardlinkLog( REQNO , REQTIME , RECEIVETIME , EntityName , LogFile , MsgId , UserID ,ServiceName,ParamStr,ResultCode,AgentId,Description ) Values( '%s','%s', '%s', '%s','%s', '%s','%s','%s','%s','%s','%s','%s') " ,
              REQNO.c_str() , REQTIME.c_str() , RECEIVETIME.c_str() , EMC->EntityName.c_str() , LogFile.c_str() , MsgId.c_str() , UserID.c_str() ,ServiceName.c_str(),ParamStr.c_str() ,ResultCode.c_str(),AgentID.c_str() , Desc.c_str()
            ) ;
        ADOQuery->SQL->Text = sSQL ;
        ADOQuery->ExecSQL();
        bOk = true ;
    }
    catch ( Exception &e ) {
        g_CommLog->WriteLog( "CardlinkLog Exception : msgid=%s , SQL:%s" , MsgId.c_str() , sSQL.c_str() );
        //MessageBox( NULL , e.Message.c_str() , "Error" , MB_OK ) ;
    }
    catch ( ... ){
        //Application->MessageBox( "CardlinkLog unknown exception " , "Error" , MB_OK ) ;
        g_CommLog->WriteLog( "CardlinkLog Exception : msgid=%s" , MsgId.c_str() );
    }
    g_csLog->Leave();
    return bOk ;
}

bool TdmgwUMS::SetData( AnsiString sSerialNo , AnsiString Item , PElement * pe )
{
    bool bOk = false ;
    if ( !m_CacheDBEnabled ) return bOk;
    try {
        g_csLog->Enter();
        ADOQuery->SQL->Text = "Insert Into tblPreloadData  (SerialNo,Data ) Values( ?, ? ) " ;
        ADOQuery->Parameters->ParamByName("Param1")->Value = sSerialNo + "." + Item ;
        TParameter *p = ADOQuery->Parameters->ParamByName( "Param2" ) ;
        pe->save( tempfile ) ;
        p->LoadFromFile( tempfile , ftBlob ) ;
        ADOQuery->ExecSQL();
        bOk = true ;
    }
    catch ( ... ){

    }
    g_csLog->Leave();
    return bOk ;
}

bool TdmgwUMS::GetData( AnsiString sSerialNo , AnsiString Item , PElement * pe )
{
    bool bOk = false ;

    if ( !m_CacheDBEnabled ) return bOk;

    try {
        g_csLog->Enter();
        pe->clear();
        ADOQuery->SQL->Text = "select * from tblPreloadData where SerialNo = ? " ;
        ADOQuery->Parameters->ParamByName("Param1")->Value = sSerialNo + "." + Item ;
        ADOQuery->Open() ;
        bOk = ADOQuery->RecordCount > 0 ;
        if ( bOk ) {
            TBlobField * param =(TBlobField *) ADOQuery->FieldByName("Data") ;
            param->SaveToFile(tempfile);
            pe->load(tempfile);
        }
    }
    catch ( ... ){

    }
    g_csLog->Leave();
    return bOk ;
}

//---------------------------------------------------------------------------
__fastcall TdmgwUMS::TdmgwUMS(TComponent* Owner)
    : TDataModule(Owner)
{
    m_CacheDBEnabled = false ;
    m_LogDBEnabled = false ;
}
//---------------------------------------------------------------------------
void __fastcall TdmgwUMS::DataModuleCreate(TObject *Sender)
{
    frmResourceConsole->MonitorEntity( EMC ) ;
}
//---------------------------------------------------------------------------
/*
bool TfrmEncryptor::IsDesKeyBLOBExists()
{
    char sBLOB[512];

    DWORD rc = GetPrivateProfileString( "DBInfo", "BLOB", "", sBLOB, sizeof(sBLOB), m_FileName.c_str());

    if (rc > 0)
        return true;   //
    return false;   // key BLOB 不存在
}

AnsiString TdmgwUMS::DecryptKey( AnsiString EncryptedPassword )
{
    char 		l_Buffer[ 1024 ],
    			l_Hex[ 1024 ];
    DWORD		l_Length;
    char        l_Data[ 1024 ];
    AnsiString  m_ContainerName = "3DES_Engsound";
    try
    {
        AnsiString error_msg;

        GetPrivateProfileString("DBInfo", "Password", "", l_Data, sizeof(l_Data), m_FileName.c_str());
	    l_Length = HexToBin( EncryptedPassword.c_str() , l_Buffer, sizeof( l_Buffer ) );
       	try
   		{
            error_msg = "TripleDesCtx ";
    		TripleDesCtx l_tdc( m_ContainerName.c_str() );
			TripleDesKey l_tdk( &l_tdc );

            error_msg = "loadKey ";
            l_tdk.loadKey( "DBInfo", "BLOB", m_FileName.c_str() );
            error_msg = "decrypt ";
			l_Length = l_tdk.decrypt(l_Buffer, l_Length, true);
        	l_Buffer[ l_Length ] = 0;
            return AnsiString( l_Buffer );
        }
		catch (crypt_exception& e)
        {
		    ShowMessage( e.what() );
        }
		catch (...)
		{
			ShowMessage( "UnKnown Exception" );
		}
        return "";
	}
    catch( ... )
    {
    }
    return "";
}    */

void __fastcall TdmgwUMS::EMCOpen(TObject *Sender, bool &bOk)
{
    AnsiString sSection , sKey , sValue , sResName , sDBPWD , sSQL ;
    int i , nDBCount ;
    ThEmulatorAction * UMSThread ;
    TListConsoleItem * ConsoleItem ;

    try {
        g_csLog         = new TCriticalSection() ;
        m_ThreadList    = new TList() ;

        // 建立 Log 物件 - g_CommLog
        g_CommLog = new ECommLog( "Emu" , (ExtractFilePath( Application->ExeName ) + "Log").c_str() );
        g_CommLog->StartService( "CardlinkService" , "" ) ;
        g_CommLog->WriteLog( "=====================================================" );

        // 讀入相關設定值
        g_tmSetup = new TMemIniFile( ExtractFilePath( Application->ExeName ) + "Configure.Ini" );

        sSection = "UMSInfo" ;

        int bWaitDebug = g_tmSetup->ReadInteger( sSection , "WaitDebug" , 0 ) ;
        if ( bWaitDebug )
            Application->MessageBoxA("Wait ofr Debug ","Message",MB_OK);
        m_ClassName                     = g_tmSetup->ReadString( sSection , "ClassName" , "" ) ;
        EMC->ApplicationName            = g_tmSetup->ReadString( sSection , "ApplicationName" , "" ) ;
        EMC->EntityName                 = g_tmSetup->ReadString( sSection , "EntityName" , "" ) ;
        EMC->ProfileName                = g_tmSetup->ReadString( sSection , "ProfileName" , "" ) ;
        ADOConn->ConnectionString       = g_tmSetup->ReadString( sSection , "CacheDB" , "" ) ;
        if ( ADOConn->ConnectionString.Length() ) {
            ADOConn->Open() ;
            m_CacheDBEnabled = g_tmSetup->ReadInteger( sSection , "CacheDBEnabled" , 0 ) ==1 ;
            m_LogDBEnabled = g_tmSetup->ReadInteger( sSection , "LogDBEnabled" , 0 ) ==1 ;
        }
        //gEntityName 					= EMC->EntityName ;
        g_AutoRestart.WaitFor(EMC->EntityName.c_str(), 10000);
        Sleep(1000) ;

        EMC->EngsoundGatewayEnabled     = g_tmSetup->ReadInteger( sSection , "IsGateway" , 0 ) ;

        // 啟動 DB 執行緒
        nDBCount = g_tmSetup->ReadInteger( sSection , "ResCount" , 0 );
        
        for( i = 1 ; i <= nDBCount ; i++ ) {
            sKey = "ResName" + IntToStr( i ) ;
            sResName = g_tmSetup->ReadString( sSection , sKey , "" ) ;
            AnsiString uid , password ;

            uid = g_tmSetup->ReadString( sResName , "Userid" , "" ) ;
            password = g_tmSetup->ReadString( sResName , "Password" , "PHILIP" ) ;
            ConsoleItem = frmResourceConsole->CreateConsoleItem( sResName.c_str() ) ;
            UMSThread = new ThEmulatorAction( EMC , ConsoleItem , m_ClassName , sResName , uid , password );
            g_CommLog->WriteLog( "[EMCOpen] Append Resource, ClassName=[%s], ResName=[%s] , UserID=%s " , m_ClassName.c_str(), sResName.c_str() , uid.c_str() );
            m_ThreadList->Add( UMSThread );
            
        } // for
        EMC->DBConnectionString = g_tmSetup->ReadString( sSection , "MSGConn" , "" ) ;
    } // try
    catch( Exception & E ) {
        g_CommLog->WriteLog( "[EMCOpen Exception]: %s" , E.Message.c_str() );
    } // catch

}
//---------------------------------------------------------------------------
void __fastcall TdmgwUMS::EMCClose(TObject *Sender, bool &bOk)
{                                            
    if( g_tmSetup ) delete g_tmSetup ;
    g_CommLog->WriteLog( "Application End" ) ;

}
//---------------------------------------------------------------------------
void __fastcall TdmgwUMS::EMCGetMessage(TObject *Sender, void *msg,
      bool &Handled)
{
    ThEmulatorAction * UMSAction ;

    Message * gwmsg = (Message *)msg ;
    char evtmsg[4096] ;
    sprintf( evtmsg , "msgid = %d , evt ( %08x) = %s , src =%s , ticket = %u " , gwmsg->m_id , gwmsg->m_evt , GetEvtDescription(gwmsg->m_evt,NULL).c_str() ,gwmsg->m_src.c_str() ,gwmsg->m_ticket ) ;
    frmResourceConsole->OutputEventLog( evtmsg );

    UMSAction = ( ThEmulatorAction * )gwmsg->m_attach ;
    if( UMSAction ) {
        if( gwmsg->isResponse() ) {
            UMSAction->m_RespMsg.deserialize( gwmsg->m_data ) ;

            if( UMSAction->m_WaitMsgId == (int)gwmsg->m_id ) {
                if( UMSAction->m_IsWaiting )
                    UMSAction->m_IsWaiting = false ;
            } // if
        } // if
//        else if( ( UMSAction ) && ( UMSAction->m_WaitMsgId == gwmsg->m_id ) ) {
//            UMSAction->m_WaitMsgId = 0 ;
//        } // else if
    } // if

}
//---------------------------------------------------------------------------  

void __fastcall TdmgwUMS::EMCEnumMsgInfo(TObject *Sender, EMsgInfo *msginfo,
      bool &Handled)
{
    try {
        if (m_exportlog) {
            if ( strcmpi( msginfo->m_Action.c_str() , "COMMAND" ) == 0 ) {
                m_exportlog->Write( "seq=%s\taction=%s\t" , msginfo->m_seq.c_str() , msginfo->m_Action.c_str() ) ;
                PEStringMap *iparams = *msginfo->m_Params["IParams"] ;
                if (iparams == NULL) return ;
                PEStringMap::iterator it = iparams->begin() ;
                for ( ; it != iparams->end(); it++) {
                    string key = it->first ;
                    PElement * pe = it->second ;
                    string value ;
                    if ( pe->is_string_type() )
                         value = *pe ;
                    else {
                        if ( pe->is_numeric_type() ) {
                            long lvalue = *pe ;
                            AnsiString s = lvalue ;
                            value = s.c_str() ;
                        }
                        else
                            continue ;
                    }
                    m_exportlog->Write( "%s=%s\t" , key.c_str() , value.c_str() ) ;
                }
                m_exportlog->Write( "\n" ) ;
            }
        }
    }
    catch( Exception &E ) {
        g_CommLog->WriteLog( "[EMCEnumMsgInfo Exception]: %s" , E.Message.c_str() );
    }
}
//---------------------------------------------------------------------------

void __fastcall TdmgwUMS::EMCBeginEnumMsgInfo(TObject *Sender, bool &bOk)
{
    m_exportlog = new LogFile( "mpsdump" ) ;
}
//---------------------------------------------------------------------------

void __fastcall TdmgwUMS::EMCEndEnumMsgInfo(TObject *Sender, bool &bOk)
{
    delete m_exportlog ;
    m_exportlog = NULL ;
}
//---------------------------------------------------------------------------

