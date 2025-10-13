//---------------------------------------------------------------------------

#ifndef PComUnitH
#define PComUnitH

#include <ADODB.hpp>
#include <StrUtils.hpp>
#include <IniFiles.hpp>
#include <stdio.h>
#include <fcntl.h>
#include <sys\stat.h>

#include "EhllapiImpl.h"
#include "EmuScreenUtil.h"
#include "Ehlapi32.h"
#include "APUtil.h"
//---------------------------------------------------------------------------

#define WAITFOR_PROCESSING      7000
#define WAITBEFORE_GETPS        1500
#define WAITBEFORE_SENDKEY      500
#define WAITENTER               1000
#define WAITCOMMON              40

int GetParamsN(TStrings *tsParams) {
    tsParams->Clear();
    for (int i = 0; i < ParamCount(); i++) {
        tsParams->Add(ParamStr(i + 1));
    }
    return tsParams->Count;
}

class CBaseEmulator {
protected :
    bool m_Connected ;
    bool InternalOpenSQL( AnsiString sSQL , TADOQuery * qry )
    {   bool bRc = false ;
        try {
            s_cs->Enter();
            bRc = OpenSQL( sSQL , qry ) ;
        }
        __finally {
            s_cs->Leave();
        }
        return bRc ;
    }

public :
    enum EmulatorStatus { esUnexpected = 0 , esIdle , esBusy , esInputError } ;
    TIniFile * m_IniFile ;

    __fastcall virtual bool doOpenLink( const AnsiString SessionID ) = 0 ;
    __fastcall virtual void doCloseLink() = 0 ;
    __fastcall virtual bool doSetCursor( AnsiString offset ) = 0 ;
    __fastcall virtual EmulatorStatus doGetOIA( ) = 0 ;
    __fastcall virtual AnsiString doGetScreenText( ) = 0 ;
    __fastcall virtual bool doExecMacroKey( const AnsiString MacroKey ) = 0 ;
    __fastcall virtual AnsiString doGetSessionList( ) = 0 ;
    __fastcall virtual int doGetCursorLocation( ) = 0 ;

protected :
    EmulatorStatus m_Status ;
    AnsiString m_ScreenPath ;
    static TCriticalSection *s_cs ;
    AnsiString m_AppPath ;
    static void Initialize()
    {
        if ( s_cs == NULL )
            s_cs = new TCriticalSection() ;
    }
public :
    AnsiString m_SessionID ;
    int m_TraceMode ;  // 0 : None , 3 : Write Sample Data , 2 : Read Sample Data , 1 : Write Trace Log 
    int m_CacheMode ;
    int m_WaitCommon ; // for common interval
    int m_WaitEnter;
    int m_WaitForProcessing ;
    int m_WaitBeforeSendKey ;
    int m_WaitBeforeGetPS ;
    int m_ScreenTextFormat ;
    int m_TimerInterval ;
    int m_MaxSubmitRetry ;
    int m_MaxRecordCount ;
    int m_MaxPanelPages ;
    int m_ReturnByRecord ;
    
    ECommLog * m_logger ;
    //AnsiString m_LastStatus ;
    CBaseEmulator() : m_Connected(false) {
        Initialize();
        m_logger = new ECommLog( "Emulator" , ( ExtractFilePath( Application->ExeName ) + "Log" ).c_str() );
        m_AppPath = ExtractFilePath( Application->ExeName ) ;
        m_IniFile   = new TIniFile( ExtractFilePath( Application->ExeName )+"Script\\as400.ini" );
        m_Connected = false ;
        if( !m_IniFile )
        {
            m_logger->WriteLog("Error: Cannot open [%s].","as400.ini");
            DevproDebugString(AnsiString().sprintf("Error: Cannot open [%s].","as400.ini").c_str());
            Application->Terminate();
        }
        ReadConfigFile();
    }

    virtual ~CBaseEmulator() {
        delete m_IniFile ;
    }

