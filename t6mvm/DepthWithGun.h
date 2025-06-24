#pragma once
#include "IMVMStream.h"
class Stream_DepthWithGun : public IMVMStream
{
private:

	bool prevDrawGun = true;
	bool prevDraw2D = true;
	BYTE prevBaseTechType = 0x04;
	BYTE prevEmissiveTechType = 0x03;
public:
	void Enable()
	{
		prevBaseTechType = T6SDK::Addresses::GfxDrawMethod.Value().baseTechType;
		prevEmissiveTechType = T6SDK::Addresses::GfxDrawMethod.Value().emissiveTechType;
		prevDrawGun = T6SDK::Dvars::GetBool(*T6SDK::Dvars::DvarList::cg_drawGun);
		prevDraw2D = T6SDK::Dvars::GetBool(*T6SDK::Dvars::DvarList::cg_draw2D);
		T6SDK::Addresses::GfxDrawMethod.Value().baseTechType = 0x00;
		T6SDK::Addresses::GfxDrawMethod.Value().emissiveTechType = 0x00;
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_bloomTweaks, true);
		T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_clearColor, 0x00FFFFFF);
		T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_clearColor2, 0x00FFFFFF);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_bias, 0.0f);
		T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_dofHDR, 0);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_dof_tweak, true);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_dof_enable, true);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_farBlur, 1.0f);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_farStart, 0.0f);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_farEnd, 1000.0f);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_nearBlur, 1.0f);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_nearStart, 1.0f);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_nearEnd, 1.0f);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_viewModelStart, 0.0f);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_viewModelEnd, 0.0f);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::cg_drawGun, true);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_ssao, false);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_ssaoScale, 0.0f);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_ssaoRadius, 0.0f);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_ssaoBias, 0.0f);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_ssaoIntensity, 0.0f);
		Enabled = true;
	}
	void Disable()
	{
		T6SDK::Addresses::GfxDrawMethod.Value().baseTechType = prevBaseTechType;
		T6SDK::Addresses::GfxDrawMethod.Value().emissiveTechType = prevEmissiveTechType;
		StreamsCommon::SetPreviousDof();
		StreamsCommon::SetPreviousSsao();
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::cg_drawGun, prevDrawGun);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::cg_draw2D, prevDraw2D);
		Enabled = false;
	}
	void Init()
	{
		IMVMStream::Name = "DepthWithGun";
		IMVMStream::toggle = T6SDK::Dvars::RegisterBool("mvm_streams_depth_withGun", false, "Depth buffer stream with gun.");
		IMVMStream::IgnoreReshade = T6SDK::Dvars::RegisterBool("mvm_streams_depth_withGun_ignoreReshade", false, "Should depth with gun pass ignore reshade.");
		IMVMStream::preview = T6SDK::Dvars::RegisterBool("mvm_streams_depth_withGun_preview", false, "Toggle preview for depth with gun stream.");
	}
};