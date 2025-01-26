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
	inline static T6SDK::Drawing::UI_RadioButton StreamsTabButton("^7STREAMS", 0, 2, 15, T6SDK::AnchorPoint::TopLeft, 0x00);
	inline static T6SDK::Drawing::UI_RadioButton WeaponTabButton("^7WEAPON", 0, 2, 17, T6SDK::AnchorPoint::TopLeft, 0x00);

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

	//STREAMS controls
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_ToggleStreamsCheckBox{};
	inline static T6SDK::Drawing::UI_ClickableButton UI_StreamsDirectoryButton{};
	inline static T6SDK::Drawing::UI_ClickableButton UI_StreamsStartTickButton{};
	inline static T6SDK::Drawing::UI_ClickableButton UI_StreamsEndTickButton{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_StreamsJPGCheckBox{};
	inline static T6SDK::Drawing::UI_EnumButton UI_StreamsFPSEnum{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_StreamsAVICheckBox{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_StreamsNoFlashCheckBox{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_RecordCamCheckBox{};
	//streams passes
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_StreamsPass1CheckBox{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_StreamsPass2CheckBox{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_StreamsPass3CheckBox{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_StreamsPass4CheckBox{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_StreamsPass5CheckBox{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_StreamsPass6CheckBox{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_StreamsPass7CheckBox{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_StreamsPass8CheckBox{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_StreamsPass9CheckBox{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_StreamsPass10CheckBox{};


	//WEAPON controls
	inline static T6SDK::Drawing::UI_Slider UI_GunXOffset{};
	inline static T6SDK::Drawing::UI_Slider UI_GunYOffset{};
	inline static T6SDK::Drawing::UI_Slider UI_GunZOffset{};
	inline static T6SDK::Drawing::UI_Slider UI_GunYaw{};
	inline static T6SDK::Drawing::UI_Slider UI_GunPitch{};
	inline static T6SDK::Drawing::UI_Slider UI_GunRoll{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_CamoChanging{};
	inline static T6SDK::Drawing::UI_EnumButton UI_PrimaryCamo{};
	inline static T6SDK::Drawing::UI_EnumButton UI_SecondaryCamo{};


	//MENU controls
	inline static T6SDK::Drawing::UI_CheckBoxButton MenuBlurCheckBox{};
	inline static T6SDK::Drawing::UI_ClickableButton CloseMenuButton{};

	inline static void Init()
	{
		MainCameraTabButton.ToolTip = "Manage free camera, dolly camera and etc!";
		MiscTabButton.ToolTip = "Various misc features such as green screen and depth!";
		BoneCameraTabButton.ToolTip = "Create cool stabilized camera angles using boner camera!"; //TODO replace with actual description
		SunSkyTabButton.ToolTip = "Change sun and sky settings such as sun position and color!";
		LightsTabButton.ToolTip = "Create and manage your custom lights on map!";
		StreamsTabButton.ToolTip = "Record multiple passes simultaneously as screensots to get perfect sync!";
		WeaponTabButton.ToolTip = "Change weapon camo and viewmodel offsets!";
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
		StreamsTabButton.Draw();
		WeaponTabButton.Draw();
	}
}