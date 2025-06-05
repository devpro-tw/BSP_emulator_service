//---------------------------------------------------------------------------

#ifndef EmuServiceUtilH
#define EmuServiceUtilH
//---------------------------------------------------------------------------
#include "ServiceUtil.h"
#include "PComUnit.h"
#include "APUtil.h"
#include "EStepDebug.h"
#include "EmuInfoBase.h"

class CServiceStatusInfo {
public :
    CServiceStatusInfo() {
       Params = new CParamInfo ;
    } ;
    virtual ~CServiceStatusInfo() {
       delete Params ;
    } ;
    AnsiString ServiceName ;
    CParamInfo * Params ;
    AnsiString ScreenName ;
    AnsiString PanelName ;
    AnsiString ScreenPath ;
    long ResultCode ; 
} ;

class CServiceInterface {
    enum EProcessStatus { epsInit , epsServiceStart , epsEnumScreen , epsEnumPanel , epsEnumRecord , epsEnumChildScreen , epsServiceEnd } ;
    enum EProcessAction { paAccept , paHandled , paAbort , paReturn } ;

    virtual void OnProcessEvent( EProcessStatus ProcessStatus , CServiceStatusInfo StatusInfo , EProcessAction &Ation ) {} ;
    virtual void OnGetScreen( CServiceStatusInfo StatusInfo , EProcessAction &Ation ) {} ;
    virtual void OnGetPanel( CServiceStatusInfo StatusInfo , EProcessAction &Ation) {} ;
    virtual void OnGetRecord( CServiceStatusInfo StatusInfo , EProcessAction &Ation) {} ;
    virtual void OnInputField( CServiceStatusInfo StatusInfo , EProcessAction &Ation) {} ;
    virtual void OnGetParam( CServiceStatusInfo StatusInfo , AnsiString ParamName , AnsiString &Value ) {} ;
} ;

bool RecordsetLocate( TADODataSet * rs , CParamInfo * Params , ECommLog * m_logger) {
    Variant rc = false ;
    TLocateOptions Opts;
    Opts.Clear();
    Opts << loPartialKey ; // loCaseInsensitive;
    AnsiString LocateFieldNames , FindValues ;
    try {
        int PrevRecNo = rs->RecNo ;
        if ( Params->Count && Params->Text.Pos("=") > 1 ) {
            if ( Params->Count == 1 ) {
                LocateFieldNames = Params->Names[0] ;
                bool bParamExists = Params->GetValue( FindValues , LocateFieldNames ) ;
                rc = rs->Locate(LocateFieldNames, Variant(FindValues), Opts);
            }
            else { // Multi-Key
                Variant locvalues[20] ;
                int nKeyCount = 0 ;
                for ( int i = 0 ; i < Params->Count ; i ++ ) {
                    if  ( Params->GetValue( FindValues , Params->Names[i] ) ) {
                        LocateFieldNames = LocateFieldNames + AnsiString( i ? ";" : "" ) + Params->Names[i] ;
                        locvalues[nKeyCount] = Variant( FindValues ) ;
                        nKeyCount ++ ;
                    }
                }
                Variant v = VarArrayOf(locvalues, nKeyCount - 1 ) ;
                rc = rs->Locate( LocateFieldNames, v , Opts) ;
            }
        }
        else {
            m_logger->WriteLog( "Seek Set Param Not Fond !" ) ;
            OutputDebugString( "Seek Set Param Not Fond !" ) ;
            rc = false ;
        }
        if ( PrevRecNo == rs->RecNo ) {
            m_logger->WriteLog( "Seek No effect !" ) ;
            OutputDebugString( "Seek No effect !" ) ;
        }
        if ( rc == Variant(false) )
           m_logger->WriteLog( "Seek Data not found %s=[%s]!" , LocateFieldNames.c_str() , FindValues.c_str() ) ;
    }
    catch ( Exception &e ) {
        m_logger->WriteLog( "Seek Data Exception %s , %s=[%s]!" , e.Message.c_str() , LocateFieldNames.c_str() , FindValues.c_str() ) ;
        OutputDebugString( AnsiString().sprintf("Seek Data Exception %s , %s=[%s]!" , e.Message.c_str() , LocateFieldNames.c_str() , FindValues.c_str()).c_str() ) ;
    }
    return rc ;
}

int RecordsetFilter( TADODataSet * ds , AnsiString sFilter , CParamInfo * ServiceParams ) {
    ds->Filtered = false ;
    for ( int i = 0 ; i < ServiceParams->Count ; i ++ )
        sFilter = AnsiReplaceStr( sFilter , AnsiString("$") + ServiceParams->Names[i] , ServiceParams->Values[ServiceParams->Names[i]] ) ;
    ds->Filter = sFilter ;
    ds->Filtered = true ;
    return ds->RecordCount ;
}

class CEmuService : public CEHLLEmulator
{
private :
    TStrings * m_ServiceInterfaceList ;
    TStringList * g_tsFlowTree ;
    TADOConnection * m_conn ;
    CEmuScreen * m_scr ;
    int m_servicecount ;
    bool m_DebugMode ;
    bool m_Running ;
    bool m_UserStop ;
    bool *m_TermFlag ;
    AnsiString m_userid ;
    AnsiString m_password ;
    bool IsStop() {
        return ( m_TermFlag && (*m_TermFlag) ) ;
    }

