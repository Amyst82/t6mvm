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
#include <filesystem>
#include "LocalAddresses.h"
#include "ScreenGrab.h"
#include <cassert>
#include "..\MinHook\MinHook.h"
#pragma comment(lib, "libMinHook.x86.lib")
#include <sstream>
#include <libgmavi.h>
#include "Settings.h"
#include "kiero.h"
#include "WinToastHelper.h"
#include <codecvt>


#pragma comment(lib, "d3d11.lib")

namespace Streams
{
	void* gmav = 0x00;
#pragma region AviFile methods
	// Function to capture the back buffer and write it to an AVI file
	static bool CaptureBackBufferAndWriteToAVI(ID3D11DeviceContext* pDeviceContext, ID3D11Device* pDevice, ID3D11Texture2D* pBackBuffer)
	{
		// Create a staging texture to copy the back buffer
		D3D11_TEXTURE2D_DESC desc;
		pBackBuffer->GetDesc(&desc);
		desc.Usage = D3D11_USAGE_STAGING;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		desc.BindFlags = 0;

		ID3D11Texture2D* pStagingTexture = nullptr;
		HRESULT hr = pDevice->CreateTexture2D(&desc, nullptr, &pStagingTexture);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create staging texture!", "Error", MB_OK);
			return false;
		}

		// Copy the back buffer to the staging texture
		pDeviceContext->CopyResource(pStagingTexture, pBackBuffer);

		// Map the staging texture to access its data
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		hr = pDeviceContext->Map(pStagingTexture, 0, D3D11_MAP_READ, 0, &mappedResource);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to map staging texture!", "Error", MB_OK);
			pStagingTexture->Release();
			return false;
		}

		// Allocate a buffer for the RGB frame
		const UINT width = desc.Width;
		const UINT height = desc.Height;
		const UINT frameSize = width * height * 3; // 3 bytes per pixel (RGB)
		BYTE* pFrameData = new BYTE[frameSize];

		// Convert the back buffer data (BGRA) to RGB (bottom first)
		BYTE* pSource = static_cast<BYTE*>(mappedResource.pData);

		for (int y = 0; y < height; y++)
		{
			int src_y = height - 1 - y;  // Start from the bottom row
			for (int x = 0; x < width; x++)
			{
				int src_idx = (src_y * width + x) * 4;  // RGBA source index
				int dst_idx = (y * width + x) * 3;      // RGB destination index

				pFrameData[dst_idx + 0] = pSource[src_idx + 2];  // R
				pFrameData[dst_idx + 1] = pSource[src_idx + 1];  // G
				pFrameData[dst_idx + 2] = pSource[src_idx + 0];  // B
			}
		}

		// Unmap the staging texture
		pDeviceContext->Unmap(pStagingTexture, 0);
		pStagingTexture->Release();

		if (!gmav_add(gmav, pFrameData))
		{
			if(gmav != NULL)
				gmav_finish(gmav);

		}

		// Clean up
		delete[] pFrameData;
		return true;
	}
