object dlgStepDebug: TdlgStepDebug
  Left = 386
  Top = 208
  Width = 310
  Height = 146
  Caption = 'Step Debug'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 16
    Top = 24
    Width = 46
    Height = 13
    Caption = 'Send Key'
  end
  object AutoAccept: TCheckBox
    Left = 72
    Top = 56
    Width = 97
    Height = 17
    Caption = 'Auto Accept'
    Enabled = False
    TabOrder = 0
    OnClick = AutoAcceptClick
  end
  object BitBtn1: TBitBtn
    Left = 48
    Top = 80
    Width = 75
    Height = 25
    TabOrder = 1
    Kind = bkYes
  end
  object BitBtn2: TBitBtn
    Left = 216
    Top = 80
    Width = 75
    Height = 25
    TabOrder = 2
    Kind = bkCancel
  end
  object edText: TMaskEdit
    Left = 72
    Top = 24
    Width = 217
    Height = 21
    TabOrder = 3
  end
  object BitBtn3: TBitBtn
    Left = 128
    Top = 80
    Width = 81
    Height = 25
    TabOrder = 4
    Kind = bkNo
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 2000
    Left = 16
    Top = 56
  end
end
