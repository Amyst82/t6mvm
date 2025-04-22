#pragma once
#include "UIControls.h"
#include "CustomDvars.h"
namespace SunMenu
{
	float RadianToDegree(float angle)
	{
		return angle * 180.0 / 3.141592653589793f;
	}
	vec3_t SunPositionToAngles(vec3_t position)
	{
		// Normalize the position vector to ensure it has a length of 1
		double length = sqrt(position.x * position.x + position.y * position.y + position.z * position.z);
		double x = position.x / length;
		double y = position.y / length;
		double z = position.z / length;

		// Calculate pitch (elevation angle)
		double pitch = asin(z); // z = sin(pitch)
		double pitchDegrees = RadianToDegree(pitch);

		// Calculate yaw (azimuth angle)
		double yaw = atan2(y, x); // y = sin(yaw) * cos(pitch), x = cos(yaw) * cos(pitch)
		double yawDegrees = RadianToDegree(yaw);

		// Ensure yaw is in the range [0, 360)
		if (yawDegrees < 0)
		{
			yawDegrees += 360.0;
		}
		if(position.x == 0.0f && position.y == 0.0f && position.z == 0.0f)
			return vec3_t{ 0.0f, 0.0f, 0.0f };
		// Return the angles as a Vector3 (pitch in X, yaw in Y, and 0 in Z)
		return vec3_t
		{
			abs((float)pitchDegrees),
			(float)yawDegrees,
			0.0f // Z is unused in this context
		};
	}

	static string GetCFG()
	{
		vec3_t pos = SunPositionToAngles(T6SDK::Addresses::SunPosition.Value());
		vec3_t col = T6SDK::Addresses::SunColor.Value();
		std::string cfg =
			"//SUN&SKY\n" +
			std::string("r_lightTweakSunDirection ") + std::to_string(pos.x) + " " + std::to_string(pos.y) + " " + std::to_string(pos.z) + ";\n" +
			std::string("r_lightTweakSunColor ") + std::to_string(col.x) + " " + std::to_string(col.y) + " " + std::to_string(col.z) + ";\n" +
			std::string("r_skyColorTemp ") + std::to_string((*T6SDK::Dvars::DvarList::r_skyColorTemp)->current.value) + ";\n" +
			std::string("r_sky_intensity_factor0 ") + std::to_string((*T6SDK::Dvars::DvarList::r_sky_intensity_factor0)->current.value) + ";\n" + 
			std::string("r_skyRotation ") + std::to_string((*T6SDK::Dvars::DvarList::r_skyRotation)->current.value) + ";\n";
		return cfg;
	}
	bool Inited = false;
	static void Init()
	{
		float defR = T6SDK::Addresses::SunColor.Value().x;
		float defG = T6SDK::Addresses::SunColor.Value().y;
		float defB = T6SDK::Addresses::SunColor.Value().z;
		T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, false, "SUNSKY", "Sun color: %f %f %f", defR, defG, defB);
		UIControls::UI_SunColorR = T6SDK::Drawing::UI_Slider("SUN COLOR R", (float*)T6SDK::Addresses::SunColor.EndPointerAddress(), defR, 0.0f, 99.0f, 4, 6, T6SDK::Drawing::REDCOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_SunColorG = T6SDK::Drawing::UI_Slider("SUN COLOR G", (float*)(T6SDK::Addresses::SunColor.EndPointerAddress() + 0x04), defG, 0.0f, 99.0f, 4, 9, T6SDK::Drawing::GREENCOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_SunColorB = T6SDK::Drawing::UI_Slider("SUN COLOR B", (float*)(T6SDK::Addresses::SunColor.EndPointerAddress() + 0x08), defB, 0.0f, 99.0f, 4, 12, T6SDK::Drawing::BLUECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		
		float defX = T6SDK::Addresses::SunPosition.Value().x;
		float defY = T6SDK::Addresses::SunPosition.Value().y;
		float defZ = T6SDK::Addresses::SunPosition.Value().z;
		UIControls::UI_SunX = T6SDK::Drawing::UI_Slider("SUN X", (float*)(T6SDK::Addresses::SunPosition.EndPointerAddress() + 0x00), defX, 0.0f, 1.0f, 7, 6, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_SunY = T6SDK::Drawing::UI_Slider("SUN Y", (float*)(T6SDK::Addresses::SunPosition.EndPointerAddress() + 0x04), defY, 0.0f, 1.0f, 7, 9, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_SunZ = T6SDK::Drawing::UI_Slider("SUN Z", (float*)(T6SDK::Addresses::SunPosition.EndPointerAddress() + 0x08), defZ, 0.0f, 1.0f, 7, 12, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);

		UIControls::UI_SkyTemp = T6SDK::Drawing::UI_Slider("SKY TEMP", &(*T6SDK::Dvars::DvarList::r_skyColorTemp)->current.value, 6500.0f, 1650.0f, 25000.0f, 10, 6, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_SkyBrightness = T6SDK::Drawing::UI_Slider("SKY BRIGHTNESS", &(*T6SDK::Dvars::DvarList::r_sky_intensity_factor0)->current.value, 1.0f, 0.0f, 10.0f, 10, 9, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_SkyRotation = T6SDK::Drawing::UI_Slider("SKY ROTATION", &(*T6SDK::Dvars::DvarList::r_skyRotation)->current.value, 0.0f, -360.0f, 360.0f, 10, 12, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
	}
	static void Draw()
	{
		if (!T6SDK::Theater::IsInTheater())
		{
			Inited = false;
			return;
		}
		if (!*UIControls::SunSkyTabButton.isChecked)
			return;
		
		vec2_t coords = T6SDK::Drawing::GetGridCellCoords(8, 5);
		T6SDK::Drawing::DrawTextAbsolute("^9SUN/SKY SETTINGS", coords.x, coords.y, 1.0f, T6SDK::Drawing::T_WHITECOLOR, T6SDK::AnchorPoint::Center, 0x00);

		UIControls::UI_SunColorR.Draw();
		UIControls::UI_SunColorG.Draw();
		UIControls::UI_SunColorB.Draw();

		UIControls::UI_SunX.Draw();
		UIControls::UI_SunY.Draw();
		UIControls::UI_SunZ.Draw();

		UIControls::UI_SkyTemp.Draw();
		UIControls::UI_SkyBrightness.Draw();
		UIControls::UI_SkyRotation.Draw();
	}

}