    int g_level ;
    CEmuInfoBase *m_emuinfo ;

protected :
    AnsiString m_LatestErrorScreen ;
    AnsiString m_LatestErrorPath ;
    AnsiString m_EmulatorWindowName ;
    AnsiString m_LockEmulator ;
    HANDLE m_EmulatorHandle ;
public :
    //AnsiString AppPath ;
    CDataSets * m_DataSets ;
    CEmuService( TStrings * Params ) : CEHLLEmulator( /*Params*/ )
    {   try  {
        AnsiString DBName = Params->Values["DBName"];
        AnsiString DBPWDString = Params->Values["DBPWD"];
        if ( DBPWDString.Length() >0 ) DBPWDString = ";Jet OLEDB:Database Password=" + DBPWDString ;
        m_EmulatorWindowName = Params->Values["EmulatorWindowName"];
        m_LockEmulator = Params->Values["LockEmulator"];
        //AppPath = ExtractFilePath(Application->ExeName) ;
        if ( DBName.Length() == 0 ) DBName = m_AppPath + "ScreenLayoutInfo.mdb" ;
        TADOConnection* m_ADOConn = new TADOConnection( NULL ) ;
        m_ADOConn->ConnectionString = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + DBName + DBPWDString + ";Mode=Read;Persist Security Info=False" ;
        // Provider=Microsoft.Jet.OLEDB.4.0;Data Source=TEST.MDB;Jet OLEDB:Database Password=1111;Mode=Read;Persist Security Info=False
        m_ADOConn->LoginPrompt = false ;
        m_ADOConn->Open();
        OutputDebugString( "DB Open successed !" ) ;
        Params->Values["DBPWD"] = "@@" ;
        m_conn = m_ADOConn ;
        m_ServiceInterfaceList = new TStringList ;
        m_DataSets = new CDataSets ;
        g_tsFlowTree = new TStringList ;
        m_emuinfo = new CEmuInfoBase(m_conn);
        m_scr = new CEmuScreen( m_conn , m_logger , m_emuinfo) ;
        m_Running = false ;
        OutputDebugString( "Init CEmuService successed !" ) ;
        }
        catch ( Exception &e)
        {
             MessageBox(   0 , e.Message.c_str() , "Error" , MB_OK ) ;
        }
    } ;

    virtual ~ CEmuService() {
        delete m_scr ;
        delete m_ServiceInterfaceList ;
    } ;

    bool LockEmulator()
    {
        m_EmulatorHandle = FindWindow(NULL, m_EmulatorWindowName.c_str());
        if (m_EmulatorHandle) {
            SetWindowLong(m_EmulatorHandle, GWL_STYLE, GetWindowLong(m_EmulatorHandle, GWL_STYLE) | WS_DISABLED );
            ShowWindow( m_EmulatorHandle, SW_MINIMIZE ) ;
            Application->ProcessMessages();
        }
        return (m_EmulatorHandle != NULL);
    }

    void UnlockEmulator()
    {
        if (m_EmulatorWindowName.Length() && m_EmulatorHandle) {
            SetWindowLong(m_EmulatorHandle, GWL_STYLE, GetWindowLong(m_EmulatorHandle, GWL_STYLE) & (~WS_DISABLED) );
            Application->ProcessMessages();
        }
    }

    AnsiString GetLatestErrorScreen()
    {
        return m_LatestErrorScreen ;
    }

    AnsiString GetLatestErrorPath()
    {
        return m_LatestErrorPath ;
    }

    void ServiceBegin( ) {
        m_LatestErrorScreen = "" ;
        m_LatestErrorPath = "" ;
    } ;

    void ServiceEnd() {

    } ;

    virtual int RegisterService( AnsiString ServiceName , CServiceInterface *ServIntr ) {
        ServiceName = ServiceName.UpperCase() ;
        if ( m_ServiceInterfaceList->IndexOf(ServiceName) == -1 )
            m_ServiceInterfaceList->AddObject( ServiceName , (TObject*) ServIntr ) ;
    } ;

    CServiceInterface * GetServiceInterface( AnsiString ServiceName ) {
        ServiceName = ServiceName.UpperCase() ;
        CServiceInterface * servintf = NULL ;
        int idx ;
        if ( idx = m_ServiceInterfaceList->IndexOf(ServiceName) >= 0 )
            servintf = ( CServiceInterface *) m_ServiceInterfaceList->Objects[idx] ;
        return servintf ;
    } ;   

    EScreenResult DoStepDebug( AnsiString sProg , AnsiString sPanel , AnsiString sPrompt , AnsiString sKey , EScreenResult CurrentReslut , bool UncheckAutoAccept = false ) {
        EScreenResult rc ;
        if ( m_DebugMode ) {
            if ( UncheckAutoAccept ) dlgStepDebug->AutoAccept->Checked = false ;
            int mrResult = dlgStepDebug->Execute( sProg , sPanel , sKey , sPrompt );
            switch ( mrResult ) {
                case mrYes :
                    rc = srSuccessed ;
                    break ;
                case mrNo :
                    rc = CurrentReslut ;
                    break ;
                case mrCancel :
                    rc = srUserCancel ;
                    break ;
            }
        }
        else {
            rc = CurrentReslut ;
        }
        return rc ;
    }

    bool CheckSignatureData( AnsiString Screen , AnsiString sScreenSignature )
    {
        TReplaceFlags rf ;
        rf << rfReplaceAll ;
        sScreenSignature = StringReplace( sScreenSignature , " " , "" , rf ) ;
        AnsiString sCurrentScreenPackData ;
        sCurrentScreenPackData =  StringReplace( Screen , " " , "" , rf ) ;
        sCurrentScreenPackData = StringReplace( sCurrentScreenPackData , "�@" , "" , rf ) ; // BIG5 BLANK
        bool bSignaturePass = ( sCurrentScreenPackData.Pos( sScreenSignature ) != 0 ) ;
        return bSignaturePass ;
    }

    bool CheckScreenSignature( AnsiString & sScreenText , AnsiString sScreenSignature , TStrings * tsAttrib , bool bIsOptional ) {
        TReplaceFlags rf ;
        rf << rfReplaceAll ;
        sScreenSignature = StringReplace( sScreenSignature , " " , "" , rf ) ;
        // Execute Panel Action
        sScreenText = "" ;
        bool bScreenSignatureMatch = GetData( sScreenText , sScreenSignature , tsAttrib , 200 , "" , bIsOptional ) ;
        return bScreenSignatureMatch ;
    }

