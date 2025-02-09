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

	//C1 FA 05 8B EA C1 ED 1F 03 EA B8 1F 85 EB 51 F7 AF 8C 80 04 00 C1 FA 05 8B C2 C1 E8 1F 03 C2 3B C5
	inline static T6SDK::MemoryPatch FakeTickDetour1(T6SDK::CrossVersion::CrossValue<DWORD>(
		T6SDK::Addresses::t6mpv43 + 0x528F33, T6SDK::Addresses::t6mp + 0x5290E3,
		T6SDK::Addresses::t6zmv41 + 0x525FA3, T6SDK::Addresses::t6zm + 0x5261D3).GetValue(), { 0xA4 });

	inline static T6SDK::MemoryPatch FakeTickDetour2(T6SDK::CrossVersion::CrossValue<DWORD>(
		T6SDK::Addresses::t6mpv43 + 0x528F67, T6SDK::Addresses::t6mp + 0x529117,
		T6SDK::Addresses::t6zmv41 + 0x525FD7, T6SDK::Addresses::t6zm + 0x526207).GetValue(), { 0xA4 });

	inline static T6SDK::MemoryPatch InitialTickStopPatch(T6SDK::CrossVersion::CrossValue<DWORD>(
		T6SDK::Addresses::t6mpv43 + 0x16C2D1, T6SDK::Addresses::t6mp + 0x14F521,
		T6SDK::Addresses::t6zmv41 + 0x1614B1, T6SDK::Addresses::t6zm + 0x170801).GetValue(), { 0xB8 });

	inline static void InitAddresses()
	{

	}
}