#pragma endregion
#pragma region Streams defs
	int frameCount = 0;
	inline static bool IsStreamsStarted = false;
	inline static bool ScreenshotRequested = false;
	inline static bool IsAnyOtherStream = false;
	inline static bool StreamShouldIgnoreReshade = false;
	bool FreeBufferRequested = false;
	string filename = "";
	inline static bool IsStreamsRunning = false;
	static const char* newStreamFolder = "";
	static string folderToDeleteWhenAborted = "";

	inline static vector<IMVMStream*> Streams{};
	inline static Stream_Default Default;
	inline static Stream_GreenScreen GreenScreen;
	inline static Stream_NoPlayers NoPlayers;
	inline static Stream_NoGun NoGun;
	inline static Stream_Depth Depth;
	inline static Stream_DepthWithGun DepthWithGun;
	inline static Stream_FxOnly FxOnly;
	inline static Stream_GreenSky GreenSky;

	cmd_function_s cmd_streams_start_VAR{};
	cmd_function_s cmd_streams_stop_VAR{};
	cmd_function_s cmd_streams_abort_VAR{};

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

			char buffer2[256];
			sprintf(buffer2, "Press ^3%s ^7to stop. Press ^1%s ^7to abort recording and remove recorded files.", T6SDK::Input::GetKeyByCode(Settings::Settings::KeyBinds["StreamsStartStop"])->KeyName.c_str(), T6SDK::Input::GetKeyByCode(Settings::Settings::KeyBinds["StreamsAbort"])->KeyName.c_str());
			T6SDK::Drawing::DrawTextAbsolute(buffer2, coords3.x, coords3.y, 1.0f, T6SDK::Drawing::WHITECOLOR, T6SDK::AnchorPoint::Center, 0x00);
		}
		else
		{
			char buffer1[256];
			sprintf(buffer1, "Captured ^3%i ^7frames", frameCount);
			vec2_t coords = T6SDK::Drawing::GetGridCellCoords(8, 19);
			T6SDK::Drawing::DrawTextAbsolute(buffer1, coords.x, coords.y, 1.5f, T6SDK::Drawing::WHITECOLOR, T6SDK::AnchorPoint::Center, 0x00);

			vec2_t coords2 = T6SDK::Drawing::GetGridCellCoords(8, 21);
			char buffer3[256];
			sprintf(buffer3, "Press ^3%s ^7to stop. Press ^1%s ^7to abort recording and remove recorded files.", T6SDK::Input::GetKeyByCode(Settings::Settings::KeyBinds["StreamsStartStop"])->KeyName.c_str(), T6SDK::Input::GetKeyByCode(Settings::Settings::KeyBinds["StreamsAbort"])->KeyName.c_str());
			T6SDK::Drawing::DrawTextAbsolute(buffer3, coords2.x, coords2.y, 1.0f, T6SDK::Drawing::WHITECOLOR, T6SDK::AnchorPoint::Center, 0x00);
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
		if (StreamShouldIgnoreReshade == false)
		{
			if (T6SDK::Dvars::GetBool(CustomDvars::dvar_streams_noFlash))
			{
				if (!IsAnyOtherStream)
					HRES = p_present(p_swap_chain, sync_interval, flags);
			}
			else
				HRES = p_present(p_swap_chain, sync_interval, flags);
		}


		if (ScreenshotRequested)
		{
			if (T6SDK::Dvars::GetBool(CustomDvars::dvar_streams_avi))
			{
				ID3D11Texture2D* backBuffer;
				HRESULT hr = p_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&backBuffer));
				CaptureBackBufferAndWriteToAVI(p_context, p_device, backBuffer);
				ScreenshotRequested = false;
				backBuffer->Release();
			}
			else
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
		}

		if (StreamShouldIgnoreReshade == true)
		{
			if (T6SDK::Dvars::GetBool(CustomDvars::dvar_streams_noFlash))
			{
				if (!IsAnyOtherStream)
					HRES = p_present(p_swap_chain, sync_interval, flags);
			}
			else
				HRES = p_present(p_swap_chain, sync_interval, flags);
		}

		return HRES;
	}
	static long __stdcall detour_ResizeBuffers(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
	{
		T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, false, "STREAMS", "Resizing buffers to %ix%i", Width, Height);
		IsAnyOtherStream = false;
		return p_ResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
	}

	static long __stdcall detour_ResizeTarget(IDXGISwapChain* pSwapChain, const DXGI_MODE_DESC* pNewTargetParameters)
	{
		IsAnyOtherStream = false;
		T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, false, "STREAMS", "Resizing target to %ix%i", pNewTargetParameters->Width, pNewTargetParameters->Height);
		return p_ResizeTarget(pSwapChain, pNewTargetParameters);
	}

	bool initDX11()
	{
		if (!get_present_pointer())
		{
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, false, "STREAMS", "Failed to get present pointer");
			return false;
		}

		MH_STATUS status = MH_Initialize();
		if (status != MH_OK)
		{
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, false, "STREAMS", "Failed to initialize MinHook");
			return false;
		}
		
		//MinHook (used to be used in v1.0.0)
		if (MH_CreateHook(reinterpret_cast<void**>(p_present_target), &detour_present, reinterpret_cast<void**>(&p_present)) != MH_OK) 
		{
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, false, "STREAMS", "Failed to hook present");
			return false;
		}
		else
		{
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_INFO, false, "STREAMS", "Hooked present => 0x%X", (reinterpret_cast<void**>(p_present_target)));
		}

		if (MH_EnableHook(p_present_target) != MH_OK) 
		{
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, false, "STREAMS", "Failed to enable present");
			return false;
		}


		//if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		//{
		//	kiero::bind((uint16_t)8, (void**)&p_present, detour_present);
		//	T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_INFO, false, "STREAMS", "Hooked present => 0x%X", (reinterpret_cast<void**>(p_present_target)));
		//}
		//else
		//{
		//	T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, false, "STREAMS", "Failed to init kiero");
		//	return false;
		//}


		if (MH_CreateHook(reinterpret_cast<void**>(p_ResizeBuffers_target), &detour_ResizeBuffers, reinterpret_cast<void**>(&p_ResizeBuffers)) != MH_OK) 
		{
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, false, "STREAMS", "Failed to hook ResizeBuffers");
			return false;
		}

		if (MH_EnableHook(p_ResizeBuffers_target) != MH_OK) 
		{
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, false, "STREAMS", "Failed to enable ResizeBuffers");
			return false;
		}

		if (MH_CreateHook(reinterpret_cast<void**>(p_ResizeTarget_target), &detour_ResizeTarget, reinterpret_cast<void**>(&p_ResizeTarget)) != MH_OK) 
		{
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, false, "STREAMS", "Failed to hook ResizeTarget");
			return false;
		}

		if (MH_EnableHook(p_ResizeTarget_target) != MH_OK) 
		{
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, false, "STREAMS", "Failed to enable ResizeTarget");
			return false;
		}

		return true;
	}
