#pragma once
#include "IMVMStream.h"
class Stream_GreenSky : public IMVMStream
{
private:
	bool prevFlareEffects = true;
	bool prevSunFlares = true;
public:
	void Enable()
	{
		T6SDK::Addresses::Patches::RemoveSkyBoxPatch.Patch();
		prevFlareEffects = T6SDK::Dvars::GetBool(*T6SDK::Dvars::DvarList::flareDisableEffects);
		prevSunFlares = T6SDK::Dvars::GetBool(*T6SDK::Dvars::DvarList::r_superFlareDraw);
		//T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_clearColor, 0x0000FF00);
		//T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_clearColor2, 0x0000FF00);

		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_bloomTweaks, true);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::flareDisableEffects, true);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_superFlareDraw, false);

		float clrR = max(0.0f, min(1.0f, CustomDvars::dvar_greenScreenColor->current.vector[0]));
		(*T6SDK::Dvars::DvarList::r_clearColor)->current.color[0] = static_cast<unsigned char>(clrR * 255.0f + 0.5f);
		(*T6SDK::Dvars::DvarList::r_clearColor2)->current.color[0] = static_cast<unsigned char>(clrR * 255.0f + 0.5f);

		float clrG = max(0.0f, min(1.0f, CustomDvars::dvar_greenScreenColor->current.vector[1]));
		(*T6SDK::Dvars::DvarList::r_clearColor)->current.color[1] = static_cast<unsigned char>(clrG * 255.0f + 0.5f);
		(*T6SDK::Dvars::DvarList::r_clearColor2)->current.color[1] = static_cast<unsigned char>(clrG * 255.0f + 0.5f);

		float clrB = max(0.0f, min(1.0f, CustomDvars::dvar_greenScreenColor->current.vector[2]));
		(*T6SDK::Dvars::DvarList::r_clearColor)->current.color[2] = static_cast<unsigned char>(clrB * 255.0f + 0.5f);
		(*T6SDK::Dvars::DvarList::r_clearColor2)->current.color[2] = static_cast<unsigned char>(clrB * 255.0f + 0.5f);

		Enabled = true;
	}
	void Disable()
	{
		T6SDK::Addresses::Patches::RemoveSkyBoxPatch.UnPatch();
		T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_clearColor,  0x00000000);
		T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_clearColor2, 0x00000000);
		StreamsCommon::SetPreviousDof();
		StreamsCommon::SetPreviousSsao();
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::flareDisableEffects, prevFlareEffects);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_superFlareDraw, prevSunFlares);
		Enabled = false;
	}
	void Init()
	{
		IMVMStream::Name = "GreenSky";
		IMVMStream::toggle = T6SDK::Dvars::RegisterBool("mvm_streams_greenSky", false, "Green sky pass.");
		IMVMStream::IgnoreReshade = T6SDK::Dvars::RegisterBool("mvm_streams_greenSky_ignoreReshade", false, "Should green sky pass ignore reshade.");
		IMVMStream::preview = T6SDK::Dvars::RegisterBool("mvm_streams_greenSky_preview", false, "Toggle preview for green sky pass.");
	}
};