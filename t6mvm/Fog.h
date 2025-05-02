#pragma once
#include <StdInclude.h>
#include "CustomDvars.h"

namespace Fog
{
	inline static vector<const char*> FogPresets = {
		"Aqua",
		"Denpa",
		"Foggy Day",
		"Forest Fire",
		"Nova",
		"Pitch Black",
		"Smog"
	};

	static void SetFogPreset(int preset)
	{
		std::string fogCfg = "";
		switch (preset)
		{
		case 0: //Aqua
			fogCfg = "z_fogStartDist 60; z_fogFadeDist 1000; z_fogStartHeight -400;	z_fogFadeHeight 500; z_fogOpacity 1; z_fogSunPitch 1; z_fogSunYaw 220.995; z_fogSunInner 0; z_fogSunOuter 29.68312;	z_fogColor 0.0392 0.058 0.0784;	z_fogSunOpacity 1; z_fogSunColor 0.0392 0.058 0.0784;";
			break;
		case 1: //Denpa
			fogCfg = "z_fogStartDist 100; z_fogFadeDist 10000; z_fogStartHeight -400; z_fogFadeHeight 50000; z_fogOpacity 1; z_fogSunPitch 1; z_fogSunYaw 220.995; z_fogSunInner 0; z_fogSunOuter 29.68312; z_fogColor 0.1569 0.0784 0.1176; z_fogSunOpacity 1; z_fogSunColor 0.1569 0.0784 0.1176;";
			break;
		case 2: //Foggy Day
			fogCfg = "z_fogStartDist 150; z_fogFadeDist 1000; z_fogStartHeight -400; z_fogFadeHeight 50000; z_fogOpacity 1; z_fogSunPitch 1; z_fogSunYaw 220.995; z_fogSunInner 0; z_fogSunOuter 29.68312; z_fogColor 0.0392 0.0392 0.0392; z_fogSunOpacity 1; z_fogSunColor 0.0392 0.0392 0.0392;";
			break;
		case 3: //Forest Fire
			fogCfg = "z_fogStartDist 100; z_fogFadeDist 3000; z_fogStartHeight -400; z_fogFadeHeight 1000; z_fogOpacity 1; z_fogSunPitch 1; z_fogSunYaw 220.995; z_fogSunInner 0; z_fogSunOuter 29.68312; z_fogColor 0.1569 0.0392 0.0204; z_fogSunOpacity 1; z_fogSunColor 0.1569 0.0392 0.0204;";
			break;
		case 4: //Nova
			fogCfg = "z_fogStartDist 79.56989; z_fogFadeDist 2056; z_fogStartHeight 13057; z_fogFadeHeight 69948; z_fogOpacity 0.5849853; z_fogSunPitch 89; z_fogSunYaw 152; z_fogSunInner 0; z_fogSunOuter 0; z_fogColor 0.0563 0.0437 0.2275; z_fogSunOpacity 0.01; z_fogSunColor 0.0563 0.0437 0.2275;";
			break;
		case 5: //Pitch Black
			fogCfg = "z_fogStartDist 50; z_fogFadeDist 100; z_fogStartHeight -400; z_fogFadeHeight 1200; z_fogOpacity 1; z_fogSunPitch 1; z_fogSunYaw 220.995; z_fogSunInner 0; z_fogSunOuter 29.68312; z_fogColor 0 0 0; z_fogSunOpacity 1; z_fogSunColor 0 0 0;";
			break;
		case 6: //Smog
			fogCfg = "z_fogStartDist 100; z_fogFadeDist 300; z_fogStartHeight -400; z_fogFadeHeight 10000; z_fogOpacity 1; z_fogSunPitch 1; z_fogSunYaw 220.995; z_fogSunInner 0; z_fogSunOuter 29.68312; z_fogColor 0.0047 0.0047 0.0047; z_fogSunOpacity 1; z_fogSunColor 0.0047 0.0047 0.0047;";
			break;
		default:
			fogCfg = "";
			break;
		}
		T6SDK::Dvars::cbuf_AddText(fogCfg.c_str());
	}

