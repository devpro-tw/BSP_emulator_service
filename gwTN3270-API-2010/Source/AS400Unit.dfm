object frmAS400: TfrmAS400
  Left = 177
  Top = 121
  Width = 316
  Height = 204
  Caption = 'TN5250 Gateway'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 308
    Height = 177
    ActivePage = TabSheet2
    Align = alClient
    TabIndex = 1
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = 'AS400 Session'
      object GroupBox1: TGroupBox
        Left = 0
        Top = 0
        Width = 297
        Height = 157
        Caption = 'Services'
        TabOrder = 0
        object Label1: TLabel
          Left = 8
          Top = 16
          Width = 70
          Height = 13
          Caption = 'Active Session'
        end
        object Label6: TLabel
          Left = 8
          Top = 104
          Width = 43
          Height = 13
          Caption = 'SendKey'
        end
        object Label7: TLabel
          Left = 8
          Top = 128
          Width = 30
          Height = 13
          Caption = 'Status'
        end
        object btnRefresh: TButton
          Left = 168
          Top = 56
          Width = 105
          Height = 25
          Caption = 'Session Refresh'
          TabOrder = 0
          OnClick = btnRefreshClick
        end
        object lbSession: TListBox
          Left = 8
          Top = 32
          Width = 153
          Height = 65
          ItemHeight = 13
          TabOrder = 1
          OnClick = lbSessionClick
        end
        object lbleTopic: TLabeledEdit
          Left = 168
          Top = 28
          Width = 105
          Height = 21
          EditLabel.Width = 27
          EditLabel.Height = 13
          EditLabel.Caption = 'Topic'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 2
          OnChange = lbleTopicChange
        end
        object edKey: TEdit
          Left = 72
          Top = 104
          Width = 129
          Height = 21
          TabOrder = 3
        end
        object edStatus: TEdit
          Left = 72
          Top = 128
          Width = 129
          Height = 21
          TabOrder = 4
        end
        object BitBtn1: TBitBtn
          Left = 208
          Top = 104
          Width = 75
          Height = 25
          Caption = 'SendKey'
          TabOrder = 5
        end
        object Button2: TButton
          Left = 208
          Top = 128
          Width = 75
          Height = 25
          Caption = 'GetStatus'
          TabOrder = 6
          OnClick = Button2Click
        end
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'Service'
      ImageIndex = 1
      object GroupBox2: TGroupBox
        Left = 0
        Top = 4
        Width = 302
        Height = 149
        Caption = 'Functions'
        TabOrder = 0
        object Label4: TLabel
          Left = 8
          Top = 12
          Width = 27
          Height = 13
          Caption = 'Script'
        end
        object Label5: TLabel
          Left = 8
          Top = 80
          Width = 31
          Height = 13
          Caption = 'Target'
        end
        object lbResult: TLabel
          Left = 160
          Top = 131
          Width = 30
          Height = 13
          Caption = 'Result'
        end
        object lbleParms: TLabeledEdit
          Left = 8
          Top = 48
          Width = 273
          Height = 21
          EditLabel.Width = 53
          EditLabel.Height = 13
          EditLabel.Caption = 'Parameters'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 1
          Text = 'ORGANIZATION=001;CUSTOMER=C220343074;ACCOUNT=5184340005891004'
        end
        object edScript: TEdit
          Left = 72
          Top = 16
          Width = 209
          Height = 21
          TabOrder = 0
          Text = 'PCHI'
        end
        object btnRunNew: TButton
          Left = 160
          Top = 104
          Width = 89
          Height = 25
          Caption = 'Local Test'
          TabOrder = 5
          OnClick = btnRunNewClick
        end
        object edTarget: TEdit
          Left = 48
          Top = 80
          Width = 97
          Height = 21
          TabOrder = 2
          Text = 'AS400.0'
        end
        object Button1: TButton
          Left = 48
          Top = 104
          Width = 97
          Height = 25
          Caption = 'Remote Test'
          TabOrder = 3
          OnClick = Button1Click
        end
        object cbStepDebug: TCheckBox
          Left = 160
          Top = 80
          Width = 89
          Height = 17
          Caption = 'Step Debug'
          TabOrder = 4
        end
        object CSpinEdit1: TCSpinEdit
          Left = 256
          Top = 104
          Width = 41
          Height = 22
          TabOrder = 6
          Value = 1
        end
      end
      object cbResponseFromFile: TCheckBox
        Left = 176
        Top = 32
        Width = 121
        Height = 17
        Caption = 'Response From File'
        TabOrder = 1
        Visible = False
      end
    end
    object TabSheet3: TTabSheet
      Caption = 'Recordset'
      ImageIndex = 2
      object DBGrid1: TDBGrid
        Left = 0
        Top = 25
        Width = 300
        Height = 124
        Align = alClient
        DataSource = DataSource1
        TabOrder = 0
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = clWindowText
        TitleFont.Height = -11
        TitleFont.Name = 'MS Sans Serif'
        TitleFont.Style = []
      end
      object Panel1: TPanel
        Left = 0
        Top = 0
        Width = 300
        Height = 25
        Align = alTop
        TabOrder = 1
        object ComboBox1: TComboBox
          Left = 0
          Top = 2
          Width = 302
          Height = 21
          ItemHeight = 0
          TabOrder = 0
          Text = 'ResultRecordset'
          OnChange = ComboBox1Change
        end
      end
    end
    object TabSheet4: TTabSheet
      Caption = 'Gateway'
      ImageIndex = 3
      object GroupBox3: TGroupBox
        Left = 0
        Top = 0
        Width = 297
        Height = 41
        Caption = 'Message Gateway'
        TabOrder = 0
        object Label2: TLabel
          Left = 8
          Top = 16
          Width = 26
          Height = 13
          Caption = 'Entity'
        end
        object edEntity: TEdit
          Left = 48
          Top = 16
          Width = 241
          Height = 21
          TabOrder = 0
          Text = 'AS400.UAT.1'
          OnChange = edEntityChange
        end
      end
      object btnOpen: TButton
        Left = 216
        Top = 88
        Width = 83
        Height = 25
        Caption = 'Open'
        TabOrder = 1
        OnClick = btnOpenClick
      end
    end
    object TabSheet5: TTabSheet
      Caption = 'AOSQL'
      ImageIndex = 4
      object mlScreen: TMemo
        Left = 0
        Top = 0
        Width = 291
        Height = 145
        ScrollBars = ssBoth
        TabOrder = 0
        WordWrap = False
      end
      object btnRun: TButton
        Left = 240
        Top = 96
        Width = 59
        Height = 21
        Caption = 'Run'
        TabOrder = 1
        OnClick = btnRunClick
      end
      object DBGrid2: TDBGrid
        Left = 304
        Top = 8
        Width = 273
        Height = 233
        DataSource = DataSource2
        TabOrder = 2
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = clWindowText
        TitleFont.Height = -11
        TitleFont.Name = 'MS Sans Serif'
        TitleFont.Style = []
      end
    end
  end
  object DataSource1: TDataSource
    Left = 272
    Top = 144
  end
  object esmc: EMiddlewareController
    EngsoundGatewayEnabled = True
    MonitorEnabled = True
    FixRes = 10
    QueueSize = 10
    ErrorDetectEnabled = True
    ErrorDetectUnitTicks = 600000
    ErrorDetectUnitHits = 10
    ApplicationName = 'TN5250'
    OnOpen = esmcOpen
    OnGetRequest = esmcGetRequest
    OnGetResponse = esmcGetResponse
    Left = 240
    Top = 144
  end
  object ADOConnection1: TADOConnection
    ConnectionString = 
      'Provider=Microsoft.Jet.OLEDB.4.0;Data Source=ScreenLayoutInfo.md' +
      'b'
    LoginPrompt = False
    Mode = cmShareDenyNone
    Provider = 'Microsoft.Jet.OLEDB.4.0'
    Left = 208
    Top = 144
  end
  object DataSource2: TDataSource
    Left = 320
    Top = 64
  end
end
