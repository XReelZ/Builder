//---------------------------------------------------------------------------
#ifndef UD3DGraphicsH
#define UD3DGraphicsH
//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
#include <d3d9.h>
//---------------------------------------------------------------------------
class D3DGraphics
{
  public:
    void PutPixel( int x,int y,int r,int g,int b );
    void BeginFrame();
    void EndFrame();
    void GetFrameRect(RECT &aRect);
    //
    D3DGraphics(HWND hWnd);
    ~D3DGraphics();
  private:
    IDirect3D9* pDirect3D;
    IDirect3DDevice9* pDevice;
};
//---------------------------------------------------------------------------
#endif
