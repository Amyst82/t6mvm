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
		if (T6SDK::Addresses::cg->client[entityState->clientNum].weaponSlot2 == 0x00) //If primary weapon
		{
			if (T6SDK::Dvars::GetInt(CustomDvars::dvar_primaryCamo) > -1) //if any camo is selected
			{
				entityState->renderOptions.s = (BYTE)T6SDK::Dvars::GetInt(CustomDvars::dvar_primaryCamo);
			}
		}
		else if(T6SDK::Addresses::cg->client[entityState->clientNum].weaponSlot2 == 0x01) //If secondary weapon
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
			return;
		int demo_client = T6SDK::Dvars::GetInt(*T6SDK::Dvars::DvarList::demo_client);
		if(T6SDK::Addresses::cg->client[demo_client].weaponSlot2 == 0x00) //If primary weapon
		{
			if (T6SDK::Dvars::GetInt(CustomDvars::dvar_primaryCamo) > -1) //if any camo is selected
			{
				T6SDK::InternalFunctions::CG_GetEntity(demo_client)->nextState.renderOptions.s = (BYTE)T6SDK::Dvars::GetInt(CustomDvars::dvar_primaryCamo);
			}
		}
		else if(T6SDK::Addresses::cg->client[demo_client].weaponSlot2 == 0x01) //If secondary weapon
		{
			if (T6SDK::Dvars::GetInt(CustomDvars::dvar_secondaryCamo) > -1) //if any camo is selected
			{
				T6SDK::InternalFunctions::CG_GetEntity(demo_client)->nextState.renderOptions.s = (BYTE)T6SDK::Dvars::GetInt(CustomDvars::dvar_secondaryCamo);
			}
		}
	}

	uintptr_t eaxTMP, ecxTMP, edxTMP, esiTMP, ediTMP, espTMP, ebpTMP;
	__declspec(naked) void CinesCamoPatch()
	{
		__asm
		{
			mov[eaxTMP], eax
			mov[edxTMP], edx
			mov[ecxTMP], ecx
			mov[esiTMP], esi
			mov[ediTMP], edi
			mov[espTMP], esp
			mov[ebpTMP], ebp
		}
		if (T6SDK::MAIN::ENABLED)
		{
			_asm
			{
				mov eax, [eaxTMP]
				mov edx, [edxTMP]
				mov ecx, [ecxTMP]
				mov esi, [esiTMP]
				mov edi, [ediTMP]
				mov esp, [espTMP]
				mov ebp, [ebpTMP]
				call Update
				mov eax, [eaxTMP]
				mov edx, [edxTMP]
				mov ecx, [ecxTMP]
				mov esi, [esiTMP]
				mov edi, [ediTMP]
				mov esp, [espTMP]
				mov ebp, [ebpTMP]
				mov ecx, 0x3E
				jmp[T6SDK::Addresses::HookAddresses::h_CamoChangingCinePatch.JumpBackAddress]
			}
		}
		else
		{
			_asm
			{
				mov eax, [eaxTMP]
				mov edx, [edxTMP]
				mov ecx, [ecxTMP]
				mov esi, [esiTMP]
				mov edi, [ediTMP]
				mov esp, [espTMP]
				mov ebp, [ebpTMP]
				mov ecx, 0x3E
				jmp[T6SDK::Addresses::HookAddresses::h_CamoChangingCinePatch.JumpBackAddress]
			}
		}
	}

	static void Init()
	{
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnActiveFrameDrawn, (uintptr_t)&Update);
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnPovCamoWriting, (uintptr_t)&PovCamoUpdate);
		T6SDK::Addresses::HookAddresses::h_CamoChangingCinePatch.Hook(CinesCamoPatch);
	}
}