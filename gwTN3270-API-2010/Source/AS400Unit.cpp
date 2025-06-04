//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AS400Unit.h"
#include "PComUnit.h"
#include "CustomService.h"
#include "winbase.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "EMiddlewareController"
#pragma link "CSPIN"
#pragma resource "*.dfm"
TfrmAS400 *frmAS400;
CEmuService *pcom;
int WaitCommon ;
int WaitEnter;
AnsiString l_BuffData;
TADODataSet *rsBuff;
TStrings *tsParams ;
bool m_Terminated = false ;
int g_Ret = 0 ;
bool m_GatewayOpened = false ;
TIniFile * m_AOSQLCfg ;
TADODataSet * m_ds;

//---------------------------------------------------------------------------
__fastcall TfrmAS400::TfrmAS400(TComponent* Owner)
    : TForm(Owner)
{
    m_TempDBFile.sprintf( "temp-%0X.mdb" , this ) ;
    CopyFile( "ScreenLayoutInfo.mdb" , m_TempDBFile.c_str() , false ) ;
    ADOConnection1->ConnectionString = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + m_TempDBFile ;
    m_AOSQLCfg = new TIniFile( "./AOSQL.ini" ) ;
    pcom = new CEmuService( ADOConnection1 );
    m_ds = new TADODataSet(Application) ;
    pcom->RegisterService( "Service1" , new CCustomService1 ) ;
    pcom->RegisterService( "Service2" , new CCustomService2 ) ;
    tsParams = new TStringList ;
    GetParams( tsParams );
    if ( tsParams->Values["EntityName"].Length() ) edEntity->Text = tsParams->Values["EntityName"] ;
    if ( tsParams->Values["EmulatorID"].Length() ) lbleTopic->Text = tsParams->Values["EmulatorID"] ;
    if ( tsParams->Values["Title"].Length() ) Application->Title = tsParams->Values["Title"] ;
    if ( tsParams->Values["Caption"].Length() ) Caption = tsParams->Values["Caption"] ;
    //if ( tsParams->Values["ClassName"].Length() ) edClass->Text = tsParams->Values["ClassName"] ;
    //m_AOSQLCfg->ReadSections( edAOService->Items ) ;
    if ( tsParams->Values["AutoOpen"] == "1" ) btnOpenClick( NULL ) ;
}

void TfrmAS400::RefreshSession()
{
    AnsiString sSessions ;
    TStrings *l_DataList = new TStringList();
    lbSession->Items->Clear();
    sSessions = pcom->GetSessionList(  );
    l_DataList->Text = AnsiReplaceText( sSessions , "\t", "\n");
    if((l_DataList->Count>0) )
    {
        for (int i=0; i<l_DataList->Count; i++)
        {
            lbSession->Items->Add(l_DataList->Strings[i].Trim());
        }
    }
}

int TfrmAS400::ExecStep( AnsiString StepString, TADODataSet *rsRet=NULL)
{
    return 1;
}

//---------------------------------------------------------------------------
void __fastcall TfrmAS400::lbSessionClick(TObject *Sender)
{
    lbleTopic->Text = lbSession->Items->Strings[lbSession->ItemIndex];
}

//---------------------------------------------------------------------------
void __fastcall TfrmAS400::FormCreate(TObject *Sender)
{
    lbSession->Items->Clear();
    //RefreshSession();  
}
//---------------------------------------------------------------------------
void __fastcall TfrmAS400::btnRefreshClick(TObject *Sender)
{
    RefreshSession();
}
//---------------------------------------------------------------------------   
void __fastcall TfrmAS400::btnRunNewClick(TObject *Sender)
{
    //pcom->l_Topic = lbleTopic->Text;
    //if ( m_GatewayOpened )
    //    esmc->Suspend() ;
    DWORD tick = GetTickCount();
    ComboBox1->Enabled = false ;
    btnRunNew->Enabled = false ;
    GroupBox1->Enabled = false ;
    for ( int i = 1 ; i <= CSpinEdit1->Value ; i ++ ) {

        if ( cbResponseFromFile->Checked ) {
            /*TADODataSet * ds = new TADODataSet( Application ) ;
            ds->LoadFromFile( GetTempFile().c_str() );
            pcom->AppendResultRecordset( "RS1" , ds ) ;
            pcom->AppendResultRecordset( "RS2" , ds ) ;
            delete ds ; */
        }
        else {
            g_Ret = pcom->ExecService( edScript->Text, lbleParms->Text , lbleTopic->Text , true , cbStepDebug->Checked , &m_Terminated ) ;
            AnsiString sMsg ;
            sMsg.sprintf( "process %s in %d m-secs" , (g_Ret == 0) ? "Ok" : "Fail" , GetTickCount() - tick ) ;
            lbResult->Caption = sMsg ;
            //g_Ret = 0 ;
        }
    }
    ComboBox1->Items->Assign(pcom->m_DataSets->m_RecordsetList) ;
    GroupBox1->Enabled = true ;
    ComboBox1->Enabled = true ;
    btnRunNew->Enabled = true ;

    //if ( m_GatewayOpened )
    //    esmc->Resume() ;
}
//---------------------------------------------------------------------------


void __fastcall TfrmAS400::ComboBox1Change(TObject *Sender)
{
    DataSource1->DataSet = (TADODataSet *)pcom->m_DataSets->m_RecordsetList->Objects[ ComboBox1->ItemIndex ] ;
}
//---------------------------------------------------------------------------

