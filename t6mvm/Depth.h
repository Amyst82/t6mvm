#pragma once
#include "IMVMStream.h"
class Stream_Depth : public IMVMStream
{
private:
	bool prevBloomTweaks = false;
	int prevClearColor = 0;
	int prevClearColor2 = 0;
	float prevDOFBias = 0.5f;
	int prevDOFHDR = 2;
	bool prevDOFTweak = false;
	bool prevDOFEnable = false;
	float prevDOFFarBlur = 1.0f;
	float prevDOFFarStart = 1000.0f;
	float prevDOFFarEnd = 7000.0f;
	float prevDOFNearBlur = 1.0f;
	float prevDOFNearStart = 100.0f;
	float prevDOFNearEnd = 2000.0f;
	float prevDOFViewModelStart = 0.5f;
	float prevDOFViewModelEnd = 0.5f;
	bool prevDrawGun = true;
	BYTE prevBaseTechType = 0x04;
	BYTE prevEmissiveTechType = 0x03;
public:
	void Enable()
	{
		prevBaseTechType = T6SDK::Addresses::GfxDrawMethod.Value().baseTechType;
		prevEmissiveTechType = T6SDK::Addresses::GfxDrawMethod.Value().emissiveTechType;
		prevBloomTweaks = T6SDK::Dvars::GetBool(*T6SDK::Dvars::DvarList::r_bloomTweaks);
		prevClearColor = T6SDK::Dvars::GetInt(*T6SDK::Dvars::DvarList::r_clearColor);
		prevClearColor2 = T6SDK::Dvars::GetInt(*T6SDK::Dvars::DvarList::r_clearColor2);
		prevDOFBias = T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_dof_bias);
		prevDOFTweak = T6SDK::Dvars::GetBool(*T6SDK::Dvars::DvarList::r_dof_tweak);
		prevDOFEnable = T6SDK::Dvars::GetBool(*T6SDK::Dvars::DvarList::r_dof_enable);
		prevDOFHDR = T6SDK::Dvars::GetInt(*T6SDK::Dvars::DvarList::r_dofHDR);
		prevDOFFarBlur = T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_dof_farBlur);
		prevDOFFarStart = T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_dof_farStart);
		prevDOFFarEnd = T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_dof_farEnd);
		prevDOFNearBlur = T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_dof_nearBlur);
		prevDOFNearStart = T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_dof_nearStart);
		prevDOFNearEnd = T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_dof_nearEnd);
		prevDOFViewModelStart = T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_dof_viewModelStart);
		prevDOFViewModelEnd = T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_dof_viewModelEnd);
		prevDrawGun = T6SDK::Dvars::GetBool(*T6SDK::Dvars::DvarList::cg_drawGun);

		T6SDK::Addresses::GfxDrawMethod.Value().baseTechType = 0x00;
		T6SDK::Addresses::GfxDrawMethod.Value().emissiveTechType = 0x00;
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_bloomTweaks, true);
		T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_clearColor,  0x00FFFFFF);
		T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_clearColor2, 0x00FFFFFF);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_bias, 0.0f);
		T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_dofHDR, 0);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_dof_tweak, true);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_dof_enable, true);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_farBlur, 1.0f);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_farStart, 0.0f);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_farEnd, T6SDK::Dvars::GetFloat(CustomDvars::dvar_zdepthDistance));
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_nearBlur, 1.0f);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_nearStart, 1.0f);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_nearEnd, 1.0f);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_viewModelStart, 0.0f);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_viewModelEnd, 0.0f);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::cg_drawGun, false);
		Enabled = true;
	}
	void Disable()
	{
		T6SDK::Addresses::GfxDrawMethod.Value().baseTechType = prevBaseTechType;
		T6SDK::Addresses::GfxDrawMethod.Value().emissiveTechType = prevEmissiveTechType;
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_bloomTweaks, prevBloomTweaks);
		T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_clearColor, prevClearColor);
		T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_clearColor2, prevClearColor2);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_bias, prevDOFBias);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_dof_tweak, prevDOFTweak);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_dof_enable, prevDOFEnable);
		T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_dofHDR, prevDOFHDR);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_farBlur, prevDOFFarBlur);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_farStart, prevDOFFarStart);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_farEnd, prevDOFFarEnd);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_nearBlur, prevDOFNearBlur);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_nearStart, prevDOFNearStart);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_nearEnd, prevDOFNearEnd);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_viewModelStart, prevDOFViewModelStart);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_viewModelEnd, prevDOFViewModelEnd);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::cg_drawGun, prevDrawGun);
		Enabled = false;
	}
	void Init()
	{

		IMVMStream::Name = "Depth";
		IMVMStream::toggle = T6SDK::Dvars::RegisterBool("mvm_streams_depth", false, "Depth buffer stream.");
	}
};