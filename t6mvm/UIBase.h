#pragma once
#include <StdInclude.h>
#include "UIControls.h"
#include "BoneCameraMenu.h"
#include "LightsMenu.h"
#include "MiscMenu.h"
#include "WeaponMenu.h"
#include "StreamsMenu.h"
namespace UIBase
{
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
			BoneCameraMenu::Draw();
			MiscMenu::Draw();
			LightsMenu::Draw();
			StreamsMenu::Draw();
			WeaponMenu::Draw();
			UIControls::MenuBlurCheckBox.Draw();
			UIControls::CloseMenuButton.Draw();
		}

	}

	inline static void Init()
	{
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnEndFrameDrawn, (uintptr_t)&OnEndFrameDrawn);
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnKeyPressed, (uintptr_t)&OnKeyPressed);
		//Init buttons and sliders here
		UIControls::Init();
		MiscMenu::Init();
		BoneCameraMenu::Init();
		LightsMenu::Init();
		StreamsMenu::Init();
		WeaponMenu::Init();
		UIControls::CloseMenuButton = T6SDK::Drawing::UI_ClickableButton("^3TAB ^7Back", 2, 35, T6SDK::AnchorPoint::TopLeft, (uintptr_t)&UIBase::CloseMenu);
		UIControls::CloseMenuButton.ToolTip = "Press ^3TAB ^7or ^3ESC ^7 or just click here to close the menu.";
	}
}