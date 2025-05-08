#pragma once
#include <StdInclude.h>
#include "CustomDvars.h"
#include "BoneCamera.h"
#include "Lights.h"
#include "Misc.h"
#include "FreeCamera.h"
#include "DollyCamera.h"
#include "Weapon.h"
#include "Fog.h"
#include "HoldGun.h"

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
				Camera::DollyCamera::GoToFirstMarker();
			}
		}
		if (key == T6SDK::Input::Keys::F5.KeyCode)
		{
			Streams::StreamsSwitchState();
		}
		if(key == T6SDK::Input::Keys::F6.KeyCode)
		{
			Streams::AbortStreams();
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
	void HandleGameModeChanged(int mode)
	{
		if (mode == 32) //theater
		{
			T6SDK::InternalFunctions::GrabRefDefViewAxisMatrix(false);
			T6SDK::Theater::GrabFreeCameraAngles = false;
			T6SDK::Dvars::SetBool(CustomDvars::dvar_frozenCam, false);
			CustomDvars::dvar_frozenCam->modified = true;
			Camera::DollyCamera::PreventChangingRotation(false);
			Camera::DollyCamera::PreventChangingPosition(false);
			Common::CurrentLoadedDemo.Clear();
		}
		else
		{
			Common::CurrentLoadedDemo.Clear();
			if (T6SDK::Addresses::DemoEndTick.Value() > 0)
			{
				T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "DEMO", "Demo recording ended.");
			}
		}
	}

	inline static void Init()
	{
		T6SDK::Theater::SetDemoTimescaleCustomStep(true, &CustomDvars::dvar_demoTimescaleStep->current.value);
		T6SDK::Theater::SetFreeRoamCameraSpeed(CustomDvars::dvar_freeRoamSpeed->current.value);
		T6SDK::Input::PreventScoreboardOpen(true);
		T6SDK::Addresses::Patches::DisableClipRecordingPatch.Patch();

		Camera::FreeCamera::Init();
		Camera::DollyCamera::Init();
		Misc::Init();
		BoneCamera::Init();
		Lights::Init();
		Weapon::Init();

		T6SDK::Events::RegisterListener(T6SDK::EventType::OnCameraModeChanged, (uintptr_t)&CameraModeChanged);
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnActiveFrameDrawn, (uintptr_t)&BoneCamera::Update);
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnActiveFrameDrawn, (uintptr_t)&Lights::Update);
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnActiveFrameDrawn, (uintptr_t)&Fog::Update);
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnKeyPressed, (uintptr_t)&OnKeyPressed);
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnSafeStringTranslated, (uintptr_t)&HandleSafeStringTranslate);
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnGameModeChanged, (uintptr_t)&HandleGameModeChanged);
		T6SDK::Addresses::Patches::JumpToDollyCamMarkerPatch.Patch();
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnProcessEntity, (uintptr_t)&HoldGun::Update);
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnCgItemDrawn, (uintptr_t)&HoldGun::UpdateItem);
		//T6SDK::Events::RegisterListener(T6SDK::EventType::OnEndFrameDrawn, (uintptr_t)&HoldGun::OnEndScene); //Used for debugging Holdgun in dollycamera
		//T6SDK::Theater::SetFreeRoamCameraSpeed(0.2f);

		//T6SDK::Dvars::Cmd_AddCommandInternal("mvm_removeTimelineBookmarks", UIControls::UI_TimelineSlider.RemoveAllBookmarks, &UIControls::UI_TimelineSlider.cmd_removeBookmarks_VAR);
	}
}