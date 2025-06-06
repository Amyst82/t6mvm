#pragma once
#include "IMVMStream.h"
class Stream_Default : public IMVMStream
{
public:
	void Enable()
	{
		StreamsCommon::SetPreviousDof();
	}
	void Disable()
	{
		StreamsCommon::SetPreviousDof();
	}
	void Init()
	{
		IMVMStream::Name = "Default";
		IMVMStream::toggle = T6SDK::Dvars::RegisterBool("mvm_streams_default", false, "Default stream with no changes.");
		IMVMStream::toggle->current.enabled = true;
	}
};