    enum EScreenResult RunStep( AnsiString ServiceName, AnsiString ScreenName , CParamInfo * ServiceParams )
    {
        CParamInfo * ParentParams = new CParamInfo( ServiceParams ) ;
        TADODataSet * rsPanelData = NULL ;
        if ( !m_CacheMode )
            rsPanelData = new TADODataSet( NULL ) ;
            
        CParamInfo * PanelParams = new CParamInfo() ;
        TStrings *tsAttrib = new TStringList;
        AnsiString sProg, sPanel, sPanelType , sParam, sTmp, sParamDesc, sChildScreen, sStepNo;
        AnsiString sScreenText ;
        EScreenResult ExecResult = srUnknown ;
        bool bOK;

        TADOQuery * ADOQryService ; //= new TADOQuery( NULL ) ;
        //ADOQryService->Connection = m_conn ;
        TADOQuery * ADOProgList ; // = new TADOQuery( NULL ) ;
        //ADOProgList->Connection = m_conn ;
        TADOQuery * ADOProgPanelList ; //= new TADOQuery( NULL ) ;
        //ADOProgPanelList->Connection = m_conn ;

        AnsiString sSQL ;
        g_level ++ ;
        int idx = -1 ;

        if ( g_level >15 ) {
            m_logger->WriteLog( "Warring : Level Stack Over Flow ! " ) ;
            OutputDebugString( "Warring : Level Stack Over Flow ! " ) ;
        }

        if ( IsStop() )
            goto lbexit ;

        /*if ( g_level == 1 )
        {
            sSQL.sprintf( "select top 1 * from tblServiceFlow where ServiceName = '%s' order by StepNo "
                , ServiceName.c_str() ) ;

        }
        else
        {
            if ( ScreenName.Length() )
                sSQL.sprintf( "select * from tblServiceFlow where ServiceName = '%s' and StepNo = %s"
                , ServiceName.c_str(), ScreenName.c_str() ) ;
            else
                m_logger->WriteLog( "No Screen Name !" ) ;
        } */

