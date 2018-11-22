//---------------------------------------------------------------------------
#ifndef UEngineH
#define UEngineH
//---------------------------------------------------------------------------
#include <vcl.h>
//---------------------------------------------------------------------------
class SGameObject
{
  private:
  protected:
    int x;
    int y;
    int width;
    int height;
    Graphics::TBitmap *sprite;
  public:
    __fastcall SGameObject();
    __fastcall ~SGameObject();
};
//---------------------------------------------------------------------------
class SNPC : SGameObject
{
  private:
    int speed;
    int orientation;
    TPoint activeBitmap;
    TRect collisionBox;
    //
  protected:
  public:
    void __fastcall SetActiveBitmap();
    void __fastcall SetSpeed(int aSpeed);
    bool __fastcall LoadPicture(AnsiString aFilePath);
    void __fastcall Move(TPoint tp);
    int __fastcall GetSpeed();
    void __fastcall GetCoords(TPoint &tp);
    int __fastcall GetOrientation();
    void __fastcall SetOrientation(int aOrientation);
    //
    TRect __fastcall GetCollisionBox();
    //
    Graphics::TBitmap *__fastcall GetActiveBitmap(bool aIncludeHitbox);
    //
    __fastcall SNPC();
    __fastcall ~SNPC();
};
//---------------------------------------------------------------------------
class SEngine
{
  private:
    //TList *npcList;
    SNPC *npc;
    //
    void __fastcall LoadNPCs();
  protected:
  public:
    void __fastcall MovePlayer(TPoint aPoint);
    bool __fastcall DrawNPCs(Graphics::TBitmap *aBackGround);
    //
    __fastcall SEngine();
    __fastcall ~SEngine();
};
//---------------------------------------------------------------------------
#endif