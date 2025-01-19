#pragma once
#include "IMVMStream.h"
class Stream_NoGun : public IMVMStream
{
private:
	bool prevDrawGun = false;
public:
	void Enable() 
	{
		prevDrawGun = T6SDK::Dvars::GetBool(*T6SDK::Dvars::DvarList::cg_drawGun);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::cg_drawGun, false);
		Enabled = true;
	}
	void Disable() 
	{
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::cg_drawGun, prevDrawGun);
		Enabled = false;
	}
	void Init()
	{
		IMVMStream::Name = "NoGun";
		IMVMStream::toggle = T6SDK::Dvars::RegisterBool("mvm_streams_noGun", false, "No gun stream.");
	}
};