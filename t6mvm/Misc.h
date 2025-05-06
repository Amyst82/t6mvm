#pragma once
#include <StdInclude.h>
#include "UIBase.h"
#include "Streams.h"

namespace Misc
{
	void DetouredR_AddDObjToScene(void* obj, entity_t* entity, unsigned int entnum, unsigned int renderFxFlags, vec3_t* lightingOrigin, float* materialTime, float* burnFraction, char altXModel, int textureOverrideIndex, void* dobjConstantSet, float lightingOriginToleranceSq, float scale, bool isMarkableViewmodel)
	{	

		if (entity->pose.eType == (BYTE)T6SDK::EntityType::PLAYERENTITY || entity->pose.eType == (BYTE)T6SDK::EntityType::TEMPENTITY || entity->pose.eType == (BYTE)T6SDK::EntityType::ACTOR || entity->pose.eType == (BYTE)T6SDK::EntityType::ACTOR_CORPSE || entity->pose.eType == (BYTE)T6SDK::EntityType::PLAYERCORPSEENTITY || entity->pose.eType == (BYTE)T6SDK::EntityType::ITEMENTITY)
		{
			if (T6SDK::Dvars::GetBool(CustomDvars::dvar_greenScreen) || (Streams::GreenScreen.toggle->current.enabled && Streams::GreenScreen.Enabled))
			{
				if (CustomDvars::dvar_greenScreenPlayers->current.integer == 0) //No players
				{
					if (entity->pose.eType == (BYTE)T6SDK::EntityType::TEMPENTITY)
						renderFxFlags = 0xFF44444;// 0xFF000006;
				}
				else if (CustomDvars::dvar_greenScreenPlayers->current.integer == 1) //Selected player
				{
					if (entity->pose.eType == (BYTE)T6SDK::EntityType::PLAYERENTITY || entity->pose.eType == (BYTE)T6SDK::EntityType::PLAYERCORPSEENTITY)
					{
						if (entity->nextState.clientNum == T6SDK::Dvars::GetInt(*T6SDK::Dvars::DvarList::demo_client))
							renderFxFlags = 0xFF44444;//  0xFF000006;
					}
					else if (entity->pose.eType == (BYTE)T6SDK::EntityType::ITEMENTITY) //Include weapon
					{
						if (entity->nextState.clientNum == T6SDK::Dvars::GetInt(*T6SDK::Dvars::DvarList::demo_client))
							renderFxFlags = 0xFF44444;//  0xFF000006;
					}
				}
				else if (CustomDvars::dvar_greenScreenPlayers->current.integer == 2) //Threshold
				{
					if (T6SDK::Addresses::DemoPlayback.Value()->CameraMode == T6SDK::DemoCameraMode::NONE) //POV
					{
						if (T6SDK::InternalFunctions::Distance(T6SDK::Addresses::cg->RefDef.vOrigin, entity->pose.origin) < T6SDK::Dvars::GetFloat(CustomDvars::dvar_greenScreenPlayersThreshold))
							renderFxFlags = 0xFF44444;//  0xFF000006;
					}
					else
					{
						if (T6SDK::InternalFunctions::Distance(T6SDK::Addresses::DemoPlayback.Value()->FreeRoamCamera.Origin, entity->pose.origin) < T6SDK::Dvars::GetFloat(CustomDvars::dvar_greenScreenPlayersThreshold))
							renderFxFlags = 0xFF44444;//  0xFF000006;
					}
				}
				else //All players
					renderFxFlags = 0xFF44444;//  0xFF000006;
			}
			//If we have NoPLayers stream enabled and its currently being used we just dont draw players
			if (Streams::NoPlayers.Enabled == true)
				return;
			//call original function
			T6SDK::Addresses::DetoursAddresses::DetouredAddDObjToScene.call_original_noreturn<T6SDK::Typedefs::R_AddDObjToScene_t, void*, entity_t*, unsigned int, unsigned int, vec3_t*, float*, float*, char, int, void*, float, float, bool>
				(obj, entity, entnum, renderFxFlags, lightingOrigin, materialTime, burnFraction, altXModel, textureOverrideIndex, dobjConstantSet, lightingOriginToleranceSq, scale, isMarkableViewmodel);
		}
		else
		{
			if (T6SDK::Dvars::GetBool(CustomDvars::dvar_greenScreen) || (Streams::GreenScreen.toggle->current.enabled && Streams::GreenScreen.Enabled))
				return;
			//Just original function
			T6SDK::Addresses::DetoursAddresses::DetouredAddDObjToScene.call_original_noreturn<T6SDK::Typedefs::R_AddDObjToScene_t, void*, entity_t*, unsigned int, unsigned int, vec3_t*, float*, float*, char, int, void*, float, float, bool>
				(obj, entity, entnum, renderFxFlags, lightingOrigin, materialTime, burnFraction, altXModel, textureOverrideIndex, dobjConstantSet, lightingOriginToleranceSq, scale, isMarkableViewmodel);
		}
	}

