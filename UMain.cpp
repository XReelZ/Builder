//---------------------------------------------------------------------------
#pragma hdrstop
//---------------------------------------------------------------------------
#include "UMain.h"
//---------------------------------------------------------------------------
#include <memory>
#include <vcl.h>
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
  Cursor=crDefault;
  Graphics::TBitmap *cursor=new Graphics::TBitmap();
  try
  {
    cursor->LoadFromFile(resourcePath+"\\Cross.bmp");
  }
  catch(Exception &e)
  {
  }
//  const int crMyCursor=5;
//  Graphics::TBitmap *bmpMask=new Graphics::TBitmap();
//  Graphics::TBitmap *bmpColor=new Graphics::TBitmap();
//  TIconInfo *iconInfo=new TIconInfo();
//  try
//  {
//    bmpMask->LoadFromFile(resourcePath+"\\SquareMask.bmp");
//    bmpColor->LoadFromFile(resourcePath+"\\Square.bmp");
//  }
//  catch(Exception &e)
//  {
//  }
//  bmpMask->Transparent=true;
//  bmpColor->Transparent=true;
//  //
//  iconInfo->fIcon = false;
//  iconInfo->xHotspot = 20;
//  iconInfo->yHotspot = 20;
//  iconInfo->hbmMask = bmpMask->Handle;
//  iconInfo->hbmColor = bmpColor->Handle;
//  //
//  Screen->Cursors[crMyCursor]=CreateIconIndirect(iconInfo);
//  //
//  Screen->Cursor = crMyCursor;
//  delete iconInfo;
//  delete bmpColor;
  delete cursor;
  //
  ::GetCursorPos(&mousePos);
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
  ::GetCursorPos(&mousePos);
  //
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
  return;
  //
  AnsiString key=(AnsiString)(char)aMessage.WParamLo;
  key=key.UpperCase();
  //
  crKeyInput->Enter();
  //
  int index=keyParams->IndexOf(key);
  //
  if(aMessage.Msg==WM_KEYDOWN)
  {
    if(index<0)
      keyParams->Add(key);
  }
  if(aMessage.Msg==WM_KEYUP)
  {
    if(index>=0)
      keyParams->Delete(keyParams->IndexOf(key));
  }
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
void __fastcall TMain::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
  AnsiString key=(AnsiString)(char)Key;
  key=key.UpperCase();
  //
  if(key=="S")
    int a=0;
  crKeyInput->Enter();
  //
  int index=keyParams->IndexOf(key);
  //
  if(index<0)
    keyParams->Add(key);
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
void __fastcall TMain::FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift)
{
  AnsiString key=(AnsiString)(char)Key;
  key=key.UpperCase();
  //
  crKeyInput->Enter();
  //
  int index=keyParams->IndexOf(key);
  //
  if(index>=0)
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
