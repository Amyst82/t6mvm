#pragma once
#include "UIControls.h"
#include "Streams.h"
#include "CustomDvars.h"
namespace StreamsMenu
{
	static vector<int> streamsAvailableFPS = { 50, 100, 200, 250, 500, 1000 };
	static int selectedFPS = 5;

	void SetStartTick()
	{
		int startTick = T6SDK::Dvars::GetInt(CustomDvars::dvar_streams_tickStart);
		if (startTick == -1)
		{
			if (T6SDK::Addresses::Tick.Value() > T6SDK::Dvars::GetInt(CustomDvars::dvar_streams_tickEnd) && T6SDK::Dvars::GetInt(CustomDvars::dvar_streams_tickEnd) != -1)
			{
				T6SDK::Theater::Demo_Error("Unable to set start tick.", "We can't record backwards yet.");
				return;
			}
			T6SDK::Dvars::SetInt(CustomDvars::dvar_streams_tickStart, T6SDK::Addresses::Tick.Value());
			int tick = T6SDK::Dvars::GetInt(CustomDvars::dvar_streams_tickStart);
			char buffer[256];
			sprintf(buffer, "Start: ^3%i ^7(%02i:%02i)", tick, tick / 60000, tick % 60000 / 1000);
			UIControls::UI_StreamsStartTickButton.Text = buffer;
		}
		else if(startTick > -1)
		{
			T6SDK::Dvars::SetInt(CustomDvars::dvar_streams_tickStart, -1);
			UIControls::UI_StreamsStartTickButton.Text = "Start tick is not set";
		}
	}
	void SetEndTick()
	{
		int endTick = T6SDK::Dvars::GetInt(CustomDvars::dvar_streams_tickEnd);
		if (endTick == -1)
		{
			if (T6SDK::Addresses::Tick.Value() < T6SDK::Dvars::GetInt(CustomDvars::dvar_streams_tickStart) && T6SDK::Dvars::GetInt(CustomDvars::dvar_streams_tickStart) != -1)
			{
				T6SDK::Theater::Demo_Error("Unable to set end tick.", "We can't record backwards yet.");
				return;
			}
			T6SDK::Dvars::SetInt(CustomDvars::dvar_streams_tickEnd, T6SDK::Addresses::Tick.Value());
			int tick = T6SDK::Dvars::GetInt(CustomDvars::dvar_streams_tickEnd);
			char buffer[256];
			sprintf(buffer, "End: ^3%i ^7(%02i:%02i)", tick, tick / 60000, tick % 60000 / 1000);
			UIControls::UI_StreamsEndTickButton.Text = buffer;
		}
		else if(endTick > -1)
		{
			T6SDK::Dvars::SetInt(CustomDvars::dvar_streams_tickEnd, -1);
			UIControls::UI_StreamsEndTickButton.Text = "End tick is not set";
		}
	}

	std::string selectedFolder{};
	void SetStreamsDirectory()
	{


		// Open the folder dialog
		if (T6SDK::InternalFunctions::OpenFolderDialog(selectedFolder)) 
		{
			CustomDvars::dvar_streams_directory->current.string = selectedFolder.c_str();
			std::cout << "Selected folder: " << CustomDvars::dvar_streams_directory->current.string << std::endl;
		}
		else 
		{
			std::cerr << "No folder selected or an error occurred." << std::endl;
		}
	}

