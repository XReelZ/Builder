object Main: TMain
  Left = 0
  Top = 0
  Caption = 'Builder'
  ClientHeight = 288
  ClientWidth = 682
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object tmrUpdate: TTimer
    Enabled = False
    Interval = 10
    OnTimer = tmrUpdateTimer
    Left = 108
    Top = 64
  end
  object tmrMove: TTimer
    Enabled = False
    Interval = 1
    OnTimer = tmrMoveTimer
    Left = 256
    Top = 60
  end
end