        ///if ( InternalOpenSQL( sSQL , ADOQryService ) ) {
        m_logger->WriteLog( "Tick Start : GetServiceStepsByStepNo , Screen Stack Level %d , ServiceName=%s ,ScreenName=%s" , g_level , ServiceName.c_str() , ScreenName.c_str() ) ;
        OutputDebugString( AnsiString().sprintf("Tick Start : GetServiceStepsByStepNo , Screen Stack Level %d , ServiceName=%s ,ScreenName=%s" , g_level , ServiceName.c_str() , ScreenName.c_str()).c_str() ) ;
        if ( ADOQryService = m_emuinfo->GetServiceStepsByStepNo(ServiceName,ScreenName) ) {
            m_logger->WriteLog( "Tick End : GetServiceStepsByStepNo " ) ;
            OutputDebugString( "Tick End : GetServiceStepsByStepNo " ) ;
            if  ( ADOQryService->Eof ) {
                m_logger->WriteLog( "Screen Link Error or Service Not Found , SQL : %s" , sSQL.c_str() ) ;
                OutputDebugString( AnsiString().sprintf("Screen Link Error or Service Not Found , SQL : %s" , sSQL.c_str()).c_str() ) ;
                ExecResult = srServiceNotFound ;
                goto lbexit ;
            }
            int PanelIdx = 0 ;
            while ( !ADOQryService->Eof ) {
                int n_PanelPages = 0;
                sStepNo = ADOQryService->FieldByName("StepNo")->AsString;
                sProg = ADOQryService->FieldByName("ScreenId")->AsString;
                AnsiString SN = ADOQryService->FieldByName("SN")->AsString;
                AnsiString sSchemaFile = ADOQryService->FieldByName("SchemaFile")->AsString;
                AnsiString sSubmitKey = ADOQryService->FieldByName("SubmitKey")->AsString;
                AnsiString sPrevKey = ADOQryService->FieldByName("PrevKey")->AsString;
                AnsiString sAttributes = ADOQryService->FieldByName("Attributes")->AsString;
                sPanel = ADOQryService->FieldByName("PANEL")->AsString;
                sPanelType = ADOQryService->FieldByName("PANELTYPE")->AsString;
                AnsiString sSeekSetParam = ADOQryService->FieldByName("SeekSetParam")->AsString;
                sParamDesc = ADOQryService->FieldByName("SetParam")->AsString;
                PanelParams->SetParamStr( sParamDesc ) ;
                sChildScreen = ADOQryService->FieldByName("ChildScreen")->AsString;
                AnsiString sPanelReturnData = ADOQryService->FieldByName("PanelReturnData")->AsString.UpperCase() ;
                AnsiString sRecordsetName = ADOQryService->FieldByName("RecordsetName")->AsString.UpperCase() ;
                AnsiString sFilter = ADOQryService->FieldByName("Filter")->AsString.Trim() ;
                AnsiString sScreenSignature ;
                AnsiString sScrollDown ;
                AnsiString sPrevPanelText ;
                bool bScreenSignatureMatch ;
                bool bIsOptionalPanel = sPanelType.UpperCase().Pos( "OPTIONAL" ) > 0 ;

                PanelIdx ++ ;

                int nPanelCount = ADOQryService->RecordCount ;
                m_logger->WriteLog( "SN:%s Enter Panel (%d/%d) servicename:%s step:%s\\%s-%s" , SN.c_str() , PanelIdx , nPanelCount , ServiceName.c_str() , ScreenName.c_str() , sProg.c_str() , sPanel.c_str() );
                OutputDebugString( AnsiString().sprintf("SN:%s Enter Panel (%d/%d) servicename:%s step:%s\\%s-%s" , SN.c_str() , PanelIdx , nPanelCount , ServiceName.c_str() , ScreenName.c_str() , sProg.c_str() , sPanel.c_str()).c_str() );

                AnsiString sPrevPath = m_ScreenPath ;
                m_ScreenPath = ServiceName + "." + ScreenName + "."+ sProg + "." + sPanel + "-" + IntToStr(PanelIdx)  ;

                tsAttrib->Text = AnsiReplaceStr( sAttributes , ";" , "\n" ) ;
                AnsiString IdleMsg , BusyMsg ;
                AnsiString NoMoreDataMsg = tsAttrib->Values["NoMoreDataMessage"] ;
                AnsiString DataNotFoundMsg = tsAttrib->Values["DataNotFoundMessage"] ;

                idx = -1 ; // g_tsFlowTree->IndexOf(sStepNo) ;
                if ( idx==-1 ) {
                    //g_tsFlowTree->Add(sStepNo) ;
                    // Check Form Signature

                    bScreenSignatureMatch = false ;
                    AnsiString sSchemaFile = ADOQryService->FieldByName("SchemaFile")->AsString ;
                    //ReadConfigFile( sSchemaFile.c_str() ) ;
                    //sSQL.sprintf( "select * from ProgPanelsList where progid='%s' and panelid='%s' " , sSchemaFile.c_str() , sPanel.c_str()) ;
                    //if ( InternalOpenSQL( sSQL , ADOProgPanelList ) ) {
                    m_logger->WriteLog( "Tick Start : GetProgPanelInfo" ) ;
                    OutputDebugString( "Tick Start : GetProgPanelInfo" ) ;
                    if ( ADOProgPanelList = m_emuinfo->GetProgPanelInfo( sSchemaFile.c_str() , sPanel.c_str() ) ) {
                        m_logger->WriteLog( "Tick End : GetProgPanelInfo" ) ;
                        OutputDebugString( "Tick End : GetProgPanelInfo" ) ;
                        if  ( ADOProgPanelList->Eof ) {
                            m_logger->WriteLog( "Screen Link Error or Service Not Found , SQL : %s" , sSQL.c_str() ) ;
                            OutputDebugString( AnsiString().sprintf("Screen Link Error or Service Not Found , SQL : %s" , sSQL.c_str()).c_str() ) ;
                            ExecResult = srScriptError ;
                            goto lbexit ;
                        }
                        else {
                            sScrollDown = ADOProgPanelList->FieldByName( "ScrollDown" )->AsString ;
                        }
                    }
                    else {
                        m_logger->WriteLog( "Screen Link error [%s]" , sSQL.c_str() );
                        OutputDebugString( AnsiString().sprintf("Screen Link error [%s]" , sSQL.c_str()).c_str() );
                    }

                    int nPanelType = ADOProgPanelList->FieldByName("Type")->AsInteger ;

                    //sSQL.sprintf( "select * from proglist where ProgID = '%s' " , sSchemaFile.c_str() ) ;
                    //if ( sSchemaFile.Length() && InternalOpenSQL( sSQL , ADOProgList ) && !ADOProgList->Eof ) {
                    m_logger->WriteLog( "Tick Start : GetProgInfo" ) ;
                    OutputDebugString( "Tick Start : GetProgInfo" ) ;
                    if ( sSchemaFile.Length() && (ADOProgList = m_emuinfo->GetProgInfo( sSchemaFile )) && !ADOProgList->Eof ) {
                        m_logger->WriteLog( "Tick End : GetProgInfo" ) ;
                        OutputDebugString( "Tick End : GetProgInfo" ) ;
                         sScreenSignature = ADOProgList->FieldByName("ProgName")->AsString ;
                         bScreenSignatureMatch = CheckScreenSignature( sScreenText , sScreenSignature , tsAttrib , bIsOptionalPanel ) ;
                    }
                    else {
                         ExecResult = srScriptError ;
                    }

                    if ( !bScreenSignatureMatch ) {
                        AnsiString sMessage ;
                        sMessage.sprintf("Signature \"%s\" Screen : %s , %s" ,
                            sScreenSignature.c_str() , ScreenName.c_str() , sSchemaFile.c_str()) ;

                        if ( DataNotFoundMsg.Length() ) {
                            if ( sScreenText.Pos( DataNotFoundMsg ) ) {
                                m_logger->WriteLog( "Data Not Found Message Matched : '%s' " , DataNotFoundMsg.c_str() );
                                OutputDebugString( AnsiString().sprintf("Data Not Found Message Matched : '%s' " , DataNotFoundMsg.c_str()).c_str() ) ;
                                ExecResult = srDataNotFound ;
                            }
                            else
                                m_logger->WriteLog( "Data Not Found Message Not Matched : '%s' " , DataNotFoundMsg.c_str() );
                                OutputDebugString( AnsiString().sprintf("Data Not Found Message Not Matched : '%s' " , DataNotFoundMsg.c_str()).c_str() ) ;
                        }
                        else if ( g_level == 1 ) {
                            if ( bIsOptionalPanel ) {
                                m_logger->WriteLog( "Optional Panel was skipped : %s" , sScreenSignature.c_str() );
                                OutputDebugString( AnsiString().sprintf("Optional Panel was skipped : %s" , sScreenSignature.c_str()).c_str() ) ;
                                ADOQryService->Next();
                                ExecResult = srSuccessed ;
                                continue ;
                            }
                            else
                                ExecResult = srInitScreenError ; // if Init Screen Error , try relogin
                        }
                        else if ( sPanelType.UpperCase().Pos( "OPTIONAL" ) ) {
                            m_logger->WriteLog( "Optional Panel was skipped : %s" , sScreenSignature.c_str() );
                            OutputDebugString( AnsiString().sprintf("Optional Panel was skipped : %s" , sScreenSignature.c_str()).c_str() ) ;
                            ADOQryService->Next();
                            ExecResult = srSuccessed ;
                            continue ;
                        }
                        else {
                            if ( g_level <= 2 )
                                ExecResult = srInitScreenError ;
                            else
                                ExecResult = srWrongScreen ;
                        }
                        ExecResult = DoStepDebug( sProg.c_str() , sPanel.c_str() , AnsiString("Wrong Screen") + sMessage , "" , ExecResult , true )  ;
                    }
                    else {
                        ExecResult = srSuccessed ;
                    }
                    //

                    if ( ExecResult != srSuccessed ) {
                        //ExecResult = srSuccessed ;
                        goto lbexit ;
                    }

                    bool bNoMoreData = false ;
                    rsPanelData = m_scr->BuildRecordSet(sSchemaFile , sPanel , rsPanelData ) ;

                    m_scr->FillDataToRecordSet( sSchemaFile , sPanel, rsPanelData, sScreenText, true , m_ScreenTextFormat ) ;
                    sPrevPanelText = m_scr->GetPanelText( sSchemaFile , sPanel ) ;
                    if ( sFilter.Length() ) {
                        int reccount = RecordsetFilter( rsPanelData , sFilter , ServiceParams ) ;
                        if ( reccount == 0 ) bNoMoreData = true ;
                    }
                    if ( sPanelReturnData == "1" || sPanelReturnData == "M" ) {
                        if ( ! sRecordsetName.Length() ) sRecordsetName = sProg + "_" + sPanel ;
                        bool bAppendRecordset = m_DataSets->AppendRecordset( sRecordsetName , rsPanelData ) ;
                    }

                    if ( NoMoreDataMsg.Length() && sScreenText.Pos( NoMoreDataMsg ) > 0 ) {
                        m_logger->WriteLog( "Scroll down stop : (no more data message : %s)" , NoMoreDataMsg.c_str() ) ;
                        bNoMoreData = true ;
                    }

                    bool bScrollNextPage = (sScrollDown.Length() > 0) && (!bNoMoreData) ;
                    bool bPanelAlreadySeek = false ;
                    if ( sChildScreen.Length() || sSubmitKey.Length() || bScrollNextPage ) {
                        do {
                            m_logger->WriteLog( "Panel Scroll down !" ) ;
                            bool bSeekOk = true ;
                            if ( !bPanelAlreadySeek ) {
                                if ( sSeekSetParam.Length() ) {
                                    CParamInfo * SeekSetParam = new CParamInfo(sSeekSetParam) ;
                                    SeekSetParam->SetParentVariable(ServiceParams) ;
                                    // clear filter mode for recordser locate
                                    rsPanelData->Filtered = false ;
                                    bSeekOk = RecordsetLocate( rsPanelData , SeekSetParam , m_logger ) ;
                                    if ( bSeekOk ) {
                                        bPanelAlreadySeek = true ;
                                        bScrollNextPage = false ;
                                    }
                                    /*  // ����I�s
                                        else {
                                        m_logger->WriteLog( "Seek Value Not Exists!" ) ;
                                        ExecResult = srWrongScreen ;
                                        goto lbexit ;
                                    }  */
                                    delete  SeekSetParam ;
                                }
                            }

                            while ( ! rsPanelData->Eof && bSeekOk ) {
                                CEmuRect rect ;
                                // Input All Field Data
                                for ( int i = 0 ; i < PanelParams->Count ; i++ ) {
                                    bOK = m_scr->FindFieldOffset( sSchemaFile , sPanel, PanelParams->Names[i] , rsPanelData, rect );
                                    if ( bOK ) {
                                        // Replace Variable '$xxx'
                                        AnsiString Param ;
                                        bool bParamExists = false ;
                                        if ( bParamExists = PanelParams->GetValue( Param , PanelParams->Names[i] , ServiceParams ) ) {
                                            ExecResult = DoStepDebug( sProg.c_str() , sPanel.c_str() , "Input Field Prompt" , Param , ExecResult , false )  ;
                                            bool bEraseField = true ;
                                            if ( sProg.UpperCase() == "ENTER" )
                                                bEraseField = true ;

                                            AnsiString OrgData = m_scr->GetScreenData( rect.Left , rect.Top , rect.Width , rect.Height ) ;
                                            m_logger->WriteLog( "Input Field %s=\"%s\" at offset %d (OrgData=%s) %s" ,PanelParams->Names[i].c_str() ,Param.c_str() , rect.Offset ,OrgData.c_str() , bEraseField ? "With Erase Field" : "" ) ;

                                        /*#ifdef TN5250API
                                           InputField( Param , rect.Offset , bEraseField ) ;
                                        #else */
                                            if ( bEraseField && Param.Length() == 0 ) {
                                                char temp[150] ;
                                                memset( temp , ' ' , 124  ) ;
                                                memcpy( temp , Param.c_str() , Param.Length() ) ;
                                                temp[rect.Width] = 0 ;
                                                Param = temp ;
                                            }
                                            //Param = AnsiReplaceStr( Param , "�@" , "��" ) ;
                                            InputField( "\"" + Param + "\"" , rect.Offset , bEraseField ) ;
                                        //#endif

                                        }
                                        else {
                                            // * ExecResult = srParamError ;
                                            m_logger->WriteLog( "Params %s Not Exists!" , PanelParams->Names[i].c_str() ) ;
                                            OutputDebugString( AnsiString().sprintf("Params %s Not Exists!" , PanelParams->Names[i].c_str()).c_str() ) ;
                                        }
                                    }
                                    else {
                                        ExecResult = srParamError ;
                                        m_logger->WriteLog( "Find Field [%s] Offset Error !" , PanelParams->Names[i].c_str() ) ;
                                        OutputDebugString( AnsiString().sprintf("Find Field [%s] Offset Error !" , PanelParams->Names[i].c_str()).c_str() ) ;
                                    }
                                }

                                /*AnsiString sOIA ;
                                if ( GetOIA( sOIA ) ) {
                                    if ( sOIA.Pos("X") )
                                        ExecResult = DoStepDebug( sProg.c_str() , sPanel.c_str() , "Input Field Error" , "" , srInputError , false )  ;
                                } */

                                if ( ExecResult != srSuccessed ) {
                                    m_logger->WriteLog( "Input Field Error , exit the group" );
                                    OutputDebugString( "Input Field Error , exit the group" );
                                    goto lbexit ;
                                }

                                if ( sSubmitKey.Length() ) {
                                    m_logger->WriteLog( "Submit Key [%s] to child panel %s\\%s " , sSubmitKey.c_str() , ServiceName.c_str() , sChildScreen.c_str() ) ;
                                    OutputDebugString( AnsiString().sprintf("Submit Key [%s] to child panel %s\\%s " , sSubmitKey.c_str() , ServiceName.c_str() , sChildScreen.c_str()).c_str() ) ;
                                    ExecResult = DoStepDebug( sProg.c_str() , sPanel.c_str() , "Send Submit Key Prompt" , sSubmitKey , ExecResult , false )  ;
                                    if ( ExecResult != srSuccessed ) {
                                        m_logger->WriteLog( "Run Child Error , exit the group" );
                                        OutputDebugString( "Run Child Error , exit the group" );
                                        goto lbexit ;
                                    }
                                    SubmitKey( sSubmitKey )  ;
                                }

                                if ( sChildScreen.Length() ) {
                                    ExecResult = RunStep( ServiceName , sChildScreen , ServiceParams ) ;
                                    m_logger->WriteLog( "Return on Panel (%d/%d) servicename:%s step:%s\\%s-%s" , PanelIdx , nPanelCount , ServiceName.c_str() , ScreenName.c_str() , sProg.c_str() , sPanel.c_str() );
                                    OutputDebugString( AnsiString().sprintf("Return on Panel (%d/%d) servicename:%s step:%s\\%s-%s" , PanelIdx , nPanelCount , ServiceName.c_str() , ScreenName.c_str() , sProg.c_str() , sPanel.c_str()).c_str() );
                                    //ReadConfigFile( sSchemaFile.c_str() ) ; // reload config
                                    m_ScreenPath = ServiceName + "." + ScreenName + "."+ sProg + "." + sPanel + "-" + IntToStr(PanelIdx)  ;
                                    if ( nPanelType == 1 ) {// some panel need pagedown when return on previous panel
                                        m_logger->WriteLog( "Panel Type : 1" );
                                        OutputDebugString( "Panel Type : 1" );
                                        for ( int i = 0 ; i < n_PanelPages ; i++ )
                                            if ( bScrollNextPage ) {
                                                ExecResult = DoStepDebug( sProg.c_str() , sPanel.c_str() , "Send ScrollDown Key Prompt" , sScrollDown , ExecResult , false )  ;
                                                m_logger->WriteLog( "Scroll Pagedown Key [%s] " , sScrollDown.c_str() ) ;
                                                OutputDebugString( AnsiString().sprintf("Scroll Pagedown Key [%s] " , sScrollDown.c_str()).c_str() ) ;
                                                SubmitKey( sScrollDown ) ;
                                            }
                                    }
                                }

                                if ( ExecResult != srSuccessed ) {
                                    m_logger->WriteLog( "Run Child Error , exit the group" );
                                    OutputDebugString( "Run Child Error , exit the group" );
                                    goto lbexit ;
                                }

                                // !!! if no SeekSetParam , then enum each record , otherwise , just enter one child panel ...
                                if ( sSeekSetParam.Length() )
                                    break ;
                                else {
                                    rsPanelData->Next() ;
                                }
                            } // while

                            // try scrolldown
                            if ( bScrollNextPage ) {
                                ExecResult = DoStepDebug( sProg.c_str() , sPanel.c_str() , "Send ScrollDown Key Prompt" , sScrollDown , ExecResult , false )  ;
                                m_logger->WriteLog( "Scroll Pagedown Key [%s] " , sScrollDown.c_str() ) ;
                                OutputDebugString( AnsiString().sprintf("Scroll Pagedown Key [%s] " , sScrollDown.c_str()).c_str() ) ;
                                if ( !SubmitKey( sScrollDown ) ) {//if ( m_LastStatus.Pos("X II") ) // bScrollNextPage = false ;
                                    m_logger->WriteLog( "Exception : Scroll down stop , no allow key for scroll down") ;
                                    OutputDebugString( "Exception : Scroll down stop , no allow key for scroll down") ;
                                    bScrollNextPage = false ;
                                }
                                else {
                                    n_PanelPages ++ ;
                                    GetData( sScreenText , "" , tsAttrib , m_WaitForProcessing ) ;
                                    if ( !CheckSignatureData( sScreenText , sScreenSignature ) ) {
                                        m_logger->WriteLog( "Scroll down stop : No Signature Data") ;
                                        OutputDebugString( "Scroll down stop : No Signature Data") ;
                                        bNoMoreData = true ;
                                        bScrollNextPage = false ;
                                    }
                                    else {
                                        rsPanelData = m_scr->BuildRecordSet(sSchemaFile , sPanel , rsPanelData ) ;
                                        m_scr->FillDataToRecordSet( sSchemaFile , sPanel , rsPanelData , sScreenText , true , m_ScreenTextFormat ) ;
                                        AnsiString sCurrentPanelText = m_scr->GetPanelText( sSchemaFile , sPanel ) ;
                                        if ( sPrevPanelText != sCurrentPanelText ) {
                                            if ( sFilter.Length() ) {
                                                int reccount = RecordsetFilter( rsPanelData , sFilter , ServiceParams ) ;
                                                if ( reccount == 0 ) bNoMoreData = true ;
                                            }
                                            if ( ! sRecordsetName.Length() ) sRecordsetName = sProg + "_" + sPanel ;
                                            if ( sPanelReturnData == "1" || sPanelReturnData == "M" ) {
                                                bool bAppendRecordset = m_DataSets->AppendRecordset( sRecordsetName , rsPanelData ) ;
                                            }
                                            TADODataSet * ds = m_DataSets->GetRecordsetByName(sRecordsetName) ;
                                            if ( ds && ds->RecordCount >= m_MaxRecordCount ) {
                                                m_logger->WriteLog( "Scroll down stop : too many record count") ;
                                                bScrollNextPage = false ;
                                            }
                                            if ( n_PanelPages >= m_MaxPanelPages ) {
                                                m_logger->WriteLog( "Scroll down stop : too many pages count") ;
                                                bScrollNextPage = false ;
                                            }
                                            if ( !sCurrentPanelText.Trim().Length() ) {
                                                m_logger->WriteLog( "Scroll down stop : empty panel text (no more data)") ;
                                                bNoMoreData = true ;
                                            }
                                            if ( NoMoreDataMsg.Length() && sScreenText.Pos( NoMoreDataMsg ) > 0 ) {
                                                m_logger->WriteLog( "Scroll down stop : (no more data message : %s)" , NoMoreDataMsg.c_str() ) ;
                                                bNoMoreData = true ;
                                            }
                                            if ( bNoMoreData )
                                                bScrollNextPage = false ;
                                        }
                                        else { // pagedown fail
                                            m_logger->WriteLog( "Scroll down stop : panel data no changed") ;
                                            bScrollNextPage = false ;
                                        }
                                        sPrevPanelText = sCurrentPanelText ;
                                    }
                                }
                            }
                        }
                        while ( bScrollNextPage ) ; // do
                    }
                    else
                        m_logger->WriteLog( "No Child") ;
                }

                // Execute Panel Exit
                ExecResult = DoStepDebug( sProg.c_str() , sPanel.c_str() , "Send PrevKey Prompt" , sPrevKey , ExecResult , false )  ;
                if ( ExecResult != srSuccessed ) {
                    m_logger->WriteLog( "Send PrevKey , exit the group" );
                    goto lbexit ;
                }
                m_logger->WriteLog( "Leave: %s\\%s-%s , PrevKey[%s]" , ServiceName , sProg , sPanel , sPrevKey.c_str() ) ;
                if ( sPrevKey.Length() ) {
                    SubmitKey( sPrevKey , false , "-PrevKey" ) ;
                }

                if ( bIsOptionalPanel && !ADOQryService->Eof ) {
                    m_logger->WriteLog( "Warring : Next Step was skipped by optional panel " ) ;
                    break ; // if optional screen match , then break same step ...
                }
                ADOQryService->Next();
            }
        }
        lbexit :
        g_level--;
        //ADOProgList->Close();
        //ADOQryService->Close();
        //ADOProgPanelList->Close();
        /// ToDo : 
        //delete ADOProgList ;
        //delete ADOQryService ;
        //delete ADOProgPanelList ;

