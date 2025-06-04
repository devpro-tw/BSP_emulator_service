//---------------------------------------------------------------------------

#ifndef ServiceUtilH
#define ServiceUtilH

#include <vcl.h>
#include "gwcore.hpp"
#include "ADODB.hpp"
#include "PElement.hpp"
#include "gwType.hpp"
#include "Forms.hpp"
#include "Classes.hpp"
#include "StrUtils.hpp"   

//---------------------------------------------------------------------------

int GetParams(TStrings *tsParams) {
    tsParams->Clear();
    for (int i = 0; i < ParamCount(); i++) {
        tsParams->Add(ParamStr(i + 1));
    }
    return tsParams->Count;
}

enum EScreenResult {
        srSuccessed = 0 ,
        srServiceNotFound = 1 ,
        srParamError = 2 ,
        srWrongScreen = 3 , // Screen not expected or No Data
        srInputError = 4 ,
        srScreenTimeout = 5 ,
        srEmulatorError = 6 ,
        srInternalError = 7 ,
        srDBOperatorError = 8 ,
        srScriptError = 9 ,
        srInitScreenError = 10 ,
        srBackendServiceDown = 11 ,
        srBackendSignOnFail = 12 ,
        srDataNotFound = 20 ,
        srUserCancel = 99 ,
        srUnknown = 100 } ;

AnsiString GetTempFile( AnsiString sExt ) {
    AnsiString fname ;
    fname.sprintf( "./tmp%d-%d.%s" , GetCurrentThreadId() , GetTickCount() , sExt.c_str() ) ;
    return fname ;
} ;

class CParamInfo : public TStringList {
protected :
    CParamInfo * m_ParentVariable ;
public :
    SetParentVariable( CParamInfo * ParentVariable ) {
        m_ParentVariable = ParentVariable ;
    } ;
    SetParamStr( AnsiString ParamStr ) {
        Text = AnsiReplaceStr( ParamStr , ";" , "\r" ) ;
    }
    __fastcall CParamInfo( AnsiString ParamStr = "" ) {
        SetParamStr( ParamStr );
    }
    __fastcall CParamInfo( CParamInfo * Params) {
        Assign( Params ) ;
    }
    __fastcall virtual ~CParamInfo(void) {} ;
    bool GetValue( AnsiString &Value , AnsiString Name , CParamInfo * ParentParams = NULL ) {
        bool bOk = false ;
        AnsiString LocalValue = Value ;
        //m_logger->WriteLog( "Get Value [%s] ..." , Name ) ;
        if ( ParentParams == NULL ) ParentParams = m_ParentVariable ;
        if ( IndexOfName( Name ) >= 0 ) {
            LocalValue = Values[ Name ] ;
            bOk = true ;
        }
        if ( LocalValue.Length() && LocalValue[1]== '$' ) {
            AnsiString VarName = LocalValue.SubString(2,LocalValue.Length()-1);
            if ( ParentParams ) {
                //bOk = ParentParams->GetValue( LocalValue , VarName , ParentParams ) ;
                bOk = ( ParentParams->IndexOfName( VarName ) >= 0 ) ;
                LocalValue = ParentParams->Values[VarName] ;
            }
            else {
                bOk = false ;
                //m_logger->WriteLog( "Param [%s] Not Fond !" , Name ) ;
            }
        }
        Value = LocalValue ;
        return bOk ;
    } ;
} ;


class CDataSets {
public :
    TStrings * m_RecordsetList ;
    CDataSets() {
        m_RecordsetList = new TStringList ;
    }

    virtual ~CDataSets() {
       delete m_RecordsetList ;
    }
    
    static bool IsBlankRecordset( TADODataSet * rs )
    {
        bool bIsBlank = true ;
        rs->First();
        while ( !rs->Eof ) {
            for ( int fc = 0 ; fc < rs->FieldCount ; fc ++ ) {
                AnsiString data = rs->Fields->Fields[fc]->Value ;
                if ( data.Trim().Length() ) {
                    bIsBlank = false ;
                    break ;
                }
            }
        }
        rs->First();
        return bIsBlank ;
    } ;

