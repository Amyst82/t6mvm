#pragma once
#include "StdInclude.h"
#include "Settings.h"
namespace SettingsMenu
{
	inline static int PrevKeyCatcher = 0;
	//Flags
	inline static bool IsSettingsMenuOpen = false;

	inline static bool CameraForwardAwaitingKey = false;
	inline static bool CameraLeftAwaitingKey = false;
	inline static bool CameraBackAwaitingKey = false;
	inline static bool CameraRightAwaitingKey = false;
	inline static bool CameraUpAwaitingKey = false;
	inline static bool CameraDownAwaitingKey = false;
	inline static bool FirstMarkerAwaitingKey = false;
	inline static bool StreamsStartStopAwaitingKey = false;
	inline static bool StreamsAbortAwaitingKey = false;

	//Controls
	inline static T6SDK::Drawing::UI_IconClickableButton UI_SettingsButton{};
	inline static T6SDK::Drawing::UI_ClickableButton UI_CloseSettingsMenu{};

	inline static T6SDK::Drawing::UI_CheckBoxButton UI_CameraForwardCheckbox{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_CameraLeftCheckbox{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_CameraBackCheckbox{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_CameraRightCheckbox{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_CameraUpCheckbox{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_CameraDownCheckbox{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_FirstMarkerCheckBox{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_StreamsStartStopCheckbox{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_StreamsAbortCheckbox{};

	inline static T6SDK::Drawing::UI_CheckBoxButton UI_NotificationEnabled{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_OpenStreamsFolder{};

	inline static T6SDK::Drawing::UI_ClickableButton UI_OpenDemosDirectoryDialog{};
	inline static T6SDK::Drawing::UI_ClickableButton UI_OpenStreamsDirectoryDialog{};
	
	//Helper methods
	void ResetFlags()
	{
		//Resetting flags
		CameraForwardAwaitingKey = false;
		CameraLeftAwaitingKey = false;
		CameraBackAwaitingKey = false;
		CameraRightAwaitingKey = false;
		CameraUpAwaitingKey = false;
		CameraDownAwaitingKey = false;
		FirstMarkerAwaitingKey = false;
		StreamsStartStopAwaitingKey = false;
		StreamsAbortAwaitingKey = false;
	}
	static void CloseSettingsMenu()
	{
		IsSettingsMenuOpen = false;
		ResetFlags();
		T6SDK::InternalFunctions::Key_SetCatcher(PrevKeyCatcher);
	}
	void OnKeyPressed(BYTE keyCode)
	{
		if (!IsSettingsMenuOpen)
			return;

		if (keyCode == T6SDK::Input::Keys::ESCAPE.KeyCode && IsSettingsMenuOpen)
		{
			CloseSettingsMenu();
			return;
		}

		auto keyPointer = (T6SDK::Input::InputKey*)T6SDK::MAIN::CharKeys[keyCode];
		if (keyPointer->CanBeUsedAsKeybind == false)
		{
			if (CameraForwardAwaitingKey || CameraLeftAwaitingKey || CameraBackAwaitingKey || CameraRightAwaitingKey || CameraUpAwaitingKey || CameraDownAwaitingKey || FirstMarkerAwaitingKey)
			{
				ResetFlags();
				//T6SDK::MAIN::UI_ShowNotification("SETTINGS", "You can't use this key as a keybind.", 2000);
				return;
			}
		}

		if(CameraForwardAwaitingKey)
			Settings::Settings::KeyBinds["CameraForward"] = (uint32_t)keyCode;
		else if(CameraLeftAwaitingKey)
			Settings::Settings::KeyBinds["CameraLeft"] = (uint32_t)keyCode;
		else if(CameraBackAwaitingKey)
			Settings::Settings::KeyBinds["CameraBack"] = (uint32_t)keyCode;
		else if(CameraRightAwaitingKey)
			Settings::Settings::KeyBinds["CameraRight"] = (uint32_t)keyCode;
		else if(CameraUpAwaitingKey)
			Settings::Settings::KeyBinds["CameraUp"] = (uint32_t)keyCode;
		else if(CameraDownAwaitingKey)
			Settings::Settings::KeyBinds["CameraDown"] = (uint32_t)keyCode;
		else if(FirstMarkerAwaitingKey)
			Settings::Settings::KeyBinds["GotoFirstMarker"] = (uint32_t)keyCode;
		else if(StreamsStartStopAwaitingKey)
			Settings::Settings::KeyBinds["StreamsStartStop"] = (uint32_t)keyCode;
		else if(StreamsAbortAwaitingKey)
			Settings::Settings::KeyBinds["StreamsAbort"] = (uint32_t)keyCode;

		if(CameraForwardAwaitingKey || CameraLeftAwaitingKey || CameraBackAwaitingKey || CameraRightAwaitingKey || CameraUpAwaitingKey || CameraDownAwaitingKey || FirstMarkerAwaitingKey || StreamsStartStopAwaitingKey || StreamsAbortAwaitingKey)
			Settings::Settings::Save();

		ResetFlags();
	}

	std::string selectedFolder{};
	void ChangeDemoDirectory()
	{
		// Open the folder dialog
		if (T6SDK::InternalFunctions::OpenFolderDialog(selectedFolder))
		{
			Settings::Settings::SetDemosDirectory(selectedFolder);
			T6SDK::Dvars::SetString(CustomDvars::dvar_demos_directory, selectedFolder.c_str());
			CustomDvars::dvar_demos_directory->modified = true;
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, false, "SETTINGS", "Selected folder: %s", Settings::Settings::GetDemosDirectory().c_str());
			Common::LoadAllDemos();
			*UIControls::UI_SelectedDemoNumber.SelectedValue = 0;
		}
		else
		{
			std::cerr << "No folder selected or an error occurred." << std::endl;
		}
	}

	//Methods
	static void DrawSettingsButton()
	{
		if(IsSettingsMenuOpen)
			return;
		UI_SettingsButton.Draw();
	}
	static void DrawSettingsMenu()
	{
		int keybindsColumn1 = 3;
		int keybindsColumn2 = 9;
		int keybindsRow = 4;

		if (!IsSettingsMenuOpen)
			return;
		//Drawing bg
		T6SDK::Drawing::DrawRectRelative(0.0f, 0.0f, 1.0f, 1.0f, T6SDK::Drawing::T_GRAYCOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		T6SDK::Drawing::DrawRectRelative(0.0f, 0.0f, 1.0f, 1.0f, T6SDK::Drawing::T_BLACKCOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		T6SDK::Drawing::DrawRectRelative(0.0f, 0.0f, 1.0f, 1.0f, T6SDK::Drawing::T_BLACKCOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);

		//Drawing controls
		T6SDK::Drawing::DrawTextAbsolute("^9KEY BINDS", T6SDK::Drawing::GetGridCellCoords(8, 4).x, T6SDK::Drawing::GetGridCellCoords(8, 2).y, 1.0f, T6SDK::Drawing::T_WHITECOLOR, T6SDK::AnchorPoint::Center, 0x00);

		T6SDK::Drawing::DrawTextAbsolute("CAMERA FORWARD", T6SDK::Drawing::GetGridCellCoords(keybindsColumn1, keybindsRow).x, T6SDK::Drawing::GetGridCellCoords(keybindsColumn1, keybindsRow).y, 1.5f, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UI_CameraForwardCheckbox.UnCheckedText = T6SDK::Input::GetKeyByCode(Settings::Settings::KeyBinds["CameraForward"])->KeyName.c_str();
		UI_CameraForwardCheckbox.Draw();

		T6SDK::Drawing::DrawTextAbsolute("CAMERA LEFT", T6SDK::Drawing::GetGridCellCoords(keybindsColumn1, keybindsRow+3).x, T6SDK::Drawing::GetGridCellCoords(keybindsColumn1, keybindsRow+3).y, 1.5f, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UI_CameraLeftCheckbox.UnCheckedText = T6SDK::Input::GetKeyByCode(Settings::Settings::KeyBinds["CameraLeft"])->KeyName.c_str();
		UI_CameraLeftCheckbox.Draw();
		
		T6SDK::Drawing::DrawTextAbsolute("CAMERA BACK", T6SDK::Drawing::GetGridCellCoords(keybindsColumn1, keybindsRow + 6).x, T6SDK::Drawing::GetGridCellCoords(keybindsColumn1, keybindsRow + 6).y, 1.5f, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UI_CameraBackCheckbox.UnCheckedText = T6SDK::Input::GetKeyByCode(Settings::Settings::KeyBinds["CameraBack"])->KeyName.c_str();
		UI_CameraBackCheckbox.Draw();

		T6SDK::Drawing::DrawTextAbsolute("CAMERA RIGHT", T6SDK::Drawing::GetGridCellCoords(keybindsColumn1, keybindsRow + 9).x, T6SDK::Drawing::GetGridCellCoords(keybindsColumn1, keybindsRow + 9).y, 1.5f, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UI_CameraRightCheckbox.UnCheckedText = T6SDK::Input::GetKeyByCode(Settings::Settings::KeyBinds["CameraRight"])->KeyName.c_str();
		UI_CameraRightCheckbox.Draw();


		T6SDK::Drawing::DrawTextAbsolute("CAMERA UP", T6SDK::Drawing::GetGridCellCoords(keybindsColumn2, keybindsRow).x, T6SDK::Drawing::GetGridCellCoords(keybindsColumn2, keybindsRow).y, 1.5f, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UI_CameraUpCheckbox.UnCheckedText = T6SDK::Input::GetKeyByCode(Settings::Settings::KeyBinds["CameraUp"])->KeyName.c_str();
		UI_CameraUpCheckbox.Draw();

		T6SDK::Drawing::DrawTextAbsolute("CAMERA DOWN", T6SDK::Drawing::GetGridCellCoords(keybindsColumn2, keybindsRow+3).x, T6SDK::Drawing::GetGridCellCoords(keybindsColumn2, keybindsRow + 3).y, 1.5f, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UI_CameraDownCheckbox.UnCheckedText = T6SDK::Input::GetKeyByCode(Settings::Settings::KeyBinds["CameraDown"])->KeyName.c_str();
		UI_CameraDownCheckbox.Draw();

		T6SDK::Drawing::DrawTextAbsolute("GO TO FIRST MARKER CTRL+", T6SDK::Drawing::GetGridCellCoords(keybindsColumn2, keybindsRow + 6).x, T6SDK::Drawing::GetGridCellCoords(keybindsColumn2, keybindsRow + 6).y, 1.5f, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UI_FirstMarkerCheckBox.UnCheckedText = T6SDK::Input::GetKeyByCode(Settings::Settings::KeyBinds["GotoFirstMarker"])->KeyName.c_str();
		UI_FirstMarkerCheckBox.Draw();

		T6SDK::Drawing::DrawTextAbsolute("STREAMS START/STOP", T6SDK::Drawing::GetGridCellCoords(keybindsColumn2, keybindsRow + 9).x, T6SDK::Drawing::GetGridCellCoords(keybindsColumn2, keybindsRow + 9).y, 1.5f, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UI_StreamsStartStopCheckbox.UnCheckedText = T6SDK::Input::GetKeyByCode(Settings::Settings::KeyBinds["StreamsStartStop"])->KeyName.c_str();
		UI_StreamsStartStopCheckbox.Draw();

		T6SDK::Drawing::DrawTextAbsolute("STREAMS ABORT", T6SDK::Drawing::GetGridCellCoords(keybindsColumn2, keybindsRow + 12).x, T6SDK::Drawing::GetGridCellCoords(keybindsColumn2, keybindsRow + 12).y, 1.5f, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UI_StreamsAbortCheckbox.UnCheckedText = T6SDK::Input::GetKeyByCode(Settings::Settings::KeyBinds["StreamsAbort"])->KeyName.c_str();
		UI_StreamsAbortCheckbox.Draw();

		T6SDK::Drawing::DrawTextAbsolute("^9ACTIONS AFTER STREAMS RECORDING FINISHED", T6SDK::Drawing::GetGridCellCoords(8, 20).x, T6SDK::Drawing::GetGridCellCoords(8, 20).y, 1.0f, T6SDK::Drawing::T_WHITECOLOR, T6SDK::AnchorPoint::Center, 0x00);

		UI_NotificationEnabled.Draw();
		UI_OpenStreamsFolder.Draw();
		
		T6SDK::Drawing::DrawTextAbsolute("^9DIRECTORTIES", T6SDK::Drawing::GetGridCellCoords(8, 26).x, T6SDK::Drawing::GetGridCellCoords(8, 26).y, 1.0f, T6SDK::Drawing::T_WHITECOLOR, T6SDK::AnchorPoint::Center, 0x00);

		std::string demosPath;
		if(Settings::Settings::GetDemosDirectory().empty())
			demosPath = "DEMOS: No directory selected.";
		else
			demosPath = "DEMOS: " + Settings::Settings::GetDemosDirectory();
		UI_OpenDemosDirectoryDialog.Text = demosPath.c_str();
		UI_OpenDemosDirectoryDialog.Draw();

		std::string streamsPath;
		if (Settings::Settings::StreamsDirectory.empty())
			streamsPath = "STREAMS: No directory selected.";
		else
			streamsPath = "STREAMS: " + Settings::Settings::StreamsDirectory;
		UI_OpenStreamsDirectoryDialog.Text = streamsPath.c_str();
		UI_OpenStreamsDirectoryDialog.Draw();

		UI_CloseSettingsMenu.Draw();
	}

	static void Init()
	{
		int keybindsColumn1 = 6;
		int keybindsColumn2 = 12;
		int keybindsRow = 4;
		UI_SettingsButton = T6SDK::Drawing::UI_IconClickableButton("SETTINGS", "menu_mp_lobby_icon_customgamemode", 13, 13, T6SDK::AnchorPoint::TopLeft, false, []() 
		{
				IsSettingsMenuOpen = true;
				PrevKeyCatcher = T6SDK::Addresses::CurrentKeyCatcher.Value();
				T6SDK::InternalFunctions::Key_SetCatcher(!T6SDK::Theater::IsInTheater() ? 15 : 17);
		});
		UI_CloseSettingsMenu = T6SDK::Drawing::UI_ClickableButton("^3ESC ^7Back", 3, 38, T6SDK::AnchorPoint::Center, (uintptr_t)&CloseSettingsMenu);

		UI_CameraForwardCheckbox = T6SDK::Drawing::UI_CheckBoxButton("^3W", "^9...", keybindsColumn1, keybindsRow, T6SDK::AnchorPoint::TopLeft, &CameraForwardAwaitingKey, 0x00);
		UI_CameraLeftCheckbox = T6SDK::Drawing::UI_CheckBoxButton("^3A", "^9...", keybindsColumn1, keybindsRow+3, T6SDK::AnchorPoint::TopLeft, &CameraLeftAwaitingKey, 0x00);
		UI_CameraBackCheckbox = T6SDK::Drawing::UI_CheckBoxButton("^3S", "^9...", keybindsColumn1, keybindsRow+6, T6SDK::AnchorPoint::TopLeft, &CameraBackAwaitingKey, 0x00);
		UI_CameraRightCheckbox = T6SDK::Drawing::UI_CheckBoxButton("^3D", "^9...", keybindsColumn1, keybindsRow+9, T6SDK::AnchorPoint::TopLeft, &CameraRightAwaitingKey, 0x00);
		UI_CameraUpCheckbox = T6SDK::Drawing::UI_CheckBoxButton("^3E", "^9...", keybindsColumn2, keybindsRow, T6SDK::AnchorPoint::TopLeft, &CameraUpAwaitingKey, 0x00);
		UI_CameraDownCheckbox = T6SDK::Drawing::UI_CheckBoxButton("^3Q", "^9...", keybindsColumn2, keybindsRow+3, T6SDK::AnchorPoint::TopLeft, &CameraDownAwaitingKey, 0x00);
		UI_FirstMarkerCheckBox = T6SDK::Drawing::UI_CheckBoxButton("^3M", "^9...", keybindsColumn2, keybindsRow+6, T6SDK::AnchorPoint::TopLeft, &FirstMarkerAwaitingKey, 0x00);
		UI_StreamsStartStopCheckbox = T6SDK::Drawing::UI_CheckBoxButton("^3F5", "^9...", keybindsColumn2, keybindsRow+9, T6SDK::AnchorPoint::TopLeft, &StreamsStartStopAwaitingKey, 0x00);
		UI_StreamsAbortCheckbox = T6SDK::Drawing::UI_CheckBoxButton("^3F6", "^9...", keybindsColumn2, keybindsRow+12, T6SDK::AnchorPoint::TopLeft, &StreamsAbortAwaitingKey, 0x00);

		UI_NotificationEnabled = T6SDK::Drawing::UI_CheckBoxButton("ALERTZY NOTIFICATION OFF", "ALERTZY NOTIFICATION ON", keybindsColumn1-3, 22, T6SDK::AnchorPoint::TopLeft, &Settings::Settings::Notification_proxy, [](bool e)
		{ 
				std::string s("Notification");
				Settings::Settings::SetPostStreamsAction(s, e);
				//Settings::Settings::Save();
		});
		UI_OpenStreamsFolder = T6SDK::Drawing::UI_CheckBoxButton("OPEN FOLDER OFF", "OPEN FOLDER ON", keybindsColumn2 - 3, 22, T6SDK::AnchorPoint::TopLeft, &Settings::Settings::OpenStreamsFolder_proxy, [](bool e)
		{
				std::string s("OpenStreamsFolder");
				Settings::Settings::SetPostStreamsAction(s, e);
				//Settings::Settings::Save();
		});

		UI_OpenDemosDirectoryDialog = T6SDK::Drawing::UI_ClickableButton("DEMOS: No directory selected.",keybindsColumn1-3, 28, T6SDK::AnchorPoint::TopLeft, (uintptr_t)&ChangeDemoDirectory);
		UI_OpenStreamsDirectoryDialog = T6SDK::Drawing::UI_ClickableButton("STREAMS: No directory selected.", keybindsColumn1 - 3, 31, T6SDK::AnchorPoint::TopLeft, []()
		{
				// Open the folder dialog
				if (T6SDK::InternalFunctions::OpenFolderDialog(selectedFolder))
				{
					Settings::Settings::SetStreamsDirectory(selectedFolder);
					T6SDK::Dvars::SetString(CustomDvars::dvar_demos_directory, selectedFolder.c_str());
				}
				else
				{
					std::cerr << "No folder selected or an error occurred." << std::endl;
				}
		});

		T6SDK::Events::RegisterListener(T6SDK::EventType::OnKeyPressed, (uintptr_t)&OnKeyPressed);
	}
}