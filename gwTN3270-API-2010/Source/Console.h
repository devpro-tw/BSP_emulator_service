//---------------------------------------------------------------------------

#ifndef ConsoleH
#define ConsoleH
//---------------------------------------------------------------------------
#include "EMiddlewareController.h"
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <ActnList.hpp>
#include <ActnMan.hpp>
#include <ImgList.hpp>

#include "winlib\Application.hpp"
#include "APUtil.h"
//---------------------------------------------------------------------------

using namespace engsound ;

#define UM_UPDATESTATUS WM_USER + 1
#define UM_APPENDEVENTLOG WM_USER + 2
class TfrmResourceConsole ;

class TListConsoleItem : public TConsoleItem
{
friend TfrmResourceConsole ;
private :
    TListItem * m_listitem ;
    HANDLE m_hwnd ;
public :
    AnsiString Status ;
    AnsiString Description ;
    EResItem * ResItem ;
    TListConsoleItem( AnsiString itemname , TListItem * item , HANDLE hWnd , EResItem * resitem ) : TConsoleItem(itemname) {
        m_listitem = item ;
        m_hwnd = hWnd ;
        ResItem = resitem ;
        item->SubItems->Clear() ;
        item->SubItems->Add( "" ) ; // Status
        item->SubItems->Add( "" ) ; // Description
    } ;

    virtual void Update() {
        SendMessage( m_hwnd , UM_UPDATESTATUS , (unsigned long)this , (unsigned long)this ) ;
    } ;

    virtual void ShowStatus( const char * status , bool AppendToEventLog = false ) {
        Status = status ;
        Update();
        if ( AppendToEventLog )
            AppendEventLog( status );
    } ;

    virtual void ShowDescription( const char * desc ) {
        Description = desc ;
        Update();
    } ;

    virtual void AppendEventLog( const char * evtmsg ) {
        SendMessage( m_hwnd , UM_APPENDEVENTLOG , (unsigned long)evtmsg , (unsigned long)evtmsg ) ;
    } ;
} ;

class TfrmResourceConsole : public TForm
{
__published:	// IDE-managed Components
        TListView *lvResourceList;
        TMemo *mlMessage;
        TStatusBar *StatusBar1;
        TSplitter *Splitter1;
        TMainMenu *MainMenu1;
        TMenuItem *System1;
        TMenuItem *Shutdown1;
        TControlBar *ControlBar1;
        TImageList *ImageList1;
        TActionManager *ActionManager1;
   TMenuItem *mnuRunning;
         TMenuItem *N1;
    TMenuItem *Message1;
    TMenuItem *Enum1;
    TBitBtn *btnRestart;
    TMenuItem *N2;
    TMenuItem *N3;
    TMenuItem *N4;
         void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
   void __fastcall mnuRunningClick(TObject *Sender);
    void __fastcall Enum1Click(TObject *Sender);
    void __fastcall btnRestartClick(TObject *Sender);
    void __fastcall Shutdown1Click(TObject *Sender);
    void __fastcall Test11Click(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
        LogFile * m_logfile ;
        EMiddlewareController * m_mwctrl ;
protected:
        bool m_suspended ;
public:		// User declarations
        __fastcall TfrmResourceConsole(TComponent* Owner ) : TForm(Owner) {
            m_logfile = new LogFile( "con" );
            m_suspended = false ;
        } ;

        void MonitorEntity( EMiddlewareController * mwctrl )  {
            m_mwctrl = mwctrl ;
//            Caption = m_mwctrl->ApplicationName ;
            Show();
            OutputEventLog("Entity Opening...");
            try {
                if ( m_mwctrl ) m_mwctrl->Open();
                Caption = Caption + " - " + m_mwctrl->EntityName + GetVersionStr() ;
                OutputEventLog("Entity Open finish!");
            }
            catch ( Exception &e ) {
                OutputEventLog( AnsiString(" Entity Open fail! ") + e.Message );
            }               
        } ;

        TListConsoleItem * CreateConsoleItem( const char * InstanceName = "" , const char * Status = "" ) {
            TListItem * it = lvResourceList->Items->Add();
            TListConsoleItem * item = new TListConsoleItem( InstanceName , it , this->Handle , NULL ) ;
            it->Caption = InstanceName ;
            it->SubItems->Add( AnsiString(Status) ) ;
            it->SubItems->Add( "" ) ; // Description
            it->Data = item ;
            return item ;
        } ;

        void DeleteConsoleItem( TListItem * item ) {
           int idx = lvResourceList->Items->IndexOf(item) ;
           if ( idx >=0 ) {
               TListConsoleItem * item = (TListConsoleItem *)(lvResourceList->Items->Item[idx]->Data) ;
               delete item ;
               lvResourceList->Items->Delete(idx) ;
           }
        } ;

        /*EResItem * AppendResource( const char * classname , const char * resource_instance_name , void * resourceptr , ETaskWorkingThread * HandlerThread ) {
            EResItem * resitem = NULL ;
            if ( m_mwctrl ) {
                resitem = m_mwctrl->AppendResource( classname , resource_instance_name , resourceptr , HandlerThread , NULL ) ;
                TListConsoleItem * it = CreateItem( resitem->InstanceName.c_str() , "Init..." , resitem );
                it->Data = resitem ;
                resitem->AttachPtr = it ;
            }
            return resitem ;
        }  */

        void OutputEventLog( AnsiString EventMsg , bool WriteToSystemEvnetLog = false ) {
            m_logfile->WriteLog( EventMsg.c_str() ) ;
            //OutputDebugString( EventMsg.c_str() ) ;
            if ( mlMessage->Lines->Count > 1000 )
                mlMessage->Lines->Clear();
            mlMessage->Lines->Add( Now().FormatString("yyyy/mm/dd hh:nn:ss") + " : " + EventMsg ) ;
        }

protected:
    BEGIN_MESSAGE_MAP
        VCL_MESSAGE_HANDLER( UM_APPENDEVENTLOG , TMessage , AddEventLog)
        VCL_MESSAGE_HANDLER( UM_UPDATESTATUS , TMessage , UpdateStatus)
    END_MESSAGE_MAP(TForm)
    void AddEventLog(TMessage&);
    void UpdateStatus(TMessage&);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmResourceConsole *frmResourceConsole;
//---------------------------------------------------------------------------
#endif
