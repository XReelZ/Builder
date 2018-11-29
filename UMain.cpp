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
  crKeyInput=new TCriticalSection();
  keyParams=new TStringList();
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
  delete crKeyInput;
  delete keyParams;
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
  crKeyInput->Enter();
  //
  AnsiString key=(AnsiString)(char)aMessage.WParamLo;
  key=key.UpperCase();
  //
  if(aMessage.Msg==WM_KEYDOWN)
  {
    int index=keyParams->IndexOf(key);
    if(index<0)
      keyParams->Add(key);
  }
  if(aMessage.Msg==WM_KEYUP)
    keyParams->Delete(keyParams->IndexOf(key));
  //
  if(keyParams->Count>0)
  {
    if(!tmrMove->Enabled)
      tmrMove->Enabled=true;
  }
  else
    tmrMove->Enabled=false;
  //
  crKeyInput->Leave();
}
//---------------------------------------------------------------------------
void __fastcall TMain::tmrMoveTimer(TObject *Sender)
{
  gm->RaiseKeyEvent(keyParams->Text);
}
//---------------------------------------------------------------------------
