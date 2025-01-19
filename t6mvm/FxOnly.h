#pragma once
#include "IMVMStream.h"
class Stream_FxOnly : public IMVMStream
{
private:
	GfxDrawMethod_t prevGfxDrawMethod{};
	bool prevBloomTweaks = false;
	int prevClearColor = 0;
	int prevClearColor2 = 0;
	bool prevDOFTweak = false;
	bool prevDOFEnable = false;
	int prevModelLimit = 0;
public:
	void Enable()
	{
		prevGfxDrawMethod = T6SDK::Addresses::GfxDrawMethod.Value();
		prevBloomTweaks = T6SDK::Dvars::GetBool(*T6SDK::Dvars::DvarList::r_bloomTweaks);
		prevClearColor = T6SDK::Dvars::GetInt(*T6SDK::Dvars::DvarList::r_clearColor);
		prevClearColor2 = T6SDK::Dvars::GetInt(*T6SDK::Dvars::DvarList::r_clearColor2);
		prevDOFTweak = T6SDK::Dvars::GetBool(*T6SDK::Dvars::DvarList::r_dof_tweak);
		prevDOFEnable = T6SDK::Dvars::GetBool(*T6SDK::Dvars::DvarList::r_dof_enable);
		prevModelLimit = T6SDK::Dvars::GetInt(*T6SDK::Dvars::DvarList::r_modelLimit);

		T6SDK::Addresses::GfxDrawMethod.Value().baseTechType = 0x00;
		T6SDK::Addresses::GfxDrawMethod.Value().emissiveTechType = 0x03;
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_bloomTweaks, true);
		T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_clearColor, 0x0000FF00);
		T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_clearColor2, 0x0000FF00);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_dof_tweak, false);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_dof_enable, false);
		T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_modelLimit, 0);
		Enabled = true;
	}
	void Disable()
	{
		T6SDK::Addresses::GfxDrawMethod.Value() = prevGfxDrawMethod;
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_bloomTweaks, prevBloomTweaks);
		T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_clearColor, prevClearColor);
		T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_clearColor2, prevClearColor2);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_dof_tweak, prevDOFTweak);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_dof_enable, prevDOFEnable);
		T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_modelLimit, prevModelLimit);
		Enabled = false;
	}
	void Init()
	{
		IMVMStream::Name = "FxOnly";
		IMVMStream::toggle = T6SDK::Dvars::RegisterBool("mvm_streams_fxOnly", false, "FX only stream.");
	}
};