//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Console.h"
#include "Global.h"
#include "IniFiles.hpp"
#include "StrUtils.hpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmResourceConsole *frmResourceConsole;
//---------------------------------------------------------------------------
void __fastcall TfrmResourceConsole::FormCloseQuery(TObject *Sender,
      bool &CanClose)
{
//    int rc = 0 ;
//    CanClose = ( rc = MessageBox( this->Handle , "Close Application ?"  , "Confirm" , MB_YESNOCANCEL	| MB_ICONASTERISK ) == IDYES ) ;
//    if ( CanClose ) {
        //ResourceTerminate();
        m_mwctrl->Close();
//    }
}
//---------------------------------------------------------------------------

void TfrmResourceConsole::AddEventLog(TMessage& msg)
{
    OutputEventLog((char*)msg.LParam) ;
}

void TfrmResourceConsole::UpdateStatus(TMessage& msg)
{
    TListConsoleItem *consoleitem =(TListConsoleItem *) msg.WParam ;
    consoleitem->m_listitem->Caption = consoleitem->ItemName ;
    consoleitem->m_listitem->SubItems->Strings[0] = consoleitem->Status ;      // Status
    consoleitem->m_listitem->SubItems->Strings[1] = consoleitem->Description ; // Description
}

void __fastcall TfrmResourceConsole::mnuRunningClick(TObject *Sender)
{
    if (m_suspended)
      m_mwctrl->Resume();
    else
      m_mwctrl->Suspend();
    m_suspended = !m_suspended ;
    mnuRunning->Caption = m_suspended ? "繼續服務" : "暫停服務" ;
}
//---------------------------------------------------------------------------

void __fastcall TfrmResourceConsole::Enum1Click(TObject *Sender)
{
    m_mwctrl->EnumMsgInfo( " [msgstatus]>0" ) ;
}
//---------------------------------------------------------------------------


void __fastcall TfrmResourceConsole::btnRestartClick(TObject *Sender)
{
    Close();
    g_AutoRestart.CreateProcess();
}
//---------------------------------------------------------------------------

void __fastcall TfrmResourceConsole::Shutdown1Click(TObject *Sender)
{
    Close() ;    
}
//---------------------------------------------------------------------------

void __fastcall TfrmResourceConsole::Test11Click(TObject *Sender)
{
    /*for (int i = 0; i < m_mwctrl->m_ResItemList->GetCount(); i++) {
        EMsgInfo *msginfo = m_mwctrl->CreateMsgInfo( GW_ONGETINFO ,
                                                          mwUnknown,
                                                          "",
                                                          "",
                                                          NULL,
                                                          "",
                                                          ctMessageGateway);

        ::PostThreadMessage((*(m_mwctrl->m_ResItemList))[i]->HandlerPtr->ThreadID, msginfo->m_appmsgtype, 0, (unsigned long)msginfo);
        EResItem *resitem = m_mwctrl->mp_resitemlist->FindResource(msginfo->m_ResourceName.c_str());
    }   */
    TMenuItem * mi = (TMenuItem*) Sender;
    for ( int s = 0 ; s < lvResourceList->Items->Count ; s ++ ) {
        TListItem *li = lvResourceList->Items->Item[s];
        if ( li && li->Selected ) {
            EResItem *resitem = m_mwctrl->m_ResItemList->FindResourceByName( li->Caption.c_str() , false );
            if ( resitem ) {
                EMsgInfo *msginfo = m_mwctrl->CreateMsgInfo( GW_ONGETINFO ,
                                                                  mwUnknown,
                                                                  "",
                                                                  AnsiReplaceStr( mi->Caption , "&" , "" ).c_str() ,
                                                                  NULL,
                                                                  "",
                                                                  ctMessageGateway);

                int rc = ::PostThreadMessage( resitem->HandlerPtr->ThreadID, msginfo->m_appmsgtype, 0, (unsigned long)msginfo);
                if ( rc == 0 )
                    Application->MessageBoxA( "PostThreadMessage" ,  "error" , MB_OK );
            }
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmResourceConsole::FormCreate(TObject *Sender)
{
    TStrings * tsTestList = new TStringList ;
    TIniFile * iniTest = new TIniFile( "./test.ini" ) ;
    iniTest->ReadSections(tsTestList) ;
    for ( int i = 0 ; i < tsTestList->Count ; i ++ ) {
        if ( UpperCase(tsTestList->Strings[i]) != "COMMON" ) {
            TMenuItem * NewItem = new TMenuItem(N4);
            NewItem->Caption = tsTestList->Strings[i] ;
            NewItem->OnClick = Test11Click ;
            N4->Add(NewItem);
        }
    }   
}
//---------------------------------------------------------------------------

