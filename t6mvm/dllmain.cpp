#include "pch.h"
#include <StdInclude.h>
#include "CustomDvars.h"
#include "UIBase.h"
#include "TheaterBase.h"
#include "WeaponAnimations.h"
#include "Settings.h"

void SetDemoDirectoryDvar()
{
    if (!Settings::Settings::GetDemosDirectory().empty())
        return;
    if (T6SDK::CrossVersion::GetGameVersion() == T6SDK::CrossVersion::GameVersion::V43)
    {
        std::string redactedPath = std::string(T6SDK::Dvars::GetString(*T6SDK::Dvars::DvarList::fs_homepath)) + "\\Plugins\\OpenNetStorage\\Cloud\\FILM_MP\\";
        Settings::Settings::SetDemosDirectory(redactedPath);
        T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_INFO, false, "T6MVM", "Demos directory was restored from redacted mp game path.");
    }
    else if (T6SDK::CrossVersion::GetGameVersion() == T6SDK::CrossVersion::GameVersion::V41)
    {
        std::string redactedPath = std::string(T6SDK::Dvars::GetString(*T6SDK::Dvars::DvarList::fs_homepath)) + "\\Plugins\\OpenNetStorage\\Cloud\\FILM_ZM\\";
        Settings::Settings::SetDemosDirectory(redactedPath);
        T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_INFO, false, "T6MVM", "Demos directory was restored from redacted zm game path.");
    }
}

void OnGameLoaded()
{
    CustomDvars::Init();
    UIBase::Init();
    TheaterBase::Init();
    if (T6SDK::Typedefs::UI_OpenToastPopup)
        T6SDK::InternalFunctions::UI_OpenToastPopup("^5T6MVM loaded!", "Have fun editing!", 10000);
    Streams::InitStreams();
    T6SDK::MAIN::InitializeDevConsole();
    WeaponAnimations::Init();
    //Read settings
    Settings::Settings::Load();
    SetDemoDirectoryDvar();
    Common::LoadAllDemos();
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