	static void SwitchGreenScreenState()
	{
		if (T6SDK::Dvars::GetBool(CustomDvars::dvar_greenScreen)) //GreenScreen enabled
		{
			Streams::GreenScreen.Enable();
		}
		else
		{
			Streams::GreenScreen.Disable();
		}
	}
	static void SwitchDepthState()
	{
		if (T6SDK::Dvars::GetBool(CustomDvars::dvar_zdepth)) //ZDepth enabled
		{
			Streams::Depth.Enable();
		}
		else
		{
			Streams::Depth.Disable();
		}
	}
	static void UpdateClearColors(float value)
	{
		float clrR = max(0.0f, min(1.0f, CustomDvars::dvar_greenScreenColor->current.vector[0]));
		(*T6SDK::Dvars::DvarList::r_clearColor)->current.color[0] = static_cast<unsigned char>(clrR * 255.0f + 0.5f);
		(*T6SDK::Dvars::DvarList::r_clearColor2)->current.color[0] = static_cast<unsigned char>(clrR * 255.0f + 0.5f);

		float clrG = max(0.0f, min(1.0f, CustomDvars::dvar_greenScreenColor->current.vector[1]));
		(*T6SDK::Dvars::DvarList::r_clearColor)->current.color[1] = static_cast<unsigned char>(clrG * 255.0f + 0.5f);
		(*T6SDK::Dvars::DvarList::r_clearColor2)->current.color[1] = static_cast<unsigned char>(clrG * 255.0f + 0.5f);

		float clrB = max(0.0f, min(1.0f, CustomDvars::dvar_greenScreenColor->current.vector[2]));
		(*T6SDK::Dvars::DvarList::r_clearColor)->current.color[2] = static_cast<unsigned char>(clrB * 255.0f + 0.5f);
		(*T6SDK::Dvars::DvarList::r_clearColor2)->current.color[2] = static_cast<unsigned char>(clrB * 255.0f + 0.5f);
	}
	
	static void UpdateDepthDistanceValue(float value)
	{
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_farEnd, value);
	}
	static void GreenSkySwitchStates(T6SDK::Drawing::UI_CheckBoxButton* control)
	{
		bool state = *control->isChecked;
		T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "MISC", "Green sky state: %s", state ? "true":"false");
		if (!state)
		{
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "MISC", "Disabling green sky...");
			Streams::GreenSky.Disable();
			Streams::GreenSky.Enabled = false;
			*control->isChecked = false;
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "MISC", "Green sky disabled.");
		}
		else
		{
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "MISC", "Enabling green sky...");
			Streams::GreenSky.Enable();
			Streams::GreenSky.Enabled = true;
			*control->isChecked = true;
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "MISC", "Green sky enabled.");
		}
	}

	static void TickRateChanging(int value)
	{
		if (value <= 0)
		{
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_WARNING, true, "MISCTICK", "Tick below zero!");
			return;
		}
		T6SDK::Theater::GoToTick(value);
		T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "MISCTICK", "Tick set!");
	}
	static void Update()
	{
		if (!T6SDK::Theater::IsInTheater())
			return;
		if (!T6SDK::Theater::IsPlaybackInited())
			return;
		if (CustomDvars::dvar_tick->modified)
		{
			T6SDK::Theater::GoToTick(T6SDK::Dvars::GetInt(CustomDvars::dvar_tick));
			//UIControls::UI_TimelineSlider.AddBookmark(T6SDK::Dvars::GetInt(CustomDvars::dvar_tick));
			T6SDK::Dvars::SetInt(CustomDvars::dvar_tick, 0);
			CustomDvars::dvar_tick->modified = false;
		}
		if(CustomDvars::dvar_greenScreen->modified)
		{
			SwitchGreenScreenState();
			CustomDvars::dvar_greenScreen->modified = false;
		}
		if (CustomDvars::dvar_greenScreenColor->modified)
		{
			float clrR = max(0.0f, min(1.0f, CustomDvars::dvar_greenScreenColor->current.vector[0]));
			(*T6SDK::Dvars::DvarList::r_clearColor)->current.color[0] = static_cast<unsigned char>(clrR * 255.0f + 0.5f);
			(*T6SDK::Dvars::DvarList::r_clearColor2)->current.color[0] = static_cast<unsigned char>(clrR * 255.0f + 0.5f);

			float clrG = max(0.0f, min(1.0f, CustomDvars::dvar_greenScreenColor->current.vector[1]));
			(*T6SDK::Dvars::DvarList::r_clearColor)->current.color[1] = static_cast<unsigned char>(clrG * 255.0f + 0.5f);
			(*T6SDK::Dvars::DvarList::r_clearColor2)->current.color[1] = static_cast<unsigned char>(clrG * 255.0f + 0.5f);

			float clrB = max(0.0f, min(1.0f, CustomDvars::dvar_greenScreenColor->current.vector[2]));
			(*T6SDK::Dvars::DvarList::r_clearColor)->current.color[2] = static_cast<unsigned char>(clrB * 255.0f + 0.5f);
			(*T6SDK::Dvars::DvarList::r_clearColor2)->current.color[2] = static_cast<unsigned char>(clrB * 255.0f + 0.5f);

			CustomDvars::dvar_greenScreenColor->modified = false;
		}
		if (CustomDvars::dvar_zdepth->modified)
		{
			SwitchDepthState();
			CustomDvars::dvar_zdepth->modified = false;
		}
		if (CustomDvars::dvar_zdepthDistance->modified)
		{
			T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_farEnd, T6SDK::Dvars::GetFloat(CustomDvars::dvar_zdepthDistance));
			CustomDvars::dvar_zdepthDistance->modified = false;
		}
	}
	static void Init()
	{
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnActiveFrameDrawn, (uintptr_t)&Update);
		T6SDK::Addresses::DetoursAddresses::DetouredAddDObjToScene.Hook(DetouredR_AddDObjToScene);
	}
}