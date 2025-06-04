//---------------------------------------------------------------------------

#ifndef dmUMSH
#define dmUMSH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "EMiddlewareController.h"

#include <ExtCtrls.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
//---------------------------------------------------------------------------
class TdmgwUMS : public TDataModule
{
__published:	// IDE-managed Components
    EMiddlewareController *EMC;
    TTimer *tmReLoadParams;
    TADOConnection *ADOConn;
    TADOQuery *ADOQuery;
    void __fastcall DataModuleCreate(TObject *Sender);
    void __fastcall EMCOpen(TObject *Sender, bool &bOk);
    void __fastcall EMCClose(TObject *Sender, bool &bOk);
    void __fastcall EMCGetMessage(TObject *Sender, void *msg,
          bool &Handled);
    void __fastcall EMCEnumMsgInfo(TObject *Sender, EMsgInfo *msginfo,
          bool &Handled);
    void __fastcall EMCBeginEnumMsgInfo(TObject *Sender, bool &bOk);
    void __fastcall EMCEndEnumMsgInfo(TObject *Sender, bool &bOk);
private:	// User declarations
    TList * m_ThreadList ;
    AnsiString m_ClassName ;
    LogFile * m_exportlog ;
    bool m_CacheDBEnabled ;
    bool m_LogDBEnabled ;

public:		// User declarations
    __fastcall TdmgwUMS(TComponent* Owner);
    bool CheckDataExist( AnsiString sSerialNo , AnsiString Item , bool Booking ) ;
    bool SetData( AnsiString sSerialNo , AnsiString Item , PElement * pe ) ;
    bool GetData( AnsiString sSerialNo , AnsiString Item , PElement * pe ) ;
    bool CardlinkLog( AnsiString REQNO , AnsiString REQTIME , AnsiString RECEIVETIME , AnsiString MsgId , AnsiString UserID , AnsiString AgentID , AnsiString ServiceName , AnsiString ParamStr , AnsiString ResultCode , AnsiString desc , AnsiString LogFle );
};



//---------------------------------------------------------------------------
extern PACKAGE TdmgwUMS *dmgwUMS;
//---------------------------------------------------------------------------
#endif
