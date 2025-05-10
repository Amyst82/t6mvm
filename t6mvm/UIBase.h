#pragma once
#include <StdInclude.h>
#include "UIControls.h"
#include "CameraMenu.h"
#include "BoneCameraMenu.h"
#include "LightsMenu.h"
#include "MiscMenu.h"
#include "WeaponMenu.h"
#include "StreamsMenu.h"
#include "SunMenu.h"
#include "SsaoMenu.h"
#include "FogMenu.h"
#include "DofMenu.h"
#include "WeaponAnimationChangerMenu.h"
#include "MainMenu.h"
#include "DemoBrowserMenu.h"
#include "CustomSlidersMenu.h"

namespace UIBase
{
	static void CopyCFGtoClipboard()
	{
		bool result = false;
		if (*UIControls::MainCameraTabButton.isChecked)
			result = T6SDK::InternalFunctions::SetClipboardText(CameraMenu::GetCFG());
		else if (*UIControls::MiscTabButton.isChecked)
			result = T6SDK::InternalFunctions::SetClipboardText(MiscMenu::GetCFG());
		else if (*UIControls::SsaoTabButton.isChecked)
			result = T6SDK::InternalFunctions::SetClipboardText(SsaoMenu::GetCFG());
		else if (*UIControls::FogTabButton.isChecked)
			result = T6SDK::InternalFunctions::SetClipboardText(FogMenu::GetCFG());
		else if (*UIControls::DofTabButton.isChecked)
			result = T6SDK::InternalFunctions::SetClipboardText(DofMenu::GetCFG());
		else if (*UIControls::SunSkyTabButton.isChecked)
			result = T6SDK::InternalFunctions::SetClipboardText(SunMenu::GetCFG());
		else if (*UIControls::StreamsTabButton.isChecked)
			result = T6SDK::InternalFunctions::SetClipboardText(StreamsMenu::GetCFG());
		else if (*UIControls::WeaponTabButton.isChecked)
			result = T6SDK::InternalFunctions::SetClipboardText(WeaponMenu::GetCFG());
		if(!result)
		{
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, false, "UI", "Failed to copy to clipboard.");
			T6SDK::MAIN::UI_ShowNotification("CFG", "Failed to copy to clipboard.",200);
		}
		else 
		{
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_SUCCESS, false, "UI", "Copied to clipboard.");
			T6SDK::MAIN::UI_ShowNotification("CFG", "Copied to clipboard!", 200);
		}
	}
	static void CloseMenu()
	{
		(*T6SDK::Dvars::DvarList::r_blur)->current.value = 0.0f;
		*UIControls::UI_WeaponAnimChanging.isChecked = false;
		T6SDK::Input::CloseBlankMenu();
	}
	bool gameUnpaused = false;
	void unpauseGame()
	{
		T6SDK::Addresses::IsDemoPaused.SetValueSafe(0);
		gameUnpaused = true;
	}
	static void OpenMenu()
	{
		UIControls::UI_TimelineSlider = UI_TimelineNS::UI_Timeline(T6SDK::Addresses::DemoEndTick.Value(), 8, 28, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopCenter);
		UIControls::UI_DemoClient = T6SDK::Drawing::UI_EnumButton("Demo client", 0, T6SDK::Addresses::cg->activeSnapshots[0].numClients-1, &(*T6SDK::Dvars::DvarList::demo_client)->current.integer, 8, 35, T6SDK::AnchorPoint::TopCenter, (uintptr_t)&unpauseGame);
		T6SDK::Input::OpenBlankMenu();

	}
	void OnKeyPressed(BYTE keyCode)
	{
		if (!T6SDK::Theater::IsInTheater())
			return;
		if (keyCode == T6SDK::Input::Keys::TAB.KeyCode)
		{
			if (T6SDK::MAIN::DevConsoleOpened == true)
			{
				T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_WARNING, false, "UI", "Console is opened! Aborting.");
				return;
			}
			if (T6SDK::Input::Keys::ALT.IsAnyPressState())
				return;
			if(!T6SDK::Input::BlankMenuOpened)
			{
				Streams::StopStreams();
				OpenMenu();
			}
			else
			{
				CloseMenu();
			}
		}
		if(T6SDK::Input::BlankMenuOpened && keyCode == T6SDK::Input::Keys::ESCAPE.KeyCode)
		{
			CloseMenu();
		}
		Common::UI_BookmarkDialog.OnInputKey(keyCode);
		CustomSlidersMenu::UI_CustomSliderDvarNameDialog.OnInputKey(keyCode);
	}

	void DrawUIGrid()
	{
		//Drawing columns
		for (int column = 0; column < 16; column++)
		{
			vec2_t gridXcoords = T6SDK::Drawing::GetGridCellCoords(column, 0);
			T6SDK::Drawing::DrawRectAbsolute(gridXcoords.x, gridXcoords.y, 2.0f, T6SDK::Addresses::ScreenHeight.Value(), tColor{ 1.0f, 1.0f, 1.0f, 0.25f }, T6SDK::AnchorPoint::TopLeft, 0x00);
		}
		//Drawing rows
		for (int row = 0; row < 40; row++)
		{
			vec2_t gridYcoords = T6SDK::Drawing::GetGridCellCoords(0, row);
			T6SDK::Drawing::DrawRectAbsolute(0.0f, gridYcoords.y, T6SDK::Addresses::ScreenWidth.Value(), 2.0f, tColor{ 1.0f, 1.0f, 1.0f, 0.25f }, T6SDK::AnchorPoint::TopLeft, 0x00);
		}
		//Drawing grid numbers
		for (int column = 0; column < 16; column++)
		{
			for (int row = 0; row < 40; row++)
			{
				vec2_t gridCoords = T6SDK::Drawing::GetGridCellCoords(column, row);
				char buffer[64];
				sprintf(buffer, "%i / %i", column, row);
				T6SDK::Drawing::DrawTextAbsolute(buffer, gridCoords.x, gridCoords.y, 1.0f, T6SDK::Drawing::T_WHITECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
			}
		}
	}

	void OnEndFrameDrawn()
	{
		if (!T6SDK::Input::InputLockedByTextBoxDialog)
		{
			if (T6SDK::MAIN::DevConsoleOpened == true)
				return;
			if (gameUnpaused)
			{
				T6SDK::Addresses::IsDemoPaused.SetValueSafe(1);
				gameUnpaused = false;
			}
			if (!T6SDK::Theater::IsInTheater() || T6SDK::Input::BlankMenuOpened)
			{
				T6SDK::Drawing::DrawTextAbsolute("^5T6^7MVM v1.0.0 build 1515 May 10th 2025", 20.0f, 10.0f, 1.0f, tColor{ 1.0f, 1.0f, 1.0f, 0.2f }, T6SDK::AnchorPoint::TopLeft, 0x00);
			}
			//Draw tabs here
			if (T6SDK::Input::BlankMenuOpened)
			{
				//bg blur
				(*T6SDK::Dvars::DvarList::r_blur)->current.value = CustomDvars::dvar_menuBlur->current.enabled ? 9.0f : 0.0f;
				//bg color
				T6SDK::Drawing::DrawRectRelative(0.0f, 0.0f, 1.0f, 1.0f, CustomDvars::dvar_menuBlur->current.enabled ? T6SDK::Drawing::T_BLACKCOLOR : tColor{ 0.0f, 0.0f, 0.0f, 0.0f }, T6SDK::AnchorPoint::TopLeft, 0x00);

				if (*UIControls::UI_WeaponAnimChanging.isChecked)
				{
					WeaponAnimationChangerMenu::Draw();
				}
				else
				{
					UIControls::DrawTabs();
					MainMenu::Draw();
					CameraMenu::Draw();
					BoneCameraMenu::Draw();
					SsaoMenu::Draw();
					FogMenu::Draw();
					DofMenu::Draw();
					SunMenu::Draw();
					MiscMenu::Draw();
					LightsMenu::Draw();
					StreamsMenu::Draw();
					WeaponMenu::Draw();
					CustomSlidersMenu::Draw();
					UIControls::MenuBlurCheckBox.Draw();
					UIControls::CloseMenuButton.Draw();
					if (*UIControls::MainCameraTabButton.isChecked || *UIControls::MiscTabButton.isChecked || *UIControls::SsaoTabButton.isChecked || *UIControls::FogTabButton.isChecked || *UIControls::DofTabButton.isChecked || *UIControls::SunSkyTabButton.isChecked || *UIControls::StreamsTabButton.isChecked || *UIControls::WeaponTabButton.isChecked)
						UIControls::AddToCFGButton.Draw();

					if (*UIControls::MainTabButton.isChecked || *UIControls::BoneCameraTabButton.isChecked)
					{
						char buffer[64];
						sprintf_s(buffer, "Player: ^3%s", T6SDK::Addresses::cg->client[T6SDK::Dvars::GetInt(*T6SDK::Dvars::DvarList::demo_client)].szName);
						UIControls::UI_DemoClient.Text = buffer;
						UIControls::UI_DemoClient.Draw();
					}
				}
			}
			if (!T6SDK::Theater::IsInTheater() && !T6SDK::Input::BlankMenuOpened)
			{
				if (T6SDK::MAIN::ENABLED && T6SDK::Addresses::GameMode.Value() == 32)
				{
					if(!DemoBrowserMenu::isShown)
					{
						UIControls::UI_DemoBrowseCheckButton.Draw();
						UIControls::UI_OpenDemoBrowserButton.Draw();
					}
					if (CustomDvars::dvar_demos_directory->modified)
					{
						DemoBrowserMenu::Init();
						CustomDvars::dvar_demos_directory->modified = false;
					}
					DemoBrowserMenu::Draw();

				}
			}
			if (!Streams::ScreenshotRequested && !Streams::IsAnyOtherStream && Streams::IsStreamsRunning)
			{
				Streams::ShowProgressOnScreen();
			}
			if (CustomDvars::dvar_uiGridDebug->current.enabled)
				DrawUIGrid();
		}
		Common::UI_BookmarkDialog.Draw();
		DemoBrowserMenu::UI_DemoRenameDialog.Draw();
		CustomSlidersMenu::UI_CustomSliderDvarNameDialog.Draw();
		((T6SDK::Drawing::UI_Notification*)(T6SDK::MAIN::GetNotificationControl()))->Draw();

	}
	void OnSunInited()
	{
		SunMenu::Init();
		FogMenu::Init();
	}
	inline static void Init()
	{
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnEndFrameDrawn, (uintptr_t)&OnEndFrameDrawn);
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnKeyPressed, (uintptr_t)&OnKeyPressed);
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnDemoPlaybackInited, (uintptr_t)&OnSunInited);
		//Init buttons and sliders here
		UIControls::Init();
		MainMenu::Init();
		CameraMenu::Init();
		MiscMenu::Init();
		BoneCameraMenu::Init();
		SsaoMenu::Init();
		DofMenu::Init();
		LightsMenu::Init();
		StreamsMenu::Init();
		WeaponMenu::Init();
		CustomSlidersMenu::Init();
		WeaponAnimationChangerMenu::Init();

		UIControls::CloseMenuButton = T6SDK::Drawing::UI_ClickableButton("^3TAB ^7Back", 2, 35, T6SDK::AnchorPoint::TopLeft, (uintptr_t)&UIBase::CloseMenu);
		UIControls::CloseMenuButton.ToolTip = "Press ^3TAB ^7or ^3ESC ^7 or just click here to close the menu.";

		UIControls::AddToCFGButton = T6SDK::Drawing::UI_ClickableButton("Copy CFG to clipboard", 8, 35, T6SDK::AnchorPoint::TopCenter, (uintptr_t)&UIBase::CopyCFGtoClipboard, true);
		UIControls::AddToCFGButton.ToolTip = "^7Copy the current settings to clipboard as ^5CFG ^7commands.";

		UIControls::UI_TimelineSlider = UI_TimelineNS::UI_Timeline(99999, 8, 28, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopCenter);
		UIControls::UI_TimelineSlider.ToolTip = "Jump to any tick you want!";
		UIControls::UI_DemoClient = T6SDK::Drawing::UI_EnumButton("Demo client", 0, 32, &(*T6SDK::Dvars::DvarList::demo_client)->current.integer, 8, 35, T6SDK::AnchorPoint::TopCenter, 0x00);
		UIControls::UI_DemoClient.ToolTip = "Switch between players.";
		UIControls::UI_OpenDemoBrowserButton = T6SDK::Drawing::UI_IconClickableButton("BROWSE DEMOS", "menu_mp_lobby_icon_clip", 13, 7, T6SDK::AnchorPoint::TopLeft, false, []()
		{
			DemoBrowserMenu::Show();
		});
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnMouseWheelDown, (uintptr_t)&DemoBrowserMenu::OnMouseWheelDown);
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnMouseWheelUp, (uintptr_t)&DemoBrowserMenu::OnMouseWheelUp);
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnKeyPressed, (uintptr_t)&DemoBrowserMenu::OnKeyPressed);


	}
}