// EMULATORMANAGERIMPL.H : Declaration of the TEmulatorManagerImpl

#ifndef EmulatorManagerImplH
#define EmulatorManagerImplH

#define ATL_FREE_THREADED

#include "EmulatorService_TLB.H"
#include "EmuServiceUtil.h"
/////////////////////////////////////////////////////////////////////////////
// TEmulatorManagerImpl     Implements IEmulatorManager, default interface of EmulatorManager
// ThreadingModel : Free
// Dual Interface : FALSE
// Event Support  : FALSE
// Default ProgID : EmulatorService.EmulatorManager
// Description    : 
/////////////////////////////////////////////////////////////////////////////
class ATL_NO_VTABLE TEmulatorManagerImpl : 
  public CComObjectRootEx<CComMultiThreadModel>,
  public CComCoClass<TEmulatorManagerImpl, &CLSID_EmulatorManager>,
  public IEmulatorManager
{
private :
  CEmuService * m_emu_executor ;
  TStrings * m_Params ;
  AnsiString m_AppPath ;
public:

  TEmulatorManagerImpl()
  {
     m_emu_executor = NULL ;
     m_Params = NULL ;
  }

  // Data used when registering Object 
  //
  DECLARE_THREADING_MODEL(otFree);
  DECLARE_PROGID("EmulatorService.EmulatorManager");
  DECLARE_DESCRIPTION("");

  // Function invoked to (un)register object
  //
  static HRESULT WINAPI UpdateRegistry(BOOL bRegister)
  {
    TTypedComServerRegistrarT<TEmulatorManagerImpl> 
    regObj(GetObjectCLSID(), GetProgID(), GetDescription());
    return regObj.UpdateRegistry(bRegister);
  }


BEGIN_COM_MAP(TEmulatorManagerImpl)
  COM_INTERFACE_ENTRY(IEmulatorManager)
END_COM_MAP()

// IEmulatorManager
public:
 
  ExecResult STDMETHODCALLTYPE Init(BSTR Params);
  STDMETHOD(Uninit());
  ExecResult STDMETHODCALLTYPE ExecService(BSTR ServiceName, BSTR ParamsStr,
      BSTR TerminalID);
  int STDMETHODCALLTYPE GetResultCount();
  int STDMETHODCALLTYPE GetResultRecordsetByIndex(int Index,
      Ador_tlb::Recordset15* Recordset, BSTR* RecordsetName);
  BSTR STDMETHODCALLTYPE GetLatestScreen();
  BSTR STDMETHODCALLTYPE GetTransactionID();
  BSTR STDMETHODCALLTYPE DecryptFubonString(BSTR CipherText);
};

#endif //EmulatorManagerImplH
