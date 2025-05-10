#pragma once
#include <StdInclude.h>
#include "UI_Timeline.h"
#include "UI_DemoBrowseButton.h"
#include "WeapsListBoxControl.h"
#include "WeapAnimListBoxControl.h"
namespace UIControls
{

	//TABS
	inline static T6SDK::Drawing::UI_RadioButton MainTabButton("^7MAIN", 0, 2, 5, T6SDK::AnchorPoint::TopLeft, 0x00);
	inline static T6SDK::Drawing::UI_RadioButton MainCameraTabButton("^7CAMERA", 0, 2, 7, T6SDK::AnchorPoint::TopLeft, 0x00);
	inline static T6SDK::Drawing::UI_RadioButton MiscTabButton("^7MISC", 0, 2, 9, T6SDK::AnchorPoint::TopLeft, 0x00);
	inline static T6SDK::Drawing::UI_RadioButton BoneCameraTabButton("^7BONE CAMERA", 0, 2, 11, T6SDK::AnchorPoint::TopLeft, 0x00);
	inline static T6SDK::Drawing::UI_RadioButton SsaoTabButton("^7SSAO", 0, 2, 13, T6SDK::AnchorPoint::TopLeft, 0x00);
	inline static T6SDK::Drawing::UI_RadioButton FogTabButton("^7FOG", 0, 2, 15, T6SDK::AnchorPoint::TopLeft, 0x00);
	inline static T6SDK::Drawing::UI_RadioButton DofTabButton("^7DOF", 0, 2, 17, T6SDK::AnchorPoint::TopLeft, 0x00);
	inline static T6SDK::Drawing::UI_RadioButton SunSkyTabButton("^7SUN", 0, 2, 19, T6SDK::AnchorPoint::TopLeft, 0x00);
	inline static T6SDK::Drawing::UI_RadioButton LightsTabButton("^7LIGHTS", 0, 2, 21, T6SDK::AnchorPoint::TopLeft, 0x00);
	inline static T6SDK::Drawing::UI_RadioButton StreamsTabButton("^7STREAMS", 0, 2, 23, T6SDK::AnchorPoint::TopLeft, 0x00);
	inline static T6SDK::Drawing::UI_RadioButton WeaponTabButton("^7WEAPON", 0, 2, 25, T6SDK::AnchorPoint::TopLeft, 0x00);
	inline static T6SDK::Drawing::UI_RadioButton SlidersTabButton("^7SLIDERS", 0, 2, 27, T6SDK::AnchorPoint::TopLeft, 0x00);