	static void LoadFogPresetFromFile()
	{
		char openedFileName[MAX_PATH];
		const TCHAR* FilterSpec = (const TCHAR*)"BO2 FOG (*.zfog)\0*.zfog*\0";
		const TCHAR* Title = (const TCHAR*)"Load fog from file";
		if (T6SDK::InternalFunctions::OpenFileDialog((TCHAR*)openedFileName, false, FilterSpec, Title))
		{
			string str(openedFileName);
			ofstream ImportFile(str, ios_base::in);
			if (ImportFile.good())
			{
				ifstream fileopen(str);
				int counter = 0;
				bool successStatus = false;
				if (fileopen.is_open())
				{
					try
					{
						T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, false, "FOG", "Fog file opened!");
						successStatus = true;
					}
					catch (exception ex)
					{
						{
							fileopen.close();
							T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, false, "FOG", "Error during loading fog file!");
							T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_WARNING, false, "FOG", ex.what());
							successStatus = false;
							return;
						}

					}
					if (successStatus)
					{
						fileopen.close();
						std::ifstream file(str);
						json data = json::parse(file);

						T6SDK::Addresses::fog->startDist = data.value("startDist", 0.0f);
						T6SDK::Addresses::fog->fadeDist = data.value("fadeDist", 0.0f);
						T6SDK::Addresses::fog->startHeight = data.value("startHeight", 0.0f);
						T6SDK::Addresses::fog->fadeHeight = data.value("fadeHeight", 0.0f);
						T6SDK::Addresses::fog->sunHazePitchAngle = data.value("sunHazePitchAngle", 0.0f);
						T6SDK::Addresses::fog->sunHazeYawAngle = data.value("sunHazeYawAngle", 0.0f);
						T6SDK::Addresses::fog->hazeInnerRadius = data.value("hazeInnerRadius", 0.0f);
						T6SDK::Addresses::fog->hazeOuterRadius = data.value("hazeOuterRadius", 0.0f);
						T6SDK::Addresses::fog->fogColor.x = data["fogColor"].value("r", 0.0f);
						T6SDK::Addresses::fog->fogColor.y = data["fogColor"].value("g", 0.0f);
						T6SDK::Addresses::fog->fogColor.z = data["fogColor"].value("b", 0.0f);
						T6SDK::Addresses::fog->fogOpacity = data.value("fogOpacity", 0.0f);
						T6SDK::Addresses::fog->sunHazeColor.x = data["sunHazeColor"].value("r", 0.0f);
						T6SDK::Addresses::fog->sunHazeColor.y = data["sunHazeColor"].value("g", 0.0f);
						T6SDK::Addresses::fog->sunHazeColor.z = data["sunHazeColor"].value("b", 0.0f);
						T6SDK::Addresses::fog->sunHazeOpacity = data.value("sunHazeOpacity", 0.0f);
						T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_SUCCESS, false, "FOG", "Fog preset loaded!");
					}
				}
			}
		}
		else
		{
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_WARNING, false, "FOG", "Fog file not found or use cancelled the operation!");
			return;
		}
	}
	static void SaveFogPresetToFile()
	{
		char openedFileName[MAX_PATH];
		const TCHAR* FilterSpec = (const TCHAR*)"BO2 FOG(.zfog)\0*.zfog*\0";
		const TCHAR* Title = (const TCHAR*)"Save current fog";
		if (T6SDK::InternalFunctions::OpenFileDialog((TCHAR*)openedFileName, true, FilterSpec, Title))
		{
			string str(openedFileName);
			str.append(".zfog");
			ofstream ExportFile(str, ios_base::out);
			if (ExportFile.good())
			{
				json fogData;
				fogData["startDist"] = T6SDK::Addresses::fog->startDist;
				fogData["fadeDist"] = T6SDK::Addresses::fog->fadeDist;
				fogData["startHeight"] = T6SDK::Addresses::fog->startHeight;
				fogData["fadeHeight"] = T6SDK::Addresses::fog->fadeHeight;
				fogData["sunHazePitchAngle"] = T6SDK::Addresses::fog->sunHazePitchAngle;
				fogData["sunHazeYawAngle"] = T6SDK::Addresses::fog->sunHazeYawAngle;
				fogData["hazeInnerRadius"] = T6SDK::Addresses::fog->hazeInnerRadius;
				fogData["hazeOuterRadius"] = T6SDK::Addresses::fog->hazeOuterRadius;
				fogData["fogColor"]["r"] = T6SDK::Addresses::fog->fogColor.x;
				fogData["fogColor"]["g"] = T6SDK::Addresses::fog->fogColor.y;
				fogData["fogColor"]["b"] = T6SDK::Addresses::fog->fogColor.z;
				fogData["fogOpacity"] = T6SDK::Addresses::fog->fogOpacity;
				fogData["sunHazeColor"]["r"] = T6SDK::Addresses::fog->sunHazeColor.x;
				fogData["sunHazeColor"]["g"] = T6SDK::Addresses::fog->sunHazeColor.y;
				fogData["sunHazeColor"]["b"] = T6SDK::Addresses::fog->sunHazeColor.z;
				fogData["sunHazeOpacity"] = T6SDK::Addresses::fog->sunHazeOpacity;
				ExportFile << std::setw(4) << fogData << std::endl;
				ExportFile.close();
				T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_SUCCESS, false, "FOG", "Fog was exported!");
			}
		}
		else
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_WARNING, false, "FOG", "User cancelled fog export operation");
	}

	static void CheckFogDvars()
	{
		#pragma region fog dvars
		if (CustomDvars::dvar_fogStartDist->modified)
		{
			T6SDK::Addresses::fog->startDist = CustomDvars::dvar_fogStartDist->current.value;
			CustomDvars::dvar_fogStartDist->modified = false;
		}
		if (CustomDvars::dvar_fogFadeDist->modified)
		{
			T6SDK::Addresses::fog->fadeDist = CustomDvars::dvar_fogFadeDist->current.value;
			CustomDvars::dvar_fogFadeDist->modified = false;
		}
		if (CustomDvars::dvar_fogStartHeight->modified)
		{
			T6SDK::Addresses::fog->startHeight = CustomDvars::dvar_fogStartHeight->current.value;
			CustomDvars::dvar_fogStartHeight->modified = false;
		}
		if (CustomDvars::dvar_fogFadeHeight->modified)
		{
			T6SDK::Addresses::fog->fadeHeight = CustomDvars::dvar_fogFadeHeight->current.value;
			CustomDvars::dvar_fogFadeHeight->modified = false;
		}
		if (CustomDvars::dvar_fogOpacity->modified)
		{
			T6SDK::Addresses::fog->fogOpacity = CustomDvars::dvar_fogOpacity->current.value;
			CustomDvars::dvar_fogOpacity->modified = false;
		}
		if (CustomDvars::dvar_fogSunOuter->modified)
		{
			T6SDK::Addresses::fog->hazeOuterRadius = CustomDvars::dvar_fogSunOuter->current.value;
			CustomDvars::dvar_fogSunOuter->modified = false;
		}
		if (CustomDvars::dvar_fogSunInner->modified)
		{
			T6SDK::Addresses::fog->hazeInnerRadius = CustomDvars::dvar_fogSunInner->current.value;
			CustomDvars::dvar_fogSunInner->modified = false;
		}
		if (CustomDvars::dvar_fogSunPitch->modified)
		{
			T6SDK::Addresses::fog->sunHazePitchAngle = CustomDvars::dvar_fogSunPitch->current.value;
			CustomDvars::dvar_fogSunPitch->modified = false;
		}
		if (CustomDvars::dvar_fogSunYaw->modified)
		{
			T6SDK::Addresses::fog->sunHazeYawAngle = CustomDvars::dvar_fogSunYaw->current.value;
			CustomDvars::dvar_fogSunYaw->modified = false;
		}
		if (CustomDvars::dvar_fogSunOpacity->modified)
		{
			T6SDK::Addresses::fog->sunHazeOpacity = CustomDvars::dvar_fogSunOpacity->current.value;
			CustomDvars::dvar_fogSunOpacity->modified = false;
		}
		if (CustomDvars::dvar_fogSunColor->modified)
		{
			T6SDK::Addresses::fog->sunHazeColor.x = CustomDvars::dvar_fogSunColor->current.vector[0] * 255.0f;
			T6SDK::Addresses::fog->sunHazeColor.y = CustomDvars::dvar_fogSunColor->current.vector[1] * 255.0f;
			T6SDK::Addresses::fog->sunHazeColor.z = CustomDvars::dvar_fogSunColor->current.vector[2] * 255.0f;
			CustomDvars::dvar_fogSunColor->modified = false;
		}
		if (CustomDvars::dvar_fogColor->modified)
		{
			T6SDK::Addresses::fog->fogColor.x = CustomDvars::dvar_fogColor->current.vector[0] * 255.0f;
			T6SDK::Addresses::fog->fogColor.y = CustomDvars::dvar_fogColor->current.vector[1] * 255.0f;
			T6SDK::Addresses::fog->fogColor.z = CustomDvars::dvar_fogColor->current.vector[2] * 255.0f;
			CustomDvars::dvar_fogColor->modified = false;
		}
#pragma endregion
	}
	static void Update()
	{
		CheckFogDvars();
	}
}