	static void Draw()
	{
		if (!*UIControls::StreamsTabButton.isChecked)
			return;
		bool streamsEnabled = T6SDK::Dvars::GetBool(CustomDvars::dvar_streams);
		UIControls::UI_ToggleStreamsCheckBox.Draw();

		if(strlen(CustomDvars::dvar_streams_directory->current.string) < 2)
			UIControls::UI_StreamsDirectoryButton.Text = "Output directory is not set";
		else
			UIControls::UI_StreamsDirectoryButton.Text = (std::string("Output: ") + std::string(CustomDvars::dvar_streams_directory->current.string)).c_str();
		UIControls::UI_StreamsDirectoryButton.Draw(streamsEnabled);

		int starttick = T6SDK::Dvars::GetInt(CustomDvars::dvar_streams_tickStart);
		if(starttick > -1)
		{
			char buffer0[256];
			sprintf(buffer0, "Start: ^3%i ^7(%02i:%02i)", starttick, starttick / 60000, starttick % 60000 / 1000);
			UIControls::UI_StreamsStartTickButton.Text = buffer0;
		}
		else
			UIControls::UI_StreamsStartTickButton.Text = "Start tick is not set";
		UIControls::UI_StreamsStartTickButton.Draw(streamsEnabled);

		int endtick = T6SDK::Dvars::GetInt(CustomDvars::dvar_streams_tickEnd);
		if (endtick > -1)
		{
			char buffer1[256];
			sprintf(buffer1, "End: ^3%i ^7(%02i:%02i)", endtick, endtick / 60000, endtick % 60000 / 1000);
			UIControls::UI_StreamsEndTickButton.Text = buffer1;
		}
		else
			UIControls::UI_StreamsEndTickButton.Text = "End tick is not set";
		UIControls::UI_StreamsEndTickButton.Draw(streamsEnabled);
		UIControls::UI_StreamsJPGCheckBox.Draw(streamsEnabled && !T6SDK::Dvars::GetBool(CustomDvars::dvar_streams_avi));

		T6SDK::Dvars::SetInt(CustomDvars::dvar_streams_fps, streamsAvailableFPS[selectedFPS]);
		char buffer[100];
		sprintf(buffer, "FPS: %i", T6SDK::Dvars::GetInt(CustomDvars::dvar_streams_fps));
		UIControls::UI_StreamsFPSEnum.Text = buffer;
		UIControls::UI_StreamsFPSEnum.Draw(streamsEnabled);

		UIControls::UI_StreamsAVICheckBox.Draw(streamsEnabled);
		UIControls::UI_StreamsNoFlashCheckBox.Draw(streamsEnabled);
		UIControls::UI_RecordCamCheckBox.Draw(streamsEnabled);

		//Draw stream passes

		//vec4_t margin = { -15.0f, 10.0f, -10.0f, -5.0f };
		//T6SDK::Drawing::DrawRectAbsolute(12, 4, 15, 24, margin, T6SDK::Drawing::T_BLACKCOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);

		UIControls::UI_StreamsPass1CheckBox.Draw(streamsEnabled);
		UIControls::UI_StreamsPass2CheckBox.Draw(streamsEnabled);
		UIControls::UI_StreamsPass3CheckBox.Draw(streamsEnabled);
		UIControls::UI_StreamsPass4CheckBox.Draw(streamsEnabled);
		UIControls::UI_StreamsPass5CheckBox.Draw(streamsEnabled);
		UIControls::UI_StreamsPass6CheckBox.Draw(streamsEnabled);
		UIControls::UI_StreamsPass7CheckBox.Draw(streamsEnabled);
		UIControls::UI_StreamsPass8CheckBox.Draw(streamsEnabled);
		//UIControls::UI_StreamsPass9CheckBox.Draw(streamsEnabled);
		//UIControls::UI_StreamsPass10CheckBox.Draw(streamsEnabled);
	}
	static void Init()
	{
		UIControls::UI_ToggleStreamsCheckBox = T6SDK::Drawing::UI_CheckBoxButton("STREAMS OFF","STREAMS ON",4,5, T6SDK::AnchorPoint::TopLeft, &CustomDvars::dvar_streams->current.enabled, 0x00);
		UIControls::UI_ToggleStreamsCheckBox.ToolTip = "Toggle recording streams.";

		UIControls::UI_StreamsStartTickButton = T6SDK::Drawing::UI_ClickableButton("Start tick is not set", 4, 8, T6SDK::AnchorPoint::TopLeft, (uintptr_t)&SetStartTick);
		UIControls::UI_StreamsStartTickButton.ToolTip = "Tick value at which streams recording starts.";

		UIControls::UI_StreamsEndTickButton = T6SDK::Drawing::UI_ClickableButton("End tick is not set", 4, 10, T6SDK::AnchorPoint::TopLeft, (uintptr_t)&SetEndTick);
		UIControls::UI_StreamsEndTickButton.ToolTip = "Tick value at which streams recording stops.";

		UIControls::UI_StreamsFPSEnum = T6SDK::Drawing::UI_EnumButton("FPS: ", 0, 5, &selectedFPS, 4, 13, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_StreamsFPSEnum.ToolTip = "Choose the desired FPS for the streams recording.";

		UIControls::UI_StreamsJPGCheckBox = T6SDK::Drawing::UI_CheckBoxButton("IMAGE FORMAT: PNG", "IMAGE FORMAT: JPG", 4, 15, T6SDK::AnchorPoint::TopLeft, &CustomDvars::dvar_streams_JPG->current.enabled, 0x00);
		UIControls::UI_StreamsJPGCheckBox.ToolTip = "^7Switch between ^5PNG ^7and ^5JPG ^7formats. PNG provides better quality. JPG takes less space.";

		UIControls::UI_StreamsAVICheckBox = T6SDK::Drawing::UI_CheckBoxButton("RECORD AS AVI", "RECORD AS IMAGE SEQUENCE", 4, 17, T6SDK::AnchorPoint::TopLeft, &CustomDvars::dvar_streams_avi->current.enabled, 0x00);
		UIControls::UI_StreamsAVICheckBox.ToolTip = "Record as AVI file.";

		UIControls::UI_StreamsNoFlashCheckBox = T6SDK::Drawing::UI_CheckBoxButton("NO FLASH OFF", "NO FLASH ON", 4, 19, T6SDK::AnchorPoint::TopLeft, &CustomDvars::dvar_streams_noFlash->current.enabled, 0x00);
		UIControls::UI_StreamsNoFlashCheckBox.ToolTip = "Don't want to see you screen flashing dureing recording each pass? Turn this on!";

		UIControls::UI_RecordCamCheckBox = T6SDK::Drawing::UI_CheckBoxButton("RECORD CAMERA DATA\tOFF", "RECORD CAMERA DATA\tON", 4, 21, T6SDK::AnchorPoint::TopLeft, &CustomDvars::dvar_streams_recordCam->current.enabled, 0x00);
		UIControls::UI_RecordCamCheckBox.ToolTip = "Get camera orientation and position data exported for each recorded frame.";

		UIControls::UI_StreamsDirectoryButton = T6SDK::Drawing::UI_ClickableButton("Directory is not set", 4, 30, T6SDK::AnchorPoint::TopLeft, (uintptr_t)&SetStreamsDirectory);
		UIControls::UI_StreamsDirectoryButton.ToolTip = "Select the directory where the streams will be saved.";

		
		//UIControls::UI_StreamsPass9CheckBox{};
		//UIControls::UI_StreamsPass10CheckBox{};
	}
}