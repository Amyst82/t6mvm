#pragma once
#include "IMVMStream.h"
class Stream_GreenSky : public IMVMStream
{
public:
	void Enable()
	{
		T6SDK::Addresses::Patches::RemoveSkyBoxPatch.Patch();
		T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_clearColor, 0x0000FF00);
		T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_clearColor2, 0x0000FF00);
	}
	void Disable()
	{
		T6SDK::Addresses::Patches::RemoveSkyBoxPatch.UnPatch();
		T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_clearColor,  0x00000000);
		T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_clearColor2, 0x00000000);
	}
	void Init()
	{
		IMVMStream::Name = "GreenSky";
		IMVMStream::toggle = T6SDK::Dvars::RegisterBool("mvm_streams_greenSky", false, "Green sky pass.");
	}
};