//---------------------------------------------------------------------------

#ifndef EStepDebugH
#define EStepDebugH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Mask.hpp>
//---------------------------------------------------------------------------
class TdlgStepDebug : public TForm
{
__published:	// IDE-managed Components
    TCheckBox *AutoAccept;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
    TMaskEdit *edText;
    TLabel *Label1;
    TTimer *Timer1;
    TBitBtn *BitBtn3;
    void __fastcall AutoAcceptClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TdlgStepDebug(TComponent* Owner);
    int Execute( AnsiString  ProgName , AnsiString  PanelName , AnsiString &Text , AnsiString sPrompt ) {
         edText->Text = Text ;
         Caption = ProgName + "-" + PanelName + " " + sPrompt ;
         int rc = ShowModal() ;
         Text = edText->Text ;
         return rc ;
    } ;
};
//---------------------------------------------------------------------------
extern PACKAGE TdlgStepDebug *dlgStepDebug;
//---------------------------------------------------------------------------
#endif
