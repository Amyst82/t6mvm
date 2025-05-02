#pragma once
#include "UIControls.h"
#include "CustomDvars.h"
#include "Fog.h"
#include <math.h>
namespace FogMenu
{
	int fogPreset = 0;
	float roundUp4Digits(float value) {
		return std::ceil(value * 10000) / 10000;
	}
	static string GetCFG()
	{
		std::string cfg =
			"//FOG (compatible with BO2 Console V4)\n" +
			std::string("z_fogStartDist ") + std::to_string(T6SDK::Addresses::fog->startDist) + ";\n" +
			std::string("z_fogFadeDist ") + std::to_string(T6SDK::Addresses::fog->fadeDist) + ";\n" +
			std::string("z_fogStartHeight ") + std::to_string(T6SDK::Addresses::fog->startHeight) + ";\n" +
			std::string("z_fogFadeHeight ") + std::to_string(T6SDK::Addresses::fog->fadeHeight) + ";\n" +
			std::string("z_fogOpacity ") + std::to_string(T6SDK::Addresses::fog->fogOpacity) + ";\n" +
			std::string("z_fogSunPitch ") + std::to_string(T6SDK::Addresses::fog->sunHazePitchAngle) + ";\n" +
			std::string("z_fogSunYaw ") + std::to_string(T6SDK::Addresses::fog->sunHazeYawAngle) + ";\n" +
			std::string("z_fogSunInner ") + std::to_string(T6SDK::Addresses::fog->hazeInnerRadius) + ";\n" +
			std::string("z_fogSunOuter ") + std::to_string(T6SDK::Addresses::fog->hazeOuterRadius) + ";\n" +
			std::string("z_fogColor ") + std::to_string(roundUp4Digits(T6SDK::Addresses::fog->fogColor.x / 255.0f)) + " " + std::to_string(roundUp4Digits(T6SDK::Addresses::fog->fogColor.y / 255.0f)) + " " + std::to_string(roundUp4Digits(T6SDK::Addresses::fog->fogColor.z / 255.0f)) + ";\n" +
			std::string("z_fogSunOpacity ") + std::to_string(T6SDK::Addresses::fog->sunHazeOpacity) + ";\n" +
			std::string("z_fogSunColor ") + std::to_string(roundUp4Digits(T6SDK::Addresses::fog->sunHazeColor.x / 255.0f)) + " " + std::to_string(roundUp4Digits(T6SDK::Addresses::fog->sunHazeColor.y / 255.0f)) + " " + std::to_string(roundUp4Digits(T6SDK::Addresses::fog->sunHazeColor.z / 255.0f)) + ";\n";
		return cfg;
	}
	void ResetFog()
	{
		UIControls::UI_FogStart.Reset();
		UIControls::UI_FogFade.Reset();
		UIControls::UI_FogHeightStart.Reset();
		UIControls::UI_FogHeightFade.Reset();
		UIControls::UI_FogOpacity.Reset();
		UIControls::UI_FogSunHazeOuterRadius.Reset();
		UIControls::UI_FogSunHazeInnerRadius.Reset();
		UIControls::UI_FogSunHazeOpacity.Reset();
		UIControls::UI_FogSunHazeYaw.Reset();
		UIControls::UI_FogSunHazePitch.Reset();
		UIControls::UI_FogColorR.Reset();
		UIControls::UI_FogColorG.Reset();
		UIControls::UI_FogColorB.Reset();
		UIControls::UI_FogSunHazeColorR.Reset();
		UIControls::UI_FogSunHazeColorG.Reset();
		UIControls::UI_FogSunHazeColorB.Reset();
	}
	#pragma region Colors syncing
	bool SyncFogColors = false;
	void FogColorChanged()
	{
		if (*UIControls::UI_SyncFogColors.isChecked)
		{
			T6SDK::Addresses::fog->sunHazeColor = T6SDK::Addresses::fog->fogColor;
		}
	}
	void FogSunHazeColorChanged()
	{
		if (*UIControls::UI_SyncFogColors.isChecked)
		{
			T6SDK::Addresses::fog->fogColor = T6SDK::Addresses::fog->sunHazeColor;
		}
	}
#pragma endregion

