#pragma once
#define _CRT_SECURE_NO_WARNINGS 
#define WIN32_LEAN_AND_MEAN
#include <d3d11.h>

#include "IMVMStream.h"
#include "Default.h"
#include "GreenScreen.h"
#include "NoPlayers.h"
#include "NoGun.h"
#include "Depth.h"
#include "FxOnly.h"
#include "DepthWithGun.h"
#include "GreenSky.h"

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
	int frameCount = 0;
	inline static bool IsStreamsStarted = false;
	inline static bool ScreenshotRequested = false;
	inline static bool IsAnyOtherStream = false;
	bool FreeBufferRequested = false;
	string filename = "";
	inline static bool IsStreamsRunning = false;
	const char* newStreamFolder = "";

	inline static vector<IMVMStream*> Streams{};
	inline static Stream_Default Default;
	inline static Stream_GreenScreen GreenScreen;
	inline static Stream_NoPlayers NoPlayers;
	inline static Stream_NoGun NoGun;
	inline static Stream_Depth Depth;
	inline static Stream_DepthWithGun DepthWithGun;
	inline static Stream_FxOnly FxOnly;
	inline static Stream_GreenSky GreenSky;

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

	static void ShowProgressOnScreen()
	{
		if(!T6SDK::Dvars::GetBool(CustomDvars::dvar_streams))
			return;
		if(!IsStreamsRunning)
			return;
		if (T6SDK::Dvars::GetInt(CustomDvars::dvar_streams_tickStart) > -1 && T6SDK::Dvars::GetInt(CustomDvars::dvar_streams_tickEnd) > -1)
		{
			int totalFrames = (T6SDK::Dvars::GetInt(CustomDvars::dvar_streams_tickEnd) - T6SDK::Dvars::GetInt(CustomDvars::dvar_streams_tickStart))/(1000 / T6SDK::Dvars::GetInt(CustomDvars::dvar_streams_fps));
			float percent = (float)frameCount / (float)totalFrames;

			char buffer0[256];
			sprintf(buffer0, "Progress: ^3%i/%i ^7(%.0f%%)", frameCount, totalFrames, percent * 100.0f);
			vec2_t coords = T6SDK::Drawing::GetGridCellCoords(8, 19);
			T6SDK::Drawing::DrawTextAbsolute(buffer0, coords.x, coords.y, 1.5f, T6SDK::Drawing::WHITECOLOR, T6SDK::AnchorPoint::Center, 0x00);

			char formatBuffer1[64];
			sprintf(formatBuffer1, "%s", T6SDK::Dvars::GetBool(CustomDvars::dvar_streams_avi) ? "Recording as ^3AVI" : T6SDK::Dvars::GetBool(CustomDvars::dvar_streams_JPG) ? "Recording as ^3JPG ^7sequence" : "Recording as ^3PNG ^7sequence");
			
			vec2_t coords2 = T6SDK::Drawing::GetGridCellCoords(8, 21);
			T6SDK::Drawing::DrawTextAbsolute(formatBuffer1, coords2.x, coords2.y, 1.0f, T6SDK::Drawing::WHITECOLOR, T6SDK::AnchorPoint::Center, 0x00);

			vec2_t coords3 = T6SDK::Drawing::GetGridCellCoords(8, 22);
			T6SDK::Drawing::DrawTextAbsolute("Press ^3F5 ^7to stop. Press ^1ESC ^7to abort recording and remove recorded files.", coords3.x, coords3.y, 1.0f, T6SDK::Drawing::WHITECOLOR, T6SDK::AnchorPoint::Center, 0x00);
		}
		else
		{
			char buffer1[256];
			sprintf(buffer1, "Captured ^3%i ^7frames", frameCount);
			vec2_t coords = T6SDK::Drawing::GetGridCellCoords(8, 19);
			T6SDK::Drawing::DrawTextAbsolute(buffer1, coords.x, coords.y, 1.5f, T6SDK::Drawing::WHITECOLOR, T6SDK::AnchorPoint::Center, 0x00);

			vec2_t coords2 = T6SDK::Drawing::GetGridCellCoords(8, 21);
			T6SDK::Drawing::DrawTextAbsolute("Press ^3F5 ^7to stop. Press ^1ESC ^7to abort recording and remove recorded files.", coords2.x, coords2.y, 1.0f, T6SDK::Drawing::WHITECOLOR, T6SDK::AnchorPoint::Center, 0x00);
		}
	}

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
			//T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTGREEN, "Screenshot requested!");
			REFGUID GUID_ContainerFormatJpeg{ 0x19e4a5aa, 0x5662, 0x4fc5, 0xa0, 0xc0, 0x17, 0x58, 0x2, 0x8e, 0x10, 0x57 };
			REFGUID GUID_ContainerFormatPng{ 0x1b7cfaf4, 0x713f, 0x473c, 0xbb, 0xcd, 0x61, 0x37, 0x42, 0x5f, 0xae, 0xaf };
			

			ID3D11Texture2D* backBuffer;
			HRESULT hr = p_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&backBuffer));

			if (texture_to_save == NULL)
			{
				D3D11_TEXTURE2D_DESC td;
				backBuffer->GetDesc(&td);
				p_device->CreateTexture2D(&td, NULL, &texture_to_save);
			}

			p_context->CopyResource(texture_to_save, backBuffer);

			DirectX::SaveWICTextureToFile(p_context, texture_to_save, T6SDK::Dvars::GetBool(CustomDvars::dvar_streams_JPG) ? GUID_ContainerFormatJpeg : GUID_ContainerFormatPng, std::wstring(filename.begin(), filename.end()).c_str(), NULL, NULL, true);
			ScreenshotRequested = false;
			backBuffer->Release();
			texture_to_save->Release();
			texture_to_save = NULL;
		}

		if (!IsAnyOtherStream)
			HRES = p_present(p_swap_chain, sync_interval, flags);

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
	void CaptureScreenshot(IMVMStream* currentStream)
	{
		char formatBuffer[32];
		sprintf(formatBuffer, "%s",T6SDK::Dvars::GetBool(CustomDvars::dvar_streams_JPG) ? "JPG" : "PNG");
		char buffer[512];
		sprintf(buffer, "%s\\%s_%i.%s", currentStream->Dir.c_str(), currentStream->Name, frameCount, formatBuffer);
		filename = currentStream->Dir+ "\\" +currentStream->Name+"_"+std::to_string(frameCount)+"."+formatBuffer;
		//T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTYELLOW, "Capturing screenshot to: %s", filename);
		T6SDK::InternalFunctions::SCR_UpdateScreen(2);
		ScreenshotRequested = true;
		T6SDK::InternalFunctions::SCR_UpdateScreen(2);
	}

	inline static void StopStreams()
	{
		if (!T6SDK::Dvars::GetBool(CustomDvars::dvar_streams))
			return;
		T6SDK::Addresses::IsDemoPaused.Value() = true;
		ScreenshotRequested = false;
		IsAnyOtherStream = false;
		LocalAddresses::h_TickIncreasing.UnHook();
		IsStreamsRunning = false;
		frameCount = 0;
		T6SDK::Addresses::DemoPlayback.Value()->DemoHudHidden = false;
		IsStreamsStarted = false;
	}

	int StreamsPassesEnabled = 0;
	inline static void Update()
	{
		if (!T6SDK::Dvars::GetBool(CustomDvars::dvar_streams))
			return;
		if (!IsStreamsStarted)
			return;
		if (T6SDK::Dvars::GetInt(CustomDvars::dvar_streams_tickStart) > -1 && T6SDK::Dvars::GetInt(CustomDvars::dvar_streams_tickEnd) > -1)
		{
			if (T6SDK::Addresses::Tick.Value() >= T6SDK::Dvars::GetInt(CustomDvars::dvar_streams_tickEnd))
			{
				StopStreams();
				return;
			}
		}
		IsStreamsRunning = true;
		IsAnyOtherStream = false;
		T6SDK::InternalFunctions::SCR_UpdateScreen(2);
		StreamsPassesEnabled = 0;
		if(T6SDK::Theater::IsInTheater())
		{
			for (auto& stream : Streams)
			{
				if (stream->toggle->current.enabled)
				{
					StreamsPassesEnabled++;
					if(T6SDK::Dvars::GetBool(CustomDvars::dvar_streams_noFlash))
						IsAnyOtherStream = true; //NOFLASH
					stream->Enable();
					CaptureScreenshot(stream);
					stream->Disable();
				}
			}
		}
		if(StreamsPassesEnabled == 0)
			CaptureScreenshot(&Default);

		frameCount++;
		//T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTGREEN, "Successfully captured %i", frameCount);
	}

	uintptr_t eaxTMP, ecxTMP, edxTMP, esiTMP, ediTMP, espTMP, ebpTMP;
	int tickStep = 0;
	__declspec(naked) void OnTickIncreasing()
	{
		tickStep = 1000 / T6SDK::Dvars::GetInt(CustomDvars::dvar_streams_fps);
		if(T6SDK::Dvars::GetBool(CustomDvars::dvar_frozenCam))
			tickStep = 0;
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

			mov eax, tickStep
			mov edx, [edxTMP]
			mov ecx, [ecxTMP]
			mov esi, [esiTMP]
			mov edi, [ediTMP]
			mov esp, [espTMP]
			mov ebp, [ebpTMP]
			call[LocalAddresses::InternalTickIncreaseFunc]
			jmp[LocalAddresses::h_TickIncreasing.JumpBackAddress]
		}
	}

	inline static void InitStreams()
	{
		//Default
		Default.Init();
		Streams.push_back(&Default);
		T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTGREEN, "Default stream registered!");

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

		//DepthWithGun
		DepthWithGun.Init();
		Streams.push_back(&DepthWithGun);
		T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTGREEN, "Depth with gun stream registered!");

		//Green sky
		GreenSky.Init();
		Streams.push_back(&GreenSky);
		T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTGREEN, "Green sky stream registered!");


		T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTGREEN, "Total streams registered: %i", Streams.size());
		
		T6SDK::ConsoleLog::Log("Initializing DX11...");
		if(initDX11() == false)
		{
			T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTRED,"Failed to initialize DX11!");
			return;
		}
		T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTGREEN,"DX11 initialized.");

		//Init UI for passes
		T6SDK::ConsoleLog::Log("Initializing streams passes UI...");
		UIControls::UI_StreamsPass1CheckBox = T6SDK::Drawing::UI_CheckBoxButton("DEFAULT", "DEFAULT", 12, 6, T6SDK::AnchorPoint::TopLeft, &Streams::Default.toggle->current.enabled, 0x00);
		UIControls::UI_StreamsPass2CheckBox = T6SDK::Drawing::UI_CheckBoxButton("NO GUN", "NO GUN", 12, 8, T6SDK::AnchorPoint::TopLeft, &Streams::NoGun.toggle->current.enabled, 0x00);
		UIControls::UI_StreamsPass3CheckBox = T6SDK::Drawing::UI_CheckBoxButton("GREEN SCREEN", "GREEN SCREEN", 12, 10, T6SDK::AnchorPoint::TopLeft, &Streams::GreenScreen.toggle->current.enabled, 0x00);
		UIControls::UI_StreamsPass4CheckBox = T6SDK::Drawing::UI_CheckBoxButton("NO PLAYERS", "NO PLAYERS", 12, 12, T6SDK::AnchorPoint::TopLeft, &Streams::NoPlayers.toggle->current.enabled, 0x00);
		UIControls::UI_StreamsPass5CheckBox = T6SDK::Drawing::UI_CheckBoxButton("DEPTH", "DEPTH", 12, 14, T6SDK::AnchorPoint::TopLeft, &Streams::Depth.toggle->current.enabled, 0x00);
		UIControls::UI_StreamsPass6CheckBox = T6SDK::Drawing::UI_CheckBoxButton("DEPTH WITH GUN", "DEPTH WITH GUN", 12, 16, T6SDK::AnchorPoint::TopLeft, &Streams::DepthWithGun.toggle->current.enabled, 0x00);
		UIControls::UI_StreamsPass7CheckBox = T6SDK::Drawing::UI_CheckBoxButton("FX ONLY", "FX ONLY", 12, 18, T6SDK::AnchorPoint::TopLeft, &Streams::FxOnly.toggle->current.enabled, 0x00);
		UIControls::UI_StreamsPass8CheckBox = T6SDK::Drawing::UI_CheckBoxButton("GREEN SKY", "GREEN SKY", 12, 20, T6SDK::AnchorPoint::TopLeft, &Streams::GreenSky.toggle->current.enabled, 0x00);
		T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTGREEN,"Streams passes UI initialized.");

	} 

	inline static void StartStreams()
	{
		if (!T6SDK::Dvars::GetBool(CustomDvars::dvar_streams))
			return;
		//Creating a new directory for streams
		if (std::string(T6SDK::Dvars::GetString(CustomDvars::dvar_streams_directory)).empty())
		{
			T6SDK::Theater::Demo_Error("Streams directory is not set!", "Use ^5mvm_streams_directory ^7dvar or use ^5TAB ^7menu to set a directory for streams.");
			return;
		}
		char streamsDirectory[256];
		sprintf(streamsDirectory, "%s\\Streams_%s", T6SDK::Dvars::GetString(CustomDvars::dvar_streams_directory), T6SDK::InternalFunctions::getCurrentDateTimeString().c_str());
		T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTGREEN, "Creating directory for streams at: %s", streamsDirectory);
		if (!T6SDK::InternalFunctions::CreateNewDirectory(streamsDirectory))
		{
			T6SDK::Theater::Demo_Error("Error occured!", "Failed to create directory for streams!");
			T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTRED, "Failed to create directory for streams!");
			StopStreams();
			return;
		}
		newStreamFolder = streamsDirectory;
		T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTGREEN, "newStreamFolder: %s", newStreamFolder);
		if(T6SDK::Dvars::GetInt(CustomDvars::dvar_streams_tickStart) > -1)
			T6SDK::Theater::Demo_JumpToTick(T6SDK::Dvars::GetInt(CustomDvars::dvar_streams_tickStart));
		T6SDK::Addresses::DemoPlayback.Value()->DemoHudHidden = true;
		CustomDvars::dvar_greenScreen->current.enabled = false;
		for (auto& stream : Streams)
		{
			stream->Disable();
			if (stream->toggle->current.enabled)
			{
				char buffer[256];
				sprintf(buffer, "%s\\%s", newStreamFolder, stream->Name);
				T6SDK::InternalFunctions::CreateNewDirectory(buffer);
				//strcpy(stream->Dir, buffer);
				stream->Dir = buffer;
				T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTGREEN, "New stream directory for %s", stream->Dir.c_str());
			}
		}
		LocalAddresses::h_TickIncreasing.Hook(OnTickIncreasing);
		T6SDK::Addresses::IsDemoPaused.Value() = false;
		IsStreamsStarted = true;
	}
	inline static void StreamsSwitchState() 
	{
		if (!T6SDK::Dvars::GetBool(CustomDvars::dvar_streams))
			return;
		if (IsStreamsRunning)
			StopStreams();
		else
			StartStreams();
	}
#pragma endregion
}