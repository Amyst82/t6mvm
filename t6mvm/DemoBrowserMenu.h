#pragma once
#include "UIControls.h"
#include "CustomDvars.h"
namespace DemoBrowserMenu
{
	int selectedDemoNumber = 0;
	bool isShown = false;

	static void Close()
	{
		T6SDK::InternalFunctions::Key_SetCatcher(16);
		isShown = false;
	}

	std::string selectedFolder{};
	void ChangeDemoDirectory()
	{
		// Open the folder dialog
		if (T6SDK::InternalFunctions::OpenFolderDialog(selectedFolder))
		{
			CustomDvars::dvar_demos_directory->current.string = selectedFolder.c_str();
			CustomDvars::dvar_demos_directory->modified = true;
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, false, "DEMOBROWSER", "Selected folder: %s", CustomDvars::dvar_demos_directory->current.string);
			Common::LoadAllDemos();
			//Saving directory to settings
			std::string settingsPath = std::string(T6SDK::Dvars::GetString(*T6SDK::Dvars::DvarList::fs_homepath)) + "\\Plugins\\t6mvm.json";
			if (!std::filesystem::exists(settingsPath))
			{
				T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_WARNING, false, "DEMOBROWSER", "Unable to save output directory. Settings JSON not found.");
			}
			else
			{
				try
				{
					// 1. Read JSON file
					std::ifstream input_file(settingsPath);
					if (!input_file.is_open())
					{
						T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, false, "DEMOBROWSER", "Could not open settings file.");
						return;
					}
					json data;
					input_file >> data;
					input_file.close();
					// 2. Modify JSON data
					data["DemosDirectory"] = selectedFolder.c_str();
					// 3. Write modified JSON back to file
					std::ofstream output_file(settingsPath);
					if (!output_file.is_open())
					{
						T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, false, "DEMOBROWSER", "Could not write to settings file.");
					}
					// Write with pretty printing (indentation = 4)
					output_file << data.dump(4);
					output_file.close();

