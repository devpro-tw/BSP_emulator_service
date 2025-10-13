// EMULATORMANAGERIMPL : Implementation of TEmulatorManagerImpl (CoClass: EmulatorManager, Interface: IEmulatorManager)

#include <vcl.h>

#pragma hdrstop

#include "EMULATORMANAGERIMPL.H"
#include "APUtil.h"



/////////////////////////////////////////////////////////////////////////////
// TEmulatorManagerImpl

ExecResult STDMETHODCALLTYPE TEmulatorManagerImpl::Init(BSTR Params)
{
  try
  {

     //Application->MessageBoxA( m_emu_executor==NULL ? "NULL" : "NOT NULL" , "" , MB_OK ) ;
     //if ( m_emu_executor == NULL ) {
         m_Params = new TStringList();
         AnsiString params = Params ;
         DevproDebugString( params.c_str() ) ;
         m_Params->Text = params ;
         m_emu_executor = new CEmuService( m_Params );
         DevproDebugString( "TEmulatorManagerImpl::Init" ) ;

     //}
     m_AppPath = ExtractFilePath(Application->ExeName);
  }
  catch(Exception &e)
  {
    DevproDebugString( e.Message.c_str() ) ;
    return Error(e.Message.c_str(), IID_IEmulatorManager);
  }
  return S_OK;
};

STDMETHODIMP TEmulatorManagerImpl::Uninit()
{
  try
  {

  }
  catch(Exception &e)
  {
    return Error(e.Message.c_str(), IID_IEmulatorManager);
  }
  return S_OK;
};

ExecResult STDMETHODCALLTYPE TEmulatorManagerImpl::ExecService(
  BSTR ServiceName, BSTR ParamsStr, BSTR TerminalID)
{
    AnsiString service = ServiceName;
    AnsiString params = ParamsStr;
    AnsiString resname = TerminalID ;
    bool btemp = false ;
    int rc = 0 ;

    DevproDebugString( ("ExecService:" + service).c_str() ) ;
    if ( service.UpperCase() == "ECHO" ) {
        rc = 0 ;
        m_emu_executor->m_DataSets->ClearRecordset();
        //TADODataSet *ds=new TADODataSet(NULL);
        /*ds->LoadFromFile( m_AppPath + "rs1.adtg" ) ;
        m_emu_executor->m_DataSets->AppendRecordset( "ECHO1" , ds );
        ds->LoadFromFile( m_AppPath + "rs2.adtg" ) ;
        m_emu_executor->m_DataSets->AppendRecordset( "ECHO2" , ds ); */
        //delete ds ;
    }
    else {
        try {
            rc = m_emu_executor->ExecService( service.c_str() , params.c_str() , resname , true , btemp , NULL ) ;
            ///m_emu_executor->ExecService( "InitEnter" , "" , resname , false , btemp , NULL ) ;
        }
        catch( Exception &e ) {
            DevproDebugString( (AnsiString("ExecService Catch:")+ e.Message).c_str() ) ;
        }
        catch( ... ) {
            DevproDebugString( "Unknown Exception" ) ;
        }
    }

    /*rc = m_pcom->ExecService( ServiceName.c_str() , ParamStr.c_str() , m_ResName , true , btemp , NULL ) ;
    m_pcom->m_DataSets->Save( dataset ) ;
    ResultRecordList = m_pcom->m_DataSets->GetRecordsetNameList() ; */
    return (ExecResult) rc ;
}

int STDMETHODCALLTYPE TEmulatorManagerImpl::GetResultCount()
{
    int c = m_emu_executor->m_DataSets->GetRecordsetCount();
    return c ;
}

int STDMETHODCALLTYPE TEmulatorManagerImpl::GetResultRecordsetByIndex(
  int Index, Ador_tlb::Recordset15* Recordset, BSTR* RecordsetName)
{
    AnsiString fname = m_AppPath + GetTempFile( "adtg" ) ; // m_AppPath + "rs1-temp.adtg" ;
    TVariant v = WideString(fname.c_str());
    AnsiString rsname = m_emu_executor->m_DataSets->GetRecordsetNameByIndex(Index);
    TADODataSet *ds = m_emu_executor->m_DataSets->GetRecordsetByIndex(Index);
    DeleteFile( fname ) ;
    ds->SaveToFile(fname);
    Recordset->Open(v, Variant::NoParam(), adOpenUnspecified, adLockUnspecified , 0);
    (*RecordsetName) = WideString(rsname) ;
    DeleteFile(fname);
}


BSTR STDMETHODCALLTYPE TEmulatorManagerImpl::GetLatestScreen()
{
    WideString s ;
    s = "Script Path:" + m_emu_executor->GetLatestErrorPath() + "\nScreen:\n" + m_emu_executor->GetLatestErrorScreen() ;
    return s ;

};


BSTR STDMETHODCALLTYPE TEmulatorManagerImpl::GetTransactionID()
{
    return WideString(m_emu_executor->m_logger->GetFileName()) ;
};



BSTR STDMETHODCALLTYPE TEmulatorManagerImpl::DecryptFubonString(
  BSTR CipherText)
{
    return WideString( "" /*Elaes::DecryptFubonString(CipherText)*/ ) ;
}


