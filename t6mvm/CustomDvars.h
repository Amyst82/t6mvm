#pragma once
#include <StdInclude.h>

namespace CustomDvars
{
	//CAMERA DVARS
	static dvar_s* dvar_shake;
	static dvar_s* dvar_shakeSpeed;
	static dvar_s* dvar_shakeAmount;
	static dvar_s* dvar_shakePosition;
	static dvar_s* dvar_shakeRotation;

	static dvar_s* dvar_rollStep;
	static dvar_s* dvar_showMarkerInfo;
	static dvar_s* dvar_frozenCam;
	static dvar_s* dvar_demoTimescaleStep;
	static dvar_s* dvar_dollyCamOffsetPos;
	static dvar_s* dvar_dollyCamOffsetRot;

	static dvar_s* dvar_freeRoamSpeed;
	static dvar_s* dvar_accelerationFactor;
	static dvar_s* dvar_slowingFactor;
	static dvar_s* dvar_cameraFov;
	static dvar_s* dvar_cameraRoll;


	//MISC DVARS
	static dvar_s* dvar_greenScreen;
	const char* gsPlayersEnum[4] = {"No", "Selected player", "Threshold", "All players"};
	static dvar_s* dvar_greenScreenPlayers;
	static dvar_s* dvar_greenScreenPlayersThreshold;
	static dvar_s* dvar_greenScreenColor;
	static dvar_s* dvar_zdepth;
	static dvar_s* dvar_zdepthDistance;

	//FOG DVARS
	static dvar_s* dvar_fogStartDist;
	static dvar_s* dvar_fogFadeDist;
	static dvar_s* dvar_fogStartHeight;
	static dvar_s* dvar_fogFadeHeight;
	static dvar_s* dvar_fogOpacity;
	static dvar_s* dvar_fogSunPitch;
	static dvar_s* dvar_fogSunYaw;
	static dvar_s* dvar_fogSunInner;
	static dvar_s* dvar_fogSunOuter;
	static dvar_s* dvar_fogColor;
	static dvar_s* dvar_fogSunOpacity;
	static dvar_s* dvar_fogSunColor;

	//LIGHTS DVARS
	static dvar_s* dvar_lightRadiusLimit;
	static dvar_s* dvar_lightColorLimit;


	//STREAMS DVARS
	static dvar_s* dvar_streams;
	static dvar_s* dvar_streams_noFlash;
	static dvar_s* dvar_streams_directory;
	static dvar_s* dvar_streams_fps;
	static dvar_s* dvar_streams_JPG;
	static dvar_s* dvar_streams_avi;
	static dvar_s* dvar_streams_tickStart;
	static dvar_s* dvar_streams_tickEnd;
	static dvar_s* dvar_streams_recordCam;

	//WEAPONS DVARS
	static dvar_s* dvar_camoChanging;
	static dvar_s* dvar_primaryCamo;
	static dvar_s* dvar_secondaryCamo;
	static dvar_s* dvar_cameraSway;
	static dvar_s* dvar_cameraSwayMaxAngle;
	//HOLDGUN DVARS
	static dvar_s* dvar_holdgun;
	static dvar_s* dvar_holdgunWrist;
	static dvar_s* dvar_holdgunSearchRadius;

	//MENU DVARS
	static dvar_s* dvar_menuBlur;
	static dvar_s* dvar_showBone;
	static dvar_s* dvar_tick;
	static dvar_s* dvar_uiGridDebug;

	//DEMO DVARS
	static dvar_s* dvar_demos_directory;