#pragma endregion

#pragma region Streams methods

	ofstream ExportCameraFile{};
	void RecordCamera()
	{
		if (ExportCameraFile.is_open())
		{
			char exportStringline[256];
			bool isPov = T6SDK::Addresses::DemoPlayback.Value()->CameraMode == T6SDK::DemoCameraMode::NONE;
			if (isPov)
			{
				//FOV;X;Y;Z;PITCH;YAW;ROLL
				sprintf(exportStringline, "%f,%f,%f,%f,%f,%f,%f", T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::cg_fov),
					T6SDK::Addresses::cg->RefDef.vOrigin.x, T6SDK::Addresses::cg->RefDef.vOrigin.y, T6SDK::Addresses::cg->RefDef.vOrigin.z,
					T6SDK::Addresses::cg->RefDefViewAngles.x, T6SDK::Addresses::cg->RefDefViewAngles.y, T6SDK::Addresses::cg->RefDefViewAngles.z);
			}
			else
			{
				//FOV;X;Y;Z;PITCH;YAW;ROLL
				sprintf(exportStringline, "%f,%f,%f,%f,%f,%f,%f", T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::cg_fov),
					T6SDK::Addresses::DemoPlayback.Value()->FreeRoamCamera.Origin.x, T6SDK::Addresses::DemoPlayback.Value()->FreeRoamCamera.Origin.y, T6SDK::Addresses::DemoPlayback.Value()->FreeRoamCamera.Origin.z,
					T6SDK::Addresses::DemoPlayback.Value()->FreeRoamCamera.Angles.x, T6SDK::Addresses::DemoPlayback.Value()->FreeRoamCamera.Angles.y, T6SDK::Addresses::DemoPlayback.Value()->FreeRoamCamera.Angles.z);
			}
			ExportCameraFile << exportStringline << endl;
		}
	}

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
	bool abortingStreams = false;
	void NotifyOnStreamsFinished()
	{
		if (abortingStreams == false)
		{
			WinToastHelper::send_toast_notification(L"T6MVM STREAMS FINISHED", L"Recording Complete!");
			if (Settings::Settings::PostStreamsActions["Notification"] == true && !Settings::Settings::AlertzyKey.empty())
			{
				T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_INFO, false, "STREAMS", "Streams finished. Sending notification via Alertzy.");
				try
				{
					// Get the current time
					auto now = std::chrono::system_clock::now();
					auto in_time_t = std::chrono::system_clock::to_time_t(now);

					// Format the time as a string (e.g., "2023-10-05_14-30-45")
					std::stringstream ss{};
					ss << std::put_time(std::localtime(&in_time_t), "%B %d %Y at %I:%M %p");
					std::string message = "Streams recording finished on " + ss.str();
					std::string script = "curl -s -X POST https://alertzy.app/send \\ -d \"accountKey=" + Settings::Settings::AlertzyKey + "\" \\ -d \"title=T6MVM\" \\ -d \"message=" + message + "\"";
					std::system(script.c_str());
					T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_SUCCESS, false, "STREAMS", "Notification sent.");
				}
				catch (const char* error)
				{
					T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, false, "STREAMS", "Notification was not sent dut to an error.");
				}
			}
			if (Settings::Settings::PostStreamsActions["OpenStreamsFolder"] == true)
			{
				//Open folder when streams are done
				T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_INFO, false, "STREAMS", "Streams finished. Attempting to open the streams folder...");
				std::string cmd = "explorer " + folderToDeleteWhenAborted;
				std::system(cmd.c_str());
			}
		}
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
		if (frameCount > 0)
		{
			CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)NotifyOnStreamsFinished, nullptr, 0, nullptr);
		}
		frameCount = 0;
		T6SDK::Addresses::DemoPlayback.Value()->DemoHudHidden = false;
		IsStreamsStarted = false;
		/*if(T6SDK::Dvars::GetBool(CustomDvars::dvar_frozenCam))
			T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::timescale, 1.0f);*/
		if (CustomDvars::dvar_streams_avi->current.enabled)
		{
			for (auto& stream : Streams)
			{
				if (stream->toggle->current.enabled)
				{
					if (stream->gmav)
					{
						gmav_finish(stream->gmav);
						stream->gmav = 0x00;
					}
				}
			}
		}
		if (ExportCameraFile.is_open())
		{
			ExportCameraFile.close();
		}
	}

	int StreamsPassesEnabled = 0;
	static int FrameTimeMeasured = 0;
	static vector<std::chrono::nanoseconds> FrameTimes{};
	static std::chrono::steady_clock::time_point start_time;
	static std::chrono::steady_clock::time_point end_time;

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
		if(FrameTimeMeasured < 3)
			start_time = std::chrono::high_resolution_clock::now();
		IsStreamsRunning = true;
		IsAnyOtherStream = false;
		StreamShouldIgnoreReshade = false;
		T6SDK::InternalFunctions::SCR_UpdateScreen(2);
		StreamsPassesEnabled = 0;
		if(T6SDK::Theater::IsInTheater())
		{
			for (auto& stream : Streams)
			{
				if (stream->toggle->current.enabled)
				{
					StreamsPassesEnabled++;
					IsAnyOtherStream = true; //NOFLASH
					StreamShouldIgnoreReshade = stream->IgnoreReshade->current.enabled;
					stream->Enable();
					gmav = stream->gmav;
					CaptureScreenshot(stream);
					stream->Disable();
				}
			}
		}
		if(StreamsPassesEnabled == 0)
			CaptureScreenshot(&Default);

		frameCount++;
		if (T6SDK::Dvars::GetBool(CustomDvars::dvar_streams_recordCam))
		{
			RecordCamera();
		}

		if (FrameTimeMeasured < 3)
		{
			end_time = std::chrono::high_resolution_clock::now();
			auto duration = end_time - start_time;
			FrameTimes.push_back(duration);
			FrameTimeMeasured++;
		}
		else if(FrameTimeMeasured == 3)
		{
			//Calculate average frame time
			auto total_duration = std::chrono::nanoseconds(0);
			for (const auto& frame_time : FrameTimes)
			{
				total_duration += frame_time;
			}
			auto average_duration = total_duration / FrameTimes.size();
			int average_frame_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(average_duration).count();
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_INFO, false, "STREAMS", "Average frame time: %d ms", average_frame_time_ms);
			if (T6SDK::Dvars::GetInt(CustomDvars::dvar_streams_tickStart) > -1 && T6SDK::Dvars::GetInt(CustomDvars::dvar_streams_tickEnd) > -1)
			{
				int totalFrames = (T6SDK::Dvars::GetInt(CustomDvars::dvar_streams_tickEnd) - T6SDK::Dvars::GetInt(CustomDvars::dvar_streams_tickStart)) / (1000 / T6SDK::Dvars::GetInt(CustomDvars::dvar_streams_fps));
				int estimatedDuration = totalFrames * average_frame_time_ms;
				T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_INFO, false, "STREAMS", "Approximate estimated duration: %i hour(s) %i minute(s) %i second(s)	", estimatedDuration / 3600000, (estimatedDuration / 60000) % 60, estimatedDuration / 1000);
			}
			FrameTimeMeasured++;
		}

		//T6SDK::ConsoleLog::LogFormatted(CONSOLETEXTGREEN, "Successfully captured %i", frameCount);
	}

	uintptr_t eaxTMP, ecxTMP, edxTMP, esiTMP, ediTMP, espTMP, ebpTMP;
	int tickStep = 0;
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
		}
		if (T6SDK::MAIN::ENABLED)
		{
			tickStep = 1000 / T6SDK::Dvars::GetInt(CustomDvars::dvar_streams_fps);
			__asm
			{
				mov edx, [edxTMP]
				mov ecx, [ecxTMP]
				mov esi, [esiTMP]
				mov edi, [ediTMP]
				mov esp, [espTMP]
				mov ebp, [ebpTMP]
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
		else
		{
			__asm
			{
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
	}

	inline static void StartStreams()
	{
		if (!T6SDK::Dvars::GetBool(CustomDvars::dvar_streams))
			return;
		//Creating a new directory for streams
		if (Settings::Settings::StreamsDirectory.empty())
		{
			T6SDK::Theater::Demo_Error("Streams directory is not set!", "Use ^5mvm_streams_directory ^7dvar or use ^5TAB ^7menu to set a directory for streams.");
			return;
		}
		char streamsDirectory[256];
		sprintf(streamsDirectory, "%s\\Streams_%s", Settings::Settings::StreamsDirectory.c_str(), T6SDK::InternalFunctions::getCurrentDateTimeString().c_str());
		T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, false, "STREAMS", "Creating directory for streams at: %s", streamsDirectory);
		if (!T6SDK::InternalFunctions::CreateNewDirectory(streamsDirectory))
		{
			T6SDK::Theater::Demo_Error("Error occured!", "Failed to create directory for streams!");
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, false, "STREAMS", "Failed to create directory for streams!");
			StopStreams();
			return;
		}
		StreamsCommon::CapturePreviousDof();
		StreamsCommon::CapturePreviousSsao();
		newStreamFolder = streamsDirectory;
		folderToDeleteWhenAborted = string(streamsDirectory);
		T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_INFO, false, "STREAMS", "newStreamFolder: %s", newStreamFolder);
		if(T6SDK::Dvars::GetInt(CustomDvars::dvar_streams_tickStart) > -1)
			T6SDK::Theater::Demo_JumpToTick(T6SDK::Dvars::GetInt(CustomDvars::dvar_streams_tickStart));
		T6SDK::Addresses::DemoPlayback.Value()->DemoHudHidden = true;
		CustomDvars::dvar_greenScreen->current.enabled = false;
		//Create directories for screenshots or avi files for each enabled pass
		for (auto& stream : Streams)
		{
			stream->Disable();
			if (stream->toggle->current.enabled)
			{
				if (CustomDvars::dvar_streams_avi->current.enabled)
				{
					char buffer[256];
					sprintf(buffer, "%s\\%s.avi", newStreamFolder, stream->Name);
					std::string aviPath(buffer);
					stream->gmav = gmav_open(aviPath.c_str(), T6SDK::Addresses::ScreenWidth.Value(), T6SDK::Addresses::ScreenHeight.Value(), T6SDK::Dvars::GetInt(CustomDvars::dvar_streams_fps));
				}
				else
				{
					char buffer[256];
					sprintf(buffer, "%s\\%s", newStreamFolder, stream->Name);
					T6SDK::InternalFunctions::CreateNewDirectory(buffer);
					//strcpy(stream->Dir, buffer);
					stream->Dir = buffer;
					T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_INFO, false, "STREAMS", "New stream directory for %s", stream->Dir.c_str());
				}
			}
		}

		if (T6SDK::Dvars::GetBool(CustomDvars::dvar_streams_recordCam))
		{
			bool isPov = T6SDK::Addresses::DemoPlayback.Value()->CameraMode == T6SDK::DemoCameraMode::NONE;
			char CameraFileBuffer[256];
			sprintf(CameraFileBuffer, "%s\\%s_CameraData.t6cr", newStreamFolder, isPov ? "Pov" : "Cine");
			string str(CameraFileBuffer);
			ExportCameraFile.open(str, ios_base::out);
		}

		LocalAddresses::h_TickIncreasing.Hook(OnTickIncreasing);
		T6SDK::Addresses::IsDemoPaused.Value() = false;
		IsStreamsStarted = true;

		char buffer2[256];
		sprintf(buffer2, "Press %s to stop. Press %s to abort recording and remove recorded files.", 
			T6SDK::Input::GetKeyByCode(Settings::Settings::KeyBinds["StreamsStartStop"])->KeyName.c_str(), 
			T6SDK::Input::GetKeyByCode(Settings::Settings::KeyBinds["StreamsAbort"])->KeyName.c_str());
		// Create converter
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

		// Convert to wstring
		std::wstring result = converter.from_bytes(buffer2);
		WinToastHelper::send_toast_notification(L"T6MVM STREAMS STARTED", result);
		FrameTimeMeasured = 0;
	}

	inline static void AbortStreams()
	{
		if(!IsStreamsStarted)
			return;
		abortingStreams = true;
		StopStreams();
		abortingStreams = false;
		T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_INFO, false, "STREAMS", "Attempting to delete the streams folder...");
		T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, false, "STREAMS", folderToDeleteWhenAborted.c_str());
		try 
		{
			// Check if the folder exists
			if (std::filesystem::exists(folderToDeleteWhenAborted.c_str()))
			{
				// Delete the folder and its contents
				std::filesystem::remove_all(folderToDeleteWhenAborted.c_str());
				T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_SUCCESS, false, "STREAMS", "Folder deleted successfully");
			}
			else 
			{
				T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_WARNING, false, "STREAMS", "Folder does not exist");
			}
		}
		catch (const std::filesystem::filesystem_error& e)
		{
			// Handle filesystem errors
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, false, "STREAMS", "Error deleting folder:");
		}
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


	inline static void InitStreams()
	{
		//Default
		Default.Init();
		Streams.push_back(&Default);
		T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_INFO, false, "STREAMS", "Default stream registered!");

		//NoGun
		NoGun.Init();
		Streams.push_back(&NoGun);
		T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_INFO, false, "STREAMS", "No gun stream registered!");

		//GreenScreen
		GreenScreen.Init();
		Streams.push_back(&GreenScreen);
		T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_INFO, false, "STREAMS", "Green screen stream registered!");

		//NoPlayers
		NoPlayers.Init();
		Streams.push_back(&NoPlayers);
		T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_INFO, false, "STREAMS", "No players stream registered!");

		//Depth
		Depth.Init();
		Streams.push_back(&Depth);
		T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_INFO, false, "STREAMS", "Depth stream registered!");

		//DepthWithGun
		DepthWithGun.Init();
		Streams.push_back(&DepthWithGun);
		T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_INFO, false, "STREAMS", "Depth with gun stream registered!");

		//FxOnly
		FxOnly.Init();
		Streams.push_back(&FxOnly);
		T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_INFO, false, "STREAMS", "FxOnly stream registered!");

		//Green sky
		GreenSky.Init();
		Streams.push_back(&GreenSky);
		T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_INFO, false, "STREAMS", "Green sky stream registered!");


		T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_SUCCESS, false, "STREAMS", "Total streams registered: %i", Streams.size());

		T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, false, "STREAMS", "Initializing DX11...");
		if (initDX11() == false)
		{
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, false, "STREAMS", "Failed to initialize DX11!");
			return;
		}
		T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_SUCCESS, false, "STREAMS", "DX11 initialized.");


		//Init UI for passes
		T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, false, "STREAMS", "Initializing streams passes UI...");

		UIControls::UI_StreamsPass1CheckBox = UI_StreamPassNS::UI_StreamPass(&Streams, Streams[0], std::string("DEFAULT"), 12, 7, T6SDK::AnchorPoint::TopLeft);
		UIControls::UI_StreamsPass2CheckBox = UI_StreamPassNS::UI_StreamPass(&Streams, Streams[1], std::string("NO GUN"), 12, 11, T6SDK::AnchorPoint::TopLeft);
		UIControls::UI_StreamsPass3CheckBox = UI_StreamPassNS::UI_StreamPass(&Streams, Streams[2], std::string("GREEN SCREEN"), 12, 15, T6SDK::AnchorPoint::TopLeft);
		UIControls::UI_StreamsPass4CheckBox = UI_StreamPassNS::UI_StreamPass(&Streams, Streams[3], std::string("NO PLAYERS"), 12, 19, T6SDK::AnchorPoint::TopLeft);
		UIControls::UI_StreamsPass5CheckBox = UI_StreamPassNS::UI_StreamPass(&Streams, Streams[4], std::string("DEPTH"), 12, 23, T6SDK::AnchorPoint::TopLeft);
		UIControls::UI_StreamsPass6CheckBox = UI_StreamPassNS::UI_StreamPass(&Streams, Streams[5], std::string("DEPTH WITH GUN"), 12, 27, T6SDK::AnchorPoint::TopLeft);
		UIControls::UI_StreamsPass7CheckBox = UI_StreamPassNS::UI_StreamPass(&Streams, Streams[6], std::string("FX ONLY"), 12, 31, T6SDK::AnchorPoint::TopLeft);
		UIControls::UI_StreamsPass8CheckBox = UI_StreamPassNS::UI_StreamPass(&Streams, Streams[7], std::string("GREEN SKY"), 12, 35, T6SDK::AnchorPoint::TopLeft);

		T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_SUCCESS, false, "STREAMS", "Streams passes UI initialized.");

		T6SDK::Dvars::Cmd_AddCommandInternal("mvm_streams_start", StartStreams, &cmd_streams_start_VAR);
		T6SDK::Dvars::Cmd_AddCommandInternal("mvm_streams_stop", StopStreams, &cmd_streams_stop_VAR);
		T6SDK::Dvars::Cmd_AddCommandInternal("mvm_streams_abort", AbortStreams, &cmd_streams_abort_VAR);
	}
#pragma endregion
}