					T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_INFO, false, "DEMOBROWSER", "Demo directory updated in t6mvm.json.");
				}
				catch (const std::exception& e)
				{
					T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, false, "DEMOBROWSER", e.what());
				}
			}
		}
		else
		{
			std::cerr << "No folder selected or an error occurred." << std::endl;
		}
	}

	void DrawSelectedDemoThumbnail()
	{
		float scale = 1.0f / 1080.0f * static_cast<float>(T6SDK::Addresses::ScreenHeight.Value()); //Assuming that initial size is in 1920x1080			
		float h = (float)T6SDK::Drawing::normalFont->pixelHeight * scale;
		vec2_t coordsStart = T6SDK::Drawing::GetGridCellCoords(11, 4);
		vec2_t coordsEnd = T6SDK::Drawing::GetGridCellCoords(15, 14);
		float WIDTH = coordsEnd.x - coordsStart.x;
		float HEIGHT = coordsEnd.y - coordsStart.y;
		RECT thumbnailRect{};
		bool successDraw = false;
		//Drawing orange border
		float borderWidth = 2.0f;
		successDraw = T6SDK::Drawing::DrawRectAbsolute(coordsStart.x- borderWidth, coordsStart.y- borderWidth, WIDTH + borderWidth * 2.0f, HEIGHT + borderWidth * 2.0f, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		//Getting base rect bounds
		successDraw = T6SDK::Drawing::DrawRectAbsolute(coordsStart.x, coordsStart.y, WIDTH, HEIGHT, T6SDK::Drawing::T_BLACKCOLOR, T6SDK::AnchorPoint::TopLeft, &thumbnailRect);
		if (successDraw)
		{
			//Drawing map ui image
			T6SDK::Typedefs::R_AddCmdDrawStretchPicRotateXYInternal(coordsStart.x, coordsStart.y, WIDTH, HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, T6SDK::Drawing::WHITECOLOR, T6SDK::InternalFunctions::DB_FindXAssetHeader(T6SDK::XAssetType::MATERIAL, Common::LocalDemos[selectedDemoNumber].MapUiSelect.c_str()));

			//Drawing duration
			RECT durationtextRect{};
			float durationTextX = coordsStart.x + WIDTH - 5.0f;
			float durationTextY = coordsStart.y + 5.0f;
			char durationBuffer[64];
			sprintf_s(durationBuffer, "%02i:%02i", Common::LocalDemos[selectedDemoNumber].Duration/60000, Common::LocalDemos[selectedDemoNumber].Duration%60000/1000);
			float durationTextWidth = T6SDK::InternalFunctions::R_TextWidth(durationBuffer, strlen(durationBuffer), T6SDK::Drawing::normalFont) * scale;
			T6SDK::Drawing::DrawRectAbsolute(durationTextX, durationTextY, durationTextWidth+4.0f, h+4.0f, T6SDK::Drawing::BLACKCOLOR, T6SDK::AnchorPoint::TopRight, &durationtextRect);
			T6SDK::Drawing::DrawTextAbsolute(durationBuffer,(float)durationtextRect.left+2.0f, (float)durationtextRect.top+2.0f, 1.0f, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);

			//Drawing name
			RECT nametextRect{};
			T6SDK::Drawing::DrawRectAbsolute(coordsStart.x, coordsEnd.y-h-4.0f, WIDTH, h+4.0f, T6SDK::Drawing::BLACKCOLOR, T6SDK::AnchorPoint::TopLeft, &nametextRect);
			char demoNameBuffer[256];
			sprintf_s(demoNameBuffer, "%s ^9by ^7%s", Common::LocalDemos[selectedDemoNumber].DemoName.c_str(), Common::LocalDemos[selectedDemoNumber].Author.c_str());
			T6SDK::Drawing::DrawTextAbsolute(demoNameBuffer, (float)nametextRect.left+2.0f, (float)nametextRect.top+2.0f, 1.0f, T6SDK::Drawing::WHITECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
			
			//Drawing description
			char descBuffer[256];
			sprintf_s(descBuffer, "^5> ^9%s on %s", Common::LocalDemos[selectedDemoNumber].GameMode.c_str(), Common::LocalDemos[selectedDemoNumber].MapFriendlyName.c_str());
			T6SDK::Drawing::DrawTextAbsolute(descBuffer, coordsStart.x, coordsEnd.y + 10.0f, 1.0f, T6SDK::Drawing::WHITECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);

			//Drawing creation date
			T6SDK::Drawing::DrawTextAbsolute(T6SDK::InternalFunctions::FormatUnixTime(Common::LocalDemos[selectedDemoNumber].CreateDate).c_str(), coordsStart.x, coordsEnd.y + 50.0f, 1.0f, T6SDK::Drawing::GRAYCOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);

		}
	}
	static void Draw()
	{
		if(!isShown)
			return;
		//Drawing bg
		//T6SDK::Typedefs::R_AddCmdDrawStretchPicRotateXYInternal(0.0f, 0.0f, T6SDK::Addresses::ScreenWidth.Value(), T6SDK::Addresses::ScreenHeight.Value(), 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, T6SDK::Drawing::WHITECOLOR, 
			//T6SDK::InternalFunctions::DB_FindXAssetHeader(T6SDK::XAssetType::MATERIAL, "menu_mp_background_main2"));
		T6SDK::Drawing::DrawRectRelative(0.0f, 0.0f, 1.0f, 1.0f, T6SDK::Drawing::T_GRAYCOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		T6SDK::Drawing::DrawRectRelative(0.0f, 0.0f, 1.0f, 1.0f, T6SDK::Drawing::T_BLACKCOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		T6SDK::Drawing::DrawRectRelative(0.0f, 0.0f, 1.0f, 1.0f, T6SDK::Drawing::T_BLACKCOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);

		//Drawing control bg
		if (Common::LocalDemos.size() > 0)
			T6SDK::Drawing::DrawRectAbsolute(10, 0, 16, 41, 0.0f, 0.0f, T6SDK::Drawing::T_BLACKCOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);


		//Drawing thumbnail
		if(Common::LocalDemos.size() > 0)
		{
			DrawSelectedDemoThumbnail();
			UIControls::UI_DemosDirectoryButton.cyclingFading = false;
		}
		else
		{
			char noDemoSBuffer [100];
			sprintf_s(noDemoSBuffer, "^9There are no demos in selected directory");
			T6SDK::Drawing::DrawTextRelative(noDemoSBuffer, 0.5f, 0.5f, 1.1f, T6SDK::Drawing::WHITECOLOR, T6SDK::AnchorPoint::Center, 0x00);
			char noDemoSBuffer2[100];
			sprintf_s(noDemoSBuffer2, "^9You can change directory by clicking the button below");
			T6SDK::Drawing::DrawTextRelative(noDemoSBuffer2, 0.5f, 0.53f, 1.0f, T6SDK::Drawing::WHITECOLOR, T6SDK::AnchorPoint::Center, 0x00);
			UIControls::UI_DemosDirectoryButton.cyclingFading = true;
		}

		UIControls::UI_DemosDirectoryButton.ToolTip = CustomDvars::dvar_demos_directory->current.string;
		UIControls::UI_DemosDirectoryButton.Draw();
		UIControls::UI_CloseDemoSelectMenu.Draw();
		if (Common::LocalDemos.size() <= 0)
			return;

		//Drawing controls
		char buffer[100];
		sprintf_s(buffer, "%i / %i demos", selectedDemoNumber + 1, Common::LocalDemos.size());
		UIControls::UI_SelectedDemoNumber.Text = buffer;
		UIControls::UI_SelectedDemoNumber.Draw();
		UIControls::UI_PlayDemoButton.Draw();

		UIControls::UI_NavigateToDemoFile.Draw();

	}
	static void Init()
	{
		UIControls::UI_SelectedDemoNumber = T6SDK::Drawing::UI_EnumButton("Demo number", 0, Common::LocalDemos.size()-1, &selectedDemoNumber, 8, 35, T6SDK::AnchorPoint::Center, 0x00);
		UIControls::UI_PlayDemoButton = T6SDK::Drawing::UI_RectangleClickableButton("     ^5PLAY DEMO     ", 13, 30, T6SDK::AnchorPoint::Center, true, []()
		{
				Close();
				//TODO check for boundaries
			Common::CurrentLoadedDemo.Clear();
			T6SDK::DemoHandler::LoadDemoFromFile(Common::LocalDemos[selectedDemoNumber].DemoPath.c_str(), &Common::CurrentLoadedDemo);
			if (Common::CurrentLoadedDemo.HasMetadata)
			{
				T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, false, "DEMO", "Trying to add bookmark from metadata...");
				Common::AddBookmarkFromJson(Common::CurrentLoadedDemo.Metadata);
			}
		});
		UIControls::UI_CloseDemoSelectMenu = T6SDK::Drawing::UI_ClickableButton("^3ESC ^7Back", 3, 38, T6SDK::AnchorPoint::Center, (uintptr_t)&Close);
		UIControls::UI_NavigateToDemoFile = T6SDK::Drawing::UI_ClickableButton("SHOW IN EXPLORER", 14, 37, T6SDK::AnchorPoint::TopLeft, false, 0x00);
		UIControls::UI_NavigateToDemoFile.Size = 1.0f;
		UIControls::UI_DemosDirectoryButton = T6SDK::Drawing::UI_ClickableButton("CHANGE FOLDER", 14, 35, T6SDK::AnchorPoint::TopLeft, (uintptr_t)&ChangeDemoDirectory);
		UIControls::UI_DemosDirectoryButton.ToolTip = "Select the directory where demos are stored.";
		UIControls::UI_DemosDirectoryButton.Size = 1.0f;
	}
	static void Show()
	{
		T6SDK::InternalFunctions::Key_SetCatcher(15);
		Init();
		isShown = true;
	}
}