        if ( !m_CacheMode ) delete rsPanelData ;
        delete ParentParams ;
        delete PanelParams ;
        // TODO : Check Memory 
        delete tsAttrib ;
        if ( ExecResult != srSuccessed || IsStop() )
            m_logger->WriteLog( "RunStep Error !" );

        return ExecResult ;
    }

    enum EScreenResult RunService( AnsiString ServiceName, AnsiString ScreenName , CParamInfo * ServiceParams )
    {
        g_level = 0 ;
        ReadConfigFile( ServiceName.c_str() );
        if ( IsAutoLogin(ServiceName) )
            ServiceParams->SetParamStr( GetLoginParamStr() ) ;
        m_logger->WriteLog( "Run Service Start : Command = %s " , ServiceName.c_str() );
        OutputDebugString( AnsiString().sprintf("Run Service Start : Command = %s " , ServiceName.c_str()).c_str() );
        EScreenResult rc = RunStep( ServiceName , "" , ServiceParams ) ;
        m_logger->WriteLog( "Run Service End : Command = %s " , ServiceName.c_str() );
        OutputDebugString( AnsiString().sprintf("Run Service End : Command = %s " , ServiceName.c_str()).c_str() );
        return rc ;
    }

    void SetLoginInfo( AnsiString userid , AnsiString password )
    {
        m_userid = userid;
        m_password = password ;
    }

