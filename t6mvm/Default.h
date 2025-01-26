#pragma once
#include "IMVMStream.h"
class Stream_Default : public IMVMStream
{
public:
	void Enable()
	{

	}
	void Disable()
	{

	}
	void Init()
	{
		IMVMStream::Name = "Default";
		IMVMStream::toggle = T6SDK::Dvars::RegisterBool("mvm_streams_default", false, "Default stream with no changes.");
		IMVMStream::toggle->current.enabled = true;
	}
};