    void ReadConfigFile( char * sSessionName = "Config" )
    {
        if ( !m_IniFile->SectionExists(sSessionName) ) sSessionName = "Config" ; 
        m_TraceMode  = m_IniFile->ReadInteger("Debug", "TraceMode", 0 );
        m_CacheMode  = m_IniFile->ReadInteger("Debug", "CacheMode", 0 );
        m_WaitCommon  = m_IniFile->ReadInteger(sSessionName, "WaitCommon", WAITCOMMON );
        
        m_WaitEnter   = m_IniFile->ReadInteger(sSessionName, "WaitEnter", WAITENTER );
        m_TimerInterval   = m_IniFile->ReadInteger(sSessionName, "TimerInterval", WAITCOMMON );
        m_WaitForProcessing   = m_IniFile->ReadInteger(sSessionName, "WaitForProcessing", WAITFOR_PROCESSING);
        m_WaitBeforeSendKey  = m_IniFile->ReadInteger(sSessionName, "WaitBeforeSendKey", WAITBEFORE_GETPS);
        m_WaitBeforeGetPS   = m_IniFile->ReadInteger(sSessionName, "WaitBeforeGetPS", WAITBEFORE_SENDKEY);
        m_MaxSubmitRetry   = m_IniFile->ReadInteger(sSessionName, "MaxSubmitRetry", 2 );

        m_MaxRecordCount = m_IniFile->ReadInteger(sSessionName, "MaxRecordCount", 500 );
        m_MaxPanelPages = m_IniFile->ReadInteger(sSessionName, "MaxPanelPages", 250 );
        m_ReturnByRecord = m_IniFile->ReadInteger(sSessionName, "ReturnByRecord", 1 );

        m_logger->m_WriteToFile = ( m_TraceMode != 0 ) ;
    }

    __fastcall EmulatorStatus GetOIA( ) {
        if ( m_TraceMode == 2 ) return esIdle ;
        return doGetOIA( ) ;
    } ;

    __fastcall void Wait(int ms)
    {
        DWORD tick = GetTickCount();
        while(1)
        {
            //Application->ProcessMessages();
            Sleep( m_TimerInterval );
            if((GetTickCount()-tick) > ms) break;
        }
    }

    bool OpenLink( AnsiString SessionName ) {
        bool bOk ;
        if ( m_TraceMode == 2 ) return true ;
        s_cs->Enter();
        try {
            bOk = doOpenLink(SessionName) ;
            m_Connected = bOk ;
            m_logger->WriteLog("Open Session %s %s !",SessionName.c_str() , bOk ? "Ok": "Fail");
            DevproDebugString(AnsiString().sprintf("Open Session %s %s !",SessionName.c_str() , bOk ? "Ok": "Fail").c_str());
        }
        __finally {
            s_cs->Leave();
        }
        //AnsiString sText = GetScreenText() ;
        return bOk ;
    }

    void CloseLink( ) {
        if ( m_TraceMode == 2 ) return ;
        doCloseLink();
    }

    EmulatorStatus WaitForReady( int prevtick , int nTimeOut = -1 , bool bAutoReset = true ) {
        bool bOk = false ;
        AnsiString StatusString ;
        EmulatorStatus rc = esUnexpected ;
        m_logger->WriteLog( "Wait for ready (%d) ..." , nTimeOut ) ;
        DevproDebugString(AnsiString().sprintf( "Wait for ready (%d) ..." , nTimeOut ).c_str());
        char * status = "Unexpected" ;
        if ( nTimeOut < 0 ) nTimeOut = m_WaitForProcessing ;
        while ( (GetTickCount() - prevtick ) <= nTimeOut && !bOk ) {
            rc = GetOIA( ) ;
            switch( rc ) {
                case esIdle :
                    status = "OIA Idle" ;
                    break ;
                case esBusy :
                    nTimeOut = m_WaitForProcessing ;
                    status = "OIA Busy" ;
                    break ;
                case esInputError :
                    status = "OIA InputError" ;
                    if ( bAutoReset )
                        doExecMacroKey( "Reset" ) ;
                    break ;
                case esUnexpected :
                    m_logger->WriteLog( "GetOIA Error" );
                    DevproDebugString( "GetOIA Error" );
                    break ;
            }
            bOk = ( rc == esIdle ) ;
            if ( !bOk )
                Wait( m_WaitCommon ) ;
        }
        m_logger->WriteLog( "Wait for ready : %s " , status ) ;
        DevproDebugString(AnsiString().sprintf( "Wait for ready : %s " , status ).c_str());
        m_Status = rc ;
        return rc ;
    }

    AnsiString GetSessionList( )
    {
        return doGetSessionList();
    }

