//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include "UMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMain *Main;
//---------------------------------------------------------------------------
__fastcall TMain::TMain(TComponent* Owner)
  : TForm(Owner)
{
  FormatSettings.DecimalSeparator='.';
  FormatSettings.ThousandSeparator=' ';
  FormatSettings.ShortDateFormat="dd.mm.yyyy";
  FormatSettings.LongDateFormat="dd.mm.yyyy";
  FormatSettings.ShortTimeFormat="hh:nn:ss";
  FormatSettings.LongTimeFormat="hh:nn:ss";
  FormatSettings.TimeSeparator=':';
  FormatSettings.DateSeparator='.';
  //
  appPath=ExtractFilePath(ExpandFileName(ParamStr(0)));
  dataPath=appPath+"\Data";
  resourcePath=appPath+"\Resources";
  //
  Color=clBlack;
  //
  Width=_RESX;
  Height=_RESY;
  //
  BorderStyle=bsNone;
  //
  gm=new SGame(Handle,Width,Height,32);
  bmp=new Graphics::TBitmap();
  AnsiString bckPath=resourcePath+"\\Background.bmp";
  //
  bmp->Width=Width;
  bmp->Height=Height;
  try
  {
    bmp->LoadFromFile(bckPath);
    bmp->PixelFormat=pf32bit;
  }
  catch(Exception &s)
  {
  }
  AnsiString message=gm->GetErrorMessage();
  if(message.Trim()=="")
    tmrUpdate->Enabled=true;
  else
    ShowMessage(message);
}
//---------------------------------------------------------------------------
__fastcall TMain::~TMain()
{
  delete gm;
  if(bmp)
    delete bmp;
}
//---------------------------------------------------------------------------
void __fastcall TMain::RaiseTickMessage(TMessage aMessage)
{
  gm->Draw(bmp,0,0);
  gm->Flip();
}
//---------------------------------------------------------------------------
void __fastcall TMain::tmrUpdateTimer(TObject *Sender)
{
  ::PostMessage(Handle,TICK_MSG,0,0);
}
//---------------------------------------------------------------------------
void __fastcall TMain::RaiseKeyMessage(TMessage aMessage)
{
  keyParam=(AnsiString)(char)aMessage.WParamLo;
  //
  if(aMessage.Msg==WM_KEYDOWN)
  {
    if(!tmrMove->Enabled)
      tmrMove->Enabled=true;
  }
  if(aMessage.Msg==WM_KEYUP)
  {
    if(tmrMove->Enabled)
      tmrMove->Enabled=false;
  }
}
//---------------------------------------------------------------------------
void __fastcall TMain::tmrMoveTimer(TObject *Sender)
{
  if(keyParam.UpperCase()=="W" || keyParam.UpperCase()=="S" ||
     keyParam.UpperCase()=="A" || keyParam.UpperCase()=="D")
    gm->RaiseKeyEvent(keyParam);
}
//---------------------------------------------------------------------------