	void CheckDvars()
	{

	}
	inline static void Init()
	{
		dvar_shake = T6SDK::Dvars::RegisterBool("mvm_shake", false, "Dolly camera shake.");
		dvar_shakeSpeed = T6SDK::Dvars::RegisterFloat("mvm_shakeSpeed", 1.0f, 0.0f, 10.0f, "Dolly camera shake speed.");
		dvar_shakeAmount = T6SDK::Dvars::RegisterFloat("mvm_shakeAmount", 1.0f, 0.0f, 10.0f, "Dolly camera overall shake amount.");
		dvar_shakePosition = T6SDK::Dvars::RegisterFloat("mvm_shakePosition", 1.0f, 0.0f, 10.0f, "Dolly camera position shake intensity.");
		dvar_shakeRotation = T6SDK::Dvars::RegisterFloat("mvm_shakeRotation", 1.0f, 0.0f, 10.0f, "Dolly camera rotation shake intensity.");

		dvar_rollStep = T6SDK::Dvars::RegisterFloat("mvm_rollStep", 5.0f, 1.0f, 90.0f, "Amount that camera roll angle will be changed at during mouse wheel scrolling.");
		dvar_showMarkerInfo = T6SDK::Dvars::RegisterBool("mvm_showMarkerInfo", true, "Show dolly camera markers information (fov, roll).");
		dvar_frozenCam = T6SDK::Dvars::RegisterBool("mvm_frozenCam", false, "Frozen dolly camera.");
		dvar_demoTimescaleStep = T6SDK::Dvars::RegisterFloat("mvm_demoTimescaleStep", 0.1f, 0.001f, 10.0f, "Step the demo timescale value will be changed with.");

		dvar_dollyCamOffsetPos = T6SDK::Dvars::RegisterVec3("mvm_dollyCamOffsetPos", 0.0f, 0.0f, 0.0f, -5000.0f, 5000.0f, "Dolly camera position offset (x, y, z).");
		dvar_dollyCamOffsetRot = T6SDK::Dvars::RegisterVec3("mvm_dollyCamOffsetRot", 0.0f, 0.0f, 0.0f, -360.0f, 360.0f, "Dolly camera rotation offset (pitch, yaw, roll).");

		dvar_freeRoamSpeed = T6SDK::Dvars::RegisterFloat("mvm_freeRoamSpeed", 1.0f, 0.0f, 10.0f, "Free camera speed.");
		dvar_accelerationFactor = T6SDK::Dvars::RegisterFloat("mvm_accelerationFactor", 2.0f, 0.0f, 10.0f, "Free camera acceleration factor.");
		dvar_slowingFactor = T6SDK::Dvars::RegisterFloat("mvm_slowingFactor", 0.25f, 0.0f, 10.0f, "Free camera slowing factor.");
		dvar_cameraFov = T6SDK::Dvars::RegisterFloat("mvm_cameraFov", 90.0f, 1.0f, 180.0f, "Free camera fov.");
		dvar_cameraRoll = T6SDK::Dvars::RegisterFloat("mvm_cameraRoll", 0.0f, -180.0f, 180.0f, "Free camera roll.");

		//Register misc dvars
		dvar_greenScreen = T6SDK::Dvars::RegisterBool("mvm_greenScreen", false, "Green screen.");
		dvar_greenScreenPlayers = T6SDK::Dvars::RegisterInt("mvm_greenScreenPlayers", 1, 0, 3, "Green screen players.");
		dvar_greenScreenPlayersThreshold = T6SDK::Dvars::RegisterFloat("mvm_greenScreenPlayersThreshold", 500.0f, 0.0f, 5000.0f, "Green screen players threshold.");
		dvar_greenScreenColor = T6SDK::Dvars::RegisterVec3("mvm_greenScreenColor", 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, "Green screen color");
		dvar_zdepth = T6SDK::Dvars::RegisterBool("mvm_zdepth", false, "Z depth.");
		dvar_zdepthDistance = T6SDK::Dvars::RegisterFloat("mvm_zdepthDistance", 1000.0f, 0.0f, 5000.0f, "Z depth distance.");

		//Register fog dvars
		dvar_fogStartDist = T6SDK::Dvars::RegisterFloat("z_fogstartdist", 0.0f, -200.0f, 5000.0f, "");
		dvar_fogFadeDist = T6SDK::Dvars::RegisterFloat("z_fogfadedist", 36000.0f, -200.0f, 37000.0f, "");
		dvar_fogStartHeight = T6SDK::Dvars::RegisterFloat("z_fogstartheight", 2000.0f, -200.0f, 37000.0f, "");
		dvar_fogFadeHeight = T6SDK::Dvars::RegisterFloat("z_fogfadeheight", 18070.0f, -200.0f, 50000.0f, "");
		dvar_fogOpacity = T6SDK::Dvars::RegisterFloat("z_fogopacity", 0.8f, -10.0f, 10.0f, "");
		dvar_fogSunPitch = T6SDK::Dvars::RegisterFloat("z_fogsunpitch", 0.0f, 0.0f, 360.0f, "");
		dvar_fogSunYaw = T6SDK::Dvars::RegisterFloat("z_fogsunyaw", 0.0f, 0.0f, 360.0f, "");
		dvar_fogSunInner = T6SDK::Dvars::RegisterFloat("z_fogsuninner", 0.0f, 0.0f, 30.0f, "");
		dvar_fogSunOuter = T6SDK::Dvars::RegisterFloat("z_fogsunouter", 30.0f, 0.0f, 60.0f, "");
		dvar_fogColor = T6SDK::Dvars::RegisterVec3("z_fogcolor", 255.0f, 255.0f, 255.0f, 0.0f, 255.0f, "");
		dvar_fogSunOpacity = T6SDK::Dvars::RegisterFloat("z_fogsunopacity", 0.8f, -10.0f, 10.0f, "");
		dvar_fogSunColor = T6SDK::Dvars::RegisterVec3("z_fogsuncolor", 255.0f, 255.0f, 255.0f, 0.0f, 255.0f, "");

		//Register lights dvars
		dvar_lightRadiusLimit = T6SDK::Dvars::RegisterFloat("mvm_lightRadiusLimit", 1000.0f, 0.0f, 10000.0f, "Light radius limit.");
		dvar_lightColorLimit = T6SDK::Dvars::RegisterFloat("mvm_lightColorLimit", 1.0f, 0.0f, 255.0f, "Light color limit.");

		//Register streams dvars
		dvar_streams = T6SDK::Dvars::RegisterBool("mvm_streams", false, "Enable/disable streams recording.");
		dvar_streams_noFlash = T6SDK::Dvars::RegisterBool("mvm_streams_noFlash", true, "Disable displaying any other passes on streams to prevent screen flash.");
		dvar_streams_directory = T6SDK::Dvars::RegisterString("mvm_streams_directory", "", "Directory where streams will be saved.");
		dvar_streams_fps = T6SDK::Dvars::RegisterInt("mvm_streams_fps", 50, 50, 1000, "Streams recording frame rate.");
		dvar_streams_avi = T6SDK::Dvars::RegisterBool("mvm_streams_avi", false, "Streams recording AVI format.");
		dvar_streams_JPG = T6SDK::Dvars::RegisterBool("mvm_streams_JPG", false, "Record streams in JPG format.");
		dvar_streams_tickStart = T6SDK::Dvars::RegisterInt("mvm_streams_tickStart", -1, -1, 99999999, "Streams recording tick start value.");
		dvar_streams_tickEnd = T6SDK::Dvars::RegisterInt("mvm_streams_tickEnd", -1, -1, 99999999, "Streams recording tick end value.");
		dvar_streams_recordCam = T6SDK::Dvars::RegisterBool("mvm_streams_recordCam", false, "Collect camera data on each recorded frame.");


		//Register weapon dvars
		dvar_camoChanging = T6SDK::Dvars::RegisterBool("mvm_camoChanging", false, "Enable weapon camo changing.");
		dvar_primaryCamo = T6SDK::Dvars::RegisterInt("mvm_primaryCamo", -1, -1, 44, "Primary weapon camo index.");
		dvar_secondaryCamo = T6SDK::Dvars::RegisterInt("mvm_secondaryCamo", -1, -1, 44, "Secondary weapon camo index.");
		dvar_cameraSway = T6SDK::Dvars::RegisterBool("mvm_cameraSway", false, "Enable camera sway.");
		dvar_cameraSwayMaxAngle = T6SDK::Dvars::RegisterFloat("mvm_cameraSwayMaxAngle", 20.0f, 0.0f, 90.0f, "Camera sway max angle.");
		//Register holdgun dvars
		dvar_holdgun = T6SDK::Dvars::RegisterBool("mvm_holdgun", false, "Make corpses hold gun.");
		dvar_holdgunWrist = T6SDK::Dvars::RegisterBool("mvm_holdgunWrist", false, "Attach weapon in holdgun mode to wrist, not to tag_weapon_right.");
		dvar_holdgunSearchRadius = T6SDK::Dvars::RegisterFloat("mvm_holdgunSearchRadius", 100.0f, 0.0f, 255.0f, "Radius near corpse that T6MVM will search dropped weapon in.");
		//Register menu dvars
		dvar_menuBlur = T6SDK::Dvars::RegisterBool("mvm_menuBlur", true, "Menu blur.");
		dvar_showBone = T6SDK::Dvars::RegisterBool("mvm_showBone", true, "Show selected bone.");
		dvar_tick = T6SDK::Dvars::RegisterInt("mvm_tick", 0, 11000, 99999999, "Go to desired tick.");
		dvar_uiGridDebug = T6SDK::Dvars::RegisterBool("mvm_ui_grid", false, "Show UI grid.");
		//Register demo dvars
		dvar_demos_directory = T6SDK::Dvars::RegisterString("mvm_demosDirectory", "", "Folder with all the demos you want to see in your theater.");
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnActiveFrameDrawn, (uintptr_t)&CheckDvars);
		T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_SUCCESS, false, "DVARS", "Custom dvars and commands initialized!");
	}
}