    AnsiString GetLoginParamStr()
    {
        AnsiString ParamStr =  AnsiString("Userid=") + m_userid.c_str() + ";Password=" + m_password.c_str() + ";" ;
        return  ParamStr ;
    }

    bool IsMainMenuScreen( AnsiString sScreen )
    {
        bool bRc = false ;
        sScreen = AnsiReplaceStr( sScreen , "\n" , "" ).Trim() ;
        bRc = ( sScreen.Length() == 0 || sScreen.Pos( "ENTER NEXT TRANSACTION") > 0 ) ;
        return bRc ;
    }

    bool IsSignOnScreen( AnsiString sScreen )
    {
        bool bRc = false ;
        bRc = ( sScreen.Pos( "Enter Your Choice") > 0 ) ;
        return bRc ;
    }

    int CheckScreenByCursorPosition( )
    {
        int pos = -1 ;
        /*if ( WaitForReady( GetTickCount() , 30000 , "Initial Screen" ) != esIdle ) {
            m_logger->WriteLog( "Emulator Busy Timeout phase 1" ) ;
        }
        else {
            ExecMacroKey( "CLEAR" ) ;
            ExecMacroKey( "CLEAR" ) ;
            ExecMacroKey( "CLEAR" ) ;
            pos = GetCursorLocation();
        } */

        pos = GetCursorLocation();
        if ( pos != 1 ) { // pos 1 is mainmenu
            ExecMacroKey( "CLEAR" ) ;
            ExecMacroKey( "CLEAR" ) ;
            ExecMacroKey( "CLEAR" ) ;
        }
        pos = GetCursorLocation();
        return pos ;
    }

