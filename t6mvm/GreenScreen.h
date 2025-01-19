#pragma once
#include "IMVMStream.h"
class Stream_GreenScreen : public IMVMStream
{
private:
	bool prevBloomTweaks = false;
	bool prevFxMarksDraw = false;
	float prevZNear = 4.0f;
	bool prevFlareEffects = false;
	bool prevSunFlares = false;
	bool prevDrawGun = false;
public:
	void Enable() 
	{
		prevBloomTweaks = T6SDK::Dvars::GetBool(*T6SDK::Dvars::DvarList::r_bloomTweaks);
		prevFxMarksDraw = T6SDK::Dvars::GetBool(*T6SDK::Dvars::DvarList::fx_marks_draw);
		prevZNear = T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_znear);
		prevFlareEffects = T6SDK::Dvars::GetBool(*T6SDK::Dvars::DvarList::flareDisableEffects);
		prevSunFlares = T6SDK::Dvars::GetBool(*T6SDK::Dvars::DvarList::r_superFlareDraw);
		prevDrawGun = T6SDK::Dvars::GetBool(*T6SDK::Dvars::DvarList::cg_drawGun);
		T6SDK::Addresses::Patches::RemoveSkyBoxPatch.Patch();
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_skipPvs, true);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_bloomTweaks, true);
		T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_clearColor, 0x0000FF00);
		T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_clearColor2, 0x0000FF00);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::fx_marks_draw, false);
		//T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_znear, 10000.0f);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::flareDisableEffects, true);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_superFlareDraw, false);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::cg_drawGun, true);
		Enabled = true;
	}
	void Disable()
	{
		T6SDK::Addresses::Patches::RemoveSkyBoxPatch.UnPatch();
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_skipPvs, false);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_bloomTweaks, prevBloomTweaks);
		T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_clearColor, 0x00);
		T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_clearColor2, 0x00);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::fx_marks_draw, prevFxMarksDraw);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_znear, prevZNear);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::flareDisableEffects, prevFlareEffects);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_superFlareDraw, prevSunFlares);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::cg_drawGun, prevDrawGun);
		Enabled = false;
	}
	void Init()
	{
		IMVMStream::Name = "GreenScreen";
		IMVMStream::toggle = T6SDK::Dvars::RegisterBool("mvm_streams_greenScreen", false, "Green screen stream.");
	}
};