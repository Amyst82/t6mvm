#pragma once
#include "UIControls.h"
#include "CustomDvars.h"
namespace SsaoMenu
{
	static string GetCFG()
	{
		std::string cfg =
			"//SSAO\n" + 
			std::string("r_ssao ") + std::to_string(T6SDK::Dvars::GetBool(*T6SDK::Dvars::DvarList::r_ssao) ? 1 : 0) + ";\n" +
			std::string("r_ssaoScale ") + std::to_string(T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_ssaoScale)) + ";\n" +
			std::string("r_ssaoRadius ") + std::to_string(T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_ssaoRadius)) + ";\n" +
			std::string("r_ssaoBias ") + std::to_string(T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_ssaoBias)) + ";\n" +
			std::string("r_ssaoIntensity ") + std::to_string(T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_ssaoIntensity)) + ";\n" +
			std::string("r_znear ") + std::to_string(T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_znear)) + ";\n";
		return cfg;
	}
	static void Init()
	{
		UIControls::UI_SsaoToggle = T6SDK::Drawing::UI_CheckBoxButton("SSAO OFF", "SSAO ON", 4, 5, T6SDK::AnchorPoint::TopLeft, &(*T6SDK::Dvars::DvarList::r_ssao)->current.enabled, 0x00);
		UIControls::UI_SsaoToggle.ToolTip = "Enable SSAO.";
		UIControls::UI_SsaoScale = T6SDK::Drawing::UI_Slider("SSAO SCALE", &(*T6SDK::Dvars::DvarList::r_ssaoScale)->current.value, 50.0f, 0.0f, 500.0f, 4, 8, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_SsaoScale.ToolTip = "SSAO scale.";
		UIControls::UI_SsaoRadius = T6SDK::Drawing::UI_Slider("SSAO RADIUS", &(*T6SDK::Dvars::DvarList::r_ssaoRadius)->current.value, 12.0f, 0.0f, 200.0f, 7, 8, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_SsaoRadius.ToolTip = "SSAO radius.";
		UIControls::UI_SsaoBias = T6SDK::Drawing::UI_Slider("SSAO BIAS", &(*T6SDK::Dvars::DvarList::r_ssaoBias)->current.value, 0.55f, 0.0f, 1.0f, 10, 8, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_SsaoBias.ToolTip = "SSAO bias.";
		UIControls::UI_SsaoIntensity = T6SDK::Drawing::UI_Slider("SSAO INTENSITY", &(*T6SDK::Dvars::DvarList::r_ssaoIntensity)->current.value, 0.52f, 0.0f, 5.0f, 4, 12, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_SsaoIntensity.ToolTip = "SSAO intensity.";
		UIControls::UI_Znear = T6SDK::Drawing::UI_Slider("SSAO DISTANCE", &(*T6SDK::Dvars::DvarList::r_znear)->current.value, 4.0f, 0.5f, 9.0f, 7, 12, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_Znear.ToolTip = "Distance of SSAO. Formerly ^5r_znear^7 dvar.";
	}
	static void Draw()
	{
		if (!T6SDK::Theater::IsInTheater())
			return;
		if (!*UIControls::SsaoTabButton.isChecked)
			return;

		UIControls::UI_SsaoToggle.Draw();
		UIControls::UI_SsaoScale.Draw();
		UIControls::UI_SsaoRadius.Draw();
		UIControls::UI_SsaoBias.Draw();
		UIControls::UI_SsaoIntensity.Draw();
		UIControls::UI_Znear.Draw();
	}
}