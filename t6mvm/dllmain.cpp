#include "pch.h"
#include <StdInclude.h>
#include "CustomDvars.h"
#include "UIBase.h"
#include "TheaterBase.h"

void OnGameLoaded()
{
    CustomDvars::Init();
    UIBase::Init();
    TheaterBase::Init();
    if (T6SDK::Typedefs::UI_OpenToastPopup)
        T6SDK::InternalFunctions::UI_OpenToastPopup("^5T6MVM loaded!", "Please wait for a few seconds til its completed.", 10000);
    Streams::InitStreams();
}

void Init()
{
    T6SDK::MAIN::DEBUG = true;
    T6SDK::MAIN::PreInit();
    T6SDK::Events::RegisterListener(T6SDK::EventType::OnGameLoaded, (uintptr_t)&OnGameLoaded);
}
HANDLE hThread;

DWORD WINAPI mainThread(LPVOID param)
{
    Sleep(5000);
    Init();
    SuspendThread(hThread);
    CloseHandle(hThread);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        hThread = CreateThread(nullptr, 0, &mainThread, nullptr, 0, nullptr);
        break;
    case DLL_PROCESS_DETACH:
        T6SDK::MAIN::DeInitialize();
        break;
    }
    return TRUE;
}