    void ClearRecordset( ) {
        TADODataSet * rsData = NULL ;
        while ( m_RecordsetList->Count ) {
            rsData = (TADODataSet *)m_RecordsetList->Objects[0] ;
            m_RecordsetList->Delete( 0 ) ;
            rsData->Close();
            delete rsData ;
        }
    } ;

    TADODataSet * GetRecordsetByIndex( int idx ) {
        TADODataSet * rsData = NULL ;
        if ( idx >=0 && idx < m_RecordsetList->Count )
            rsData = (TADODataSet *)m_RecordsetList->Objects[idx] ;
        return rsData ;
    } ;

    TADODataSet * GetRecordsetByName( AnsiString sRecordsetName , bool AutoCreate = false) {
        TADODataSet * rsData = NULL ;
        int idx = -1 ;
        if ( (idx = m_RecordsetList->IndexOf( sRecordsetName )) < 0 ) {
            if ( AutoCreate ) {
                rsData = new TADODataSet( NULL ) ;
                m_RecordsetList->AddObject( sRecordsetName , rsData ) ;
            }
        }
        else {
            rsData = GetRecordsetByIndex(idx) ;
        }
        return rsData ;
    } ;

    int GetRecordsetCount() {
        return m_RecordsetList->Count ;
    }

    bool AppendRecordset( AnsiString sRecordsetName , TADODataSet * rsPanelData ) {
        int RecNo = rsPanelData->RecNo ;
        int RecordCount = rsPanelData->RecordCount ;
        static int a = 0 ;
        TADODataSet * rsData = GetRecordsetByName( sRecordsetName , true ) ;
        AnsiString sBookmark = rsPanelData->Bookmark ;
        if ( rsData->State == dsInactive ) {
            AnsiString sTempFile ;
            sTempFile.sprintf( "./%s-temp%d-%d.ADTG" , sRecordsetName.c_str() , a++ , GetCurrentThreadId() );
            rsPanelData->SaveToFile( sTempFile , pfADTG );
            rsData->LoadFromFile( sTempFile ) ;
            DeleteFile( sTempFile ) ;
            //rsData->Clone( rsPanelData ) ;
        }
        else {
            while ( !rsPanelData->Eof ) {
                rsData->Append();
                for ( int fc = 0 ; fc < rsPanelData->FieldCount ; fc ++ ) {
                    rsData->Fields->Fields[fc]->Value = rsPanelData->Fields->Fields[fc]->Value ;
                }
                rsData->Post();
                rsPanelData->Next() ;
            }
        }
        rsPanelData->Bookmark = sBookmark ;
        return true ;
    } ;

    int Save( PEStringMap & ds ) {
        PElement bin ;
        AnsiString sFile = GetTempFile( "xml" ) ;
        for ( int i = 0 ; i < m_RecordsetList->Count ; i ++ ) {
            TADODataSet * rs = (TADODataSet *)m_RecordsetList->Objects[ i ] ;
            rs->SaveToFile( sFile.c_str() );
            bin.loadFromFile( sFile.c_str() );
            DeleteFile( sFile ) ;
            AnsiString  rsname = m_RecordsetList->Strings[i] ;
            ds.set( (m_RecordsetList->Strings[i]).c_str() , new PElement(bin) ) ;
        }
        return m_RecordsetList->Count ;
    } ;

    void Load( PEStringMap & Params , AnsiString PElementName = "DataSet" ) {
        PElement *pe = Params[PElementName.c_str()];
        if (pe) {
            PEStringMap::iterator	it;
            TADODataSet * ds = new TADODataSet( NULL ) ;
            PEStringMap *dsp = *pe;
            for ( it = dsp->begin(); it	!=	dsp->end();	it++) {
                string s = it->first ;
                PElement *pe = it->second ;
                string fname = string(GetTempFile( "xml" ).c_str()) ;
                pe->saveToFile( fname.c_str() ) ;   
                ds->LoadFromFile( fname.c_str() );
                DeleteFile( fname.c_str() );
                AppendRecordset( s.c_str() , ds ) ;
            }
            delete ds ;
        }
    }

    AnsiString GetRecordsetNameList () {
        return AnsiReplaceStr(m_RecordsetList->Text,"\n",";") ;
    } ;
} ; 
#endif
