#pragma once
#include "IMVMStream.h"
class Stream_NoGun : public IMVMStream
{
private:
	bool prevDrawGun = true;
public:
	void Enable() 
	{
		prevDrawGun = T6SDK::Dvars::GetBool(*T6SDK::Dvars::DvarList::cg_drawGun);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::cg_drawGun, false);
		StreamsCommon::SetPreviousDof();
		StreamsCommon::SetPreviousSsao();
		Enabled = true;
	}
	void Disable() 
	{
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::cg_drawGun, prevDrawGun);
		StreamsCommon::SetPreviousDof();
		StreamsCommon::SetPreviousSsao();
		Enabled = false;
	}
	void Init()
	{
		IMVMStream::Name = "NoGun";
		IMVMStream::toggle = T6SDK::Dvars::RegisterBool("mvm_streams_noGun", false, "No gun stream.");
		IMVMStream::IgnoreReshade = T6SDK::Dvars::RegisterBool("mvm_streams_noGun_ignoreReshade", false, "Should no gun pass ignore reshade.");
		IMVMStream::preview = T6SDK::Dvars::RegisterBool("mvm_streams_noGun_preview", false, "Toggle preview for no gun stream.");

	}
};