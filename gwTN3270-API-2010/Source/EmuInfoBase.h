//---------------------------------------------------------------------------

#ifndef EmuInfoBaseH
#define EmuInfoBaseH

#include "aputil.h"
#include "ADODB.hpp"

//---------------------------------------------------------------------------

class CEmuRect {
public:
    int Left ;
    int Top ;
    int Width ;
    int Height ;
    int Offset ;
    int GetOffset() {
        return Top * 80 + Left ;
    } ;
} ;

class CEmuInfoBase
{
    TADOConnection * m_conn ;
    /*static void Initialize( TADOConnection * conn )
    {

    }
    */
    protected :
        TStringList * m_ProgPanelList ;
        TStringList * m_PanelFieldList ;
        TStringList * m_ServiceSteps ;
        TStringList * m_ProgInfoList ;
        TStringList * m_PanelFieldInfoList ;
        TStringList * m_RecordSetList ;
    public :

    void initDataMap( TStringList * ts )
    {
        ts->Sorted = true ;
        ts->CaseSensitive = true ;
        ts->Duplicates = dupIgnore ;
    }

    CEmuInfoBase( TADOConnection * conn ) {
        m_conn = conn ;
        m_ProgPanelList = new TStringList() ;
        m_PanelFieldList = new TStringList() ;
        m_ServiceSteps = new TStringList() ;
        m_ProgInfoList = new TStringList() ;
        m_PanelFieldInfoList = new TStringList() ;
        m_RecordSetList = new TStringList() ;
        initDataMap(m_ProgPanelList);
        initDataMap(m_PanelFieldList);
        initDataMap(m_ServiceSteps);
        initDataMap(m_ProgInfoList);
        initDataMap(m_PanelFieldInfoList);
        initDataMap(m_RecordSetList);
    }

    TADOQuery * GetProgPanelInfo(  AnsiString ProgID , AnsiString PanelID )
    {
        AnsiString sKey ;
        TStringList * tsDataMap = m_ProgPanelList ;
        sKey =  ProgID.UpperCase() + "." + PanelID.UpperCase() ;
        int idx = tsDataMap->IndexOf( sKey ) ;
        if ( idx >= 0 ) {
            TADOQuery * qry = (TADOQuery *)tsDataMap->Objects[idx];
            qry->First();
            return qry ;
        }
        else {
            TADOQuery *qry = new TADOQuery( NULL ) ;
            qry->Connection = m_conn ;
            AnsiString sSQL ;
            sSQL.sprintf( "select * from ProgPanelsList where progid='%s' and panelid='%s' " , ProgID.c_str() , PanelID.c_str() ) ;
            bool bOk = OpenSQL( sSQL , qry ) ;
            tsDataMap->AddObject( sKey , qry ) ;
            return qry ;
        }
    } 

    CEmuRect * GetPanelFieldInfo( AnsiString ProgID , AnsiString PanelID , TADODataSet *rs=NULL )
    {
        AnsiString sKey ;
        CEmuRect * FieldsRect = NULL ;
        TStringList * tsDataMap = m_PanelFieldList ;
        sKey =  ProgID.UpperCase() + "." + PanelID.UpperCase() ;
        int idx = tsDataMap->IndexOf( sKey );
        if ( idx >= 0 ) {
            FieldsRect = (CEmuRect *)tsDataMap->Objects[idx];
            return FieldsRect ;
        }
        else {
            TADOQuery *qry = new TADOQuery( NULL ) ;
            qry->Connection = m_conn ;
            AnsiString sSQL ;
            sSQL.sprintf( "select * from PanelFieldInfo where progid='%s' and panelid='%s' " , ProgID.c_str() , PanelID.c_str() ) ;
            bool bOk = OpenSQL( sSQL , qry ) ;
            FieldsRect = new CEmuRect[qry->RecordCount] ;
            for ( int fc = 0 ; fc < qry->RecordCount ; fc ++ ) {
                FieldsRect[fc].Left= qry->FieldByName("Left")->AsInteger;
                FieldsRect[fc].Top = qry->FieldByName("Top")->AsInteger;
                FieldsRect[fc].Width = qry->FieldByName("Width")->AsInteger;
                FieldsRect[fc].Height = qry->FieldByName("Height")->AsInteger;
                FieldsRect[fc].Offset = FieldsRect[fc].GetOffset();  
                if ( rs && qry->FieldByName("FieldDesc")->AsString.Length() )   
                    rs->Fields->Fields[fc]->DisplayLabel = qry->FieldByName("FieldDesc")->AsString + " ["+ rs->Fields->Fields[fc]->FieldName +"]";
                qry->Next();
            }
            tsDataMap->AddObject( sKey , (TObject*) FieldsRect ) ;
            return FieldsRect ;
        }
    }

    TADOQuery * GetServiceStepsByStepNo( AnsiString ServiceName , AnsiString StepNo )
    {
        AnsiString sKey ;
        TStringList * tsDataMap = m_ServiceSteps ;
        sKey =  ServiceName.UpperCase() + "." + StepNo.UpperCase() ;
        int idx = tsDataMap->IndexOf( sKey ) ;
        if ( idx >= 0 ) {
            TADOQuery * qry = (TADOQuery *)tsDataMap->Objects[idx];
            qry->First();
            return qry ;
        }
        else {
            TADOQuery *qry = new TADOQuery( NULL ) ;
            qry->Connection = m_conn ;
            AnsiString sSQL ;
            if ( StepNo.Length() == 0 )
                sSQL.sprintf( "select * from tblServiceFlow where ServiceName = '%s' AND StepNo=1 order by StepNo,sn " , ServiceName.c_str() ) ;
            else
                sSQL.sprintf( "select * from tblServiceFlow where ServiceName = '%s' and StepNo = %s order by sn" , ServiceName.c_str(), StepNo.c_str() ) ;
            bool bOk = OpenSQL( sSQL , qry ) ;
            tsDataMap->AddObject( sKey , qry ) ;
            return qry ;
        }
    }

    TADOQuery * GetProgInfo( AnsiString ProgID )
    {
       AnsiString sKey ;
        TStringList * tsDataMap = m_ProgInfoList ;
        sKey =  ProgID.UpperCase() ;
        int idx = tsDataMap->IndexOf( sKey ) ;
        if ( idx >= 0 ) {
            TADOQuery * qry = (TADOQuery *)tsDataMap->Objects[idx];
            qry->First();
            return qry ;
        }
        else {
            TADOQuery *qry = new TADOQuery( NULL ) ;
            qry->Connection = m_conn ;
            AnsiString sSQL ;
            sSQL.sprintf( "select * from proglist where ProgID = '%s' " , ProgID.c_str() ) ;
            bool bOk = OpenSQL( sSQL , qry ) ;
            tsDataMap->AddObject( sKey , qry ) ;
            return qry ;
        }
    }

    TADODataSet * GetDataSet( AnsiString RsName )
    {
        AnsiString sKey ;
        TStringList * tsDataMap = m_RecordSetList ;
        sKey =  RsName.UpperCase() ;
        int idx = tsDataMap->IndexOf( sKey ) ;
        if ( idx >= 0 ) {
            TADODataSet * rs = (TADODataSet *)tsDataMap->Objects[idx];
            rs->First() ;
            while ( !rs->Eof )
                rs->Delete();
            return rs ;
        }
        else {
            TADODataSet * rs = NULL ;
            if ( FileExists( RsName ) ) {
                rs = new  TADODataSet(NULL);
                rs->LoadFromFile( RsName ) ;
                tsDataMap->AddObject( sKey , rs ) ;
            }    
            return rs ;
        }
    }

} ;
#endif
