//---------------------------------------------------------------------------

#ifndef EmuScreenUtilH
#define EmuScreenUtilH

#include "Forms.hpp"
#include "ADODB.hpp"
#include "mem.h"
#include "APUtil.h"
#include "EmuInfoBase.h"

//---------------------------------------------------------------------------

bool isBIG5( byte head,byte tail ){
    int iHead = head & 0xff;
    int iTail = tail & 0xff;
    return ((iHead>=0xa1 && iHead<=0xf9 &&
             (iTail>=0x40 && iTail<=0x7e ||
              iTail>=0xa1 && iTail<=0xfe)) ? true : false);
}

class CEmuScreen {
private :     
    char ScreenCharAttr[100][30] ; // E : ascii , H : big5 char 1 , T : big5 char2
    TADOQuery * ADOQry ;
    int m_CurrPanelLeft ;
    int m_CurrPanelTop ;
    int m_CurrPanelWidth ;
    int m_CurrPanelHeight ;
    int m_CurrLinesPerPanel ;
    //CEmuRect FieldsRect[100] ;
    CEmuRect * FieldsRect ;
    TADOConnection * m_conn ;
    ECommLog * m_logger ;
    CEmuInfoBase * m_emuinfo ;
    AnsiString m_AppPath ;

public:
    TStringList * ScreenData ;
    CEmuScreen( TADOConnection * conn , ECommLog * logger , CEmuInfoBase * emuinfo ) {
        m_conn = conn ;
        m_logger = logger ;
        m_emuinfo = emuinfo ;
        //ScreenCharAttrPtr = &ScreenCharAttr ;
        ADOQry = new TADOQuery( NULL ) ;
        ADOQry->Connection = m_conn ;
        ScreenData = new TStringList ;
        m_AppPath = ExtractFilePath(Application->ExeName) ;
    } ;
    ~CEmuScreen() {
        delete ADOQry ;
        delete ScreenData ;
    } ;

    AnsiString GetSchemaFileName( AnsiString ProgID , AnsiString Panel ) {
        AnsiString rsfile ;
        //rsfile.sprintf( "%s\\Schema\\%s-%s.ADTG" , ExtractFilePath(Application->ExeName).c_str() , ProgID.c_str() , Panel.c_str() );
        rsfile = m_AppPath + "\\Schema\\" + ProgID + "-" + Panel + ".ADTG" ;
        return rsfile ;
    }  ;
    
    AnsiString GetScreenData( int l , int t , int w , int h =1 ) {
        AnsiString sText ;
        char strbuf[512] ;
        if ( this == NULL )
            Application->MessageBoxA( "this=NULL" , "error" , MB_OK ) ;
        for ( int r = 0 ; r < h ; r ++ ) {
            if ( (t+r) < ScreenData->Count ) {
                strncpy( strbuf , ScreenData->Strings[t+r].c_str() + l , w ) ;
                strbuf[w]=0;
                sText += AnsiString(strbuf) ;
            }
        }
        return sText;
    } ;

    char GetScreenAttrib( int x,int y ) {
        return ScreenCharAttr[x][y] ;
    } ;

    LoadScreenData( AnsiString Data , int nFormat /* ClearnData , PCOM */ , bool SyncUI = true ) {
        switch ( nFormat ) {
            case 0 : // Pure Screen Text File
                ScreenData->Text = Data ;
                break ;                   
            case 1 : // for PCOM format , include header & screen attrib flag , from offset 16 to 1966
                ScreenData->Text = Data.SubString( 16 , 1966 ) ; //
                break ;
            default :
                ScreenData->Text = Data ;
        }
        if ( SyncUI )
            for ( int line = 0 ; line < ScreenData->Count ; line ++ ) {
                char strBuf[512] = {0} ;
                strncpy( strBuf , ScreenData->Strings[line].c_str() , 100 ) ;
                for ( int c = 0 ; c <= 80 ; c++ ) {
                    if ( isBIG5(strBuf[c],strBuf[c+1]) ) {
                        ScreenCharAttr[c+0][line+0] = 'H' ; // Big5 1st
                        ScreenCharAttr[c+1][line+0] = 'T' ; // Big5 2nd
                        c++;
                    }
                    else
                        ScreenCharAttr[c+0][line+0] = 'E' ; // Normal ASCII
                }
            }
    } ;

