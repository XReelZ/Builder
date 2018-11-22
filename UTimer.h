//---------------------------------------------------------------------------
#ifndef UTimerH
#define UTimerH
//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
#include <windows.h>
//---------------------------------------------------------------------------
class Timer
{
  public:
    Timer();
    ~Timer();
    void StartWatch();
    void StopWatch();
    float GetTimeMilli() const;
  private:
    float invFreqMilli;
    bool watchStopped;
    __int64 currentCount;
    __int64 startCount;
};
//---------------------------------------------------------------------------
#endif
