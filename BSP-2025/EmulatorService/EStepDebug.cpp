//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EStepDebug.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TdlgStepDebug *dlgStepDebug;
//---------------------------------------------------------------------------
__fastcall TdlgStepDebug::TdlgStepDebug(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TdlgStepDebug::AutoAcceptClick(TObject *Sender)
{
    Timer1->Enabled = AutoAccept->Checked ;
}
//---------------------------------------------------------------------------
