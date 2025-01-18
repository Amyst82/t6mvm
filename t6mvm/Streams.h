#pragma once
#define _CRT_SECURE_NO_WARNINGS 
#define WIN32_LEAN_AND_MEAN
#include <d3d11.h>

#include "IMVMStream.h"
#include "GreenScreen.h"
#include "NoPlayers.h"
#include "NoGun.h"
#include "LocalAddresses.h"
#include "ScreenGrab.h"
#include <cassert>

#include "..\MinHook\MinHook.h"
#pragma comment(lib, "libMinHook.x86.lib")
#include <sstream>
#pragma comment(lib, "d3d11.lib")
namespace Streams
{
#pragma region Streams defs
	bool IsStreamsEnabled = false;
	int frameCount = 0;
	bool ScreenshotRequested = false;
	bool IsAnyOtherStream = false;
	bool FreeBufferRequested = false;
	string filename = "";
	const char* Dir = "D:\\Games\\Redacted\\Test";

	inline static vector<IMVMStream*> Streams{};
	inline static Stream_GreenScreen GreenScreen;
	inline static Stream_NoPlayers NoPlayers;
	inline static Stream_NoGun NoGun;

#pragma endregion
#pragma region DX11 defs
	typedef long(__stdcall* present)(IDXGISwapChain*, UINT, UINT);
	present p_present;
	present p_present_target;
	
	bool init = false;
	HWND window = NULL;
	ID3D11Device* p_device = NULL;
	ID3D11DeviceContext* p_context = NULL;
	ID3D11RenderTargetView* mainRenderTargetView = NULL;
	DXGI_SWAP_CHAIN_DESC sd;
#pragma endregion
#pragma region DX11 methods
	bool get_present_pointer()
	{
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 2;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = GetForegroundWindow();
		sd.SampleDesc.Count = 1;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	
		IDXGISwapChain* swap_chain;
		ID3D11Device* device;
	
		const D3D_FEATURE_LEVEL feature_levels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
		if (D3D11CreateDeviceAndSwapChain(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			0,
			feature_levels,
			2,
			D3D11_SDK_VERSION,
			&sd,
			&swap_chain,
			&device,
			nullptr,
			nullptr) == S_OK)
		{
			void** p_vtable = *reinterpret_cast<void***>(swap_chain);
			swap_chain->Release();
			device->Release();
			//context->Release();
			p_present_target = (present)p_vtable[8];
			return true;
		}
		return false;
	}
	
	long HRES;
	static long __stdcall detour_present(IDXGISwapChain* p_swap_chain, UINT sync_interval, UINT flags)
	{
		if (!init)
		{
			if (SUCCEEDED(p_swap_chain->GetDevice(__uuidof(ID3D11Device), (void**)&p_device)))
			{
				p_device->GetImmediateContext(&p_context);
				p_swap_chain->GetDesc(&sd);
				p_swap_chain->ResizeBuffers(sd.BufferCount, T6SDK::Addresses::ScreenWidth.Value(), T6SDK::Addresses::ScreenHeight.Value(), sd.BufferDesc.Format, flags);
				init = true;
			}
			else
				HRES = p_present(p_swap_chain, sync_interval, flags);
		}
		//p_context->OMSetRenderTargets(1, &mainRenderTargetView, NULL);

		if (ScreenshotRequested)
		{
			ID3D11Device* device;
			HRESULT gd = p_swap_chain->GetDevice(__uuidof(ID3D11Device), (void**)&device);
			assert(gd == S_OK);

			//Get context
			ID3D11DeviceContext* context;
			device->GetImmediateContext(&context);

			//get back buffer
			ID3D11Texture2D* backbufferTex;
			HRESULT gb = p_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backbufferTex);
			assert(gb == S_OK);

			//Capture Frame
			REFGUID GUID_ContainerFormatJpeg{ 0x19e4a5aa, 0x5662, 0x4fc5, 0xa0, 0xc0, 0x17, 0x58, 0x2, 0x8e, 0x10, 0x57 };

			HRESULT hr = DirectX::SaveWICTextureToFile(context, backbufferTex, GUID_ContainerFormatJpeg, std::wstring(filename.begin(), filename.end()).c_str());
			assert(hr == S_OK);

			ScreenshotRequested = false;
		}

