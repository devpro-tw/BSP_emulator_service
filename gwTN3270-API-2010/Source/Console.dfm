object frmResourceConsole: TfrmResourceConsole
  Left = 697
  Top = 266
  Width = 638
  Height = 418
  Caption = 'Emulator Gateway'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 0
    Top = 176
    Width = 630
    Height = 3
    Cursor = crVSplit
    Align = alBottom
  end
  object lvResourceList: TListView
    Left = 0
    Top = 33
    Width = 630
    Height = 143
    Align = alClient
    Columns = <
      item
        Caption = #36039#28304#21517#31281
        Width = 100
      end
      item
        Caption = #22519#34892#29376#24907
        Width = 300
      end
      item
        AutoSize = True
        Caption = #20633#35387#35498#26126
      end>
    GridLines = True
    MultiSelect = True
    ReadOnly = True
    RowSelect = True
    TabOrder = 0
    ViewStyle = vsReport
  end
  object mlMessage: TMemo
    Left = 0
    Top = 179
    Width = 630
    Height = 171
    Align = alBottom
    Color = clBtnFace
    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
    ReadOnly = True
    ScrollBars = ssBoth
    TabOrder = 1
    WordWrap = False
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 350
    Width = 630
    Height = 22
    Panels = <>
    SimplePanel = False
  end
  object ControlBar1: TControlBar
    Left = 0
    Top = 0
    Width = 630
    Height = 33
    Align = alTop
    TabOrder = 3
    object btnRestart: TBitBtn
      Left = 11
      Top = 2
      Width = 75
      Height = 22
      Caption = #37325#26032#21855#21205
      TabOrder = 0
      OnClick = btnRestartClick
    end
  end
  object MainMenu1: TMainMenu
    Left = 8
    Top = 64
    object System1: TMenuItem
      Caption = #31995#32113
      object mnuRunning: TMenuItem
        Caption = #26283#20572#26381#21209
        OnClick = mnuRunningClick
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Shutdown1: TMenuItem
        Caption = #38364#38281
        OnClick = Shutdown1Click
      end
    end
    object N4: TMenuItem
      Caption = #28204#35430
    end
    object N2: TMenuItem
      Caption = #35373#23450
      Visible = False
      object N3: TMenuItem
        Caption = #31995#32113#21443#25976#35373#23450
      end
    end
    object Message1: TMenuItem
      Caption = 'Message'
      Visible = False
      object Enum1: TMenuItem
        Caption = 'Enum'
        OnClick = Enum1Click
      end
    end
  end
  object ImageList1: TImageList
    Left = 24
    Top = 232
  end
  object ActionManager1: TActionManager
    Images = ImageList1
    Left = 64
    Top = 232
  end
end
