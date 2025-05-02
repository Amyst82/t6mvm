#include "pch.h"
#include <StdInclude.h>
#include "CustomDvars.h"
#include "UIBase.h"
#include "TheaterBase.h"
#include "WeaponAnimations.h"

void OnGameLoaded()
{
    CustomDvars::Init();
    UIBase::Init();
    TheaterBase::Init();
    if (T6SDK::Typedefs::UI_OpenToastPopup)
        T6SDK::InternalFunctions::UI_OpenToastPopup("^5T6MVM loaded!", "Please wait for a few seconds til its completed.", 10000);
    Streams::InitStreams();
    T6SDK::MAIN::InitializeDevConsole();
    WeaponAnimations::Init();
    //Read settings
    std::string settingsPath = std::string(T6SDK::Dvars::GetString(*T6SDK::Dvars::DvarList::fs_homepath)) + "\\Plugins\\t6mvm.json";
    if (!std::filesystem::exists(settingsPath))
    {
        T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_WARNING, false, "T6MVM", "Settings JSON not found.");
    }
    else
    {
        std::ifstream file(settingsPath);
        json data = json::parse(file);
        //Getting streams output folder
        std::string StreamsDir = data.value("StreamsDirectory", "");
        if (StreamsDir.empty())
            CustomDvars::dvar_streams_directory->current.string = "";
        else
        {
            if (std::filesystem::exists(StreamsDir) && std::filesystem::is_directory(StreamsDir))
            {
                CustomDvars::dvar_streams_directory->current.string = StreamsDir.c_str();
                T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_INFO, false, "T6MVM", "Streams output directory was restored from t6mvm.json.");
            }
            else
            {
                T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_WARNING, false, "T6MVM", "Could not get streams output directory. Setting it to an empty string.");
                CustomDvars::dvar_streams_directory->current.string = "";
            }
        }
   
        //Getting demos folder
        std::string DemosDir = data.value("DemosDirectory", "");
        if (DemosDir.empty())
            CustomDvars::dvar_demos_directory->current.string = "";
        else
        {
            if (std::filesystem::exists(DemosDir) && std::filesystem::is_directory(DemosDir))
            {
                CustomDvars::dvar_demos_directory->current.string = DemosDir.c_str();
                T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_INFO, false, "T6MVM", "Demos directory was restored from t6mvm.json.");
            }
            else
            {
                T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_WARNING, false, "T6MVM", "Could not get demos directory. Setting it to an empty string.");
                CustomDvars::dvar_demos_directory->current.string = "";
            }
        }
    }
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