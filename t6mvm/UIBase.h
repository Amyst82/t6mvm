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
namespace UIBase
{
	static void CopyCFGtoClipboard()
	{
		bool result = false;
		if (*UIControls::MainCameraTabButton.isChecked)
			result = T6SDK::InternalFunctions::SetClipboardText(CameraMenu::GetCFG());
		else if (*UIControls::MiscTabButton.isChecked)
			result = T6SDK::InternalFunctions::SetClipboardText(MiscMenu::GetCFG());
		else if (*UIControls::SunSkyTabButton.isChecked)
			result = T6SDK::InternalFunctions::SetClipboardText(SunMenu::GetCFG());
		else if (*UIControls::StreamsTabButton.isChecked)
			result = T6SDK::InternalFunctions::SetClipboardText(StreamsMenu::GetCFG());
		else if (*UIControls::WeaponTabButton.isChecked)
			result = T6SDK::InternalFunctions::SetClipboardText(WeaponMenu::GetCFG());
		if(!result)
			T6SDK::ConsoleLog::LogError("Failed to copy to clipboard.");
		else 
			T6SDK::ConsoleLog::LogSuccess("Copied to clipboard.");
	}
	static void CloseMenu()
	{
		(*T6SDK::Dvars::DvarList::r_blur)->current.value = 0.0f;
		T6SDK::Input::CloseBlankMenu();
	}
	static void OpenMenu()
	{
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
				T6SDK::ConsoleLog::LogError("Console is opened! Aborting.");
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
		if (!T6SDK::Theater::IsInTheater() || T6SDK::Input::BlankMenuOpened)
		{
			T6SDK::Drawing::DrawTextAbsolute("T6MVM v1.0.0 build 1337", 20.0f, 10.0f, 1.0f, tColor{ 1.0f, 1.0f, 1.0f, 0.2f }, T6SDK::AnchorPoint::TopLeft, 0x00);
			//return;
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
			SunMenu::Draw();
			MiscMenu::Draw();
			LightsMenu::Draw();
			StreamsMenu::Draw();
			WeaponMenu::Draw();
			UIControls::MenuBlurCheckBox.Draw();
			UIControls::CloseMenuButton.Draw();
			UIControls::AddToCFGButton.Draw(*UIControls::MainCameraTabButton.isChecked || *UIControls::MiscTabButton.isChecked || *UIControls::SunSkyTabButton.isChecked || *UIControls::StreamsTabButton.isChecked || *UIControls::WeaponTabButton.isChecked);
		}
		if (!Streams::ScreenshotRequested && !Streams::IsAnyOtherStream && Streams::IsStreamsRunning)
		{
			Streams::ShowProgressOnScreen();
		}
	}

	inline static void Init()
	{
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnEndFrameDrawn, (uintptr_t)&OnEndFrameDrawn);
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnKeyPressed, (uintptr_t)&OnKeyPressed);
		//Init buttons and sliders here
		UIControls::Init();
		CameraMenu::Init();
		MiscMenu::Init();
		BoneCameraMenu::Init();
		SunMenu::Init();
		LightsMenu::Init();
		StreamsMenu::Init();
		WeaponMenu::Init();
		UIControls::CloseMenuButton = T6SDK::Drawing::UI_ClickableButton("^3TAB ^7Back", 2, 35, T6SDK::AnchorPoint::TopLeft, (uintptr_t)&UIBase::CloseMenu);
		UIControls::CloseMenuButton.ToolTip = "Press ^3TAB ^7or ^3ESC ^7 or just click here to close the menu.";

		UIControls::AddToCFGButton = T6SDK::Drawing::UI_ClickableButton("Copy CFG to clipboard", 8, 35, T6SDK::AnchorPoint::TopCenter, (uintptr_t)&UIBase::CopyCFGtoClipboard);
		UIControls::AddToCFGButton.ToolTip = "^7Copy the current settings to clipboard as ^5CFG ^7commands.";
	}
}