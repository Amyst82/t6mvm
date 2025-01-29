#pragma once
#include <StdInclude.h>
#include "Lights.h"
#include "UIControls.h"
#include "LightsListBoxControl.h"
namespace LightsMenu
{
	int LastSelectedLight = -1;
	inline static Lights::UI_ListBox UI_LightsListBox{};
	inline static void Init()
	{
		UIControls::UI_ExportLightsButton = T6SDK::Drawing::UI_ClickableButton("Export lights", 12, 30, T6SDK::AnchorPoint::TopLeft, (uintptr_t)&Lights::ExportLights);
		UIControls::UI_ImportLightsButton = T6SDK::Drawing::UI_ClickableButton("Import lights", 12, 32, T6SDK::AnchorPoint::TopLeft, (uintptr_t)&Lights::ImportLights);

		UI_LightsListBox = Lights::UI_ListBox("LIGHTS", 4, 5, 7, 28, &Lights::LightsList, &Lights::SelectedLight, T6SDK::AnchorPoint::TopLeft, 0x00);
	}
	void Reinit()
	{
		if (Lights::SelectedLight == -1 || Lights::LightsList.size() == 0)
			return;
		UIControls::UI_LightType = T6SDK::Drawing::UI_CheckBoxButton("Type: OMNI", "Type: SPOT", 12, 5, T6SDK::AnchorPoint::TopLeft, &Lights::LightsList[Lights::SelectedLight].spotLight, 0x00);
		UIControls::UI_LightColorR = T6SDK::Drawing::UI_Slider("R", &Lights::LightsList[Lights::SelectedLight].r, 1.0f, 0.0f, CustomDvars::dvar_lightColorLimit->current.value, 12, 7, T6SDK::Drawing::REDCOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_LightColorG = T6SDK::Drawing::UI_Slider("G", &Lights::LightsList[Lights::SelectedLight].g, 1.0f, 0.0f, CustomDvars::dvar_lightColorLimit->current.value, 12, 10, T6SDK::Drawing::GREENCOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_LightColorB = T6SDK::Drawing::UI_Slider("B", &Lights::LightsList[Lights::SelectedLight].b, 1.0f, 0.0f, CustomDvars::dvar_lightColorLimit->current.value, 12, 13, T6SDK::Drawing::BLUECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_LightBrightness = T6SDK::Drawing::UI_Slider("Brightness", &Lights::LightsList[Lights::SelectedLight].a, 1.0f, 0.0f, 10.0f, 12, 16, T6SDK::Drawing::WHITECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_LightRadius = T6SDK::Drawing::UI_Slider("Radius", &Lights::LightsList[Lights::SelectedLight].radius, 255.0f, 0.0f, CustomDvars::dvar_lightRadiusLimit->current.value, 12, 19, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_RepositionLightButton = T6SDK::Drawing::UI_ClickableButton("Reposition light", 12, 23, T6SDK::AnchorPoint::TopLeft, (uintptr_t)&Lights::RepositionLight);
		UIControls::UI_DeleteLightButton = T6SDK::Drawing::UI_ClickableButton("Remove light", 12, 25, T6SDK::AnchorPoint::TopLeft, (uintptr_t)&Lights::RemoveLight);
		UIControls::UI_DeleteAllLightsButton = T6SDK::Drawing::UI_ClickableButton("Remove all lights", 12, 28, T6SDK::AnchorPoint::TopLeft, (uintptr_t)&Lights::RemoveAllLights);

	}
	inline static void Draw()
	{
		if (!*UIControls::LightsTabButton.isChecked)
			return;
		if(LastSelectedLight != Lights::SelectedLight)
		{
			LastSelectedLight = Lights::SelectedLight;
			Reinit();
		}
		if (CustomDvars::dvar_lightColorLimit->modified || CustomDvars::dvar_lightRadiusLimit->modified)
		{
			CustomDvars::dvar_lightColorLimit->modified = false;
			CustomDvars::dvar_lightRadiusLimit->modified = false;
			Reinit();
		}
		if (Lights::SelectedLight != -1 && Lights::LightsList.size() > 0)
		{
			vec4_t margin = { -15.0f, 10.0f, -10.0f, -5.0f };
			T6SDK::Drawing::DrawRectAbsolute(12, 4, 15, 27, margin, T6SDK::Drawing::T_BLACKCOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
			UIControls::UI_LightType.Draw();
			UIControls::UI_LightColorR.Draw();
			UIControls::UI_LightColorG.Draw();
			UIControls::UI_LightColorB.Draw();
			UIControls::UI_LightBrightness.Draw();
			UIControls::UI_LightRadius.Draw();
			UIControls::UI_RepositionLightButton.Draw();
			UIControls::UI_DeleteLightButton.Draw();
		}
		if (Lights::LightsList.size() > 0)
		{
			UIControls::UI_DeleteAllLightsButton.Draw();
			UI_LightsListBox.Draw();
		}
		else
		{
			vec2_t coords = T6SDK::Drawing::GetGridCellCoords(8, 19);
			T6SDK::Drawing::DrawTextAbsolute("NO LIGHTS SPAWNED", coords.x, coords.y, 1.5f, T6SDK::Drawing::WHITECOLOR, T6SDK::AnchorPoint::Center, 0x00);
			vec2_t coords2 = T6SDK::Drawing::GetGridCellCoords(8, 21);
			T6SDK::Drawing::DrawTextAbsolute("^7ADD A LIGHT IN ^3LIGHTS ^7MODE", coords2.x, coords2.y, 1.0f, T6SDK::Drawing::WHITECOLOR, T6SDK::AnchorPoint::Center, 0x00);
		}
		UIControls::UI_ExportLightsButton.Draw(Lights::LightsList.size() > 0);
		UIControls::UI_ImportLightsButton.Draw();
	}
}