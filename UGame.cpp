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
  //
  dsound=NULL;
  primaryBuffer=NULL;
  //
  CoInitialize(NULL);
  //
  hr=DirectSoundCreate8(NULL,&dsound,NULL);
  if (hr!=DS_OK)
    errorMessage="Failed to create DirectSound object.";
  //
  hr=dsound->SetCooperativeLevel(GetDesktopWindow(),DSSCL_PRIORITY);
  if (hr!=DS_OK)
    errorMessage="Failed to initialize sound mode.";
  //
  DSBUFFERDESC bufferDesc;
  bufferDesc.dwSize = sizeof(DSBUFFERDESC);
  bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
  bufferDesc.dwBufferBytes = 0;
  bufferDesc.dwReserved = 0;
  bufferDesc.lpwfxFormat = NULL;
  bufferDesc.guid3DAlgorithm = GUID_NULL;
  //
  hr=dsound->CreateSoundBuffer(&bufferDesc,&primaryBuffer,NULL );
  if (hr!=DS_OK)
    errorMessage="Failed to create sound buffer.";
  //
  WAVEFORMATEX waveFormat;
  memset(&waveFormat, 0, sizeof(WAVEFORMATEX));
  waveFormat.wFormatTag = WAVE_FORMAT_PCM;
  waveFormat.nChannels = 2;
  waveFormat.nSamplesPerSec = 44100;
  waveFormat.wBitsPerSample = 16;
  waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
  waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
  waveFormat.cbSize = 0;
  //
  hr=primaryBuffer->SetFormat(&waveFormat);
  if (hr!=DS_OK)
    errorMessage="Failed to set primary buffer format.";
  //
  secondaryBufferList=new TList();
  //
  __int64 buffL=0;
  AnsiString fname=Main->resourcePath+"Footsteps.wav";
  soundSteps=LoadSoundFile(fname,buffL);
  SetSoundVolume(soundSteps,0);
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
  //
  for(int i=0;i<secondaryBufferList->Count;i++)
  {
    LPDIRECTSOUNDBUFFER8 secBuff=(LPDIRECTSOUNDBUFFER8)secondaryBufferList->Items[i];
    if (secBuff)
    {
      secBuff->Stop();
      secBuff->Release();
    }
  }
  if(primaryBuffer)
    primaryBuffer->Release();
  if(dsound)
    dsound->Release();
  //
  CoUninitialize();
}
//---------------------------------------------------------------------------
int __fastcall SGame::LoadSoundFile(AnsiString aFilename, __int64 &aBufferSize)
{
  int res=-1;
  aBufferSize=0;
  //
  if (errorMessage!="")
    return(res);
  //
  if (!FileExists(aFilename))
    return(res);
  //
  bool ok=false;
  TMemoryStream *stream=new TMemoryStream();
  try
  {
    stream->LoadFromFile(aFilename);
    //
    ok=true;
  }
  catch (Exception &e)
  {
  }
  //
  if (ok)
  {
    // WAV header 44 bytes
    const int wavHeader=44;
    int dataSize=stream->Size-wavHeader;
    //
    WAVEFORMATEX waveFormat;
    memset(&waveFormat, 0, sizeof(WAVEFORMATEX));
    waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    waveFormat.nChannels = 2;
    waveFormat.nSamplesPerSec = 44100;
    waveFormat.wBitsPerSample = 16;
    waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
    waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
    waveFormat.cbSize = 0;
    //
    DSBUFFERDESC bufferDesc;
    bufferDesc.dwSize = sizeof(DSBUFFERDESC);
    bufferDesc.dwFlags =  DSBCAPS_GETCURRENTPOSITION2
                          | DSBCAPS_GLOBALFOCUS
                          | DSBCAPS_CTRLVOLUME
                          | DSBCAPS_CTRLFREQUENCY
                          | DSBCAPS_LOCSOFTWARE;
    bufferDesc.dwBufferBytes = dataSize;
    bufferDesc.dwReserved = 0;
    bufferDesc.lpwfxFormat = &waveFormat;
    bufferDesc.guid3DAlgorithm = GUID_NULL;
    //
    LPDIRECTSOUNDBUFFER tempBuffer = NULL;
    //
    HRESULT hr = dsound->CreateSoundBuffer(&bufferDesc,&tempBuffer, NULL);
    if (hr==DS_OK)
    {
      LPDIRECTSOUNDBUFFER8 secondaryBuffer=NULL;
      hr = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*) &secondaryBuffer);
      if (hr==DS_OK)
      {
        tempBuffer->Release();
        tempBuffer=NULL;
        //
        unsigned char *bufferPtr;
        unsigned long bufferSize=0;
        //
        hr=secondaryBuffer->Lock(0,dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
        if (hr==DS_OK)
        {
          stream->Position=0;
          //
          int cnt=0;
          unsigned char* waveData = new unsigned char[dataSize];
          for (int i=wavHeader; i<stream->Size; i++)
          {
            BYTE buffer[1];
            stream->Read(buffer,1);
            waveData[cnt]=buffer[0];
            //
            cnt++;
          }
          memcpy(bufferPtr, waveData, bufferSize);
          delete []waveData;
          //
          hr=secondaryBuffer->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
          if (hr==DS_OK)
          {
            secondaryBufferList->Add(secondaryBuffer);
            //
            aBufferSize=bufferSize;
            res=secondaryBufferList->Count-1;
          }
        }
      }
    }
  }
  //
  delete stream;
  //
  return(res);
}
//---------------------------------------------------------------------------
bool __fastcall SGame::PlaySingleSound(int aSoundID, int aPosition)
{
  bool res=false;
  //
  if (errorMessage!="")
    return(res);
  //
  LPDIRECTSOUNDBUFFER8 secondaryBuffer=GetSoundBuffer(aSoundID);
  if (!secondaryBuffer)
    return(res);
  //
  HRESULT hr=secondaryBuffer->Restore();
  //
  hr=secondaryBuffer->SetCurrentPosition(aPosition);
  if (hr==DS_OK)
    hr=secondaryBuffer->Play(0,0,0);
  if (hr==DS_OK )
    res=true;
  //
  return(true);
}
//---------------------------------------------------------------------------
LPDIRECTSOUNDBUFFER8 __fastcall SGame::GetSoundBuffer(int aSoundID)
{
  LPDIRECTSOUNDBUFFER8 res=NULL;
  //
  if (aSoundID<0 || aSoundID>=secondaryBufferList->Count)
    return(res);
  //
  res=(LPDIRECTSOUNDBUFFER8)secondaryBufferList->Items[aSoundID];
  //
  return(res);
}
//---------------------------------------------------------------------------
bool __fastcall SGame::SetSoundVolume(int aSoundID, int aVolume)
{
  bool res=false;
  //
  if (errorMessage!="")
    return(res);
  //
  LPDIRECTSOUNDBUFFER8 secondaryBuffer=GetSoundBuffer(aSoundID);
  if (!secondaryBuffer)
    return(res);
  //
  if (aVolume>0)
    aVolume=0;
  else if (aVolume<-10000)
    aVolume=-10000;
  //
  HRESULT hr=secondaryBuffer->Restore();
  //
  hr=secondaryBuffer->SetVolume(aVolume);
  if (hr==DS_OK)
    res=true;
  //
  return(res);
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
  {
    eng->DrawNPCs(bmp);
    //eng->DrawCursor(bmp);
  }
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
  while(1)
  {
    HRESULT ddrval = pDDSFront->Flip(NULL, DDFLIP_WAIT);
    //
    if(ddrval==DD_OK)
      return true;
    if(ddrval==DDERR_SURFACELOST)
      pDDSFront->Restore();
    if(ddrval!=DDERR_WASSTILLDRAWING)
     return true;
  }
}
//---------------------------------------------------------------------------
AnsiString __fastcall SGame::GetErrorMessage()
{
  return errorMessage;
}
//---------------------------------------------------------------------------
void __fastcall SGame::RaiseKeyEvent(AnsiString aKey)
{
  TPoint tp=TPoint(0,0);
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
  {
    //if(tp.x!=0 || tp.y!=0)
    //  PlaySingleSound(soundSteps,0);
    eng->MovePlayer(tp);
  }
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
