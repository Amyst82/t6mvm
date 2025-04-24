#pragma once
#include "UIControls.h"
#include "CustomDvars.h"
namespace MainMenu
{
	void LoadCfgFile()
	{
		std::string cfgData{};
		char openedFileName[MAX_PATH];
		const TCHAR* FilterSpec = (const TCHAR*)"Config file(.cfg)\0*.cfg*\0";
		const TCHAR* Title = (const TCHAR*)"Load CFG file";
		if (T6SDK::InternalFunctions::OpenFileDialog((TCHAR*)openedFileName, false, FilterSpec, Title))
		{
			string str(openedFileName);
			ofstream ImportFile(str, ios_base::in);
			if (ImportFile.good())
			{
				ifstream fileopen(str);
				int counter = 0;
				bool successStatus = false;
				if (fileopen.is_open())
				{
					try
					{
						stringstream buffer;
						buffer << fileopen.rdbuf();
						successStatus = true;
						cfgData = buffer.str();
					}
					catch (exception ex)
					{
						fileopen.close();
						T6SDK::MAIN::UI_ShowNotification("MAINMENU", "Error during loading cfg", 200);
						T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, false, "MAINMENU", "Error during loading cfg!");
						T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_WARNING, false, "MAINMENU", ex.what());
						successStatus = false;
						return;
					}

				}
				if (successStatus)
				{
					fileopen.close();
					T6SDK::Dvars::cbuf_AddText(cfgData.c_str());
					T6SDK::MAIN::UI_ShowNotification("MAINMENU", "CFG file was loaded!", 200);
					T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_SUCCESS, false, "MAINMENU", "CFG was loaded!");
				}
			}
		}
	}
	void SaveAllAsCFG()
	{
		std::string cfgData = "";
		cfgData += CameraMenu::GetCFG();
		cfgData += MiscMenu::GetCFG();
		cfgData += SsaoMenu::GetCFG();
		cfgData += DofMenu::GetCFG();
		cfgData += SunMenu::GetCFG();
		cfgData += StreamsMenu::GetCFG();
		cfgData += WeaponMenu::GetCFG();

		char openedFileName[MAX_PATH];
		const TCHAR* FilterSpec = (const TCHAR*)"Config file(.cfg)\0*.cfg*\0";      
		const TCHAR* Title = (const TCHAR*)"Load CFG file";
		if (T6SDK::InternalFunctions::OpenFileDialog((TCHAR*)openedFileName, true, FilterSpec, Title))
		{
			string str(openedFileName);
			string strMirrored(openedFileName);
			str.append(".cfg");
			ofstream ExportFile(str, ios_base::out);
			if (ExportFile.good())
			{
				ExportFile << cfgData.c_str();
				ExportFile.close();
				T6SDK::MAIN::UI_ShowNotification("MAINMENU", "CFG file was exported!", 200);
				T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_SUCCESS, false, "MAINMENU", "CFG was exported!");
			}
		}
	}                                                                                                                                                                                                                                                                                               
	static void Draw()                                                                                                                                                                              
	{
		if(!*UIControls::MainTabButton.isChecked)
			return;
		UIControls::UI_TimelineSlider.Draw();
		if (UIControls::UI_TimelineSlider.CustomBookmarks.size() > 0)
			UIControls::UI_RemoveCustomBookmarks.Draw();
		UIControls::UI_LoadCFGButton.Draw();
		UIControls::UI_SaveAllAsCFGButton.Draw();
	}
	static void Init()
	{
		UIControls::UI_RemoveCustomBookmarks = T6SDK::Drawing::UI_ClickableButton("Remove all bookmarks", 8, 32, T6SDK::AnchorPoint::TopCenter, (uintptr_t)&UIControls::UI_TimelineSlider.RemoveAllBookmarks);
		UIControls::UI_RemoveCustomBookmarks.ToolTip = "Remove all custom bookmarks.";
		UIControls::UI_LoadCFGButton		= T6SDK::Drawing::UI_ClickableButton("LOAD ^3CFG ^7FILE", 8, 17, T6SDK::AnchorPoint::Center, (uintptr_t)&LoadCfgFile);
		UIControls::UI_LoadCFGButton.ToolTip = "Load and apply you cfg!";
		UIControls::UI_SaveAllAsCFGButton	= T6SDK::Drawing::UI_ClickableButton("SAVE ALL AS CFG", 8, 21, T6SDK::AnchorPoint::Center, (uintptr_t)&SaveAllAsCFG);
		UIControls::UI_SaveAllAsCFGButton.ToolTip = "Save everything from T6MVM as ^3CFG file!";
	}
}