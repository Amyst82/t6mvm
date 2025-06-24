#pragma once
#include "IMVMStream.h"
class Stream_FxOnly : public IMVMStream
{
private:
	BYTE prevBaseTechType = 0x04;
	BYTE prevEmissiveTechType = 0x03;
	int prevModelLimit = 1024;
public:
	void Enable()
	{
		prevBaseTechType = T6SDK::Addresses::GfxDrawMethod.Value().baseTechType;
		prevEmissiveTechType = T6SDK::Addresses::GfxDrawMethod.Value().emissiveTechType;
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
		T6SDK::Addresses::GfxDrawMethod.Value().baseTechType = prevBaseTechType;
		T6SDK::Addresses::GfxDrawMethod.Value().emissiveTechType = prevEmissiveTechType;
		T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_modelLimit, prevModelLimit);
		StreamsCommon::SetPreviousDof();
		StreamsCommon::SetPreviousSsao();
		Enabled = false;
	}
	void Init()
	{
		IMVMStream::Name = "FxOnly";
		IMVMStream::toggle = T6SDK::Dvars::RegisterBool("mvm_streams_fxOnly", false, "FX only stream.");
		IMVMStream::IgnoreReshade = T6SDK::Dvars::RegisterBool("mvm_streams_fxOnly_ignoreReshade", false, "Should FxOnly pass ignore reshade.");
		IMVMStream::preview = T6SDK::Dvars::RegisterBool("mvm_streams_fxOnly_preview", false, "Toggle preview for FxOnly stream.");
	}
};