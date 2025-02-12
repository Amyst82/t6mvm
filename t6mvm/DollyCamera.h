#pragma once
#include <StdInclude.h>
#include "CustomDvars.h"
#include "Shake.h"
namespace Camera
{
	namespace DollyCamera
	{
		inline static vector<CameraWayPoint_s> InterpoldatedValues{};
		float PI = 3.1415926535f;
		inline static int t = 0;
		Shake shake0(69.0f);
		Shake shake1(420.0f);
		Shake shake2(1337.0f);

#pragma region Interpolators

		float lerpf(float a, float b, float f)
		{
			return a * (1.0 - f) + (b * f);
		}
		CameraWayPoint_s lerp(CameraWayPoint_s a, CameraWayPoint_s b, float f)
		{
			return a * (1.0f - f) + (b * f);
		}
		CameraWayPoint_s Remap(float a, float b, const CameraWayPoint_s& c, const CameraWayPoint_s& d, float u)
		{
			return lerp(c, d, (u - a) / (b - a));
		}
		CameraWayPoint_s CentripetalCatmullRomInterpolate(float t, const CameraWayPoint_s& P0, const CameraWayPoint_s& P1, const CameraWayPoint_s& P2, const CameraWayPoint_s& P3)
		{
			float alpha = 0.25f; // alpha is 0.5 for centripetal catmull rom
			//Calculate knots
			const float k0 = 0;
			float k1 = std::powf((P0 - P1).LengthSquared(), 0.5f * alpha);//getKnotInterval(p0_, p1_);
			float k2 = std::powf((P1 - P2).LengthSquared(), 0.5f * alpha) + k1;//getKnotInterval(p1_, p2_) + k1;
			float k3 = std::powf((P2 - P3).LengthSquared(), 0.5f * alpha) + k2;//getKnotInterval(p2_, p3_) + k2;

			//evaluate the point
			float u = lerpf(k1, k2, t);
			CameraWayPoint_s A1 = Remap(k0, k1, P0, P1, u);
			CameraWayPoint_s A2 = Remap(k1, k2, P1, P2, u);
			CameraWayPoint_s A3 = Remap(k2, k3, P2, P3, u);
			CameraWayPoint_s B1 = Remap(k0, k2, A1, A2, u);
			CameraWayPoint_s B2 = Remap(k1, k3, A2, A3, u);
			return Remap(k1, k2, B1, B2, u);
		}
#pragma endregion
		void CheckDollyCamDvars()
		{
			if (!T6SDK::Theater::IsInTheater())
				return;
			if (CustomDvars::dvar_frozenCam->modified)
			{
				if (T6SDK::Dvars::GetBool(CustomDvars::dvar_frozenCam))
				{
					T6SDK::Addresses::Patches::PreventDemoNavigation.Patch();
					LocalAddresses::FakeTickDetour1.Patch();
					LocalAddresses::FakeTickDetour2.Patch();
					LocalAddresses::InitialTickStopPatch.Patch();
				}
				else //if not frozen cam
				{
					T6SDK::Addresses::Patches::PreventDemoNavigation.UnPatch();
					LocalAddresses::FakeTickDetour1.UnPatch();
					LocalAddresses::FakeTickDetour2.UnPatch();
					LocalAddresses::InitialTickStopPatch.UnPatch();
				}
				CustomDvars::dvar_frozenCam->modified = false;
			}
		}
		void OnTheaterControlsDrawn()
		{
			if (T6SDK::Input::IsInputLocked() || !T6SDK::Theater::IsInTheater() || T6SDK::Addresses::IsGameInFocus.Value() == 0)
				return;

			if (T6SDK::Addresses::DemoPlayback.Value()->FreeCameraMode != T6SDK::DemoFreeCameraMode::DOLLY)
				return;

		}
		vec3_t CalculateYawPitch(vec3_t p, vec3_t d)
		{
			float pitch = -atan2f(p.x - d.x, p.y - d.y) / PI * 180.0f + 270.0f;
			if (pitch > 360.0f)
				pitch -= 360.0f;
			if (pitch < 0.0f)
				pitch += 360.0f;
			float yaw = -asinf((d.z - p.z) / vec3_t().Distance(p, d)) / PI * 180.0f;
			if (yaw > 360.0f)
				yaw -= 360.0f;
			if (yaw < 0.0f)
				yaw += 360.0f;
			return vec3_t(yaw, pitch, 0.0f);

		}
		static bool CreateCamera()
		{
			InterpoldatedValues.clear();
			if (!T6SDK::Theater::IsInTheater())
				return false;
			int MarkersCount = T6SDK::Addresses::DemoPlayback.Value()->DollyCamMarkerCount;
			if (MarkersCount < 2)
				return false;
			int tickLength = T6SDK::Addresses::DemoPlayback.Value()->DollyCameraMarkers[MarkersCount - 1].Tick - T6SDK::Addresses::DemoPlayback.Value()->DollyCameraMarkers[0].Tick;

			//Creating a list of way points
			vector<CameraWayPoint_s> _wayPoints{};
			_wayPoints.clear();
			_wayPoints.push_back(-0.1f + CameraWayPoint_s(T6SDK::Addresses::DemoPlayback.Value()->DollyCameraMarkers[0].Position, T6SDK::Addresses::DemoPlayback.Value()->DollyCameraMarkers[0].Direction, T6SDK::Addresses::DemoPlayback.Value()->DollyCameraMarkers[0].Roll, T6SDK::Addresses::DemoPlayback.Value()->DollyCameraMarkers[0].Fov)); //pushing first marker
			for (int i = 0; i < MarkersCount; i++)
			{
				_wayPoints.push_back(CameraWayPoint_s(T6SDK::Addresses::DemoPlayback.Value()->DollyCameraMarkers[i].Position, T6SDK::Addresses::DemoPlayback.Value()->DollyCameraMarkers[i].Direction, T6SDK::Addresses::DemoPlayback.Value()->DollyCameraMarkers[i].Roll, T6SDK::Addresses::DemoPlayback.Value()->DollyCameraMarkers[i].Fov)); //pushing all markers
			}
			int lastMarkerIndex = MarkersCount - 1;
			_wayPoints.push_back(0.1f + CameraWayPoint_s(T6SDK::Addresses::DemoPlayback.Value()->DollyCameraMarkers[lastMarkerIndex].Position, T6SDK::Addresses::DemoPlayback.Value()->DollyCameraMarkers[lastMarkerIndex].Direction, T6SDK::Addresses::DemoPlayback.Value()->DollyCameraMarkers[lastMarkerIndex].Roll, T6SDK::Addresses::DemoPlayback.Value()->DollyCameraMarkers[lastMarkerIndex].Fov)); //pushing last marker

			for (int j = 0; j < _wayPoints.size() - 3; j++)
			{
				int steps = T6SDK::Addresses::DemoPlayback.Value()->DollyCameraMarkers[j + 1].Tick - T6SDK::Addresses::DemoPlayback.Value()->DollyCameraMarkers[j].Tick;
				for (int i = 0; i < steps; i++)
				{
					float u = (float)i / (float)steps;
					InterpoldatedValues.push_back(CentripetalCatmullRomInterpolate(u, _wayPoints[j], _wayPoints[j + 1], _wayPoints[j + 2], _wayPoints[j + 3]));
				}
			}
			return InterpoldatedValues.size() > 0; //return true if camera is not empty
		}
		static void PovCameraUpdate()
		{
			if (!T6SDK::Theater::IsInTheater())
				return;
			//if pov
			if (T6SDK::Addresses::DemoPlayback.Value()->CameraMode == T6SDK::DemoCameraMode::NONE)
			{
				if (CustomDvars::dvar_cameraSway->current.enabled)
				{
					T6SDK::Addresses::cg->RefDefViewAngles.z = min(CustomDvars::dvar_cameraSwayMaxAngle->current.value * T6SDK::Addresses::cg->swayAngles.x, CustomDvars::dvar_cameraSwayMaxAngle->current.value);
				}

			}
		}
		static void Update()
		{
			if (!T6SDK::Theater::IsInTheater())
				return;

			if (T6SDK::Addresses::DemoPlayback.Value()->CameraMode != T6SDK::DemoCameraMode::FREECAM || T6SDK::Addresses::DemoPlayback.Value()->FreeCameraMode != T6SDK::DemoFreeCameraMode::DOLLY || InterpoldatedValues.size() == 0 || T6SDK::Addresses::DemoPlayback.Value()->DollyCamMarkerCount == 0)
			{
				return;
			}
			

			bool frozenCam = T6SDK::Dvars::GetBool(CustomDvars::dvar_frozenCam);
			bool shakeEnabled = T6SDK::Dvars::GetBool(CustomDvars::dvar_shake);
			float shakeAmount = T6SDK::Dvars::GetFloat(CustomDvars::dvar_shakeAmount);
			float posShakeAmount = T6SDK::Dvars::GetFloat(CustomDvars::dvar_shakePosition);
			float rotShakeAmount = T6SDK::Dvars::GetFloat(CustomDvars::dvar_shakeRotation);
			float shakeSpeed = T6SDK::Dvars::GetFloat(CustomDvars::dvar_shakeSpeed);
			vec3_t camPosOffset = T6SDK::Dvars::GetVec3(CustomDvars::dvar_dollyCamOffsetPos);
			vec3_t camRotOffset = T6SDK::Dvars::GetVec3(CustomDvars::dvar_dollyCamOffsetRot);

			if (frozenCam)
			{
				if (T6SDK::Addresses::IsDemoPaused.Value() == 0)
				{
					float _timescale = T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::timescale) * T6SDK::Addresses::Demo_timescale.Value();
					t = T6SDK::Addresses::Tick.Value();
				}
			}
			else
			{
				t = T6SDK::Addresses::cg->Tick;
			}

