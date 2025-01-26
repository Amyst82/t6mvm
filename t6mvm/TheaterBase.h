#pragma once
#include <StdInclude.h>
#include "CustomDvars.h"
#include "BoneCamera.h"
#include "Lights.h"
#include "Misc.h"
#include "FreeCamera.h"
#include "Weapon.h"
namespace TheaterBase
{
	void OnKeyPressed(BYTE key)
	{
		if(!T6SDK::Theater::IsInTheater())
			return;
		if (key == T6SDK::Input::Keys::M.KeyCode)
		{
			if (T6SDK::Input::Keys::CTRL.IsAnyPressState())
			{
				if(T6SDK::Addresses::DemoPlayback.Value()->DollyCamMarkerCount > 0)
					T6SDK::Theater::Demo_JumpToDollyCamMarkerTime(0);
			}
		}
		if (key == T6SDK::Input::Keys::F5.KeyCode)
		{
			Streams::StreamsSwitchState();
		}
	}
	void HandleSafeStringTranslate(const char** string)
	{
		if (strcmp(*string, "MENU_DEMO_DVR_RECORD") == 0)
		{
			*string = "T6MVM";
		}
		if(strcmp(*string, "MENU_DEMO_DVR_SCREENSHOT") == 0)
		{
			*string = "1ST MARKER";
		}
		if(strcmp(*string, "PLATFORM_DEMO_TAKE_SCREENSHOT_KEYBOARD") == 0)
		{
			*string = "^7Ctrl + ^3M";
		}
		if(strcmp(*string, "PLATFORM_DEMO_RECORD_KEYBOARD") == 0)
		{
			*string = "TAB";
		}
	}
	inline static void CameraModeChanged(int newMode)
	{
		BoneCamera::OnCameraModeChanged(newMode);
	}
	inline static void Init()
	{
		T6SDK::Theater::SetDemoTimescaleCustomStep(true, &CustomDvars::dvar_demoTimescaleStep->current.value);
		T6SDK::Theater::SetFreeRoamCameraSpeed(CustomDvars::dvar_freeRoamSpeed->current.value);
		T6SDK::Input::PreventScoreboardOpen(true);
		T6SDK::Addresses::Patches::DisableClipRecordingPatch.Patch();

		Camera::FreeCamera::Init();
		Misc::Init();
		BoneCamera::Init();
		Lights::Init();
		Weapon::Init();

		T6SDK::Events::RegisterListener(T6SDK::EventType::OnCameraModeChanged, (uintptr_t)&CameraModeChanged);
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnActiveFrameDrawn, (uintptr_t)&BoneCamera::Update);
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnActiveFrameDrawn, (uintptr_t)&Lights::Update);
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnKeyPressed, (uintptr_t)&OnKeyPressed);
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnSafeStringTranslated, (uintptr_t)&HandleSafeStringTranslate);
		T6SDK::Addresses::Patches::JumpToDollyCamMarkerPatch.Patch();
		//T6SDK::Theater::SetFreeRoamCameraSpeed(0.2f);
	}
}