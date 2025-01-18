#pragma once
#include <StdInclude.h>
namespace LocalAddresses
{
	//E8 ? ? ? ? 8B 0D ? ? ? ? 83 C4 ? 8B C1
	inline static T6SDK::MemoryHook h_TickIncreasing(T6SDK::CrossVersion::CrossValue<DWORD>(
		T6SDK::Addresses::t6mpv43 + 0x0464CC, T6SDK::Addresses::t6mp + FILLIN,
		T6SDK::Addresses::t6zmv41 + FILLIN, T6SDK::Addresses::t6zm + FILLIN).GetValue(), 5);
	inline static DWORD InternalTickIncreaseFunc = T6SDK::CrossVersion::CrossValue<DWORD>(
		T6SDK::Addresses::t6mpv43 + 0x2C6C34, T6SDK::Addresses::t6mp + FILLIN,
		T6SDK::Addresses::t6zmv41 + FILLIN, T6SDK::Addresses::t6zm + FILLIN).GetValue();
	inline static void InitAddresses()
	{

	}
}