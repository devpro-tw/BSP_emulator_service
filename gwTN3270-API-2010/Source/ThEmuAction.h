//---------------------------------------------------------------------------
#ifndef ThEmuActionH
#define ThEmuActionH
//---------------------------------------------------------------------------
#include "EMiddlewareController.h"
#include <Classes.hpp>
#include <ADODB.hpp>
#include <adoctint.h>
#include <ComCtrls.hpp>
#include <inifiles.hpp>
#include "MsgThread.h"
#include "Console.h"
#include "EmuServiceUtil.h"

// =========================================================================
// Thread 物件
// =========================================================================

class ThEmulatorAction : public ETaskWorkingThread
{
private:
    AnsiString m_Message ;
    bool m_ChangeSessionOwner ;

    void __fastcall UpdateMsg( ) ;
    void UpdateMessage( AnsiString sMsg ,  bool bLog = true ) ;

    TListConsoleItem * m_ConsoleItem ;
    EMiddlewareController * mEMC ;

    //---------------------------------------------------------------------------
    //  各封包相關資訊
    //---------------------------------------------------------------------------
    long m_ResultCode ;
    long m_ReasonCode ;
    AnsiString m_ResultMsg ;

    TStrings * m_StoredProcList ;
    bool m_IsConnect ;

    // 資源名稱
    AnsiString m_ResName ;
    AnsiString m_UserID ;
    AnsiString m_Password ;
    EResItem   * m_ResItem ;
    AnsiString m_ClassName ;
    TIniFile * m_testini ;
    TIniFile * m_ConfigIni ;
    TADODataSet * m_ds;
    CEmuService * m_pcom ;
    TADOConnection * m_ADOConn ;

    // 暫存檔名
    AnsiString m_TempFile ;
    AnsiString m_TempDBFile ;
    TMemoryStream * m_TempStream ;

    // 設定傳回的 PEStringMap
    PEStringMap   m_ResultParams ;

    //---------------------------------------------------------------------------
    //  Message 相關 Function
    //---------------------------------------------------------------------------
    virtual bool OnInit() ;
    virtual bool OnResourceOpen() ;
    virtual bool OnResourceClose() ;
    virtual bool OnExit() ;
    virtual bool OnTimeOut() ;
    bool DoTimeoutService() ;
    bool CheckTimeoutProcess();
    virtual bool OnGetMessage(EMsgInfo &msginfo, bool &Handled);
    virtual bool OnGetRequest(EMsgInfo &msginfo);
    //virtual bool OnGetResponse(EMsgInfo &msginfo);
    virtual bool OnGetInfo(EMsgInfo &msginfo);
    bool RunService( EMsgInfo *msginfo ) ;
    void LoadLoginInfo();
protected:
    DWORD m_TimeoutInterval ;
    DWORD m_PrevTimeoutTick ;
public:
    __fastcall ThEmulatorAction( EMiddlewareController *EMC , TListConsoleItem * ConsoleItem , AnsiString sClassName , AnsiString sResName , AnsiString,AnsiString);
    __fastcall ~ThEmulatorAction();

    bool    m_IsWaiting ;
    int     m_WaitMsgId ;
    PEStringMap m_RespMsg ;
};
//---------------------------------------------------------------------------
#endif

