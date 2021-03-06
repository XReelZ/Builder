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
class SPlayer : SGameObject
{
  private:
    int speed;
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
    void __fastcall GetDimensions(TPoint &tp);
    //
    TRect __fastcall GetCollisionBox();
    //
    Graphics::TBitmap *__fastcall GetActiveBitmap(bool aIncludeHitbox);
    //
    __fastcall SPlayer();
    __fastcall ~SPlayer();
};
//---------------------------------------------------------------------------
class SNPC : SGameObject
{
  private:
    int speed;
    TPoint activeBitmap;
    TRect collisionBox;
    //
  protected:
  public:
    bool __fastcall LoadPicture(AnsiString aFilePath);
    void __fastcall GetCoords(TPoint &tp);
    void __fastcall GetDimensions(TPoint &tp);
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
    SPlayer *player;
    //
    void __fastcall LoadNPCs();
  protected:
  public:
    void __fastcall MovePlayer(TPoint aPoint);
    bool __fastcall DrawNPCs(Graphics::TBitmap *aBackGround);
    Graphics::TBitmap * __fastcall RotateBMP(Graphics::TBitmap *aBitmap, float aDegree);
    //
    __fastcall SEngine();
    __fastcall ~SEngine();
};
//---------------------------------------------------------------------------
#endif