    __fastcall bool ExecMacroKey( AnsiString sKey ) {

        m_Status = GetOIA( ) ;
        if ( m_Status != esIdle )
           m_logger->WriteLog( "Before ExecMacro Key [%s] error ! status : %d" , sKey.c_str() , m_Status );

        AnsiString sMacro = sKey ;
        DWORD prevtick = GetTickCount();
        bool bOk = true ;
        if ( (m_Status == esIdle) && m_TraceMode != 2 )
            bOk = doExecMacroKey( sMacro.c_str() ) ;

        m_Status = WaitForReady( GetTickCount() ) ;
        m_logger->WriteLog( "TickLog \t%s\t%d" , "doExecMacroKey" , GetTickCount()-prevtick ) ;
        DevproDebugString(AnsiString().sprintf( "TickLog \t%s\t%d" , "doExecMacroKey" , GetTickCount()-prevtick ).c_str());
        m_logger->WriteLog( "SendKey [%s] %s" , sMacro.c_str() , bOk ? "ok" : "fail" ) ;
        DevproDebugString(AnsiString().sprintf( "SendKey [%s] %s" , sMacro.c_str() , bOk ? "ok" : "fail" ).c_str()) ;

        bOk = (m_Status == esIdle) ;
        if ( !bOk )
           m_logger->WriteLog( "After ExecMacro Key [%s] error ! status : %d" , sKey.c_str() , m_Status );

        return bOk ;
    }

    __fastcall int GetCursorLocation()
    {
        int pos = doGetCursorLocation() ;
        m_logger->WriteLog( "GetCursorLocation : %d" , pos );
        DevproDebugString(AnsiString().sprintf( "GetCursorLocation : %d" , pos ).c_str());
        return pos ;
    }

    __fastcall AnsiString GetScreenText( AnsiString sReason = "" ) {
        AnsiString sScreenText ;
        AnsiString sTempFile ;
        switch ( m_TraceMode ) {
            case 0 : // None
            case 1 : // None
                sScreenText = doGetScreenText( ) ;
                break ;
            case 3 : // Write Sample Data
                {
                sScreenText = doGetScreenText( ) ;
                TStringList * ts = new TStringList ;
                sScreenText = doGetScreenText( ) ;
                ts->Text = sScreenText ;
                sTempFile = m_AppPath + m_ScreenPath + "-" + sReason + ".txt" ;
                ts->SaveToFile( sTempFile ) ;
                delete ts ;
                }
                break ;
            case 2 : // Read Sample Data
                {
                TStringList * ts = new TStringList ;
                try {
                    //s_cs->Enter();
                    Sleep(30);
                    sTempFile = m_AppPath + m_ScreenPath + "-" + sReason + ".txt" ;
                    if ( FileExists(  sTempFile ) )
                        ts->LoadFromFile( sTempFile ) ;
                    else
                        throw ;
                }
                __finally {
                    //s_cs->Leave();
                }
                sScreenText = ts->Text ;
                delete ts ;
                }
                break ;
        }
        m_logger->WriteLog( "Dump Screen Data %s\n%s\n" , sReason.c_str() , sScreenText.c_str() );
        DevproDebugString(AnsiString().sprintf( "Dump Screen Data %s\n%s\n" , sReason.c_str() , sScreenText.c_str() ).c_str());
        return sScreenText ;
    }

    __fastcall bool IsTerminalScreen( AnsiString &sScreen )
    {

    }

