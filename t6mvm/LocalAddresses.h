#pragma once
#include <StdInclude.h>
namespace LocalAddresses
{
	//E8 ? ? ? ? 8B 0D ? ? ? ? 83 C4 ? 8B C1
	inline static T6SDK::MemoryHook h_TickIncreasing(T6SDK::CrossVersion::CrossValue<DWORD>(
		T6SDK::Addresses::t6mpv43 + 0x0464CC, T6SDK::Addresses::t6mp + 0x7D24C,
		T6SDK::Addresses::t6zmv41 + 0xAf86C, T6SDK::Addresses::t6zm + 0x2958BC).GetValue(), 5);
	inline static DWORD InternalTickIncreaseFunc = T6SDK::CrossVersion::CrossValue<DWORD>(
		T6SDK::Addresses::t6mpv43 + 0x2C6C34, T6SDK::Addresses::t6mp + 0x108264,
		T6SDK::Addresses::t6zmv41 + 0x1C0864, T6SDK::Addresses::t6zm + 0x24C2E4).GetValue();

	//C1 FA 05 8B EA C1 ED 1F 03 EA B8 1F 85 EB 51 F7 AF 8C 80 04 00 C1 FA 05 8B C2 C1 E8 1F 03 C2 3B C5
	inline static T6SDK::MemoryPatch FakeTickDetour1(T6SDK::CrossVersion::CrossValue<DWORD>(
		T6SDK::Addresses::t6mpv43 + 0x528F33, T6SDK::Addresses::t6mp + 0x5290E3,
		T6SDK::Addresses::t6zmv41 + 0x525FA3, T6SDK::Addresses::t6zm + 0x5261D3).GetValue(), { 0x9C });

	inline static T6SDK::MemoryPatch FakeTickDetour2(T6SDK::CrossVersion::CrossValue<DWORD>(
		T6SDK::Addresses::t6mpv43 + 0x528F67, T6SDK::Addresses::t6mp + 0x529117,
		T6SDK::Addresses::t6zmv41 + 0x525FD7, T6SDK::Addresses::t6zm + 0x526207).GetValue(), { 0x9C });

	inline static T6SDK::MemoryPatch InitialTickStopPatch(T6SDK::CrossVersion::CrossValue<DWORD>(
		T6SDK::Addresses::t6mpv43 + 0x16C2D1, T6SDK::Addresses::t6mp + 0x14F521,
		T6SDK::Addresses::t6zmv41 + 0x1614B1, T6SDK::Addresses::t6zm + 0x170801).GetValue(), { 0xB8 });

	inline static T6SDK::MemoryHook h_DisplayMarkerInfoHook(T6SDK::CrossVersion::CrossValue<DWORD>(
		T6SDK::Addresses::t6mpv43 + 0x2DFF0E, T6SDK::Addresses::t6mp + 0x17BADE,
		T6SDK::Addresses::t6zmv41 + 0x13509E, T6SDK::Addresses::t6zm + 0x116C6E).GetValue(), 6);

	//39 3D ? ? ? ? 0F 8E ? ? ? ? BE ? ? ? ? 80 7D
	inline static DWORD RemoveTheaterBarrierAddress = T6SDK::CrossVersion::CrossValue<DWORD>(
		T6SDK::Addresses::t6mpv43 + 0x213130C, T6SDK::Addresses::t6mp + 0x213130C,
		T6SDK::Addresses::t6zmv41 + 0x2102E8C, T6SDK::Addresses::t6zm + 0x210720C).GetValue();
	inline static void InitAddresses()
	{

	}
}