void __fastcall TfrmAS400::esmcGetRequest(TObject *Sender,
      EMsgInfo *msginfo, bool &Handled )
{
    Handled = true;

    PElement *pe = msginfo->m_Params["ParamStr"];
    std::string ParamStr = pe ? *pe : PElement("");
    PElement bin;
    PEStringMap dataset;
    AnsiString ResultRecordList ;
    AnsiString rsname ;
    //esmc->SetTaskResult((unsigned long)msginfo, msginfo->m_Action + ".Result", 0, msginfo->m_Params, esmc->EntityName);
    //ExecFlowScript( "PNB7110-1" , "2" ) ;
    //int ret = pcom->ExecService( edScript->Text , lbleParms->Text );
    if ( m_GatewayOpened )
        esmc->Suspend();
    AnsiString sFile = GetTempFile( "xml" ) ;
    AnsiString ServiceName = msginfo->m_Action ;
    
    edScript->Text = msginfo->m_Action ;
    lbleParms->Text = ParamStr.c_str() ;
    btnRunNewClick( NULL );
    pcom->m_DataSets->Save( dataset ) ;
    ResultRecordList = pcom->m_DataSets->GetRecordsetNameList() ;

    msginfo->m_Params.set("DataSet", new PElement(dataset));
    msginfo->m_Params.set("RecordList", new PElement(ResultRecordList.c_str()));
    msginfo->m_Params.set("ResultCode", new PElement(g_Ret));
    esmc->SetTaskResult((unsigned long)msginfo, msginfo->m_Action + ".Result", 0, msginfo->m_Params, esmc->EntityName) ;
    pcom->ExecService( "InitEnter" , "" , lbleTopic->Text , true , cbStepDebug->Checked , &m_Terminated ) ;

    //if ( m_GatewayOpened )
        //esmc->Resume();
}
//---------------------------------------------------------------------------


void __fastcall TfrmAS400::btnOpenClick(TObject *Sender)
{
    esmc->ProfileName = "./profile.xml";
    //esmc->DynamicCreateEntity = true ;
    esmc->EntityName = edEntity->Text ;
    //esmc->ClassName = edClass->Text ;
    esmc->Open();
    btnOpen->Enabled = false ;
}
//---------------------------------------------------------------------------

void __fastcall TfrmAS400::edEntityChange(TObject *Sender)
{
    btnOpen->Enabled = edEntity->Text.Length() ;
}
//---------------------------------------------------------------------------

void __fastcall TfrmAS400::Button1Click(TObject *Sender)
{
    PEStringMap params;
    params.set("ParamStr", new PElement(AnsiString(lbleParms->Text).c_str()));
    esmc->SendRequest( mwApplication , edTarget->Text, "" ,
            edScript->Text, &params, "" ) ;
    ComboBox1->Enabled = false ;
}
//---------------------------------------------------------------------------

void __fastcall TfrmAS400::esmcGetResponse(TObject *Sender,
      EMsgInfo *msginfo, bool &Handled)
{
    Handled = true;
    PEStringMap::iterator	it;

    ComboBox1->Items->Clear();
    pcom->m_DataSets->ClearRecordset();
    if ( msginfo->m_Result == 0 ) {
        pcom->m_DataSets->Load(msginfo->m_Params) ;
        PElement *pe = msginfo->m_Params["DataSet"];
    }
    ComboBox1->Items->Assign(pcom->m_DataSets->m_RecordsetList) ;
    ComboBox1->Enabled = true ;
}
//---------------------------------------------------------------------------

void __fastcall TfrmAS400::lbleTopicChange(TObject *Sender)
{
    Application->Title = AnsiString("AS400 TEST ") + lbleTopic->Text ;
    Caption = Application->Title ;
}
//---------------------------------------------------------------------------


void __fastcall TfrmAS400::FormCloseQuery(TObject *Sender, bool &CanClose)
{
    if ( cbStepDebug->Checked )
        m_Terminated = true ;

    //if ( !btnOpen->Enabled )
        //esmc->Suspend() ;

    pcom->ExecService( "Logout" , "" , lbleTopic->Text , true , false , &m_Terminated ) ;
}
//---------------------------------------------------------------------------


void __fastcall TfrmAS400::Button2Click(TObject *Sender)
{
    AnsiString s ;
    pcom->OpenLink(lbleTopic->Text);
    pcom->GetOIA() ;
    pcom->CloseLink();
    edStatus->Text = s ;
}
//---------------------------------------------------------------------------


void __fastcall TfrmAS400::esmcOpen(TObject *Sender, bool &bOk)
{
    //m_GatewayOpened = true ;
}
//---------------------------------------------------------------------------

int TfrmAS400::ExecAOQueryService( AnsiString ServiceName , AnsiString sParamsStr , TADODataSet * ds ) {

}



void __fastcall TfrmAS400::FormClose(TObject *Sender, TCloseAction &Action)
{
    if ( ADOConnection1->Connected )
        ADOConnection1->Close() ;
    DeleteFile( m_TempDBFile ) ;
}
//---------------------------------------------------------------------------

void __fastcall TfrmAS400::btnRunClick(TObject *Sender)
{
    AnsiString sScreen ;
    pcom->OpenLink(lbleTopic->Text);
    pcom->GetScreenText( sScreen ) ;
    pcom->CloseLink();
    mlScreen->Lines->Text = sScreen ;
}
//---------------------------------------------------------------------------