    __fastcall bool GetData( AnsiString &sData , AnsiString sScreenSignature , TStrings * tsAttrib , int nTimeOut = 30000 , AnsiString ScreenPath = "" , bool bIsOptional = false )
    {
        bool bOk = false ;
        TReplaceFlags rf ;
        rf << rfReplaceAll ;
        AnsiString sScreenText ;
        DWORD tick = GetTickCount() ;
        DWORD prevtick = GetTickCount();

        AnsiString IdleMsg ;
        AnsiString BusyMsg ;

        if ( tsAttrib ) {
            AnsiString IdleMsg = tsAttrib->Values["IdleMessage"] ;
            AnsiString BusyMsg = tsAttrib->Values["BusyMessage"] ;
        }
        if  ( ! WaitForReady( tick , nTimeOut ) ) {
            m_logger->WriteLog( "Get Data Not Ready !" );
            DevproDebugString( "Get Data Not Ready !" );
            return bOk ;
        }
        bool bSignaturePass , bBusyPass , bIdlePass ;

        bSignaturePass = sScreenSignature.Length() == 0 ;
        bBusyPass = BusyMsg.Length() == 0 ;
        bIdlePass = IdleMsg.Length() == 0 ;
        //Wait( m_WaitBeforeGetPS ) ;
        m_logger->WriteLog( "Check %s ScreenSignature \"%s\" " ,
                        bIsOptional ? "Optional" : "Required" ,
                        sScreenSignature.c_str() ) ;
        DevproDebugString(AnsiString().sprintf( "Check %s ScreenSignature \"%s\" " ,
                        bIsOptional ? "Optional" : "Required" ,
                        sScreenSignature.c_str() ).c_str());
        do  {
            sScreenText = GetScreenText( "GetData" ) ;
            AnsiString sCurrentScreenPackData = StringReplace( sScreenText , "" , "" , rf ) ;
            sCurrentScreenPackData = StringReplace( sCurrentScreenPackData , "¡@" , "" , rf ) ; // BIG5 BLANK

            if ( !bSignaturePass ) {
                bSignaturePass = ( sCurrentScreenPackData.Pos( sScreenSignature ) != 0 ) ;
                if ( !bSignaturePass ) {
                    m_logger->WriteLog( "ScreenSignature \"%s\" not found in CurrentScreenPackData\n\n" ,
                        sScreenSignature.c_str() ,
                        sCurrentScreenPackData.c_str() ) ;
                    DevproDebugString(AnsiString().sprintf( "ScreenSignature \"%s\" not found in CurrentScreenPackData\n\n" ,
                        sScreenSignature.c_str() ,
                        sCurrentScreenPackData.c_str() ).c_str());

                    if ( bIsOptional )
                        nTimeOut = 0 ; // No Wait for screen ...
                    else
                        Wait( m_WaitCommon );
                }
                else
                    m_logger->WriteLog( "ScreenSignature pass !") ;
                    DevproDebugString( "ScreenSignature pass !" );
            } else if ( !bBusyPass ) {
                bBusyPass = ( sCurrentScreenPackData.Pos( BusyMsg ) == 0 ) ;
                if ( ! bBusyPass ) {
                    m_logger->WriteLog( "Busy Message \"%s\" found in CurrentScreenPackData\n\n" ,
                        BusyMsg.c_str() ,
                        sCurrentScreenPackData.c_str() ) ;
                    DevproDebugString(AnsiString().sprintf( "Busy Message \"%s\" found in CurrentScreenPackData\n\n" ,
                        BusyMsg.c_str() ,
                        sCurrentScreenPackData.c_str() ).c_str());
                    Wait( m_WaitCommon );
                }
                else
                    m_logger->WriteLog( "Busy pass !") ;
                    DevproDebugString( "Busy pass !" );
            }
            else if ( !bIdlePass ) {
                bIdlePass = ( sCurrentScreenPackData.Pos( IdleMsg ) != 0 ) ;
                if ( ! bIdlePass ) {
                    m_logger->WriteLog( "Idle Message \"%s\" not found in CurrentScreenPackData\n\n" ,
                        IdleMsg.c_str() ,
                        sCurrentScreenPackData.c_str() ) ;
                    DevproDebugString(AnsiString().sprintf( "Idle Message \"%s\" not found in CurrentScreenPackData\n\n" ,
                        IdleMsg.c_str() ,
                        sCurrentScreenPackData.c_str() ).c_str());
                    Wait( m_WaitCommon );
                }
                else
                    m_logger->WriteLog( "Idle pass !") ;
                    DevproDebugString( "Idle pass !" );
            }
            bOk = bSignaturePass && bBusyPass && bIdlePass ;
            //}
        } while ( (GetTickCount()-tick) <= nTimeOut && !bOk ) ;

        m_logger->WriteLog( "TickLog \t%s\t%d" , "GetData" , GetTickCount()-prevtick ) ;
        DevproDebugString(AnsiString().sprintf( "TickLog \t%s\t%d" , "GetData" , GetTickCount()-prevtick ).c_str()) ;

        sData = sScreenText ;
        return bOk ;
    }

