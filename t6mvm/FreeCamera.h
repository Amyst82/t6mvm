#pragma once
#include <StdInclude.h>
#include "CustomDvars.h"
namespace Camera
{
	namespace FreeCamera
	{
		void CheckFreeRoamDvars()
		{
			if (CustomDvars::dvar_freeRoamSpeed->modified)
			{
				T6SDK::Theater::SetFreeRoamCameraSpeed(CustomDvars::dvar_freeRoamSpeed->current.value);
				CustomDvars::dvar_freeRoamSpeed->modified = false;
			}
		}
		void HandleRollAndFov(float mouseWheelDelta)
		{
			if (!T6SDK::Theater::IsInTheater())
				return;
			if (T6SDK::Addresses::DemoPlayback.Value()->CameraMode == T6SDK::DemoCameraMode::NONE || T6SDK::Addresses::DemoPlayback.Value()->CameraMode == T6SDK::DemoCameraMode::THIRDPERSON)
				return;
			if (T6SDK::Addresses::DemoPlayback.Value()->FreeCameraMode == T6SDK::DemoFreeCameraMode::DOLLY)
				return;
			if (T6SDK::Input::Keys::CTRL.Hold())
			{
				(*T6SDK::Dvars::DvarList::cg_fov)->current.value -= mouseWheelDelta * 5.0f;
				T6SDK::Addresses::DemoPlayback.Value()->fov = (*T6SDK::Dvars::DvarList::cg_fov)->current.value;

				if (T6SDK::Addresses::DemoPlayback.Value()->HighlightedDollyCamMarker != -1 && T6SDK::Addresses::DemoPlayback.Value()->RepositioningDollyCamMarker)
					T6SDK::Addresses::DemoPlayback.Value()->DollyCameraMarkers[T6SDK::Addresses::DemoPlayback.Value()->HighlightedDollyCamMarker].Fov = T6SDK::Addresses::DemoPlayback.Value()->fov;
				return;
			}
			T6SDK::Addresses::DemoPlayback.Value()->FreeRoamCamera.Angles.z += mouseWheelDelta * T6SDK::Dvars::GetFloat(CustomDvars::dvar_rollStep);
			if (T6SDK::Addresses::DemoPlayback.Value()->HighlightedDollyCamMarker != -1 && T6SDK::Addresses::DemoPlayback.Value()->RepositioningDollyCamMarker)
				T6SDK::Addresses::DemoPlayback.Value()->DollyCameraMarkers[T6SDK::Addresses::DemoPlayback.Value()->HighlightedDollyCamMarker].Roll = T6SDK::Addresses::DemoPlayback.Value()->FreeRoamCamera.Angles.z;
		}
		void OnMouseWheelDown()
		{
			HandleRollAndFov(-1.0f);
		}
		void OnMouseWheelUp()
		{
			HandleRollAndFov(1.0f);
		}
		void HandleChangeSpeedInput()
		{
			if (T6SDK::Input::IsInputLocked() || !T6SDK::Theater::IsInTheater() || T6SDK::Addresses::IsGameInFocus.Value() == 0)
				return;
			if (T6SDK::Addresses::DemoPlayback.Value()->CameraMode == T6SDK::DemoCameraMode::NONE || T6SDK::Addresses::DemoPlayback.Value()->CameraMode == T6SDK::DemoCameraMode::THIRDPERSON)
				return;
			if (T6SDK::Addresses::DemoPlayback.Value()->FreeCameraMode == T6SDK::DemoFreeCameraMode::DOLLY)
				return;
			if (T6SDK::Input::Keys::MINUS.Hold()) //Minus pressed
			{
				if (CustomDvars::dvar_freeRoamSpeed->current.value > 0.0f)
				{
					CustomDvars::dvar_freeRoamSpeed->current.value -= 0.0025f;
					CustomDvars::dvar_freeRoamSpeed->modified = true;
				}
				else
					CustomDvars::dvar_freeRoamSpeed->current.value = 0.000f;
			}
			else if (T6SDK::Input::Keys::PLUS.Hold()) //Plus pressed
			{
				if (CustomDvars::dvar_freeRoamSpeed->current.value < 10.0f)
				{
					CustomDvars::dvar_freeRoamSpeed->current.value += 0.0025f;
					CustomDvars::dvar_freeRoamSpeed->modified = true;
				}
				else
					CustomDvars::dvar_freeRoamSpeed->current.value = 10.0f;
			}

			if (T6SDK::Input::Keys::SHIFT.Hold())
			{
				T6SDK::Addresses::FreeRoamSpeed.SetValueSafe(0.017f * CustomDvars::dvar_accelerationFactor->current.value);
				//ConsoleLog::Log("Speeding up!");
			}
			else if (T6SDK::Input::Keys::CTRL.Hold())
			{
				T6SDK::Addresses::FreeRoamSpeed.SetValueSafe(0.017f * CustomDvars::dvar_slowingFactor->current.value);
				//ConsoleLog::Log("Slowing down!");
			}
			else
			{
				T6SDK::Theater::SetFreeRoamCameraSpeed(CustomDvars::dvar_freeRoamSpeed->current.value);
			}
		}
		void HandleSpectatingInput()
		{
			if (T6SDK::Input::IsInputLocked() || !T6SDK::Theater::IsInTheater() || T6SDK::Addresses::IsGameInFocus.Value() == 0)
				return;
			if (T6SDK::Addresses::DemoPlayback.Value()->CameraMode == T6SDK::DemoCameraMode::NONE || T6SDK::Addresses::DemoPlayback.Value()->CameraMode == T6SDK::DemoCameraMode::THIRDPERSON)
				return;
			if (T6SDK::Addresses::DemoPlayback.Value()->FreeCameraMode == T6SDK::DemoFreeCameraMode::DOLLY)
				return;
			float PI = 3.1415926535f;
			float freeRoamSpeed = T6SDK::Addresses::FreeRoamSpeed.Value();
			float x = 0, y = 0, z = 0;
			if (T6SDK::Input::Keys::W.Hold())
				x = 1;
			if (T6SDK::Input::Keys::A.Hold())
				y = 1;
			if (T6SDK::Input::Keys::S.Hold())
				x = -1;
			if (T6SDK::Input::Keys::D.Hold())
				y = -1;
			if (T6SDK::Input::Keys::E.Hold())
				z = 1;
			if (T6SDK::Input::Keys::Q.Hold())
				z = -1;

			vec3_t v = (T6SDK::Addresses::CameraForwardVector.Value() * x + T6SDK::Addresses::CameraRightVector.Value() * y + T6SDK::Addresses::CameraUpVector.Value() * z) * freeRoamSpeed * 500.0f;
			T6SDK::Addresses::DemoPlayback.Value()->FreeRoamCamera.Origin += v;

		}
		void OnTheaterControlsDrawn()
		{
			if (T6SDK::Input::IsInputLocked() || !T6SDK::Theater::IsInTheater() || T6SDK::Addresses::IsGameInFocus.Value() == 0)
				return;
			if (T6SDK::Addresses::DemoPlayback.Value()->CameraMode == T6SDK::DemoCameraMode::NONE || T6SDK::Addresses::DemoPlayback.Value()->CameraMode == T6SDK::DemoCameraMode::THIRDPERSON)
				return;
			if (T6SDK::Addresses::DemoPlayback.Value()->FreeCameraMode == T6SDK::DemoFreeCameraMode::DOLLY)
				return;
			char buffer[255];
			sprintf_s(buffer, "^7FreeRoamSpeed: %0.3f^7\n^3-  ^3+ ^7change speed.\n^3SHIFT ^7accelerate to: %0.3f\n^3CTRL ^7slowdown to: %0.3f", CustomDvars::dvar_freeRoamSpeed->current.value, CustomDvars::dvar_accelerationFactor->current.value, CustomDvars::dvar_slowingFactor->current.value);
			T6SDK::Drawing::DrawTextRelative(buffer, 0.06f,0.06f, 1.0f, T6SDK::Drawing::WHITECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		}
		bool FreeRoamPatched = false;
		static void Update()
		{
			if(!T6SDK::Theater::IsInTheater())
				return;
			if (T6SDK::Addresses::DemoPlayback.Value()->CameraMode == T6SDK::DemoCameraMode::NONE || T6SDK::Addresses::DemoPlayback.Value()->CameraMode == T6SDK::DemoCameraMode::THIRDPERSON)
				return;
			if (T6SDK::Addresses::DemoPlayback.Value()->FreeCameraMode == T6SDK::DemoFreeCameraMode::DOLLY)
				return;
			if(!FreeRoamPatched)
			{
				T6SDK::Addresses::Patches::PreventFreeRoamCameraWriting.Patch();
				FreeRoamPatched = true;
			}
			CheckFreeRoamDvars();
			HandleSpectatingInput();
			HandleChangeSpeedInput();
		}
		static void Init()
		{
			T6SDK::Events::RegisterListener(T6SDK::EventType::OnActiveFrameDrawn, (uintptr_t)&Update);
			T6SDK::Events::RegisterListener(T6SDK::EventType::OnTheaterControlsDrawn, (uintptr_t)&OnTheaterControlsDrawn);
			T6SDK::Events::RegisterListener(T6SDK::EventType::OnMouseWheelDown, (uintptr_t)&OnMouseWheelDown);
			T6SDK::Events::RegisterListener(T6SDK::EventType::OnMouseWheelUp, (uintptr_t)&OnMouseWheelUp);		
		}
	}
}