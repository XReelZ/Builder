//---------------------------------------------------------------------------
#pragma hdrstop
//---------------------------------------------------------------------------
#include "UEngine.h"
#include "UMain.h"
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
      npc->GetCoords(tp);
      aBackGround->Canvas->Draw(tp.x,tp.y,bmp);
      delete bmp;
    }
//  }
  //
  res=true;
  return res;
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
