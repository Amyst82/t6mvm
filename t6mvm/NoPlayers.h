#pragma once
#include "IMVMStream.h"
class Stream_NoPlayers : public IMVMStream
{
public:
	void Enable() 
	{
		Enabled = true;
		StreamsCommon::SetPreviousDof();
		StreamsCommon::SetPreviousSsao();
	}
	void Disable() 
	{
		Enabled = false;
		StreamsCommon::SetPreviousDof();
		StreamsCommon::SetPreviousSsao();
	}
	void Init() 
	{
		IMVMStream::Name = "NoPlayers";
		IMVMStream::toggle = T6SDK::Dvars::RegisterBool("mvm_streams_noPlayers", false, "No players stream.");
		IMVMStream::IgnoreReshade = T6SDK::Dvars::RegisterBool("mvm_streams_noPlayers_ignoreReshade", false, "Should no player pass ignore reshade.");
		IMVMStream::preview = T6SDK::Dvars::RegisterBool("mvm_streams_noPlayers_preview", false, "Should no player pass preview.");
	}
};