	static void Init()
	{
		float defFogStart = T6SDK::Addresses::fog->startDist;
		UIControls::UI_FogStart = T6SDK::Drawing::UI_Slider("FOG START", (float*)&T6SDK::Addresses::fog->startDist, defFogStart, -1000.0f, 5000.0f, 4, 5, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);

		float defFogFade = T6SDK::Addresses::fog->fadeDist;
		UIControls::UI_FogFade = T6SDK::Drawing::UI_Slider("FOG FADE", (float*)&T6SDK::Addresses::fog->fadeDist, defFogFade, -1000.0f, 50000.0f, 7, 5, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);

		float defFogHeightStart = T6SDK::Addresses::fog->startHeight;
		UIControls::UI_FogHeightStart = T6SDK::Drawing::UI_Slider("FOG HEIGHT START", (float*)&T6SDK::Addresses::fog->startHeight, defFogHeightStart, -1000.0f, 50000.0f, 10, 5, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);

		float defFogHeightFade = T6SDK::Addresses::fog->fadeHeight;
		UIControls::UI_FogHeightFade = T6SDK::Drawing::UI_Slider("FOG HEIGHT FADE", (float*)&T6SDK::Addresses::fog->fadeHeight, defFogHeightFade, -1000.0f, 90000.0f, 4, 8, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);

		float defFogOpacity = T6SDK::Addresses::fog->fogOpacity;
		UIControls::UI_FogOpacity = T6SDK::Drawing::UI_Slider("FOG OPACITY", (float*)&T6SDK::Addresses::fog->fogOpacity, defFogOpacity, -10.0f, 10.0f, 7, 8, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);

		//sun haze
		float defHazeOuter = T6SDK::Addresses::fog->hazeOuterRadius;
		UIControls::UI_FogSunHazeOuterRadius = T6SDK::Drawing::UI_Slider("SUN HAZE OUTER RADIUS", (float*)&T6SDK::Addresses::fog->hazeOuterRadius, defHazeOuter, 0.0f, 60.0f, 4, 13, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);

		float defHazeInner = T6SDK::Addresses::fog->hazeInnerRadius;
		UIControls::UI_FogSunHazeInnerRadius = T6SDK::Drawing::UI_Slider("SUN HAZE INNER RADIUS", (float*)&T6SDK::Addresses::fog->hazeInnerRadius, defHazeInner, 0.0f, 60.0f, 7, 13, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);

		float defHazeOpacity = T6SDK::Addresses::fog->sunHazeOpacity;
		UIControls::UI_FogSunHazeOpacity = T6SDK::Drawing::UI_Slider("SUN HAZE OPACITY", (float*)&T6SDK::Addresses::fog->sunHazeOpacity, defHazeOpacity, -10.0f, 10.0f, 10, 13, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);

		float defHazeYaw = T6SDK::Addresses::fog->sunHazeYawAngle;
		UIControls::UI_FogSunHazeYaw = T6SDK::Drawing::UI_Slider("SUN HAZE YAW", (float*)&T6SDK::Addresses::fog->sunHazeYawAngle, defHazeYaw, -180.0f, 360.0f, 4, 16, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);

		float defHazePitch = T6SDK::Addresses::fog->sunHazePitchAngle;
		UIControls::UI_FogSunHazePitch = T6SDK::Drawing::UI_Slider("SUN HAZE PITCH", (float*)&T6SDK::Addresses::fog->sunHazePitchAngle, defHazePitch, -180.0f, 360.0f, 7, 16, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);

		//fog color
		float defFogColorR = T6SDK::Addresses::fog->fogColor.x;
		UIControls::UI_FogColorR = T6SDK::Drawing::UI_Slider("FOG COLOR R", (float*)&T6SDK::Addresses::fog->fogColor.x, defFogColorR, 0.0f, 255.0f, 4, 21, T6SDK::Drawing::REDCOLOR, T6SDK::AnchorPoint::TopLeft, (uintptr_t)&FogColorChanged);
		float defFogColorG = T6SDK::Addresses::fog->fogColor.y;
		UIControls::UI_FogColorG = T6SDK::Drawing::UI_Slider("FOG COLOR G", (float*)&T6SDK::Addresses::fog->fogColor.y, defFogColorG, 0.0f, 255.0f, 4, 24, T6SDK::Drawing::GREENCOLOR, T6SDK::AnchorPoint::TopLeft, (uintptr_t)&FogColorChanged);
		float defFogColorB = T6SDK::Addresses::fog->fogColor.z;
		UIControls::UI_FogColorB = T6SDK::Drawing::UI_Slider("FOG COLOR B", (float*)&T6SDK::Addresses::fog->fogColor.z, defFogColorB, 0.0f, 255.0f, 4, 27, T6SDK::Drawing::BLUECOLOR, T6SDK::AnchorPoint::TopLeft, (uintptr_t)&FogColorChanged);

		float defSunHazeColorR = T6SDK::Addresses::fog->sunHazeColor.x;
		UIControls::UI_FogSunHazeColorR = T6SDK::Drawing::UI_Slider("SUN HAZE COLOR R", (float*)&T6SDK::Addresses::fog->sunHazeColor.x, defSunHazeColorR, 0.0f, 255.0f, 7, 21, T6SDK::Drawing::REDCOLOR, T6SDK::AnchorPoint::TopLeft, (uintptr_t)&FogSunHazeColorChanged);
		float defSunHazeColorG = T6SDK::Addresses::fog->sunHazeColor.y;																									
		UIControls::UI_FogSunHazeColorG = T6SDK::Drawing::UI_Slider("SUN HAZE COLOR G", (float*)&T6SDK::Addresses::fog->sunHazeColor.y, defSunHazeColorG, 0.0f, 255.0f, 7, 24, T6SDK::Drawing::GREENCOLOR, T6SDK::AnchorPoint::TopLeft, (uintptr_t)&FogSunHazeColorChanged);
		float defSunHazeColorB = T6SDK::Addresses::fog->sunHazeColor.z;																									
		UIControls::UI_FogSunHazeColorB = T6SDK::Drawing::UI_Slider("SUN HAZE COLOR B", (float*)&T6SDK::Addresses::fog->sunHazeColor.z, defSunHazeColorB, 0.0f, 255.0f, 7, 27, T6SDK::Drawing::BLUECOLOR, T6SDK::AnchorPoint::TopLeft, (uintptr_t)&FogSunHazeColorChanged);

		//Misc
		UIControls::UI_SyncFogColors = T6SDK::Drawing::UI_CheckBoxButton("SYNC COLORS OFF", "SYNC COLORS ON", 1.0f, 9, 20, T6SDK::AnchorPoint::CenterLeft, &SyncFogColors, 0x00, false);
		UIControls::UI_FogDefaultPreset = T6SDK::Drawing::UI_EnumButton("FOG PRESET", 0, 6, &fogPreset, 12, 25, T6SDK::AnchorPoint::TopLeft, (uintptr_t)&Fog::SetFogPreset);
		UIControls::UI_FogLoadPreset = T6SDK::Drawing::UI_ClickableButton("LOAD PRESET", 12, 30, T6SDK::AnchorPoint::TopLeft, (uintptr_t)&Fog::LoadFogPresetFromFile);
		UIControls::UI_FogSavePreset = T6SDK::Drawing::UI_ClickableButton("SAVE PRESET", 12, 32, T6SDK::AnchorPoint::TopLeft, (uintptr_t)&Fog::SaveFogPresetToFile);
		UIControls::UI_FogReset = T6SDK::Drawing::UI_ClickableButton("RESET FOG", 8, 32, T6SDK::AnchorPoint::Center, (uintptr_t)&ResetFog);
	}
	static void Draw()
	{
		if (!T6SDK::Theater::IsInTheater())
			return;
		if (!*UIControls::FogTabButton.isChecked)
			return;

		T6SDK::Drawing::DrawTextAbsolute("^9FOG", T6SDK::Drawing::GetGridCellCoords(8, 4).x, T6SDK::Drawing::GetGridCellCoords(8, 4).y, 1.0f, T6SDK::Drawing::T_WHITECOLOR, T6SDK::AnchorPoint::Center, 0x00);
		UIControls::UI_FogStart.Draw();
		UIControls::UI_FogFade.Draw();
		UIControls::UI_FogHeightStart.Draw();
		UIControls::UI_FogHeightFade.Draw();
		UIControls::UI_FogOpacity.Draw();
		T6SDK::Drawing::DrawTextAbsolute("^9FOG SUN HAZE", T6SDK::Drawing::GetGridCellCoords(8, 12).x, T6SDK::Drawing::GetGridCellCoords(8, 12).y, 1.0f, T6SDK::Drawing::T_WHITECOLOR, T6SDK::AnchorPoint::Center, 0x00);
		UIControls::UI_FogSunHazeOuterRadius.Draw();
		UIControls::UI_FogSunHazeInnerRadius.Draw();
		UIControls::UI_FogSunHazeOpacity.Draw();
		UIControls::UI_FogSunHazeYaw.Draw();
		UIControls::UI_FogSunHazePitch.Draw();
		T6SDK::Drawing::DrawTextAbsolute("^9FOG COLOR", T6SDK::Drawing::GetGridCellCoords(8, 20).x, T6SDK::Drawing::GetGridCellCoords(8, 20).y, 1.0f, T6SDK::Drawing::T_WHITECOLOR, T6SDK::AnchorPoint::Center, 0x00);
		UIControls::UI_FogColorR.Draw();
		UIControls::UI_FogColorG.Draw();
		UIControls::UI_FogColorB.Draw();
		UIControls::UI_FogSunHazeColorR.Draw();
		UIControls::UI_FogSunHazeColorG.Draw();
		UIControls::UI_FogSunHazeColorB.Draw();

		UIControls::UI_SyncFogColors.Draw();
		T6SDK::Drawing::DrawTextAbsolute("^9FOG PRESET", T6SDK::Drawing::GetGridCellCoords(12, 23).x, T6SDK::Drawing::GetGridCellCoords(12, 23).y, 1.0f, T6SDK::Drawing::T_WHITECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_FogDefaultPreset.Text = Fog::FogPresets[fogPreset];
		UIControls::UI_FogDefaultPreset.Draw();
		UIControls::UI_FogLoadPreset.Draw();
		UIControls::UI_FogSavePreset.Draw();

		UIControls::UI_FogReset.Draw();
	}
}