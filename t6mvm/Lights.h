#pragma once
#include <StdInclude.h>
#include <fstream>
#include <sstream>
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
		bool flicker = false;
	};
	static T6SDK::Theater::CustomCameraMode LIGHTMODE("LIGHTS", { "OMNI", "SPOT"});
	inline static T6SDK::Drawing::UI_KeyReactiveText SpawnLightButton{};
	inline static vector<LightDef> LightsList;
	inline static int SelectedLight = 0;
	int LastSelectedLight = -1;
	inline static bool IsLightRepositioning = false;
	inline static uintptr_t OnSelectedLightChanged = 0;
	typedef void func(int);
	cmd_function_s cmd_exportLights_VAR{};
	cmd_function_s cmd_importLights_VAR{};

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
				if(i != SelectedLight)
				{
					SelectedLight = i;
					LastSelectedLight = SelectedLight;
					T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, false, "LIGHTS", "Selected light: #%i", SelectedLight);
					func* f = (func*)OnSelectedLightChanged;
					if (f)
						f(SelectedLight);
					return true;
				}
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

    float GetSHakeOutput(float x, float randomNumber)  
    {  
       // Fractional part of x  
       auto fract = [](float val) -> float {  
           return val - floor(val);  
       };  

       // Pseudo-random value from sine function  
       auto rand = [fract](float val, float randomNumber) -> float {
           return fract(randomNumber * sinf(val));
       };  

       // Random value at integer steps  
       auto randStep = [rand](float val, float randomNumber) -> float {  // Capture 'rand' explicitly
           return rand(floor(val), randomNumber);
       };  

       // Smoothstep function (cubic interpolation)  
       auto smoothstep = [](float val) -> float {  
           return val * val * (1.0f - 50.0f * (val-1.0f));  
       };  

       // Smooth interpolation between random values  
       float fractional = fract(x);  
       float smooth = smoothstep(fractional);  

       // Interpolate between current and next random value  
       return randStep(x, randomNumber) * smooth + randStep(x + 1.0f, randomNumber) * (1.0f - smooth) + 10.0f;
    }
	float shakex = 0.0f;

	static void Update()
	{
		if(!T6SDK::Theater::IsInTheater())
			return;
		if(!T6SDK::Drawing::CheckResources())
			return;
		//Draw lights here
		for (int i = 0; i < LightsList.size(); i++)
		{
			LightDef light = LightsList[i];
			float rnd = 1.0;
			if (light.flicker)
			{
				int x = 999;
				int y = 1010;
				rnd = GetSHakeOutput(shakex, i+1)/10.0f;
			}
			if (shakex < 120.0f)
			{
				float _timescale = T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::timescale) * T6SDK::Addresses::Demo_timescale.Value();
				shakex += _timescale * 0.00175f;
			}
			else
				shakex = 0.0f;

			if(light.spotLight)
			{
				int lightDef = (int)T6SDK::InternalFunctions::DB_FindXAssetHeader(T6SDK::XAssetType::LIGHT_DEF, "light_dynamic");
				T6SDK::InternalFunctions::R_AddSpotLightToScene(light.org, light.dir, light.radius, light.fovInnerFraction, light.startRadius, light.endRadius, light.r, light.g, light.b, light.a *= rnd, lightDef);
			}
			else if(!light.spotLight)
			{
				T6SDK::InternalFunctions::R_AddOmniLightToScene(light.org, light.radius, light.r, light.g, light.b, light.a *= rnd);
			}
			if (T6SDK::Addresses::DemoPlayback.Value()->DemoHudHidden)
				continue;
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
			else if( T6SDK::Addresses::DemoPlayback.Value()->FreeCameraMode == (T6SDK::DemoFreeCameraMode)0x05)//SPOT
				newLight.spotLight = true;
			newLight.org = T6SDK::Addresses::DemoPlayback.Value()->FreeRoamCamera.Origin;
			newLight.dir = T6SDK::Addresses::cg->RefDef.viewAxis;
			//if (LightsList.size() > 0)
			//{
			//	LightDef lastLight = LightsList[LightsList.size() - 1];
			//	newLight.radius = lastLight.radius;
			//	newLight.startRadius = lastLight.startRadius;
			//	newLight.endRadius = lastLight.endRadius;
			//	newLight.fovInnerFraction = lastLight.fovInnerFraction;
			//	newLight.r = lastLight.r;
			//	newLight.g = lastLight.g;
			//	newLight.b = lastLight.b;
			//	newLight.a = lastLight.a;
			//}

			LightsList.push_back(newLight);
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_INFO, false, "LIGHTS", "%s light added! Total lights: %i", newLight.spotLight ? "Spot" : "Omni", LightsList.size());
		}
	}
	static void RemoveLight()
	{
		try
		{
			if (SelectedLight == -1)
				return;
			if (SelectedLight > LightsList.size())
				return;
			LightsList.erase(LightsList.begin() + SelectedLight);
			func* f = (func*)OnSelectedLightChanged;
			if (f)
				f(LightsList.size() - 1);
		}
		catch(const char* e)
		{
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, false, "LIGHTS", e);
		}
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
		char openedFileName[MAX_PATH];
		const TCHAR* FilterSpec = (const TCHAR*)"BO2 Lights list(.t6lights)\0*.t6lights*\0";
		const TCHAR* Title = (const TCHAR*)"Save current lights list";
		if (T6SDK::InternalFunctions::OpenFileDialog((TCHAR*)openedFileName, true, FilterSpec, Title))
		{
			string str(openedFileName);
			str.append(".t6lights");
			ofstream ExportFile(str, ios_base::out);
			if (ExportFile.good())
			{
				for (int i = 0; i < (int)LightsList.size(); i++)
				{
					char exportStringline[256];
					vec3_t dir{};
					T6SDK::InternalFunctions::AxisToAngles(&LightsList[i].dir, &dir);
					//TYPE;X;Y;Z;DIRX;DIRY;DIRZ;RADIUS;R;G;B;A
					sprintf(exportStringline, "%i;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%i",
						LightsList[i].spotLight, LightsList[i].org.x, LightsList[i].org.y, LightsList[i].org.z,
						dir.x, dir.y, dir.z,
						LightsList[i].radius, LightsList[i].r, LightsList[i].g, LightsList[i].b, LightsList[i].a, LightsList[i].flicker);

					ExportFile << exportStringline << endl;
				}
				ExportFile.close();
				char buffer[256];
				sprintf(buffer, "%i light(s) exported to %s.", (int)LightsList.size(), str);
				T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_SUCCESS, false, "LIGHTS", buffer);
			}
		}
		else
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_WARNING, false, "LIGHTS", "User cancelled light export operation");
	}
	static void ImportLights()
	{
		char openedFileName[MAX_PATH];
		const TCHAR* FilterSpec = (const TCHAR*)"BO2 Lights list(.t6lights)\0*.t6lights*\0";
		const TCHAR* Title = (const TCHAR*)"Load lights list";
		if (T6SDK::InternalFunctions::OpenFileDialog((TCHAR*)openedFileName, false, FilterSpec, Title))
		{
			string str(openedFileName);
			ofstream ImportFile(str, ios_base::in);
			if (ImportFile.good())
			{
				ifstream fileopen(str);
				int counter = 0;
				bool successStatus = false;
				vector<LightDef> tempLightsList{};
				if (fileopen.is_open())
				{
					try
					{
						stringstream buffer;
						buffer << fileopen.rdbuf();
						std::string line;
						stringstream s;
						std::string segment;
						int startTick = 0;
						while (std::getline(buffer, line))
						{
							if (line == "")
								continue;
							s.clear();
							s << line;
							segment.clear();
							vector<string> list;
							while (std::getline(s, segment, ';'))
							{
								list.push_back(segment);
							}
							if (list.size() == 12)
							{
								LightDef tmpLight{};
								tmpLight.spotLight = atoi(list[0].c_str()) == 1 ? true : false;
								tmpLight.org.x = stof(list[1].c_str());
								tmpLight.org.y = stof(list[2].c_str());
								tmpLight.org.z = stof(list[3].c_str());
								tmpLight.dir;
								vec3_t lightrDir = { stof(list[4].c_str()) , stof(list[5].c_str()) , stof(list[6].c_str()) };
								T6SDK::InternalFunctions::AnglesToAxis(&lightrDir, &tmpLight.dir);
								tmpLight.radius = stof(list[7].c_str());
								tmpLight.r = stof(list[8].c_str());
								tmpLight.g = stof(list[9].c_str());
								tmpLight.b = stof(list[10].c_str());
								tmpLight.a = stof(list[11].c_str());
								tempLightsList.push_back(tmpLight);
							}
							else if (list.size() == 13)
							{
								LightDef tmpLight{};
								tmpLight.spotLight = atoi(list[0].c_str()) == 1 ? true : false;
								tmpLight.org.x = stof(list[1].c_str());
								tmpLight.org.y = stof(list[2].c_str());
								tmpLight.org.z = stof(list[3].c_str());
								tmpLight.dir;
								vec3_t lightrDir = { stof(list[4].c_str()) , stof(list[5].c_str()) , stof(list[6].c_str()) };
								T6SDK::InternalFunctions::AnglesToAxis(&lightrDir, &tmpLight.dir);
								tmpLight.radius = stof(list[7].c_str());
								tmpLight.r = stof(list[8].c_str());
								tmpLight.g = stof(list[9].c_str());
								tmpLight.b = stof(list[10].c_str());
								tmpLight.a = stof(list[11].c_str());
								tmpLight.flicker = atoi(list[12].c_str()) == 1 ? true : false;
								tempLightsList.push_back(tmpLight);
							}
							else
							{
								T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, false, "LIGHTS", "Error during loading lights!");
								successStatus = false;
								tempLightsList.clear();
								break;
							}
							counter++;
						}
						successStatus = true;
					}
					catch (const char* error)
					{
						fileopen.close();
						T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, false, "LIGHTS", "Error occured during loading lights!");
						T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_WARNING, false, "LIGHTS", error);
						successStatus = false;
						tempLightsList.clear();
						return;
					}

				}
				if (successStatus)
				{
					if (tempLightsList.size() > 0)
					{
						LightsList.clear();
						for (int i = 0; i < tempLightsList.size(); i++)
						{
							LightsList.push_back(tempLightsList[i]);
						}
						tempLightsList.clear();
					}
					fileopen.close();
					T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_SUCCESS, false, "LIGHTS", "Lights were imported!");
				}
			}
			else
				T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_WARNING, false, "LIGHTS", "User cancelled light export operation");
		}
	}
	inline static void Init()
	{
		T6SDK::Theater::RegisterCameraMode(&LIGHTMODE);
		SpawnLightButton = T6SDK::Drawing::UI_KeyReactiveText("^7Press ^3F ^7to add light", 8, 33, &T6SDK::Input::Keys::F, 0x00, T6SDK::AnchorPoint::BottomCenter, (uintptr_t)&Lights::AddEditLight);
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnTheaterControlsDrawn, (uintptr_t)&DrawLightsControls);
		LightsList = {};
		T6SDK::Dvars::Cmd_AddCommandInternal("mvm_exportLights", ExportLights, &cmd_exportLights_VAR);
		T6SDK::Dvars::Cmd_AddCommandInternal("mvm_importLights", ImportLights, &cmd_importLights_VAR);
	}
}