			int MarkersCount = T6SDK::Addresses::DemoPlayback.Value()->DollyCamMarkerCount;
			if (t < T6SDK::Addresses::DemoPlayback.Value()->DollyCameraMarkers[0].Tick)
				t = T6SDK::Addresses::DemoPlayback.Value()->DollyCameraMarkers[0].Tick;
			int increment = t - T6SDK::Addresses::DemoPlayback.Value()->DollyCameraMarkers[0].Tick;
			if (increment < 0)
				return;
			if (t >= T6SDK::Addresses::DemoPlayback.Value()->DollyCameraMarkers[MarkersCount - 1].Tick)
			{
				if (frozenCam)
				{
					T6SDK::Addresses::IsDemoPaused.SetValueSafe(1);
					t = 0;
					Streams::StopStreams();
					return;
				}
				t = T6SDK::Addresses::DemoPlayback.Value()->DollyCameraMarkers[MarkersCount - 1].Tick;

			}
			if (increment >= InterpoldatedValues.size())
			{
				increment = InterpoldatedValues.size() - 1;
			}
			float shake_time = shakeSpeed * ((float)t / (float)T6SDK::Addresses::DemoPlayback.Value()->DollyCameraMarkers[MarkersCount - 1].Tick) * 10.0f;

			//Writing position
			float ShakedX = shake2.GetShakeOutput(shake_time) * (posShakeAmount * shakeAmount);
			float ShakedY = shake1.GetShakeOutput(shake_time) * (posShakeAmount * shakeAmount);
			float ShakedZ = shake0.GetShakeOutput(shake_time) * (posShakeAmount * shakeAmount);
			T6SDK::Addresses::DemoPlayback.Value()->FreeRoamCamera.Origin.x = InterpoldatedValues[increment].src.x + (shakeEnabled == true ? ShakedX : 0.0f) + camPosOffset.x;
			T6SDK::Addresses::DemoPlayback.Value()->FreeRoamCamera.Origin.y = InterpoldatedValues[increment].src.y + (shakeEnabled == true ? ShakedY : 0.0f) + camPosOffset.y;
			T6SDK::Addresses::DemoPlayback.Value()->FreeRoamCamera.Origin.z = InterpoldatedValues[increment].src.z + (shakeEnabled == true ? ShakedZ : 0.0f) + camPosOffset.z;

