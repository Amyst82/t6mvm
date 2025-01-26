#pragma once
#include <StdInclude.h>
#include "CustomDvars.h"
namespace Weapon
{
	static void PovCamoUpdate(int entityStateEDI)
	{
		if (!T6SDK::Theater::IsInTheater())
			return;
		if (!T6SDK::Theater::IsPlaybackInited())
			return;
		if(!T6SDK::Dvars::GetBool(CustomDvars::dvar_camoChanging))
			return;
		entityState_t* entityState = (entityState_t*)(entityStateEDI);
		if(T6SDK::Dvars::GetInt(*T6SDK::Dvars::DvarList::demo_client) != entityState->clientNum)
			return;
		if (T6SDK::Addresses::cg->client[entityState->clientNum].weaponSlot == 0x00) //If primary weapon
		{
			if (T6SDK::Dvars::GetInt(CustomDvars::dvar_primaryCamo) > -1) //if any camo is selected
			{
				entityState->renderOptions.s = (BYTE)T6SDK::Dvars::GetInt(CustomDvars::dvar_primaryCamo);
			}
		}
		else if(T6SDK::Addresses::cg->client[entityState->clientNum].weaponSlot == 0x01) //If secondary weapon
		{
			if (T6SDK::Dvars::GetInt(CustomDvars::dvar_secondaryCamo) > -1) //if any camo is selected
			{
				entityState->renderOptions.s = (BYTE)T6SDK::Dvars::GetInt(CustomDvars::dvar_secondaryCamo);
			}
		}
		
	}
	static void Update()
	{
		if (!T6SDK::Theater::IsInTheater())
			return;
		if (!T6SDK::Theater::IsPlaybackInited())
			return;
		if (!T6SDK::Dvars::GetBool(CustomDvars::dvar_camoChanging))
		{
			T6SDK::Addresses::Patches::CamoChangingCinePatch.UnPatch();
			return;
		}
		T6SDK::Addresses::Patches::CamoChangingCinePatch.Patch();
		int demo_client = T6SDK::Dvars::GetInt(*T6SDK::Dvars::DvarList::demo_client);
		if(T6SDK::Addresses::cg->client[demo_client].weaponSlot == 0x00) //If primary weapon
		{
			if (T6SDK::Dvars::GetInt(CustomDvars::dvar_primaryCamo) > -1) //if any camo is selected
			{
				T6SDK::InternalFunctions::CG_GetEntity(demo_client)->nextState.renderOptions.s = (BYTE)T6SDK::Dvars::GetInt(CustomDvars::dvar_primaryCamo);
			}
		}
		else if(T6SDK::Addresses::cg->client[demo_client].weaponSlot == 0x01) //If secondary weapon
		{
			if (T6SDK::Dvars::GetInt(CustomDvars::dvar_secondaryCamo) > -1) //if any camo is selected
			{
				T6SDK::InternalFunctions::CG_GetEntity(demo_client)->nextState.renderOptions.s = (BYTE)T6SDK::Dvars::GetInt(CustomDvars::dvar_secondaryCamo);
			}
		}
	}
	static void Init()
	{
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnActiveFrameDrawn, (uintptr_t)&Update);
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnPovCamoWriting, (uintptr_t)&PovCamoUpdate);
	}
}