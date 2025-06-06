#pragma once
#include <StdInclude.h>
#include "StreamsCommon.h"
class IMVMStream
{
public:
	virtual void Init() = 0;
	virtual void Enable() = 0;
	virtual void Disable() = 0;
	bool Enabled;
	std::string Dir{};
	const char* Name;
	dvar_s* toggle;
	void* gmav = 0;
};