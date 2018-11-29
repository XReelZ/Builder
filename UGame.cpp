//---------------------------------------------------------------------------
#pragma hdrstop
//---------------------------------------------------------------------------
#include "UGame.h"
#include "UMain.h"
//---------------------------------------------------------------------------
//class SGame
//---------------------------------------------------------------------------
__fastcall SGame::SGame(HWND aWindowHWDN, int aWidth, int aHeight, int aColorBits)
{
  w_hwnd=aWindowHWDN;
  screenWidth=aWidth;
  screenHeight=aHeight;
  colorBits=aColorBits;
  //
  pDD=NULL;
  pDDSFront=NULL;
  pDDSBack=NULL;
  eng=new SEngine();
  //
  HRESULT hr=DirectDrawCreateEx(NULL, (void**)&pDD, IID_IDirectDraw7,NULL);
  if(hr!=DD_OK)
    errorMessage="Failed to create DirectDraw object.";
  //
  hr=pDD->SetCooperativeLevel(w_hwnd, DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE);
  if(hr!=DD_OK)
    errorMessage="Failed to initialize full screen.";
  hr=pDD->SetDisplayMode(screenWidth,screenHeight,colorBits,0,0);
  if(hr!=DD_OK)
    errorMessage="Unsupported screen resolution mode.";
  //
  DDSURFACEDESC2 ddsd;
  ZeroMemory(&ddsd, sizeof(ddsd));
  ddsd.dwSize = sizeof(ddsd);
  ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
  ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
  ddsd.dwBackBufferCount = 1;
  // Create the primary surface.
  hr = pDD->CreateSurface(&ddsd, &pDDSFront, NULL);
  if (hr!=DD_OK)
    errorMessage="Failed to create primary surface.";
  // Get a pointer to the back buffer.
  DDSCAPS2 ddscaps;
  ZeroMemory(&ddscaps, sizeof(ddscaps));
  ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
  hr=pDDSFront->GetAttachedSurface(&ddscaps, &pDDSBack);
  if(hr!=DD_OK)
    errorMessage="Failed to create secondary surface.";
}
//---------------------------------------------------------------------------
__fastcall SGame::~SGame()
{
  if(eng)
    delete eng;
  if(pDDSBack)
    pDDSBack->Release();
  if(pDDSFront)
    pDDSFront->Release();
  if(pDD)
  {
    pDD->RestoreDisplayMode();
    pDD->Release();
  }
}
//---------------------------------------------------------------------------
bool __fastcall SGame::Draw(Graphics::TBitmap *aBitmap, int aX, int aY)
{
  bool res=false;
  if(!aBitmap)
    return res;
  //
  Graphics::TBitmap *bmp=new Graphics::TBitmap();
  //
  bmp->Width=aBitmap->Width;
  bmp->Height=aBitmap->Height;
  bmp->Canvas->Draw(0,0,aBitmap);
  //
  if(eng)
    eng->DrawNPCs(bmp);
  //
  pDDSBack->Restore();
  //
  HDC hdcImage=CreateCompatibleDC(NULL);
  if(!hdcImage)
    return res;
  //
  HBITMAP hbm=bmp->Handle;
  SelectObject(hdcImage,hbm);
  //
  // Get size of the bitmap
  BITMAP bm;
  GetObject(hbm,sizeof(bm),&bm);
  int nWidth=bm.bmWidth;
  int nHeight=bm.bmHeight;
  //
  // Get size of surface.
  DDSURFACEDESC2 ddsd;
  ddsd.dwSize=sizeof(ddsd);
  ddsd.dwFlags=DDSD_HEIGHT | DDSD_WIDTH;
  pDDSBack->GetSurfaceDesc(&ddsd);
  //
  HDC hdc;
  HRESULT hr=pDDSBack->GetDC(&hdc);
  if(hr==DD_OK)
  {
    res=BitBlt(hdc,aX,aY,ddsd.dwWidth,ddsd.dwHeight,hdcImage,0,0,SRCCOPY);
    //
    pDDSBack->ReleaseDC(hdc);
  }
  DeleteDC(hdcImage);
  delete bmp;
  //
  return res;
}
//---------------------------------------------------------------------------
bool __fastcall SGame::Flip()
{
  HRESULT ddrval=pDDSFront->Flip(NULL,DDFLIP_WAIT);
  return(ddrval==DD_OK);
}
//---------------------------------------------------------------------------
AnsiString __fastcall SGame::GetErrorMessage()
{
  return errorMessage;
}
//---------------------------------------------------------------------------
void __fastcall SGame::RaiseKeyEvent(AnsiString aKey)
{
  TPoint tp;
  tp.x=0;
  tp.y=0;
  //
  if(aKey.UpperCase().Pos("W")>0)
    tp.y-=1;
  if(aKey.UpperCase().Pos("S")>0)
    tp.y+=1;
  if(aKey.UpperCase().Pos("A")>0)
    tp.x-=1;
  if(aKey.UpperCase().Pos("D")>0)
    tp.x+=1;
  //
  if(eng)
    eng->MovePlayer(tp);
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
