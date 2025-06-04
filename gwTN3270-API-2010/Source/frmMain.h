//---------------------------------------------------------------------------
#ifndef frmMainH
#define frmMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <inifiles.hpp>
#include "APUtil.h"
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include "EMiddlewareController.h"
//---------------------------------------------------------------------------
class TfrmDB : public TForm
{
__published:	// IDE-managed Components
    TBitBtn *BitBtn1;
    TListView *lvShowState;
    EMiddlewareController *EMC;
    TBitBtn *bbQueryTest;
    TBitBtn *bbExecuteTest;
    TBitBtn *bbStoreTest;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall bbQueryTestClick(TObject *Sender);
    void __fastcall bbExecuteTestClick(TObject *Sender);
    void __fastcall bbStoreTestClick(TObject *Sender);
private:	// User declarations
    TList * m_ThreadList ;
    AnsiString m_ClassName ;
public:		// User declarations
    __fastcall TfrmDB(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmDB *frmDB;
//---------------------------------------------------------------------------
#endif
 
