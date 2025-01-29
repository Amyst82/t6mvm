#pragma once
#include <StdInclude.h>
#include "UIBase.h"
namespace Lights
{
	class LightDef
	{
	public:
		bool spotLight = false; //If false, omni
		vec3_t org{};
		Matrix33_s dir{};
		float radius = 255.0f;
		float fovInnerFraction = 0.0f;
		float startRadius = 0.0f;
		float endRadius = 255.0f;
		float r	= 1.0f;
		float g = 0.0f;
		float b = 0.0f;
		float a = 1.0f;
	};
	static T6SDK::Theater::CustomCameraMode LIGHTMODE("LIGHTS", { "OMNI"});
	inline static T6SDK::Drawing::UI_KeyReactiveText SpawnLightButton{};
	inline static vector<LightDef> LightsList;
	inline static int SelectedLight = 0;
	inline static bool IsLightRepositioning = false;

	inline static bool IsLightsMode()
	{
		return T6SDK::Theater::IsInTheater() && T6SDK::Addresses::DemoPlayback.Value()->CameraMode == (T6SDK::DemoCameraMode)0x04;
	}
	bool CloseToLight()
	{
		if(!IsLightsMode())
			return false;
		if(IsLightRepositioning)
			return false;
		for (int i = 0; i < LightsList.size(); i++)
		{
			float threshhold = T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::demo_dollycamHighlightThreshholdDistance);
			if (T6SDK::InternalFunctions::Distance(LightsList[i].org, T6SDK::Addresses::DemoPlayback.Value()->FreeRoamCamera.Origin) < threshhold)
			{
				SelectedLight = i;
				return true;
			}
			//SelectedLight = -1;
		}
		return false;
	}

	static void DrawLightsControls()
	{
		if(!IsLightsMode())
			return;
		if (CloseToLight())
			SpawnLightButton.Text = "^7Press ^3F ^7to edit light";
		else if(IsLightRepositioning)
			SpawnLightButton.Text = "^7Press ^3F ^7to set new position";
		else
			SpawnLightButton.Text = "^7Press ^3F ^7to add light";
		SpawnLightButton.Draw();

		//Draw light text info
		for (int i = 0; i < LightsList.size(); i++)
		{
			LightDef light = LightsList[i];
			vec2_t spriteScreenPos{};

			if (T6SDK::InternalFunctions::WorldPosToScreenPos(&light.org, &spriteScreenPos))
			{
				char buffer[100];
				sprintf(buffer, "%s LIGHT #%i", light.spotLight ? "SPOT" : "OMNI", i + 1);
				T6SDK::Drawing::DrawTextAbsolute(buffer, spriteScreenPos.x, spriteScreenPos.y + 0.0f, 1.0f, tColor{ 1.0f, 1.0f, 1.0f, 1.0f }, T6SDK::AnchorPoint::Center, 0x00);
			}
		}
	}
	int HeadLightSelectedHandle = 0;

	static void Update()
	{
		if(!T6SDK::Drawing::CheckResources())
			return;
		//Draw lights here
		for (int i = 0; i < LightsList.size(); i++)
		{
			LightDef light = LightsList[i];
			if(light.spotLight)
			{
				int lightDef = (int)T6SDK::InternalFunctions::DB_FindXAssetHeader(T6SDK::XAssetType::LIGHT_DEF, "light_dynamic");
				T6SDK::InternalFunctions::R_AddSpotLightToScene(light.org, light.dir, light.radius, light.fovInnerFraction, light.startRadius, light.endRadius, light.r, light.g, light.b, light.a, lightDef);
			}
			else if(!light.spotLight)
			{
				T6SDK::InternalFunctions::R_AddOmniLightToScene(light.org, light.radius, light.r, light.g, light.b, light.a);
			}
			//Draw light bulbs
			FxSprite_t sprite{};
			sprite.Position.x = light.org.x;
			sprite.Position.y = light.org.y;
			sprite.Position.z = light.org.z;
			sprite.MaterialOffset = (int)T6SDK::Drawing::headicontalkballoon;
			sprite.minScreenRadius = T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::cg_headIconMinScreenRadius);
			sprite.Radius = sprite.minScreenRadius;
			float dist = T6SDK::InternalFunctions::Distance(T6SDK::Addresses::DemoPlayback.Value()->FreeRoamCamera.Origin, light.org);
			if(dist > 750.0f)
				dist = 750.0f;
			float visibleDist = 1.0f - (dist/750.0f);
			sprite.Radius = visibleDist * 0.1f;
			sprite.R = (BYTE)(light.r*255.0f);
			sprite.G = (BYTE)(light.g*255.0f);
			sprite.B = (BYTE)(light.b*255.0f);
			sprite.A = (BYTE)255;
			sprite.flags = -1;
			if(!T6SDK::Addresses::DemoPlayback.Value()->DemoHudHidden)
				T6SDK::InternalFunctions::FxSpriteAdd(&sprite);
		}


		//Draw light selection
		if (HeadLightSelectedHandle)
		{
			T6SDK::InternalFunctions::FX_ThroughWithEffect(HeadLightSelectedHandle, true);
		}
		if (IsLightsMode() && SelectedLight > -1 && LightsList.size() > 0)
		{
			LightDef light = LightsList[SelectedLight];
			HeadLightSelectedHandle = T6SDK::InternalFunctions::FX_PlayOrientedEffect(T6SDK::Drawing::FxCameraSelect, T6SDK::Addresses::cg->Tick, &light.org, &light.dir);
		}
	}
	static void AddEditLight()
	{
		if(!IsLightsMode())
			return;
		if(CloseToLight())
		{
			T6SDK::Input::OpenBlankMenu();
			UIControls::LightsTabButton.SetChecked();
		}
		else if(IsLightRepositioning)
		{
			LightsList[SelectedLight].org = T6SDK::Addresses::DemoPlayback.Value()->FreeRoamCamera.Origin;
			LightsList[SelectedLight].dir = T6SDK::Addresses::cg->RefDef.viewAxis;
			IsLightRepositioning = false;
		}
		else
		{
			LightDef newLight;
			if(T6SDK::Addresses::DemoPlayback.Value()->FreeCameraMode == (T6SDK::DemoFreeCameraMode)0x04)//OMNI
				newLight.spotLight = false;
			//else if( T6SDK::Addresses::DemoPlayback.Value()->FreeCameraMode == (T6SDK::DemoFreeCameraMode)0x05)//SPOT
			//	newLight.spotLight = true;
			newLight.org = T6SDK::Addresses::DemoPlayback.Value()->FreeRoamCamera.Origin;
			newLight.dir = T6SDK::Addresses::cg->RefDef.viewAxis;
			if (LightsList.size() > 0)
			{
				LightDef lastLight = LightsList[LightsList.size() - 1];
				newLight.radius = lastLight.radius;
				newLight.startRadius = lastLight.startRadius;
				newLight.endRadius = lastLight.endRadius;
				newLight.fovInnerFraction = lastLight.fovInnerFraction;
				newLight.r = lastLight.r;
				newLight.g = lastLight.g;
				newLight.b = lastLight.b;
				newLight.a = lastLight.a;
			}

			LightsList.push_back(newLight);
			T6SDK::ConsoleLog::LogFormatted("%s light added! Total lights: %i", newLight.spotLight ? "Spot" : "Omni", LightsList.size());
		}
	}
	static void RemoveLight()
	{
		if(SelectedLight == -1)
			return;
		LightsList.erase(LightsList.begin() + SelectedLight);
	}
	static void RemoveAllLights()
	{
		LightsList.clear();
		SelectedLight = -1;
	}
	static void RepositionLight()
	{
		if(!IsLightsMode())
			return;
		(*T6SDK::Dvars::DvarList::r_blur)->current.value = 0.0f;
		T6SDK::Input::CloseBlankMenu();
		if(SelectedLight != -1)
			IsLightRepositioning = true;
	}
	static void ExportLights()
	{

	}
	static void ImportLights()
	{

	}
	inline static void Init()
	{
		T6SDK::Theater::RegisterCameraMode(&LIGHTMODE);
		SpawnLightButton = T6SDK::Drawing::UI_KeyReactiveText("^7Press ^3F ^7to add light", 8, 33, &T6SDK::Input::Keys::F, 0x00, T6SDK::AnchorPoint::BottomCenter, (uintptr_t)&Lights::AddEditLight);
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnTheaterControlsDrawn, (uintptr_t)&DrawLightsControls);
		LightsList = {};
	}
}