    LoadScreenFile( char * FileName ) {  
        ScreenData->LoadFromFile( FileName ) ;
        int nfmt = ExtractFileExt( FileName ).UpperCase() == ".PCOM" ? 1 : 0 ;
        LoadScreenData( ScreenData->Text , nfmt ) ;
    } ;
    AnsiString toString() {
        return ScreenData->Text ;
    } ;
    bool DeserializeFromPE( void * ) ;
    bool SerializeToPE( void * ) ;
    int GetPanelCount ();
    int GetFieldCount ();

    TDataSet * FindPanel( char * ) ;
    TField * FindField( char * ) ;
    TDataSet * Panel ;
    TDataSet * PanelFields ;

    bool GetPanelInfo( AnsiString ProgID , AnsiString Panel ) {
        /*AnsiString sSQL ;
        sSQL.sprintf( "select * from ProgPanelsList where progid='%s' and panelid='%s' " , ProgID.c_str() , Panel.c_str() ) ;
        ADOQry->SQL->Text = sSQL ;
        ADOQry->Close() ;
        ADOQry->Open() ;  */
        ADOQry = m_emuinfo->GetProgPanelInfo(ProgID.c_str() , Panel.c_str()) ;
        if( ADOQry->Eof ) return false ;
        m_CurrPanelLeft=ADOQry->FieldByName("Left")->AsInteger;
        m_CurrPanelTop=ADOQry->FieldByName("Top")->AsInteger;
        m_CurrPanelWidth=ADOQry->FieldByName("Width")->AsInteger;
        m_CurrPanelHeight=ADOQry->FieldByName("Height")->AsInteger;
        m_CurrLinesPerPanel=ADOQry->FieldByName("LinesPerRecord")->AsInteger;
        return true ;
    } ;
    
    bool GetFieldsInfo( AnsiString ProgID , AnsiString Panel , TADOQuery *qry , TADODataSet *rs=NULL ) {
        /*AnsiString sSQL ;
        sSQL.sprintf( "select * from PanelFieldInfo where progid='%s' and panelid='%s' " , ProgID.c_str() , Panel.c_str() ) ;
        qry->SQL->Text = sSQL ;
        qry->Close() ;
        qry->Open() ; */
        FieldsRect = m_emuinfo->GetPanelFieldInfo( ProgID.c_str() , Panel.c_str() , rs ) ;
        /*if( qry->Eof ) return false ;
        FieldsRect =  m_emuinfo->GetPanelFieldsInfo( ProgID.c_str() , Panel.c_str() ) ;
        qry->First();
        for ( int fc = 0 ; fc < qry->RecordCount ; fc ++ ) {
            int fleft , ftop , fwidth , fheight ;
            FieldsRect[fc].Left = qry->FieldByName("Left")->AsInteger;
            FieldsRect[fc].Top = qry->FieldByName("Top")->AsInteger;
            FieldsRect[fc].Width = qry->FieldByName("Width")->AsInteger;
            FieldsRect[fc].Height = qry->FieldByName("Height")->AsInteger;
            FieldsRect[fc].Offset = FieldsRect[fc].GetOffset();
            if ( rs && qry->FieldByName("FieldDesc")->AsString.Length() )
                rs->Fields->Fields[fc]->DisplayLabel = qry->FieldByName("FieldDesc")->AsString + " ["+ rs->Fields->Fields[fc]->FieldName +"]";
            qry->Next();
        }  */
        return true ;
    } ;

    AnsiString GetPanelText( AnsiString ProgID , AnsiString Panel ) {
        GetPanelInfo( ProgID , Panel ) ;
        return GetScreenData( m_CurrPanelLeft , m_CurrPanelTop , m_CurrPanelWidth , m_CurrPanelHeight ) ;
    } ;

