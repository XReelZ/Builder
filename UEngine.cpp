//---------------------------------------------------------------------------
#pragma hdrstop
//---------------------------------------------------------------------------
#include "UEngine.h"
#include "UMain.h"
#include <Math.hpp>
#define min(a, b)  (((a) < (b)) ? (a) : (b))
#define max(a, b)  (((a) > (b)) ? (a) : (b))
//---------------------------------------------------------------------------
//class SGameObject
//---------------------------------------------------------------------------
__fastcall SGameObject::SGameObject()
{
  x=0;
  y=0;
  width=0;
  height=0;
  sprite=new Graphics::TBitmap();
}
//---------------------------------------------------------------------------
__fastcall SGameObject::~SGameObject()
{
  delete sprite;
}
//---------------------------------------------------------------------------
//class SNPC
//---------------------------------------------------------------------------
__fastcall SNPC::SNPC()
{
  speed=1;
  x=20;
  y=20;
  width=100;
  height=100;
  activeBitmap.x=1;
  activeBitmap.y=1;
}
//---------------------------------------------------------------------------
__fastcall SNPC::~SNPC()
{
}
//---------------------------------------------------------------------------
bool __fastcall SNPC::LoadPicture(AnsiString aFilePath)
{
  bool res=false;
  try
  {
    sprite->LoadFromFile(aFilePath);
    res=true;
  }
  catch(Exception &e)
  {
  }
  //
  return res;
}
//---------------------------------------------------------------------------
void __fastcall SNPC::SetSpeed(int aSpeed)
{
  speed=aSpeed;
}
//---------------------------------------------------------------------------
int __fastcall SNPC::GetSpeed()
{
  return speed;
}
//---------------------------------------------------------------------------
void __fastcall SNPC::Move(TPoint tp)
{
  x=tp.x;
  y=tp.y;
  SetActiveBitmap();
}
//---------------------------------------------------------------------------
void __fastcall SNPC::GetCoords(TPoint &tp)
{
  tp.x=x;
  tp.y=y;
}
//---------------------------------------------------------------------------
void __fastcall SNPC::SetActiveBitmap()
{
  activeBitmap.y=orientation;
  //
  activeBitmap.x++;
  if(activeBitmap.x>20)
    activeBitmap.x=0;
}
//---------------------------------------------------------------------------
Graphics::TBitmap *__fastcall SNPC::GetActiveBitmap(bool aIncludeHitbox)
{
  Graphics::TBitmap *res=new Graphics::TBitmap();
  //
  TPoint activebmp;
  //
  activebmp.x=activeBitmap.x-1;
  if(activebmp.x<0)
    activebmp.x=0;
  //
  activebmp.y=activeBitmap.y-1;
  if(activebmp.y<0)
    activebmp.y=0;
  //
  res->Width=width;
  res->Height=height;
  res->Transparent=true;
  //
  int x=activebmp.x*width;
  int y=activebmp.y*height;    //??
  //
  res->Canvas->CopyRect(Rect(0,0,width,height),sprite->Canvas,Rect(x,y,width+x,height+y));
  if(aIncludeHitbox)
  {
    TRect rct=GetCollisionBox();
    res->Canvas->Brush->Color=clLime;
    //res->Canvas->Pen->Color=clAqua;
    res->Canvas->FrameRect(rct);
  }
  //
  return res;
}
//---------------------------------------------------------------------------
TRect __fastcall SNPC::GetCollisionBox()
{
  TRect res=Rect(0,0,0,0);
  //
  Graphics::TBitmap *bmp=GetActiveBitmap(false);
  TColor clFiller=bmp->Canvas->Pixels[0][0];
  //
  bool found=false;
  //
  int smallestX=bmp->Width;
  int smallestY=bmp->Height;
  int largestX=0;
  int largestY=0;
  //
  for(int y=0;y<bmp->Height;y++)
  {
    for(int x=0;x<bmp->Width;x++)
    {
      TColor clr=bmp->Canvas->Pixels[y][x];
      //
      if(clFiller!=clr)
      {
        found=true;
        //
        if(smallestX>x)
          smallestX=x;
        //
        if(smallestY>y)
          smallestY=y;
      }
      //
      if(found)
      {
        if(clFiller==clr)
        {
          found=false;
          //
          if(largestX<x)
            largestX=x;
          //
          if(largestY<y)
            largestY=y;
        }
      }
    }
  }
  //
  res=Rect(smallestY,smallestX,largestY,largestX);
  //
  delete bmp;
  //
  return res;
}
//---------------------------------------------------------------------------
int __fastcall SNPC::GetOrientation()
{
  return orientation;
}
//---------------------------------------------------------------------------
void __fastcall SNPC::SetOrientation(int aOrientation)
{
  if(aOrientation<0 || aOrientation>4)
    aOrientation=0;
  //
  orientation=aOrientation;
}
//---------------------------------------------------------------------------
//class SEngine
//---------------------------------------------------------------------------
__fastcall SEngine::SEngine()
{
  //npcList=new TList();
  npc=new SNPC();
  LoadNPCs();
}
//---------------------------------------------------------------------------
__fastcall SEngine::~SEngine()
{
//  for(int i=0;i<npcList->Count;i++)
//  {
//    SNPC *npc=(SNPC *)npcList->Items[i];
//    if(npc)
//      delete npc;
//  }
//  delete npcList;
  delete npc;
}
//---------------------------------------------------------------------------
void __fastcall SEngine::LoadNPCs()
{
  AnsiString npcPath=Main->resourcePath+"\\Right_Sprite_Move_small.bmp";
  //SNPC *npc=new SNPC();
  npc->SetSpeed(5);
  //
  bool ok=npc->LoadPicture(npcPath);

//  if(ok)
//    npcList->Add(npc);
//  else
//    delete npc;
}
//---------------------------------------------------------------------------
void __fastcall SEngine::MovePlayer(TPoint aPoint)
{
//  for(int i=0;i<npcList->Count;i++)
//  {
//    SNPC *npc=(SNPC *)npcList->Items[i];
    if(npc)
    {
      if(aPoint.x>0)
        npc->SetOrientation(1);
      else if(aPoint.x<0)
        npc->SetOrientation(4);
      else if(aPoint.y<0)
        npc->SetOrientation(2);
      else if(aPoint.y>0)
        npc->SetOrientation(3);
      //
      TPoint tp;
      int speed=npc->GetSpeed();
      npc->GetCoords(tp);
      tp.x=tp.x+(aPoint.x*speed);
      tp.y=tp.y+(aPoint.y*speed);
      npc->Move(tp);
    }
//  }
}
//---------------------------------------------------------------------------
bool __fastcall SEngine::DrawNPCs(Graphics::TBitmap *aBackGround)
{
  bool res=false;
  if(!aBackGround)
    return res;
  //
//  for(int i=0;i<npcList->Count;i++)
//  {
//    SNPC *npc=(SNPC *)npcList->Items[i];
    if(npc)
    {
      TPoint tp;
      Graphics::TBitmap *bmp=npc->GetActiveBitmap(false);
      Graphics::TBitmap *bmpT=RotateBMP(bmp,45);
      npc->GetCoords(tp);
      aBackGround->Canvas->Draw(tp.x,tp.y,bmpT);
      delete bmpT;
      delete bmp;
    }
//  }
  //
  res=true;
  return res;
}
//---------------------------------------------------------------------------
Graphics::TBitmap *__fastcall SEngine::RotateBMP(Graphics::TBitmap *aBitmap, float aDegree)
{
  Graphics::TBitmap *res=new Graphics::TBitmap();
  res->Width=aBitmap->Width;
  res->Height=aBitmap->Height;
  res->Transparent=aBitmap->Transparent;
  res->Canvas->Brush->Color=(TColor)aBitmap->Canvas->Pixels[0][0];
  res->Canvas->FillRect(Rect(0,0,res->Width,res->Height));
  //
  float radians=(2*3.1416*aDegree)/360;
  //
  float cosine=(float)cos(radians);
  float sine=(float)sin(radians);
  //
  float Point1x=(-aBitmap->Height*sine);
  float Point1y=(aBitmap->Height*cosine);
  float Point2x=(aBitmap->Width*cosine-aBitmap->Height*sine);
  float Point2y=(aBitmap->Height*cosine+aBitmap->Width*sine);
  float Point3x=(aBitmap->Width*cosine);
  float Point3y=(aBitmap->Width*sine);
  //
  float minx=min(0,min(Point1x,min(Point2x,Point3x)));
  float miny=min(0,min(Point1y,min(Point2y,Point3y)));
  float maxx=max(Point1x,max(Point2x,Point3x));
  float maxy=max(Point1y,max(Point2y,Point3y));
  //
  int DestBitmapWidth=(int)ceil(fabs(maxx)-minx);
  int DestBitmapHeight=(int)ceil(fabs(maxy)-miny);
  //
  res->Height=DestBitmapHeight;
  res->Width=DestBitmapWidth;
  //
  for(int x=0;x<DestBitmapWidth;x++)
  {
    for(int y=0;y<DestBitmapHeight;y++)
    {
      int SrcBitmapx=(int)((x+minx)*cosine+(y+miny)*sine);
      int SrcBitmapy=(int)((y+miny)*cosine-(x+minx)*sine);
      if(SrcBitmapx>=0 && SrcBitmapx<aBitmap->Width && SrcBitmapy>=0 &&
         SrcBitmapy<aBitmap->Height)
        res->Canvas->Pixels[x][y]=aBitmap->Canvas->Pixels[SrcBitmapx][SrcBitmapy];
    }
  }
  //
  return res;
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
