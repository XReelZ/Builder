//---------------------------------------------------------------------------
#ifndef UGameH
#define UGameH
//---------------------------------------------------------------------------
#include <ddraw.h>
#include <vcl.h>
//---------------------------------------------------------------------------
#include "UEngine.h"
//---------------------------------------------------------------------------
class SGame
{
  private:
    HWND w_hwnd;
    int screenWidth;
    int screenHeight;
    int colorBits;
    AnsiString errorMessage;
    //
    SEngine *eng;
    IDirectDraw7 *pDD;
    IDirectDrawSurface7 *pDDSFront;
    IDirectDrawSurface7 *pDDSBack;
    //
    IDirectDrawSurface7 *__fastcall CreateOffScreenSurface(IDirectDraw *pdd, int dx, int dy);
  protected:
  public:
    void __fastcall RaiseKeyEvent(AnsiString aKey);
    AnsiString __fastcall GetErrorMessage();
    bool __fastcall Draw(Graphics::TBitmap *aBitmap, int aX, int aY);
    bool __fastcall Flip();
    //
    __fastcall SGame(HWND aWindowHWDN, int aWidth, int aHeight, int aColorBits);
    __fastcall ~SGame();
};
//---------------------------------------------------------------------------
#endif