			//Calculating yaw and pitch
			vec3_t angles = CalculateYawPitch(InterpoldatedValues[increment].src, InterpoldatedValues[increment].dst);
			float ShakedYaw = shake0.GetShakeOutput(shake_time) * (rotShakeAmount * shakeAmount);
			float ShakedPitch = shake1.GetShakeOutput(shake_time) * (rotShakeAmount * shakeAmount);
			float ShakedRoll = shake2.GetShakeOutput(shake_time) * (rotShakeAmount * shakeAmount);
			
			T6SDK::Addresses::DemoPlayback.Value()->FreeRoamCamera.Angles.x = angles.x + (shakeEnabled == true ? ShakedYaw : 0.0f) + camRotOffset.x;
			T6SDK::Addresses::DemoPlayback.Value()->FreeRoamCamera.Angles.y = angles.y + (shakeEnabled == true ? ShakedPitch : 0.0f) + camRotOffset.y;
			T6SDK::Addresses::DemoPlayback.Value()->FreeRoamCamera.Angles.z = InterpoldatedValues[increment].roll + (shakeEnabled == true ? ShakedRoll : 0.0f) + camRotOffset.z;

			T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::cg_fov, InterpoldatedValues[increment].fov);
			T6SDK::Addresses::DemoPlayback.Value()->fov = InterpoldatedValues[increment].fov;
		}

		static void OnMarkerAdded(CameraMarker_s* marker)
		{
			if (T6SDK::Dvars::GetBool(CustomDvars::dvar_frozenCam))
			{
				T6SDK::Addresses::cg->FakeTick += 500;
			}
		}
	
		void PreventChangingRotation(bool state)
		{
			if (state == true)
			{
				T6SDK::Memory::NOP(T6SDK::Addresses::Patches::PreventDollyCamAnglesWriting.EndPointerAddress(), 14);
			}
			else
			{
				BYTE* btArray = new BYTE[14]{ 0xF3, 0x0F, 0x11, 0x10, 0xF3, 0x0F, 0x11, 0x40, 0x04, 0xF3, 0x0F, 0x11, 0x48, 0x08 }; //original bytes of what writes to rotation
				T6SDK::Memory::MemoryCopySafe((LPVOID)(T6SDK::Addresses::Patches::PreventDollyCamAnglesWriting.EndPointerAddress()), btArray, 14);
			}
		}
		void PreventChangingPosition(bool state)
		{
			if (state == true)
			{
				T6SDK::Memory::NOP(T6SDK::Addresses::Patches::PreventDollyCamPositonWriting.EndPointerAddress(), 33);
			}
			else
			{
				BYTE* btArray = new BYTE[33]{ 0xF3, 0x0F, 0x11, 0x7D, 0x00, 0xF3, 0x0F, 0x11, 0x6D, 0x04, 0xF3, 0x0F, 0x11, 0x75, 0x08, 0xF3, 0x0F, 0x11, 0x7C, 0x24, 0x20, 0xF3, 0x0F, 0x11, 0x6C, 0x24, 0x24, 0xF3, 0x0F, 0x11, 0x74, 0x24, 0x28 }; //original bytes of what writes to position
				T6SDK::Memory::MemoryCopySafe((LPVOID)(T6SDK::Addresses::Patches::PreventDollyCamPositonWriting.EndPointerAddress()), btArray, 33);
			}
		}

		static void OnFreeCameraModeChanged(int mode)
		{
			if ((T6SDK::DemoFreeCameraMode)mode == T6SDK::DemoFreeCameraMode::DOLLY)
			{
				PreventChangingRotation(true);
				PreventChangingPosition(true);
				CreateCamera();
			}
			else
			{
				PreventChangingRotation(false);
				PreventChangingPosition(false);
			}
		}
		void HandleOnTickChanged()
		{
			CheckDollyCamDvars();
			if (T6SDK::Addresses::DemoPlayback.Value()->FreeCameraMode == T6SDK::DemoFreeCameraMode::DOLLY) //If camera mode is DollyCam
			{
				Update();
			}
			else if (T6SDK::Addresses::DemoPlayback.Value()->FreeCameraMode == T6SDK::DemoFreeCameraMode::FREEROAM || T6SDK::Addresses::DemoPlayback.Value()->FreeCameraMode == T6SDK::DemoFreeCameraMode::EDIT)
			{
				if(T6SDK::Addresses::DemoPlayback.Value()->DollyCamMarkerCount < 50)
				{
					T6SDK::Addresses::DemoPlayback.Value()->DollyCameraMarkers[T6SDK::Addresses::DemoPlayback.Value()->DollyCamMarkerCount].Fov = (*T6SDK::Dvars::DvarList::cg_fov)->current.value;
					T6SDK::Addresses::DemoPlayback.Value()->DollyCameraMarkers[T6SDK::Addresses::DemoPlayback.Value()->DollyCamMarkerCount].Roll = T6SDK::Addresses::DemoPlayback.Value()->FreeRoamCamera.Angles.z;
				}
			}
		}
		bool frozenCam = false;
		bool notPovMode = false;
		uintptr_t eaxTMP, ecxTMP, edxTMP, esiTMP, ediTMP, espTMP, ebpTMP;
		__declspec(naked) void OnTickChanged()
		{
			frozenCam = T6SDK::Dvars::GetBool(CustomDvars::dvar_frozenCam);
			notPovMode = T6SDK::Addresses::DemoPlayback.Value()->CameraMode != T6SDK::DemoCameraMode::NONE;
			__asm
			{
				mov[eaxTMP], eax
				mov[ecxTMP], ecx
				mov[edxTMP], edx
				mov[esiTMP], esi
				mov[ediTMP], edi
				mov[espTMP], esp
				mov[ebpTMP], ebp

				//Route pointer of main tick to a fake one so the game will be paused
				mov al, frozenCam
				cmp al, 1
				je L7
				mov[esi + 0x4808C], ebp
				mov al, notPovMode
				cmp al, 0
				je L7
				mov[esi + 0x4809C], ebp
				L7 :
				mov[esi + 0x480A4], ebp
				call HandleOnTickChanged
				mov eax, [eaxTMP]
				mov edx, [edxTMP]
				mov ecx, [ecxTMP]
				mov esi, [esiTMP]
				mov edi, [ediTMP]
				mov esp, [espTMP]
				mov ebp, [ebpTMP]
				jmp[T6SDK::Addresses::HookAddresses::h_TickChanged.JumpBackAddress]
			}
		}
		static void GoToFirstMarker()
		{
			if (T6SDK::Dvars::GetBool(CustomDvars::dvar_frozenCam))
			{
				T6SDK::Addresses::IsDemoPaused.Value() = 1;
				t = 0;
			}
			else
			{
				if (T6SDK::Addresses::DemoPlayback.Value()->DollyCamMarkerCount > 0)
					T6SDK::Theater::Demo_JumpToDollyCamMarkerTime(0);
					//T6SDK::Theater::Demo_JumpToTick(T6SDK::Addresses::DemoPlayback.Value()->DollyCameraMarkers[0].Tick);
			}
		}
		static void Init()
		{
			T6SDK::Addresses::HookAddresses::h_TickChanged.Hook(OnTickChanged);

			T6SDK::Events::RegisterListener(T6SDK::EventType::OnAxisToAngles, (uintptr_t)&PovCameraUpdate);
			T6SDK::Events::RegisterListener(T6SDK::EventType::OnTheaterControlsDrawn, (uintptr_t)&OnTheaterControlsDrawn);
			T6SDK::Events::RegisterListener(T6SDK::EventType::OnCameraMarkerAdded, (uintptr_t)&OnMarkerAdded);
			T6SDK::Events::RegisterListener(T6SDK::EventType::OnFreeCameraModeChanged, (uintptr_t)&OnFreeCameraModeChanged);
		}
	}
}