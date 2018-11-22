//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <Windows.h>
#include <wchar.h>
//---------------------------------------------------------------------------
USEFORM("UMain.cpp", Main);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
  HANDLE hMutex=::CreateMutex(NULL,true,"Builder");
  if(hMutex == NULL || GetLastError()==ERROR_ALREADY_EXISTS)
    ShowMessage("Builder is already running.");
  else
  {
    try
    {
      Application->Initialize();
      Application->MainFormOnTaskBar = true;
      Application->CreateForm(__classid(TMain), &Main);
      Application->Run();
    }
    catch (Exception &exception)
    {
      Application->ShowException(&exception);
    }
    catch(...)
    {
      try
      {
        throw Exception("");
      }
      catch (Exception &exception)
      {
        Application->ShowException(&exception);
      }
    }
  }
  if(hMutex)
  {
    ReleaseMutex(hMutex);
    CloseHandle(hMutex);
  }
  return 0;
}
//---------------------------------------------------------------------------