    int ExecService( AnsiString ServiceName , AnsiString ParamStr , AnsiString SessionName , bool bNewSerice , bool bDebugMode = false , bool *TermFlag = NULL )
    {
        EScreenResult rc = -1 ;

        if ( m_Running ) {
            Application->MessageBox(  "Service Already Running..." , "Error" ,MB_OK) ;
            return -1 ;
        }
        try {
            m_TermFlag = TermFlag ;
            DWORD m_PrevTick = GetTickCount() ;
            m_Running = true ;
            CServiceInterface * service = GetServiceInterface( ServiceName ) ;
            CServiceStatusInfo servinfo ;
            CParamInfo * ServiceParams = new CParamInfo( ParamStr ) ;
            AnsiString ScriptName = ServiceName ;
            ServiceBegin();
            if ( bNewSerice ) {
                m_logger->StartService( ServiceName , m_SessionID ) ;
                m_DataSets->ClearRecordset();
                m_logger->WriteLog( "Service Start : Command = %s , Params = %s " , ServiceName.c_str() , ParamStr.c_str() );
                OutputDebugString( AnsiString().sprintf("Service Start : Command = %s , Params = %s " , ServiceName.c_str() , ParamStr.c_str()).c_str() );
            }

            AnsiString sScreen ;
            if ( m_Connected || OpenLink( SessionName ) ) {
                OutputDebugString("OpenLink successed !" ) ;
                m_DebugMode = bDebugMode ;
                g_level = 0 ;
                bool bIsMainmenu = false ;

                int pos = CheckScreenByCursorPosition();
                switch ( pos ) {
                    case 1 : // mainmenu
                        bIsMainmenu = true ;
                        break ;
                    case 2 : // welcome blank
                        ExecMacroKey( "CLEAR" ) ;
                        break ;
                    case 746 : // password
                        break ;
                    case 1768 : // welcome
                        break ;
                }

                sScreen = GetScreenText( "Check Initial Screen" ) ;
                //bIsMainmenu = IsMainMenuScreen( sScreen ) ;
                bool bOk = true ;
                if ( !IsAutoLogin(ServiceName) && !bIsMainmenu ) {
                    bOk = ExecMacroKey( "CLEAR" ) ;
                    if ( bOk )
                        bOk = ExecMacroKey( "CLEAR" ) ;
                    else  {
                        if ( WaitForReady( GetTickCount() , 30000 , "Initial Screen" ) != esIdle ) {
                            m_logger->WriteLog( "Emulator Busy Timeout phase 2" ) ;
                            OutputDebugString("Emulator Busy Timeout phase 2" ) ;
                            bOk = false ;
                        }
                        else
                            bOk = true ;
                    }
                    sScreen = GetScreenText( "Check Initial Screen" ) ;
                    bIsMainmenu = IsMainMenuScreen( sScreen ) ;
                }

                m_logger->WriteLog( "Screen Len %d" , sScreen.Length() ) ;
                if ( !bOk ) {
                    m_logger->WriteLog( "Exit Service Emulator Busy !" ) ;
                    OutputDebugString( "Exit Service Emulator Busy !" ) ;
                }
                else {
                    if ( (!IsAutoLogin(ServiceName) && !bIsMainmenu ) || (rc = RunService( ServiceName , "" , ServiceParams )) == srInitScreenError ) {
                        m_logger->WriteLog( "srInitScreenError" );
                        GetData( sScreen , "" , NULL , m_WaitForProcessing ) ;
                        if ( ServiceName != "LOGOUT" ) {
                            if ( sScreen.Pos("COMMAND UNRECOGNIZED") || sScreen.Pos("Enter Your Choice") || sScreen.Pos("Type your userid and password"))  {
                                CParamInfo * loginparam = new CParamInfo( ParamStr ) ;
                                rc = RunService( "sign-on" , "" , loginparam ) ;
                                delete loginparam;
                            }
                            else {
                                rc = RunService( "MainMenu" , "" , ServiceParams ) ;
                            }
                        }
                        if ( rc == 0 ) {
                            rc = RunService( ServiceName , "" , ServiceParams ) ;
                        }
                    }
                }

                m_logger->WriteLog( "Service Execute %s in %d msecs , ResultCode : %d , Recordset Count %d " ,
                    (int(rc)==0) ? "Ok" : "Fail" , GetTickCount() - m_PrevTick , int(rc) , m_DataSets->m_RecordsetList->Count );
                OutputDebugString( AnsiString().sprintf("Service Execute %s in %d msecs , ResultCode : %d , Recordset Count %d " , (int(rc)==0) ? "Ok" : "Fail" , GetTickCount() - m_PrevTick , int(rc) , m_DataSets->m_RecordsetList->Count).c_str() );
                m_logger->WriteLog( "Service End" );
                OutputDebugString( "Service End" );
                delete ServiceParams ;
                //if ( !m_Connected )
                    //CloseLink();
            }
            else {
                rc = srEmulatorError ;
                m_logger->WriteLog( "Emulator session : %s open error!" , SessionName.c_str() );
                OutputDebugString( AnsiString().sprintf("Emulator session : %s open error!" , SessionName.c_str()).c_str() );
            }
            ServiceEnd();
        }
        catch ( Exception &e  ) {
            m_logger->WriteLog( "Service Exception [%s]" ,e.Message.c_str() );
            OutputDebugString( AnsiString().sprintf("Service Exception [%s]" ,e.Message.c_str()).c_str() );
            CloseLink();
        }
        catch ( ... ) {
            m_logger->WriteLog( "Service Unknown Exception !" );
            OutputDebugString( "Service Unknown Exception !" );
            CloseLink();
        }
        m_Running = false ;
        
        rc = m_ReturnByRecord == 1 ? (( m_DataSets->m_RecordsetList->Count > 0 ) ? 0 : rc) : rc ;

        return rc ;
    }
} ;
#endif


