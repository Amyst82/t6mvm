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
#include "DofMenu.h"
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
		else if (*UIControls::DofTabButton.isChecked)
			result = T6SDK::InternalFunctions::SetClipboardText(DofMenu::GetCFG());
		else if (*UIControls::SunSkyTabButton.isChecked)
			result = T6SDK::InternalFunctions::SetClipboardText(SunMenu::GetCFG());
		else if (*UIControls::StreamsTabButton.isChecked)
			result = T6SDK::InternalFunctions::SetClipboardText(StreamsMenu::GetCFG());
		else if (*UIControls::WeaponTabButton.isChecked)
			result = T6SDK::InternalFunctions::SetClipboardText(WeaponMenu::GetCFG());
		if(!result)
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, false, "UI", "Failed to copy to clipboard.");
		else 
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_SUCCESS, false, "UI", "Copied to clipboard.");
	}
	static void CloseMenu()
	{
		(*T6SDK::Dvars::DvarList::r_blur)->current.value = 0.0f;
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
		UIControls::UI_DemoClient = T6SDK::Drawing::UI_EnumButton("Demo client", 0, T6SDK::Addresses::cg->activeSnapshots[0].numClients-1, &(*T6SDK::Dvars::DvarList::demo_client)->current.integer, 8, 33, T6SDK::AnchorPoint::TopCenter, (uintptr_t)&unpauseGame);
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
	}

	void OnEndFrameDrawn()
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
			T6SDK::Drawing::DrawTextAbsolute("T6MVM v1.0.0 build 1337", 20.0f, 10.0f, 1.0f, tColor{ 1.0f, 1.0f, 1.0f, 0.2f }, T6SDK::AnchorPoint::TopLeft, 0x00);
		}
		//Draw tabs here
		if (T6SDK::Input::BlankMenuOpened)
		{
			//bg blur
			(*T6SDK::Dvars::DvarList::r_blur)->current.value = CustomDvars::dvar_menuBlur->current.enabled ? 9.0f : 0.0f;
			//bg color
			T6SDK::Drawing::DrawRectRelative(0.0f, 0.0f, 1.0f, 1.0f, CustomDvars::dvar_menuBlur->current.enabled ? T6SDK::Drawing::T_BLACKCOLOR : tColor{ 0.0f, 0.0f, 0.0f, 0.0f }, T6SDK::AnchorPoint::TopLeft, 0x00);

			UIControls::DrawTabs();
			CameraMenu::Draw();
			BoneCameraMenu::Draw();
			SsaoMenu::Draw();
			DofMenu::Draw();
			SunMenu::Draw();
			MiscMenu::Draw();
			LightsMenu::Draw();
			StreamsMenu::Draw();
			WeaponMenu::Draw();
			UIControls::MenuBlurCheckBox.Draw();
			UIControls::CloseMenuButton.Draw();
			if(*UIControls::MainCameraTabButton.isChecked || *UIControls::MiscTabButton.isChecked || *UIControls::SsaoTabButton.isChecked || *UIControls::DofTabButton.isChecked || *UIControls::SunSkyTabButton.isChecked || *UIControls::StreamsTabButton.isChecked || *UIControls::WeaponTabButton.isChecked)
				UIControls::AddToCFGButton.Draw();
			if(*UIControls::MainTabButton.isChecked)
				UIControls::UI_TimelineSlider.Draw();


			if (*UIControls::MainTabButton.isChecked || *UIControls::BoneCameraTabButton.isChecked || *UIControls::WeaponTabButton.isChecked)
			{
				char buffer[64];
				sprintf_s(buffer, "Player: ^3%s", T6SDK::Addresses::cg->client[T6SDK::Dvars::GetInt(*T6SDK::Dvars::DvarList::demo_client)].szName);
				UIControls::UI_DemoClient.Text = buffer;
				UIControls::UI_DemoClient.Draw();
			}
		}
		if (!T6SDK::Theater::IsInTheater() && !T6SDK::Input::BlankMenuOpened)
		{
			if (T6SDK::MAIN::ENABLED && T6SDK::Addresses::GameMode.Value() == 32)
				UIControls::UI_DemoBrowseCheckButton.Draw();
		}
		if (!Streams::ScreenshotRequested && !Streams::IsAnyOtherStream && Streams::IsStreamsRunning)
		{
			Streams::ShowProgressOnScreen();
		}
	}
	void OnSunInited()
	{
		SunMenu::Init();
	}
	inline static void Init()
	{
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnEndFrameDrawn, (uintptr_t)&OnEndFrameDrawn);
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnKeyPressed, (uintptr_t)&OnKeyPressed);
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnDemoPlaybackInited, (uintptr_t)&OnSunInited);
		//Init buttons and sliders here
		UIControls::Init();
		CameraMenu::Init();
		MiscMenu::Init();
		BoneCameraMenu::Init();
		SsaoMenu::Init();
		DofMenu::Init();
		LightsMenu::Init();
		StreamsMenu::Init();
		WeaponMenu::Init();
		UIControls::CloseMenuButton = T6SDK::Drawing::UI_ClickableButton("^3TAB ^7Back", 2, 35, T6SDK::AnchorPoint::TopLeft, (uintptr_t)&UIBase::CloseMenu);
		UIControls::CloseMenuButton.ToolTip = "Press ^3TAB ^7or ^3ESC ^7 or just click here to close the menu.";

		UIControls::AddToCFGButton = T6SDK::Drawing::UI_ClickableButton("Copy CFG to clipboard", 8, 35, T6SDK::AnchorPoint::TopCenter, (uintptr_t)&UIBase::CopyCFGtoClipboard);
		UIControls::AddToCFGButton.ToolTip = "^7Copy the current settings to clipboard as ^5CFG ^7commands.";

		UIControls::UI_TimelineSlider = UI_TimelineNS::UI_Timeline(99999, 8, 28, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopCenter);
		UIControls::UI_DemoClient = T6SDK::Drawing::UI_EnumButton("Demo client", 0, 32, &(*T6SDK::Dvars::DvarList::demo_client)->current.integer, 8, 33, T6SDK::AnchorPoint::TopCenter, 0x00);
	}
}