object dmgwUMS: TdmgwUMS
  OldCreateOrder = False
  OnCreate = DataModuleCreate
  Left = 409
  Top = 457
  Height = 167
  Width = 204
  object EMC: EMiddlewareController
    MonitorEnabled = True
    FixRes = 10
    QueueSize = 10
    ErrorDetectEnabled = True
    ErrorDetectUnitTicks = 600000
    ErrorDetectUnitHits = 10
    OnOpen = EMCOpen
    OnClose = EMCClose
    OnGetMessage = EMCGetMessage
    OnBeginEnumMsgInfo = EMCBeginEnumMsgInfo
    OnEndEnumMsgInfo = EMCEndEnumMsgInfo
    OnEnumMsgInfo = EMCEnumMsgInfo
    Left = 16
    Top = 8
  end
  object tmReLoadParams: TTimer
    Enabled = False
    Interval = 60000
    Left = 16
    Top = 64
  end
  object ADOConn: TADOConnection
    LoginPrompt = False
    Left = 112
    Top = 8
  end
  object ADOQuery: TADOQuery
    Connection = ADOConn
    Parameters = <>
    Left = 112
    Top = 64
  end
end
