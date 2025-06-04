//---------------------------------------------------------------------------

#ifndef CustomServiceH
#define CustomServiceH
//---------------------------------------------------------------------------
#include "EmuServiceUtil.h"

#endif

class CCustomService1 : public CServiceInterface {
    virtual void OnProcessEvent( EProcessStatus ProcessStatus , CServiceStatusInfo StatusInfo , EProcessAction &Ation ) {} ;
    virtual void OnGetScreen( CServiceStatusInfo StatusInfo , EProcessAction &Ation ) {} ;
    virtual void OnGetPanel( CServiceStatusInfo StatusInfo , EProcessAction &Ation) {} ;
    virtual void OnGetRecord( CServiceStatusInfo StatusInfo , EProcessAction &Ation) {} ;
    virtual void OnGetParam( CServiceStatusInfo StatusInfo , AnsiString ParamName , AnsiString &Value ) {} ;
} ;

class CCustomService2 : public CServiceInterface {

} ;