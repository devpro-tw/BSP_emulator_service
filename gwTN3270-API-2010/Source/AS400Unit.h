//---------------------------------------------------------------------------

#ifndef AS400UnitH
#define AS400UnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DdeMan.hpp>
#include <ExtCtrls.hpp>


#include <StrUtils.hpp>
#include <IniFiles.hpp>
#include "APUtil.h"
#include <SysUtils.hpp>
#include <CheckLst.hpp>
#include <Grids.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
#include <DBGrids.hpp>
#include <ComCtrls.hpp>
#include "EMiddlewareController.h"
#include <Buttons.hpp>
#include <OleCtrls.hpp>
#include "CSPIN.h"
//---------------------------------------------------------------------------
class TfrmAS400 : public TForm
{
__published:	// IDE-managed Components
    TDataSource *DataSource1;
    EMiddlewareController *esmc;
    TADOConnection *ADOConnection1;
    TPageControl *PageControl1;
    TTabSheet *TabSheet1;
    TTabSheet *TabSheet2;
    TTabSheet *TabSheet3;
    TTabSheet *TabSheet4;
    TGroupBox *GroupBox1;
    TLabel *Label1;
    TButton *btnRefresh;
    TListBox *lbSession;
    TLabeledEdit *lbleTopic;
    TGroupBox *GroupBox2;
    TLabel *Label4;
    TLabel *Label5;
    TLabeledEdit *lbleParms;
    TEdit *edScript;
    TButton *btnRunNew;
    TEdit *edTarget;
    TButton *Button1;
    TCheckBox *cbStepDebug;
    TDBGrid *DBGrid1;
    TGroupBox *GroupBox3;
    TLabel *Label2;
    TEdit *edEntity;
    TButton *btnOpen;
    TPanel *Panel1;
    TComboBox *ComboBox1;
    TCheckBox *cbResponseFromFile;
    TLabel *lbResult;
    TEdit *edKey;
    TEdit *edStatus;
    TBitBtn *BitBtn1;
    TLabel *Label6;
    TButton *Button2;
    TLabel *Label7;
    TTabSheet *TabSheet5;
    TMemo *mlScreen;
    TButton *btnRun;
    TDataSource *DataSource2;
    TDBGrid *DBGrid2;
    TCSpinEdit *CSpinEdit1;
    void __fastcall lbSessionClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall btnRefreshClick(TObject *Sender);
    void __fastcall btnRunNewClick(TObject *Sender);
    void __fastcall ComboBox1Change(TObject *Sender);
    void __fastcall esmcGetRequest(TObject *Sender, EMsgInfo *msginfo,
          bool &Handled);
    void __fastcall btnOpenClick(TObject *Sender);
    void __fastcall edEntityChange(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall esmcGetResponse(TObject *Sender, EMsgInfo *msginfo,
          bool &Handled);
    void __fastcall lbleTopicChange(TObject *Sender);
    void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall esmcOpen(TObject *Sender, bool &bOk);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall btnRunClick(TObject *Sender);
private:	// User declarations
    TIniFile *l_IniFile;
    void Wait(int ms);
    void RefreshSession();
    int GetData();
    int SendKey(AnsiString KeyString, AnsiString Pos);
    int SetCursor(AnsiString Pos);
    bool CheckBusy(int TimeOut) ;
    AnsiString m_TempDBFile ;

public:		// User declarations
    __fastcall TfrmAS400(TComponent* Owner);
    AnsiString l_Service; // IBM327032
    AnsiString l_Topic;    // SessionA
    int ExecScript( AnsiString ScriptName, AnsiString params, int StartStep, int EndStep, TADODataSet *rsRet1, TADODataSet *rsRet2);
    int ExecStep( AnsiString StepString, TADODataSet *rsRet);
    int TfrmAS400::ExecAOQueryService( AnsiString ServiceName , AnsiString sParamsStr , TADODataSet * ds ) ;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmAS400 *frmAS400;
//---------------------------------------------------------------------------
#endif
