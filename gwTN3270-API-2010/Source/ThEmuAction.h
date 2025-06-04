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
// Thread ����
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
    //  �U�ʥ]������T
    //---------------------------------------------------------------------------
    long m_ResultCode ;
    long m_ReasonCode ;
    AnsiString m_ResultMsg ;

    TStrings * m_StoredProcList ;
    bool m_IsConnect ;

    // �귽�W��
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

    // �Ȧs�ɦW
    AnsiString m_TempFile ;
    AnsiString m_TempDBFile ;
    TMemoryStream * m_TempStream ;

    // �]�w�Ǧ^�� PEStringMap
    PEStringMap   m_ResultParams ;

    //---------------------------------------------------------------------------
    //  Message ���� Function
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