    __fastcall int InputField(AnsiString KeyString , AnsiString Pos , bool erasefield )
    {
        bool bOk = true ;
        TStrings * tsKeyList = new TStringList ;
        AnsiString sString ;
        int nErrCount = 0;

        DWORD prevtick = GetTickCount();
        AnsiString sLog = "Input :" + KeyString + " at position : " + Pos;
        AnsiString sMacro ;
        m_logger->WriteLog(sLog.c_str());
        DevproDebugString(sLog.c_str());

        if ( Pos.ToIntDef(0) > 0) {
            SetCursor(Pos) ;
        }

        if ( WaitForReady( GetTickCount() ) != esIdle ) return -1 ;
        if (erasefield) {
            bOk = ExecMacroKey( "erase field" ) ;
            WaitForReady( GetTickCount() ) ;
        }
        tsKeyList->Text =  AnsiReplaceStr(KeyString,";","\n") ;
        for ( int i = 0 ; i < tsKeyList->Count && bOk ; i++  ) {
            AnsiString sKey = tsKeyList->Strings[i].Trim() ;
            if ( sKey.Length() ){
                bOk = ExecMacroKey( sKey ) ;
                if ( GetOIA() == esInputError )
                    nErrCount ++ ;
            }
        }
        delete tsKeyList ;
        //*WaitForReady( GetTickCount() ) ;
        m_logger->WriteLog( "TickLog \t%s\t%d" , "InputField" , GetTickCount()-prevtick ) ;
        DevproDebugString(AnsiString().sprintf( "TickLog \t%s\t%d" , "InputField" , GetTickCount()-prevtick ).c_str()) ;
        return nErrCount ;
    }

    __fastcall virtual bool SubmitKey( AnsiString SubmitKey , bool bWait = false , AnsiString Reason = "" )
    {
        bool bOk = true ;
        TStrings * tsKeyList = new TStringList ;
        AnsiString sString ;
        int nErrCount = 0 ;

        AnsiString sLog = "Key : " + SubmitKey ;
        AnsiString sMacro ;
        m_logger->WriteLog(sLog.c_str());
        DevproDebugString(sLog.c_str());

        m_Status = WaitForReady( GetTickCount() ) ;

        tsKeyList->Text =  AnsiReplaceStr(SubmitKey,";","\n") ;
        AnsiString sPrevScreen = GetScreenText( "Before SubmitKey " + Reason ) ;
        int nPrevScreenLength = sPrevScreen.Trim().Length() ;
        AnsiString sCurrScreen ;
        DWORD tick = GetTickCount();
        DWORD prevtick = GetTickCount();
        bool bContinue = true ;
        int nMaxRetry = m_MaxSubmitRetry ;
        for ( int c = 0 ; (c < nMaxRetry) && bContinue ; c ++ ) {
            // Send Key
            if ( c )
                m_logger->WriteLog( "Error : submit keys %s %d time(s)" , SubmitKey.c_str() , c );
            DevproDebugString(AnsiString().sprintf( "Error : submit keys %s %d time(s)" , SubmitKey.c_str() , c ).c_str());
            for ( int i = 0 ; i < tsKeyList->Count && bOk ; i++  ) {
                AnsiString sKey = tsKeyList->Strings[i].Trim() ;
                if ( sKey.Length() ) {
                    bOk = ExecMacroKey( sKey ) ;
                    if ( !bOk )
                        nErrCount ++ ;
                }
            }
            do {
                Wait( m_WaitCommon ) ;
                if ( GetOIA() == esInputError ) {
                    //bOK = false ;
                    bContinue = false ;
                }
                else {
                    sCurrScreen = GetScreenText( "After SubmitKey " + Reason ) ;
                    bContinue = ( sPrevScreen == sCurrScreen ) && ( nPrevScreenLength != 0) ;
                }
            }
            while ( bContinue && (GetTickCount() - tick) < 1500 ) ;
        }
        bOk = (m_Status == esIdle) ;
        m_Status = WaitForReady( GetTickCount() ) ;
        delete tsKeyList ;
        m_logger->WriteLog( "TickLog \t%s\t%d" , "SubmitKey" , GetTickCount()-prevtick ) ;
        DevproDebugString(AnsiString().sprintf( "TickLog \t%s\t%d" , "SubmitKey" , GetTickCount()-prevtick ).c_str()) ;
        return bOk ;
    }

    __fastcall bool SetCursor( AnsiString Pos )
    {
        AnsiString sLog = "SetCursor:" + Pos;
        m_logger->WriteLog(sLog.c_str());
        DevproDebugString(sLog.c_str());
        bool bOk = true ;
        if ( m_TraceMode != 2 )
            bOk = doSetCursor( Pos ) ;
        return bOk ;
    }
} ;

