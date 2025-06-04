object Form1: TForm1
  Left = 137
  Top = 247
  Width = 490
  Height = 224
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Button1: TButton
    Left = 104
    Top = 40
    Width = 145
    Height = 25
    Caption = 'Button1'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 104
    Top = 80
    Width = 145
    Height = 25
    Caption = 'Button2'
    TabOrder = 1
    OnClick = Button2Click
  end
  object CheckBox1: TCheckBox
    Left = 24
    Top = 16
    Width = 97
    Height = 17
    Caption = 'CheckBox1'
    TabOrder = 2
    OnClick = CheckBox1Click
  end
  object Timer1: TTimer
    OnTimer = Timer1Timer
    Left = 24
    Top = 48
  end
end
