#pragma once
#include "IMVMStream.h"
class Stream_Default : public IMVMStream
{
public:
	void Enable()
	{
		StreamsCommon::SetPreviousDof();
		StreamsCommon::SetPreviousSsao();
	}
	void Disable()
	{
		StreamsCommon::SetPreviousDof();
		StreamsCommon::SetPreviousSsao();
	}
	void Init()
	{
		IMVMStream::Name = "Default";
		IMVMStream::toggle = T6SDK::Dvars::RegisterBool("mvm_streams_default", false, "Default stream with no changes.");
		IMVMStream::toggle->current.enabled = true;
		IMVMStream::IgnoreReshade = T6SDK::Dvars::RegisterBool("mvm_streams_default_ignoreReshade", false, "Should default pass ignore reshade.");
		IMVMStream::preview = T6SDK::Dvars::RegisterBool("mvm_streams_default_preview", false, "Toggle preview for default stream.");
	}
};