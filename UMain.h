//---------------------------------------------------------------------------
#ifndef UMainH
#define UMainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
#include "UGame.h"
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
#define TICK_MSG (WM_USER+1024)
//#define WM_KEYDOWN 0x0100
//---------------------------------------------------------------------------
#define _RESX 800;
#define _RESY 600;
//---------------------------------------------------------------------------
class TMain : public TForm
{
__published:// IDE-managed Components
  TTimer *tmrUpdate;
  TTimer *tmrMove;
  void __fastcall tmrUpdateTimer(TObject *Sender);
  void __fastcall tmrMoveTimer(TObject *Sender);
  void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
  void __fastcall FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
private:// User declarations
  AnsiString lastPressedKey;
  TStringList *keyParams;
  SGame *gm;
public:// User declarations
  AnsiString appPath;
  AnsiString dataPath;
  AnsiString resourcePath;
  Graphics::TBitmap *bmp;
  TCriticalSection *crKeyInput;
  TPoint mousePos;
  //
  void __fastcall RaiseTickMessage(TMessage aMessage);
  void __fastcall RaiseKeyMessage(TMessage aMessage);
  //
  __fastcall TMain(TComponent* Owner);
  __fastcall ~TMain();
  //
  BEGIN_MESSAGE_MAP
    MESSAGE_HANDLER(TICK_MSG, TMessage, RaiseTickMessage)
    //MESSAGE_HANDLER(WM_KEYDOWN, TMessage, RaiseKeyMessage)
    //MESSAGE_HANDLER(WM_KEYUP, TMessage, RaiseKeyMessage)
  END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TMain *Main;
//---------------------------------------------------------------------------
#endif
