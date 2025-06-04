//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EnulatorLibTestMain.h"
#include "EmuServiceUtil.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
CEmuService *m_emu_executor;
bool btemp =false;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
     TStrings * m_Params = new TStringList();
     m_Params->Text = "DLLPATH=C:\\TN5250NF\nSESSIONID=A\nLOGENCODING=0" ;
    m_emu_executor = new CEmuService( m_Params );
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
    m_emu_executor->ExecService( "PNB7100" , "«O³æ¸¹½X=A120268769" , "A" , true , btemp , NULL ) ;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
    if ( CheckBox1->Checked ) Button2Click( NULL ) ;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::CheckBox1Click(TObject *Sender)
{
    Timer1->Enabled = CheckBox1->Checked ;
}
//---------------------------------------------------------------------------
