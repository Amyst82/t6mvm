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

	cmd_function_s cmd_exportCam_VAR{};
	cmd_function_s cmd_importCam_VAR{};

	//MISC DVARS
	static dvar_s* dvar_greenScreen;
	const char* gsPlayersEnum[4] = {"No", "Selected player", "Threshold", "All players"};
	static dvar_s* dvar_greenScreenPlayers;
	static dvar_s* dvar_greenScreenPlayersThreshold;
	static dvar_s* dvar_zdepth;


	//LIGHTS DVARS
	static dvar_s* dvar_lightRadiusLimit;
	static dvar_s* dvar_lightColorLimit;
	cmd_function_s cmd_exportLights_VAR{};
	cmd_function_s cmd_importLights_VAR{};

	//MENU DVARS
	static dvar_s* dvar_menuBlur;
	static dvar_s* dvar_showBone;


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

		T6SDK::Dvars::Cmd_AddCommandInternal("mvm_exportCam", nullptr, &cmd_exportCam_VAR); //TODO Add implementation
		T6SDK::Dvars::Cmd_AddCommandInternal("mvm_importCam", nullptr, &cmd_importCam_VAR);

		//Register misc dvars
		dvar_greenScreen = T6SDK::Dvars::RegisterBool("mvm_greenScreen", false, "Green screen.");
		dvar_greenScreenPlayers = T6SDK::Dvars::RegisterEnum("mvm_greenScreenPlayers", gsPlayersEnum, 1, "Green screen players.");
		dvar_greenScreenPlayersThreshold = T6SDK::Dvars::RegisterFloat("mvm_greenScreenPlayersThreshold", 500.0f, 0.0f, 5000.0f, "Green screen players threshold.");
		dvar_zdepth = T6SDK::Dvars::RegisterBool("mvm_zdepth", false, "Z depth.");

		//Register lights dvars
		T6SDK::Dvars::Cmd_AddCommandInternal("mvm_exportLights", nullptr, &cmd_exportLights_VAR);
		T6SDK::Dvars::Cmd_AddCommandInternal("mvm_importLights", nullptr, &cmd_importLights_VAR);
		dvar_lightRadiusLimit = T6SDK::Dvars::RegisterFloat("mvm_lightRadiusLimit", 1000.0f, 0.0f, 10000.0f, "Light radius limit.");
		dvar_lightColorLimit = T6SDK::Dvars::RegisterFloat("mvm_lightColorLimit", 1.0f, 0.0f, 255.0f, "Light color limit.");

		//Register menu dvars
		dvar_menuBlur = T6SDK::Dvars::RegisterBool("mvm_menuBlur", true, "Menu blur.");
		dvar_showBone = T6SDK::Dvars::RegisterBool("mvm_showBone", true, "Show selected bone.");


		T6SDK::ConsoleLog::LogSuccess("Custom dvars and commands initialized!");
	}
}