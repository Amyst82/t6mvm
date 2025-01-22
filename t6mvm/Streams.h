#pragma once
#define _CRT_SECURE_NO_WARNINGS 
#define WIN32_LEAN_AND_MEAN
#include <d3d11.h>

#include "IMVMStream.h"
#include "GreenScreen.h"
#include "NoPlayers.h"
#include "NoGun.h"
#include "Depth.h"
#include "FxOnly.h"
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
	inline static Stream_Depth Depth;
	inline static Stream_FxOnly FxOnly;

#pragma endregion
#pragma region DX11 defs
	typedef long(__stdcall* present)(IDXGISwapChain*, UINT, UINT);
	present p_present;
	present p_present_target;
	
	typedef long(__stdcall* resizeBuffers)(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
	resizeBuffers p_ResizeBuffers;
	resizeBuffers p_ResizeBuffers_target;

	typedef long(__stdcall* resizeTarget)(IDXGISwapChain*, const DXGI_MODE_DESC*);
	resizeTarget p_ResizeTarget;
	resizeTarget p_ResizeTarget_target;

	HWND window = NULL;
	ID3D11Device* p_device = NULL;
	ID3D11DeviceContext* p_context = NULL;
	ID3D11RenderTargetView* mainRenderTargetView = NULL;
	DXGI_SWAP_CHAIN_DESC sd;


	ID3D11Texture2D* texture_to_save = NULL;

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
			p_ResizeBuffers_target = (resizeBuffers)p_vtable[13];
			p_ResizeTarget_target = (resizeTarget)p_vtable[14];
			return true;
		}
		return false;
	}
	int r_frameCount = 0;
	long HRES;
	static long __stdcall detour_present(IDXGISwapChain* p_swap_chain, UINT sync_interval, UINT flags)
	{
		static bool init = false;
		if (!init)
		{
			if (SUCCEEDED(p_swap_chain->GetDevice(__uuidof(ID3D11Device), (void**)&p_device)))
			{
				p_device->GetImmediateContext(&p_context);
				p_swap_chain->GetDesc(&sd);
				init = true;
			}
			else
				HRES = p_present(p_swap_chain, sync_interval, flags);
		}
		p_context->OMSetRenderTargets(1, &mainRenderTargetView, NULL);

		if (ScreenshotRequested)
		{
			
			REFGUID GUID_ContainerFormatJpeg{ 0x19e4a5aa, 0x5662, 0x4fc5, 0xa0, 0xc0, 0x17, 0x58, 0x2, 0x8e, 0x10, 0x57 };
			

			ID3D11Texture2D* backBuffer;
			HRESULT hr = p_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&backBuffer));

			if (texture_to_save == NULL)
			{
				D3D11_TEXTURE2D_DESC td;
				backBuffer->GetDesc(&td);
				p_device->CreateTexture2D(&td, NULL, &texture_to_save);
			}

			p_context->CopyResource(texture_to_save, backBuffer);

			DirectX::SaveWICTextureToFile(p_context, texture_to_save, GUID_ContainerFormatJpeg, std::wstring(filename.begin(), filename.end()).c_str(), NULL, NULL, true);
			ScreenshotRequested = false;
			backBuffer->Release();
			texture_to_save->Release();
			texture_to_save = NULL;
		}

		if(!IsAnyOtherStream)
		{
			HRES = p_present(p_swap_chain, sync_interval, flags);
			//T6SDK::ConsoleLog::LogSameLineFormatted("Frame %i", r_frameCount++);
		}
		else
			//T6SDK::ConsoleLog::LogSameLineFormatted("Frozen HRES; Frame %i", r_frameCount++);
		return HRES;
	}
	static long __stdcall detour_ResizeBuffers(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
	{
		T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTGREEN, "Resizing buffers to %ix%i", Width, Height);
		IsAnyOtherStream = false;
		return p_ResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
	}

	static long __stdcall detour_ResizeTarget(IDXGISwapChain* pSwapChain, const DXGI_MODE_DESC* pNewTargetParameters)
	{
		IsAnyOtherStream = false;
		T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTGREEN, "Resizing target to %ix%i", pNewTargetParameters->Width, pNewTargetParameters->Height);
		return p_ResizeTarget(pSwapChain, pNewTargetParameters);
	}

	bool initDX11()
	{
		if (!get_present_pointer())
		{
			T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTRED, "Failed to get present pointer");
			return false;
		}

		MH_STATUS status = MH_Initialize();
		if (status != MH_OK)
		{
			T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTRED, "Failed to initialize MinHook");
			return false;
		}
		
		if (MH_CreateHook(reinterpret_cast<void**>(p_present_target), &detour_present, reinterpret_cast<void**>(&p_present)) != MH_OK) 
		{
			T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTRED, "Failed to hook present");
			return false;
		}
		
		if (MH_EnableHook(p_present_target) != MH_OK) 
		{
			T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTRED, "Failed to enable present");
			return false;
		}

		if (MH_CreateHook(reinterpret_cast<void**>(p_ResizeBuffers_target), &detour_ResizeBuffers, reinterpret_cast<void**>(&p_ResizeBuffers)) != MH_OK) 
		{
			T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTRED, "Failed to hook ResizeBuffers");
			return false;
		}

		if (MH_EnableHook(p_ResizeBuffers_target) != MH_OK) 
		{
			T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTRED, "Failed to enable ResizeBuffers");
			return false;
		}

		if (MH_CreateHook(reinterpret_cast<void**>(p_ResizeTarget_target), &detour_ResizeTarget, reinterpret_cast<void**>(&p_ResizeTarget)) != MH_OK) 
		{
			T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTRED, "Failed to hook ResizeTarget");
			return false;
		}

		if (MH_EnableHook(p_ResizeTarget_target) != MH_OK) 
		{
			T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTRED, "Failed to enable ResizeTarget");
			return false;
		}

		return true;
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
	int StreamsEnabled = 0;
	void Update()
	{
		if(!IsStreamsEnabled)
			return;
		IsAnyOtherStream = false;
		T6SDK::InternalFunctions::SCR_UpdateScreen(2);
		StreamsEnabled = 0;
		if(T6SDK::Theater::IsInTheater)
		{
			for (auto& stream : Streams)
			{
				if (stream->toggle->current.enabled)
				{
					StreamsEnabled++;
					IsAnyOtherStream = true; //NOFLASH
					stream->Enable();
					CaptureScreenshot(stream->Name);
					stream->Disable();
				}
			}
		}
		if(StreamsEnabled == 0)
			CaptureScreenshot("Default");
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

		//Depth
		Depth.Init();
		Streams.push_back(&Depth);
		T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTGREEN, "Depth stream registered!");

		//FxOnly
		FxOnly.Init();
		Streams.push_back(&FxOnly);
		T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTGREEN, "FxOnly stream registered!");



		T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTGREEN, "Total streams registered: %i", Streams.size());
		
		T6SDK::ConsoleLog::Log("Initializing DX11...");
		if(initDX11() == false)
		{
			T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTRED,"Failed to initialize DX11!");
			return;
		}
		T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTGREEN,"DX11 initialized.");
	} 

	inline static void StartStreams() 
	{
		CustomDvars::dvar_greenScreen->current.enabled = false;
		for (auto& stream : Streams)
		{
			stream->Disable();
		}
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