/*
class CDDEEmulator : public CBaseEmulator {
protected:
    TDdeClientConv *dccPCom;
    TDdeClientItem *dciPCom;

private :
    AnsiString l_BuffData;
    AnsiString l_Service;   // IBM327032
    AnsiString l_Topic;     // SessionA

public:
    CDDEEmulator()
    {
        AnsiString sTmp;
        dccPCom = new TDdeClientConv( Application );
        dciPCom = new TDdeClientItem( Application );
        l_Service = "IBM327032";
        m_ScreenTextFormat = 1 ;
    }

    virtual ~CDDEEmulator()
    {
        delete dccPCom;
        delete dciPCom;
    }

    virtual EmulatorStatus doGetOIA( )
    {
        AnsiString sString ;
        Sleep(5);
        sString = dccPCom->RequestData("OIA");
        m_logger->WriteLog( "Get Session Status : %s " , sString.c_str() );
        if ( sString.Length() == 0 ) {
            CloseLink();
            OpenLink(l_Topic);
            sString = dccPCom->RequestData("OIA");
            m_logger->WriteLog( "Reopen Session , Get Session Status : %s " , sString.c_str() );
        }

        if ( sString.Pos( "X TWAIT" ) )
            m_Status = esBusy ;
        else if ( sString.Pos( "X SYSTEM" ) )
            m_Status = esBusy ;
        else if ( sString.Pos( "X II" ) )
            m_Status = esInputError ;
        else if ( sString.Pos( "X" ) )
            m_Status = esInputError ;
        else  if (sString.Pos("LU-LU                    MW"))
            m_Status = esIdle ;
        else  if (sString.Pos("LU-LU"))
            m_Status = esIdle ;
        else
            m_Status = esUnexpected ;
        //bOk = (m_Status != esUnexpected) ;
        m_logger->WriteLog( "Get Session Status : %s(%d) " , sString.c_str() , m_Status );
        return m_Status ;
    }  */

    /*bool doGetOIA( AnsiString &sString )
    {
        bool bOk = false ;
        Sleep(5);
        doGetOIA( sString ) ;
        m_logger->WriteLog( "Get Session Status : %s " , sString.c_str() );
        if ( sString.Length() == 0 ) {
            doCloseLink();
            doOpenLink(m_SessionID);
            doGetOIA( sString ) ;
            m_logger->WriteLog( "Reopen Session , Get Session Status : %s " , sString.c_str() );
        }
        bOk = (sString.Length() >0) ;
        return bOk ;
    }  */

    /*bool doWaitForReady( int prevtick , int nTimeOut = -1 ) {
        bool bOk = false ;
        AnsiString StatusString ;
        m_logger->WriteLog( "Wait for ready .. " ) ;
        //m_LastStatus = "" ;
        if ( nTimeOut < 0 ) nTimeOut = m_WaitForProcessing ;
        while ( (GetTickCount() - prevtick ) <= nTimeOut && !bOk ) {
            StatusString = "X TWAIT" ;
            if ( doGetOIA( StatusString ) && StatusString.Trim().Length() ) {
                switch( m_Status ) {
                    case esBusy :
                        nTimeOut = m_WaitForProcessing ;
                        break ;
                    case esInputError :
                        ExecMacroKey("reset") ;
                        break ;
                } */
                /* if ( StatusString.Pos("X TWAIT") ) {
                    nTimeOut = m_WaitForProcessing ;
                }
                else if ( StatusString.Pos("X SYSTEM") ) {
                    nTimeOut = m_WaitForProcessing ;
                }
                else if ( StatusString.Pos("X")  ) {
                    m_LastStatus = StatusString ;
                    ExecMacroKey("reset") ;
                }  */  /*|| StatusString.Pos("X II")*/
            /*}
            else  {
                // Cannot get data from Emulator ...
                m_logger->WriteLog( "GetOIA Error" );
                break ;
            }  */
            /*bOk = ( StatusString.Pos("LU-LU                    MW")
                    || StatusString.Trim() == "LU-LU" ) ; */
            /*bOk = ( m_Status == esIdle ) ;
            if ( !bOk )
                Wait( m_WaitCommon ) ;
        }
        return bOk ;
    } */
    /*
    bool doOpenLink( AnsiString SessionName ) {
        m_SessionID = SessionName ;
        l_Topic = m_SessionID ;
        dciPCom->DdeConv = dccPCom;
        dccPCom->ConnectMode = ddeManual;
        dccPCom->SetLink( l_Service , l_Topic );
        return dccPCom->OpenLink() ;
    }

    void doCloseLink( ) {
        dccPCom->CloseLink();
    }

    AnsiString doGetSessionList( )
    {
        AnsiString Sessions ;
        if ( OpenLink( "System" ) )
        {
            Sessions = dccPCom->RequestData("Topics");
            CloseLink();
        }
        return Sessions ;
    }

    bool doExecMacroKey( AnsiString sKey ) {
        AnsiString sMacro = "[SENDKEY(" + sKey + ")]";
        bool bOk = dccPCom->ExecuteMacro( sMacro.c_str(), false ) ;
        return bOk ;
    }

    AnsiString doGetScreenText( ) {
        AnsiString sScreenText = dccPCom->RequestData("PS") ;
        return sScreenText ;
    }

    bool doSetCursor(AnsiString Pos)
    {
        return dccPCom->PokeData("SETCURSOR" , Pos.c_str());
    }
} ; */