	//CAMERA controls
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_FrozenCamera{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_ShowDollyCamInfo{};
	inline static T6SDK::Drawing::UI_Slider UI_DollyCamOffsetX{};
	inline static T6SDK::Drawing::UI_Slider UI_DollyCamOffsetY{};
	inline static T6SDK::Drawing::UI_Slider UI_DollyCamOffsetZ{};
	inline static T6SDK::Drawing::UI_Slider UI_DollyCamRotationOffsetX{};
	inline static T6SDK::Drawing::UI_Slider UI_DollyCamRotationOffsetY{};
	inline static T6SDK::Drawing::UI_Slider UI_DollyCamRotationOffsetZ{};
	inline static T6SDK::Drawing::UI_Slider UI_FreeRoamSpeed{};
	inline static T6SDK::Drawing::UI_Slider UI_FreeRoamAcceleration{};
	inline static T6SDK::Drawing::UI_Slider UI_FreeRoamSlowing{};

	//SHAKE controls
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_Shake{};
	inline static T6SDK::Drawing::UI_Slider UI_ShakeSpeed{};
	inline static T6SDK::Drawing::UI_Slider UI_ShakeAmount{};
	inline static T6SDK::Drawing::UI_Slider UI_ShakeX{};
	inline static T6SDK::Drawing::UI_Slider UI_ShakeY{};
	inline static T6SDK::Drawing::UI_Slider UI_ShakeZ{};
	inline static T6SDK::Drawing::UI_Slider UI_ShakePitch{};
	inline static T6SDK::Drawing::UI_Slider UI_ShakeYaw{};
	inline static T6SDK::Drawing::UI_Slider UI_ShakeRoll{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_ShakePreview{};
	inline static T6SDK::Drawing::UI_ClickableButton UI_ExportCampathButton{};
	inline static T6SDK::Drawing::UI_ClickableButton UI_ImportCampathButton{};

	//MISC controls
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_GreenScreen{};
	inline static T6SDK::Drawing::UI_EnumButton UI_PlayersGreenScreen{};
	inline static T6SDK::Drawing::UI_Slider UI_PlayersGreenScreenThreshold{};
	inline static T6SDK::Drawing::UI_Slider UI_GreenScreenColorR{};
	inline static T6SDK::Drawing::UI_Slider UI_GreenScreenColorG{};
	inline static T6SDK::Drawing::UI_Slider UI_GreenScreenColorB{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_ZDepth{};
	inline static T6SDK::Drawing::UI_Slider UI_ZDepthDistance{};
	inline static T6SDK::Drawing::UI_CheckBoxButtonInversed UI_RemoveGun{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_RemovePlayers{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_GreenSky{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_BloomTweaks{};
	inline static T6SDK::Drawing::UI_CheckBoxButtonInversed UI_RemoveTheaterBarrier{};

	//BONE CAMERA controls
	inline static T6SDK::Drawing::UI_EnumButton UI_SelectedBoneButton{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_AttachToBoneButton{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_ShowBonesButton{};

	//SSAO controls
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_SsaoToggle{};
	inline static T6SDK::Drawing::UI_Slider UI_SsaoScale{};
	inline static T6SDK::Drawing::UI_Slider UI_SsaoRadius{};
	inline static T6SDK::Drawing::UI_Slider UI_SsaoBias{};
	inline static T6SDK::Drawing::UI_Slider UI_SsaoIntensity{};
	inline static T6SDK::Drawing::UI_Slider UI_Znear{};
	inline static T6SDK::Drawing::UI_ClickableButton UI_SsaoReset{};

	//FOG controls
	inline static T6SDK::Drawing::UI_Slider UI_FogStart{};
	inline static T6SDK::Drawing::UI_Slider UI_FogFade{};
	inline static T6SDK::Drawing::UI_Slider UI_FogHeightStart{};
	inline static T6SDK::Drawing::UI_Slider UI_FogHeightFade{};
	inline static T6SDK::Drawing::UI_Slider UI_FogOpacity{};
	inline static T6SDK::Drawing::UI_Slider UI_FogSunHazeOuterRadius{};
	inline static T6SDK::Drawing::UI_Slider UI_FogSunHazeInnerRadius{};
	inline static T6SDK::Drawing::UI_Slider UI_FogSunHazeOpacity{};
	inline static T6SDK::Drawing::UI_Slider UI_FogSunHazeYaw{};
	inline static T6SDK::Drawing::UI_Slider UI_FogSunHazePitch{};
	inline static T6SDK::Drawing::UI_Slider UI_FogColorR{};
	inline static T6SDK::Drawing::UI_Slider UI_FogColorG{};
	inline static T6SDK::Drawing::UI_Slider UI_FogColorB{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_SyncFogColors{};
	inline static T6SDK::Drawing::UI_Slider UI_FogSunHazeColorR{};
	inline static T6SDK::Drawing::UI_Slider UI_FogSunHazeColorG{};
	inline static T6SDK::Drawing::UI_Slider UI_FogSunHazeColorB{};
	inline static T6SDK::Drawing::UI_EnumButton UI_FogDefaultPreset{};
	inline static T6SDK::Drawing::UI_ClickableButton UI_FogSavePreset{};
	inline static T6SDK::Drawing::UI_ClickableButton UI_FogLoadPreset{};
	inline static T6SDK::Drawing::UI_ClickableButton UI_FogReset{};

	//DOF controls
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_DofToggle{};
	inline static T6SDK::Drawing::UI_Slider			UI_DofFarBlur{};
	inline static T6SDK::Drawing::UI_Slider			UI_DofFarStart{};
	inline static T6SDK::Drawing::UI_Slider			UI_DofFarEnd{};
	inline static T6SDK::Drawing::UI_Slider			UI_DofNearBlur{};
	inline static T6SDK::Drawing::UI_Slider			UI_DofNearStart{};
	inline static T6SDK::Drawing::UI_Slider			UI_DofNearEnd{};
	inline static T6SDK::Drawing::UI_Slider			UI_DofViewmodelStart{};
	inline static T6SDK::Drawing::UI_Slider			UI_DofViewmodelEnd{};
	inline static T6SDK::Drawing::UI_Slider			UI_DofBias{};
	inline static T6SDK::Drawing::UI_EnumButton		UI_DofHDR{};
	inline static T6SDK::Drawing::UI_ClickableButton UI_DofReset{};

	//SUN controls
	inline static T6SDK::Drawing::UI_Slider UI_SunColorR{};
	inline static T6SDK::Drawing::UI_Slider UI_SunColorG{};
	inline static T6SDK::Drawing::UI_Slider UI_SunColorB{};
	inline static T6SDK::Drawing::UI_Slider UI_SkyTemp{};
	inline static T6SDK::Drawing::UI_Slider UI_SkyBrightness{};
	inline static T6SDK::Drawing::UI_Slider UI_SkyRotation{};
	inline static T6SDK::Drawing::UI_Slider UI_SunX{};
	inline static T6SDK::Drawing::UI_Slider UI_SunY{};
	inline static T6SDK::Drawing::UI_Slider UI_SunZ{};
	inline static T6SDK::Drawing::UI_EnumButton	UI_SunQuiality{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_RemoveSunFlare{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_SkyTransition{};

	/// LIGHTS controls
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_LightType{};
	inline static T6SDK::Drawing::UI_Slider UI_LightColorR{};
	inline static T6SDK::Drawing::UI_Slider UI_LightColorG{};
	inline static T6SDK::Drawing::UI_Slider UI_LightColorB{};
	inline static T6SDK::Drawing::UI_Slider UI_LightBrightness{};
	inline static T6SDK::Drawing::UI_Slider UI_LightRadius{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_LightFlicker{};
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
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_CameraSway{};
	inline static T6SDK::Drawing::UI_Slider UI_CameraSwayMaxAngle{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_Holdgun{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_HoldgunWrist{};
	inline static T6SDK::Drawing::UI_CheckBoxButton UI_WeaponAnimChanging{};
	//WEAPON ANIMATION MENU controls
	inline static T6SDK::Drawing::UI_ClickableButton UI_ReplaceWeaponAnim{};
	inline static T6SDK::Drawing::UI_ClickableButton UI_ResetSelectedWeaponAnim{};
	inline static T6SDK::Drawing::UI_ClickableButton UI_ResetAllWeaponAnims{};
	inline static T6SDK::Drawing::UI_ClickableButton UI_CloseWeaponAnimMenu{};
	inline static Weapon::UI_WeapsListBox UI_WeapsLB1{};
	inline static Weapon::UI_WeapsListBox UI_WeapsLB2{};
	inline static Weapon::UI_WeapAnimListBox UI_WeapAnimsLB1{};
	inline static Weapon::UI_WeapAnimListBox UI_WeapAnimsLB2{};

	//MENU controls
	inline static T6SDK::Drawing::UI_CheckBoxButton MenuBlurCheckBox{};
	inline static T6SDK::Drawing::UI_ClickableButton CloseMenuButton{};

	//MAIN MENU controls
	inline static UI_TimelineNS::UI_Timeline UI_TimelineSlider{};
	inline static T6SDK::Drawing::UI_ClickableButton AddToCFGButton{};
	inline static T6SDK::Drawing::UI_ClickableButton UI_RemoveCustomBookmarks{};
	inline static T6SDK::Drawing::UI_ClickableButton UI_LoadCFGButton{};
	inline static T6SDK::Drawing::UI_EnumButton UI_DemoClient{};
	inline static UIDemoBrowseButtonNS::UI_DemoBrowseButton UI_DemoBrowseCheckButton{};
	inline static T6SDK::Drawing::UI_IconClickableButton UI_OpenDemoBrowserButton{};
	inline static T6SDK::Drawing::UI_ClickableButton UI_SaveAllAsCFGButton{};
	inline static T6SDK::Drawing::UI_ClickableButton UI_SaveCustomBookmarks{};
	
	//DEMO SELECT MENU controls
	inline static T6SDK::Drawing::UI_EnumButton			UI_SelectedDemoNumber{};
	inline static T6SDK::Drawing::UI_ClickableButton	UI_PlayDemoButton{};
	inline static T6SDK::Drawing::UI_ClickableButton	UI_CloseDemoSelectMenu{};
	inline static T6SDK::Drawing::UI_ClickableButton	UI_NavigateToDemoFile{};
	inline static T6SDK::Drawing::UI_ClickableButton	UI_DemosDirectoryButton{};
	inline static T6SDK::Drawing::UI_ClickableButton	UI_DemoRename{};


	//CUSTOM SLIDER controls
	inline static T6SDK::Drawing::UI_ClickableButton UI_SlidersAddSliderButton{};
	inline static T6SDK::Drawing::UI_ClickableButton UI_SlidersRefreshButton{};


	inline static void Init()
	{
		//MainTabButton.SetChecked();
		MainTabButton.ToolTip = "Control timeline, execute ^5cfg ^7and more.";
		MainCameraTabButton.ToolTip = "Manage free camera, dolly camera and etc!";
		MiscTabButton.ToolTip = "Various misc features such as green screen and depth!";
		BoneCameraTabButton.ToolTip = "Create cool stabilized camera angles using boner camera!"; //TODO replace with actual description
		SsaoTabButton.ToolTip = "Tweak your screen space ambient occlusion.";
		FogTabButton.ToolTip = "Add a little bit of depth to the scene with FOG.";
		DofTabButton.ToolTip = "Blur things on background out with tweaking depth of field.";
		SunSkyTabButton.ToolTip = "Change sun and sky settings such as sun position and color!";
		LightsTabButton.ToolTip = "Create and manage your custom lights on map!";
		StreamsTabButton.ToolTip = "Record multiple passes simultaneously as screensots to get perfect sync!";
		WeaponTabButton.ToolTip = "Change weapon camo and viewmodel offsets!";
		MenuBlurCheckBox = T6SDK::Drawing::UI_CheckBoxButton("^7BLUR ^9OFF", "^7BLUR ON", 12, 35, T6SDK::AnchorPoint::TopLeft, &CustomDvars::dvar_menuBlur->current.enabled, 0x00);
		MenuBlurCheckBox.ToolTip = "Turn on/off menu background blur.";
		UI_DemoBrowseCheckButton = UIDemoBrowseButtonNS::UI_DemoBrowseButton(13, 4, T6SDK::AnchorPoint::TopLeft);
		UI_DemoBrowseCheckButton.ToolTip = "Load a demo from file.";
		Common::UI_BookmarkDialog = T6SDK::Drawing::UI_TextBoxDialog("Provide bookmark description (may be empty)");
		SlidersTabButton.ToolTip = "Create and manage custom sliders!";

	}
	inline static void DrawTabs()
	{
		MainTabButton.Draw();
		MainCameraTabButton.Draw();
		MiscTabButton.Draw();
		BoneCameraTabButton.Draw();
		SsaoTabButton.Draw();
		FogTabButton.Draw();
		DofTabButton.Draw();
		SunSkyTabButton.Draw();
		LightsTabButton.Draw();
		StreamsTabButton.Draw();
		WeaponTabButton.Draw();
		SlidersTabButton.Draw();
	}
}