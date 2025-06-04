//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USEFORM("Console.cpp", frmResourceConsole);
USEFORM("dmUMS.cpp", dmgwUMS); /* TDataModule: File Type */
USEFORM("EStepDebug.cpp", dlgStepDebug);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
         Application->Initialize();
         Application->CreateForm(__classid(TfrmResourceConsole), &frmResourceConsole);
         Application->CreateForm(__classid(TdmgwUMS), &dmgwUMS);
         Application->CreateForm(__classid(TdlgStepDebug), &dlgStepDebug);
         Application->Run();
    }
    catch (Exception &exception)
    {
         //Application->ShowException(&exception);
         Application->MessageBox( exception.Message.c_str() , "Application Main Thread Error" , MB_OK );
    }
    return 0;
}
//---------------------------------------------------------------------------