using namespace HACL ;

class CEHLLEmulator : public CBaseEmulator {
protected:
    EhllapiImpl * m_Ehllapi ;
    TIniFile * m_keymaps ;
private :

public:
    CEHLLEmulator()
    {
        DevproDebugString( "new CEHLLEmulator" ) ;
        TStrings *tsParams = new TStringList ;
        GetParamsN( tsParams ) ;
        //s_cs->Enter();
        //EhllapiImpl::Initialize( tsParams->Values["DLLPATH"].c_str() ) ;
        //s_cs->Leave();
        m_Ehllapi = new EhllapiImpl( tsParams->Values["DLLPATH"].c_str() ) ;
        m_keymaps = new TIniFile( "./keymap.ini" ) ;
        m_ScreenTextFormat = 0 ;
        DevproDebugString( "new CEHLLEmulator exit" ) ;
        delete tsParams ;
    }

    virtual ~CEHLLEmulator()
    {
        delete m_Ehllapi ;
        delete m_keymaps ;
    }

    __fastcall virtual EmulatorStatus doGetOIA( )
    {
        bool bOk = false ;

        int rc = m_Ehllapi->WaitForInput() ;
        switch ( rc ) {
            case SUCCESS : m_Status = esIdle ;
                break ;
            case LOCKED :  m_Status = esInputError ;
                m_logger->WriteLog( " OIA Status LOCKED : %08X" , rc ) ;
                break ;
            case BUSY :  m_Status = esBusy ;
                m_logger->WriteLog( " OIA Status BUSY : %08X" , rc ) ;
                break ;
            default :
                m_Status = esUnexpected ;
                m_logger->WriteLog( "Exception : Unexcepted OIA Status %08X" , rc ) ;
                break ;
        }
        return m_Status ;
        
        /*rc = m_Ehllapi->CopyOIA() ;
        unsigned char b1 , b2 ;

        char textdataptr[400]={0};
        BinToHex(m_Ehllapi->m_szOIA, textdataptr, 206);
        ECommLog::sWriteLog( "./TN5250.log" , "Dump OIA : %s" , textdataptr ) ;

        b1 = m_Ehllapi->m_szOIA[65] ;
        b2 = m_Ehllapi->m_szOIA[66] ;
        WORD w = b1 << 8 | b2 ;
        switch ( w ) {
            case 0x1010 : m_Status = esIdle ; break ;
            case 0xC620 : m_Status = esInputError ; break ;
            case 0xC610 : m_Status = esBusy ; break ;
            default : {
                m_Status = esUnexpected ;
                //sString = m_Ehllapi->m_szOIA ;
                char textdataptr[400]={0};
                BinToHex(m_Ehllapi->m_szOIA, textdataptr, 206);
                ECommLog::sWriteLog( "./TN5250.log" , "Unknown OIA : %0X" , w ) ;
                break ;
            }
        }
        return m_Status ;
        */
    }

    __fastcall bool doOpenLink( AnsiString SessionName ) {
        m_SessionID = SessionName ;
        bool bOk = m_Ehllapi->Connect( m_SessionID.c_str() ) ;
        return bOk ;
    }

    __fastcall void doCloseLink( ) {
        m_Ehllapi->Disconnect();
    }

    __fastcall AnsiString doGetSessionList( )
    {
        AnsiString Sessions ;
        m_Ehllapi->QuerySessions( ) ;
        Sessions = m_Ehllapi->m_szStr ;
        return Sessions ;
    }