		//if(!IsAnyOtherStream)
		HRES = p_present(p_swap_chain, sync_interval, flags);
		return HRES;
	}
	
	bool initDX11()
	{
		if (!get_present_pointer())
		{
			return 1;
		}

		MH_STATUS status = MH_Initialize();
		if (status != MH_OK)
		{
			return 1;
		}
		
		if (MH_CreateHook(reinterpret_cast<void**>(p_present_target), &detour_present, reinterpret_cast<void**>(&p_present)) != MH_OK) {
			return 1;
		}
		
		if (MH_EnableHook(p_present_target) != MH_OK) {
			return 1;
		}
	}
#pragma endregion

#pragma region Streams methods
	void CaptureScreenshot(const char* streamName)
	{
		char buffer[256];
		sprintf(buffer, "%s\\%s_%i.JPG", Dir, streamName, frameCount);
		filename = buffer;
		T6SDK::InternalFunctions::SCR_UpdateScreen(2);
		ScreenshotRequested = true;
		T6SDK::InternalFunctions::SCR_UpdateScreen(2);
	}

	void Update()
	{
		if(!IsStreamsEnabled)
			return;
		IsAnyOtherStream = false;
		T6SDK::InternalFunctions::SCR_UpdateScreen(2);
		for (auto& stream : Streams)
		{
			if (stream->toggle->current.enabled)
			{
				IsAnyOtherStream = true; //NOFLASH
				stream->Enable();
				CaptureScreenshot(stream->Name);
				stream->Disable();
			}
		}
		frameCount++;
	}
	uintptr_t eaxTMP, ecxTMP, edxTMP, esiTMP, ediTMP, espTMP, ebpTMP;
	__declspec(naked) void OnTickIncreasing()
	{
		__asm
		{
			mov[eaxTMP], eax
			mov[edxTMP], edx
			mov[ecxTMP], ecx
			mov[esiTMP], esi
			mov[ediTMP], edi
			mov[espTMP], esp
			mov[ebpTMP], ebp
			
			call Update
			
			mov eax, 0x02
			mov edx, [edxTMP]
			mov ecx, [ecxTMP]
			mov esi, [esiTMP]
			mov edi, [ediTMP]
			mov esp, [espTMP]
			mov ebp, [ebpTMP]
			call [LocalAddresses::InternalTickIncreaseFunc]
			jmp[LocalAddresses::h_TickIncreasing.JumpBackAddress]
		}
	}

	inline static void InitStreams()
	{
		//GreenScreen
		GreenScreen.Init();
		Streams.push_back(&GreenScreen);
		T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTGREEN, "Green screen stream registered!");

		//NoPlayers
		NoPlayers.Init();
		Streams.push_back(&NoPlayers);
		T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTGREEN, "No players stream registered!");

		//NoGun
		NoGun.Init();
		Streams.push_back(&NoGun);
		T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTGREEN, "No gun stream registered!");

		T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTGREEN, "Total streams registered: %i", Streams.size());
		
		T6SDK::ConsoleLog::Log("Initializing DX11...");
		if(!initDX11())
		{
			T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTRED,"Failed to initialize DX11!");
			return;
		}
		T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTGREEN,"DX11 initialized.");
	} 

	inline static void StartStreams() 
	{
		LocalAddresses::h_TickIncreasing.Hook(OnTickIncreasing);
		IsStreamsEnabled = true;
		T6SDK::Addresses::IsDemoPaused.Value() = false;
	}
	inline static void StopStreams() 
	{
		T6SDK::Addresses::IsDemoPaused.Value() = true;
		ScreenshotRequested = false;
		IsStreamsEnabled = false;
		IsAnyOtherStream = false;
		LocalAddresses::h_TickIncreasing.UnHook();
		frameCount = 0;
	}
	inline static void StreamsSwitchState() 
	{
		if (IsStreamsEnabled)
			StopStreams();
		else
			StartStreams();
	}
#pragma endregion
}