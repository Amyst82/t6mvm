#pragma once
#include "IMVMStream.h"
class Stream_NoPlayers : public IMVMStream
{
public:
	void Enable() 
	{
		Enabled = true;
	}
	void Disable() 
	{
		Enabled = false;
	}
	void Init() 
	{
		IMVMStream::Name = "NoPlayers";
		IMVMStream::toggle = T6SDK::Dvars::RegisterBool("mvm_streams_noPlayers", false, "No players stream.");
	}
};