    __fastcall bool doSendKey( AnsiString sKey ) {
        if ( sKey.Length() ) {
            bool bNoReset = false ;
            if ( sKey == "\"g\"" ) {
                doSetParams( "NORESET" ) ;
                m_Ehllapi->SendKeys( "g" ) ;
                doSetParams( "AUTORESET" ) ;
            }
            else {
                if ( sKey[1] == '"' ) {
                    sKey = AnsiReplaceStr( sKey , "@" , "@@" ) ;
                    sKey = AnsiReplaceStr( sKey , "\"" , "" ) ;
                    doSetParams( "NORESET" ) ;
                    bNoReset = true ;
                }
                m_Ehllapi->SendKeys( sKey.c_str() ) ;
            }
        }
        return true ;
    }

    __fastcall bool doSetString( AnsiString sKey , int col , int row ) {
        //sKey = AnsiReplaceStr( sKey , "\"" , "" );
        m_Ehllapi->SetString( sKey.c_str() , col , row ) ;
        return true ;
    }

    __fastcall bool doExecMacroKey( AnsiString sKey ) {
        AnsiString sMacro ; // = "[SENDKEY(" + sKey + ")]";
        if ( sKey.Length() ) {
            sMacro = m_keymaps->ReadString( "Default" , sKey , "" ) ;
            if ( sMacro.Length() )
                doSendKey( sMacro ) ;
            else {
                doSendKey( sKey ) ;
            }
        }
        return true ;
    }

    __fastcall AnsiString doGetScreenText( ) {
        int rc = m_Ehllapi->CopyPS( ) ;
        AnsiString sScreenText ;
        if ( rc == 0 ) { // successed
            sScreenText = m_Ehllapi->m_szStr ;
            char sTemp[200] ;
            for ( int i = 0 ; i < 24 ; i++ ) {
                strncpy( sTemp , m_Ehllapi->m_szStr + (i*80), 80 );
                sTemp[80] = 0 ;
                if ( i )
                    sScreenText = sScreenText + "\n" + sTemp ;
                else
                    sScreenText = sTemp ;
            }
        }
        else {
            m_logger->WriteLog( "Exception : m_Ehllapi->CopyPS( ) ret=%d" , rc ) ;
        }
        return sScreenText ;
    }

    __fastcall bool doSetCursor(AnsiString Pos)
    {
        m_Ehllapi->SetCursorPos( Pos.ToIntDef(0)+1 ) ;
        return true ;
    }

    __fastcall bool doSetParams(AnsiString Params)
    {
        m_Ehllapi->EhllapiImpl::SetParams(Params.c_str()) ;
        return true ;
    }

    __fastcall bool doWaitForReady( int prevtick , int nTimeOut = -1 )
    {
        return ( m_Ehllapi->WaitForInput() == SUCCESS );
    }

    __fastcall virtual int doGetCursorLocation( )
    {
        return m_Ehllapi->GetCursorPos();
    }
    /*virtual bool SubmitKey(AnsiString SubmitKey , bool bWait = false )
    {
        bool bOk = true ;
        TStrings * tsKeyList = new TStringList ;
        AnsiString sLog = "Key : " + SubmitKey ;
        AnsiString sMacro ;
        m_logger->WriteLog(sLog.c_str());
        AnsiString sString ;
        int nErrCount = 0 ;

        bOk = WaitForReady( GetTickCount() ) ;
        tsKeyList->Text =  AnsiReplaceStr(SubmitKey,";","\n") ;
        AnsiString sPrevScreen = GetScreenText( "Before SubmitKey" ) ;
        AnsiString sCurrScreen ;
        DWORD tick = GetTickCount();
        bool bContinue = true ;
        for ( int c = 0 ; (c < 2) && bContinue ; c ++ ) {
            // Send Key
            if ( c )
                m_logger->WriteLog( "Error : submit keys %s %d time(s)" , SubmitKey.c_str() , c );
            for ( int i = 0 ; i < tsKeyList->Count && bOk ; i++  ) {
                AnsiString sKey = tsKeyList->Strings[i].Trim() ;
                if ( sKey.Length() ) {
                    bOk = ExecMacroKey( sKey ) ;
                }
            }
            do {
                //Wait( m_WaitCommon ) ;
                bOk = WaitForReady( GetTickCount() ) ;
                sCurrScreen = GetScreenText( "After SubmitKey" ) ;
                bContinue = ( sPrevScreen == sCurrScreen ) ;
            }
            while ( bContinue && (GetTickCount() - tick) < 1500 ) ;
        }
        bOk = WaitForReady( GetTickCount() ) ;
        delete tsKeyList ;
        return bOk ;
    } */
} ;

#endif
