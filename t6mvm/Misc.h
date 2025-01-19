#pragma once
#include <StdInclude.h>
#include "UIBase.h"
#include "Streams.h"
namespace Misc
{
	void DetouredR_AddDObjToScene(void* obj, entity_t* entity, unsigned int entnum, unsigned int renderFxFlags, vec3_t* lightingOrigin, float* materialTime, float* burnFraction, char altXModel, int textureOverrideIndex, void* dobjConstantSet, float lightingOriginToleranceSq, float scale, bool isMarkableViewmodel)
	{	
		if (entity->pose.eType == (BYTE)T6SDK::EntityType::PLAYERENTITY || entity->pose.eType == (BYTE)T6SDK::EntityType::TEMPENTITY || entity->pose.eType == (BYTE)T6SDK::EntityType::ACTOR || entity->pose.eType == (BYTE)T6SDK::EntityType::ACTOR_CORPSE || entity->pose.eType == (BYTE)T6SDK::EntityType::PLAYERCORPSEENTITY)
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
			if (Streams::NoPlayers.toggle->current.enabled == true && Streams::NoPlayers.Enabled == true)
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
	static void Update()
	{
		if(CustomDvars::dvar_greenScreen->modified)
		{
			SwitchGreenScreenState();
			CustomDvars::dvar_greenScreen->modified = false;
		}
	}
	static void Init()
	{
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnActiveFrameDrawn, (uintptr_t)&Update);
		T6SDK::Addresses::DetoursAddresses::DetouredAddDObjToScene.Hook(DetouredR_AddDObjToScene);
	}
}