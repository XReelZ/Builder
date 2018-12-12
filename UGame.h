//---------------------------------------------------------------------------
#ifndef UGameH
#define UGameH
//---------------------------------------------------------------------------
#include <vcl.h>
#include <mmsystem.h>
#include <dsound.h>
#include <ddraw.h>
//---------------------------------------------------------------------------
#include "UEngine.h"
//---------------------------------------------------------------------------
#define DIRECTSOUND_VERSION 0x0900
//---------------------------------------------------------------------------
class SGame
{
  private:
    HWND w_hwnd;
    int screenWidth;
    int screenHeight;
    int colorBits;
    int soundSteps;
    AnsiString errorMessage;
    //
    SEngine *eng;
    IDirectDraw7 *pDD;
    IDirectDrawSurface7 *pDDSFront;
    IDirectDrawSurface7 *pDDSBack;
    //
    IDirectSound8* dsound;
    IDirectSoundBuffer* primaryBuffer;
    //
    TList *secondaryBufferList;
    //
    IDirectDrawSurface7 *__fastcall CreateOffScreenSurface(IDirectDraw *pdd, int dx, int dy);
  protected:
    LPDIRECTSOUNDBUFFER8 __fastcall GetSoundBuffer(int aSoundID);
  public:
    int __fastcall LoadSoundFile(AnsiString aFilename, __int64 &aBufferSize);
    bool __fastcall PlaySingleSound(int aSoundID, int aPosition);
    bool __fastcall SetSoundVolume(int aSoundID, int aVolume);
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
