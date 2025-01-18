#pragma once
#include <StdInclude.h>
namespace UIControls
{

	//TABS
	inline static T6SDK::Drawing::UI_RadioButton MainCameraTabButton("^7CAMERA", 0, 2, 5, T6SDK::AnchorPoint::TopLeft, 0x00);
	inline static T6SDK::Drawing::UI_RadioButton MiscTabButton("^7MISC", 0, 2, 7, T6SDK::AnchorPoint::TopLeft, 0x00);
	inline static T6SDK::Drawing::UI_RadioButton BoneCameraTabButton("^7BONE CAMERA", 0, 2, 9, T6SDK::AnchorPoint::TopLeft, 0x00);
	inline static T6SDK::Drawing::UI_RadioButton SunSkyTabButton("^7SUN", 0, 2, 11, T6SDK::AnchorPoint::TopLeft, 0x00);
	inline static T6SDK::Drawing::UI_RadioButton LightsTabButton("^7LIGHTS", 0, 2, 13, T6SDK::AnchorPoint::TopLeft, 0x00);

	//CAMERA controls

	//MISC controls
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_GreenScreen{};
	inline static T6SDK::Drawing::UI_EnumButton UI_PlayersGreenScreen{};
	inline static T6SDK::Drawing::UI_Slider UI_PlayersGreenScreenThreshold{};

	//BONE CAMERA controls
	inline static T6SDK::Drawing::UI_EnumButton UI_SelectedBoneButton{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_AttachToBoneButton{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_ShowBonesButton{};
	/// LIGHTS controls
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_LightType{};
	inline static T6SDK::Drawing::UI_Slider UI_LightColorR{};
	inline static T6SDK::Drawing::UI_Slider UI_LightColorG{};
	inline static T6SDK::Drawing::UI_Slider UI_LightColorB{};
	inline static T6SDK::Drawing::UI_Slider UI_LightBrightness{};
	inline static T6SDK::Drawing::UI_Slider UI_LightRadius{};
	inline static T6SDK::Drawing::UI_ClickableButton UI_DeleteLightButton{};
	inline static T6SDK::Drawing::UI_ClickableButton UI_DeleteAllLightsButton{};
	inline static T6SDK::Drawing::UI_ClickableButton UI_RepositionLightButton{};
	inline static T6SDK::Drawing::UI_ClickableButton UI_ExportLightsButton{};
	inline static T6SDK::Drawing::UI_ClickableButton UI_ImportLightsButton{};
	//MENU controls
	inline static T6SDK::Drawing::UI_CheckBoxButton MenuBlurCheckBox{};
	inline static T6SDK::Drawing::UI_ClickableButton CloseMenuButton{};

	inline static void Init()
	{
		BoneCameraTabButton.ToolTip = "Create cool stabilized camera angles using boner camera!"; //TODO replace with actual description
		LightsTabButton.ToolTip = "Create and manage your custom lights on map!";
		MenuBlurCheckBox = T6SDK::Drawing::UI_CheckBoxButton("^7BLUR ^9OFF", "^7BLUR ON", 12, 35, T6SDK::AnchorPoint::TopLeft, &CustomDvars::dvar_menuBlur->current.enabled, 0x00);
		MenuBlurCheckBox.ToolTip = "Turn on/off menu background blur.";
	}
	inline static void DrawTabs()
	{
		MainCameraTabButton.Draw();
		MiscTabButton.Draw();
		BoneCameraTabButton.Draw();
		SunSkyTabButton.Draw();
		LightsTabButton.Draw();
	}
}