    TADODataSet * BuildRecordSet( AnsiString ProgID , AnsiString Panel , TADODataSet * rs ) {
        if ( rs == NULL )
            rs = m_emuinfo->GetDataSet( GetSchemaFileName(ProgID,Panel) ) ;
        else
            rs->LoadFromFile( GetSchemaFileName(ProgID,Panel) ) ;
        return rs ;
    } ;

    void FillDataToRecordSet( AnsiString ProgID , AnsiString Panel , TADODataSet * rs , AnsiString ScrData , bool Rebuild , int nFormat ) {
        bool bpanel = GetPanelInfo(ProgID,Panel);
        LoadScreenData( ScrData , nFormat , false ) ;

        //if (Rebuild) BuildRecordSet(ProgID,Panel,rs) ;
        int panelheight = m_CurrPanelHeight ; //panel->FieldByName("Height")->AsInteger;
        int linesperrecord = m_CurrLinesPerPanel ; // panel->FieldByName("LinesPerRecord")->AsInteger ;
        int fc , line = 0 , RecCount = 0 ;

        if ( linesperrecord == 0 ) linesperrecord = panelheight ;
        if ( linesperrecord > 0 ) {
            GetFieldsInfo( ProgID ,  Panel , ADOQry , rs ) ;

            RecCount = 0 ;
            for ( line = 0 ; line < panelheight ; line += linesperrecord ) {
                rs->Append();
                int nEmptyFieldCount = 0 ;
                for ( int fc = 0 ; fc < rs->FieldCount ; fc ++ ) {
                    int nTopOffset = line  ;
                    // for fixed value in recordset
                    if ( FieldsRect[fc].Top < m_CurrPanelTop )
                        nTopOffset = 0 ;
                    AnsiString data = GetScreenData( FieldsRect[fc].Left,FieldsRect[fc].Top+nTopOffset,FieldsRect[fc].Width,FieldsRect[fc].Height) ;
                    rs->Fields->Fields[fc]->Value = data.Trim() ;
                    if ( data.Trim().Length()==0 ) nEmptyFieldCount ++ ;
                }
                // Skip Empty Record 
                if ( ( RecCount == 0 ) || nEmptyFieldCount != rs->FieldCount ) {
                    rs->Post();
                    RecCount ++ ;
                }
                else
                    rs->Cancel() ;
            }
            rs->First();
        }
    } ;
    
    bool FindFieldOffset( AnsiString ProgID , AnsiString PanelID , AnsiString OffsetFieldName , TADODataSet * rs , CEmuRect & rect, AnsiString LocateFieldNames ="", AnsiString FindValues="" ) {
        Variant rc = true ;
        if ( LocateFieldNames.Length() && rs->FindField(LocateFieldNames) ){
            TLocateOptions Opts;
            Opts.Clear();
            Opts << loCaseInsensitive;
            Variant locvalues[1];
            locvalues[0] = Variant(FindValues);
            //rs->Locate(FieldNames, VarArrayOf(locvalues, 0), Opts);
            try {
                rc = rs->Locate(LocateFieldNames, Variant(FindValues), Opts);
            }
            catch ( Exception &e ) {
                m_logger->WriteLog( e.Message.c_str() );
            }
        }
        if ( m_CurrLinesPerPanel == 0 )
            m_CurrLinesPerPanel = 1 ;
        if ( rc ) {
            TField * fd = rs->FindField( OffsetFieldName ) ;
            if ( fd ) {
                int fc = fd->FieldNo -1;
                int recno = rs->RecNo;
                GetFieldsInfo( ProgID ,  PanelID , ADOQry ) ;
                rect.Left = FieldsRect[fc].Left ;
                rect.Top = FieldsRect[fc].Top + (recno-1) * m_CurrLinesPerPanel ;
                rect.Width = FieldsRect[fc].Width ;
                rect.Height = FieldsRect[fc].Height ;
                rect.Offset = rect.Top * 80 + rect.Left;
            }
            else
                rc = false ;
        }
        return rc ;
    } ;  
} ;


#endif
