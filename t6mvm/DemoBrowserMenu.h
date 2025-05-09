#pragma once
#include "UIControls.h"
#include "CustomDvars.h"
namespace DemoBrowserMenu
{
	int selectedDemoNumber = 0;
	bool isShown = false;

	//Scrollable grid
	// Constants
	int ITEM_WIDTH = 200;
	int ITEM_HEIGHT = 100;
	const int ITEM_MARGIN = 10;
	int ITEMS_PER_ROW = 3;
	const int SCROLLBAR_WIDTH = 20;
	// State variables
	int scrollY = 0;
	int visibleAreaHeight = 600; // Height of your visible area/window
	// Scrollbar dragging state
	bool isDraggingScrollbar = false;
	int dragStartY = 0;
	int dragStartScrollY = 0;

	//scrollbar properties
	bool scrollBarThumbHovered = false;
	bool scrollBarThumbClicked = false;
	RECT scrollBarThumbRect{};
	RECT scrollBarRect{};
	bool scrollbarHovered = false;
	bool scrollbarClicked = false;
	void CalculateConstants()
	{
		ITEM_WIDTH = static_cast<int>((float)200/1280.0f * T6SDK::Addresses::ScreenWidth.Value());
		ITEM_HEIGHT = ITEM_WIDTH / 1.78;
		// Calculate the number of items per row
		vec2_t viewportBounds = T6SDK::Drawing::GetGridCellCoords(10,37) - T6SDK::Drawing::GetGridCellCoords(1, 1);
		ITEMS_PER_ROW = floor(static_cast<int>(viewportBounds.x) / (ITEM_WIDTH + ITEM_MARGIN));
		visibleAreaHeight = static_cast<int>(viewportBounds.y);
	}
	int hoveredItem = -1;
	bool clickedItem = false;
	void DrawGriditem(int itemnumber, float x, float y)
	{
		float scale = 0.8f / 1080.0f * static_cast<float>(T6SDK::Addresses::ScreenHeight.Value()); //Assuming that initial size is in 1920x1080			
		float h = (float)T6SDK::Drawing::normalFont->pixelHeight * scale;
		vec2_t coordsStart = {x, y};
		vec2_t coordsEnd = {x + ITEM_WIDTH, y + ITEM_HEIGHT};
		RECT thumbnailRect{};
		bool successDraw = false;
		//Drawing orange border
		float borderWidth = 2.0f;

		successDraw = T6SDK::Drawing::DrawRectAbsolute(coordsStart.x - borderWidth, coordsStart.y - borderWidth, ITEM_WIDTH + borderWidth * 2.0f, ITEM_HEIGHT + borderWidth * 2.0f, itemnumber == selectedDemoNumber ? T6SDK::Drawing::ORANGECOLOR : hoveredItem == itemnumber ? T6SDK::Drawing::WHITECOLOR : T6SDK::Drawing::T_GRAYCOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		//Getting base rect bounds
		successDraw = T6SDK::Drawing::DrawRectAbsolute(coordsStart.x, coordsStart.y, ITEM_WIDTH, ITEM_HEIGHT, T6SDK::Drawing::T_BLACKCOLOR, T6SDK::AnchorPoint::TopLeft, &thumbnailRect);
		if (successDraw)
		{
			//Drawing map ui image
			T6SDK::Typedefs::R_AddCmdDrawStretchPicRotateXYInternal(coordsStart.x, coordsStart.y, ITEM_WIDTH, ITEM_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  T6SDK::Drawing::WHITECOLOR, T6SDK::InternalFunctions::DB_FindXAssetHeader(T6SDK::XAssetType::MATERIAL, Common::LocalDemos[itemnumber].MapUiSelect.c_str()));

			//Make it dimmed if not selected
			if (!(itemnumber == selectedDemoNumber || hoveredItem == itemnumber))
			{
				T6SDK::Drawing::DrawRectAbsolute(coordsStart.x, coordsStart.y, ITEM_WIDTH, ITEM_HEIGHT, T6SDK::Drawing::T_BLACKCOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
			}

			//Drawing duration
			RECT durationtextRect{};
			float durationTextX = coordsStart.x + ITEM_WIDTH - 5.0f;
			float durationTextY = coordsStart.y + 5.0f;
			char durationBuffer[64];
			sprintf_s(durationBuffer, "%02i:%02i", Common::LocalDemos[itemnumber].Duration / 60000, Common::LocalDemos[itemnumber].Duration % 60000 / 1000);
			float durationTextWidth = T6SDK::InternalFunctions::R_TextWidth(durationBuffer, strlen(durationBuffer), T6SDK::Drawing::normalFont) * scale;
			T6SDK::Drawing::DrawRectAbsolute(durationTextX, durationTextY, durationTextWidth + 4.0f, h + 4.0f, T6SDK::Drawing::BLACKCOLOR, T6SDK::AnchorPoint::TopRight, &durationtextRect);
			T6SDK::Drawing::DrawTextAbsolute(durationBuffer, (float)durationtextRect.left + 2.0f, (float)durationtextRect.top + 2.0f, 0.8f, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);

			//Drawing name
			RECT nametextRect{};
			T6SDK::Drawing::DrawRectAbsolute(coordsStart.x, coordsEnd.y - h - 4.0f, ITEM_WIDTH, h + 4.0f, T6SDK::Drawing::BLACKCOLOR, T6SDK::AnchorPoint::TopLeft, &nametextRect);
			char demoNameBuffer[256];
			sprintf_s(demoNameBuffer, "%s ^9by ^7%s", Common::LocalDemos[itemnumber].DemoName.c_str(), Common::LocalDemos[itemnumber].Author.c_str());
			T6SDK::Drawing::DrawTextAbsolute(demoNameBuffer, (float)nametextRect.left + 2.0f, (float)nametextRect.top + 2.0f, 0.8f, T6SDK::Drawing::WHITECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);

			if (T6SDK::Input::MousePosX() > (float)thumbnailRect.left && T6SDK::Input::MousePosX() < (float)thumbnailRect.right && T6SDK::Input::MousePosY() > (float)thumbnailRect.top && T6SDK::Input::MousePosY() < (float)thumbnailRect.bottom)
			{
				hoveredItem = itemnumber;
				if (T6SDK::Input::Keys::LMB.IsAnyPressState())
				{
					if(clickedItem == false)
					{
						clickedItem = true;
						selectedDemoNumber = itemnumber;
						T6SDK::InternalFunctions::PlaySound("uin_main_pause");
					}
				}
				else
				{
					clickedItem = false;
				}
			}
		}
	}
	void draw_scrollable_grid(int viewportX, int viewportY, int viewportWidth, int viewportHeight) 
	{
		// Calculate grid dimensions
		int rows = static_cast<int>(std::ceil(Common::LocalDemos.size() / static_cast<float>(ITEMS_PER_ROW)));
		int contentHeight = rows * (ITEM_HEIGHT + ITEM_MARGIN)+100;
		// Calculate visible rows
		int startRow = scrollY / (ITEM_HEIGHT + ITEM_MARGIN);
		int endRow = (scrollY + viewportHeight) / (ITEM_HEIGHT + ITEM_MARGIN);
		// Draw visible items
		for (int row = startRow; row <= endRow; row++) 
		{
			for (int col = 0; col < ITEMS_PER_ROW; col++) 
			{
				int itemIndex = row * ITEMS_PER_ROW + col;
				if (itemIndex >= static_cast<int>(Common::LocalDemos.size())) continue;
				// Calculate position with scroll offset
				int x = viewportX + col * (ITEM_WIDTH + ITEM_MARGIN);
				int y = viewportY + row * (ITEM_HEIGHT + ITEM_MARGIN) - scrollY;
				if(y > viewportHeight || y + ITEM_HEIGHT > viewportHeight) continue;
				DrawGriditem( itemIndex, (float)x, (float)y);
			}
		}

		// Draw scrollbar if needed
		if (contentHeight > viewportHeight) 
		{
			int scrollbarX = viewportX + viewportWidth - SCROLLBAR_WIDTH;

			// Draw scrollbar track
			T6SDK::Drawing::DrawRectAbsolute((float)scrollbarX, (float)viewportY, (float)SCROLLBAR_WIDTH, (float)viewportHeight, T6SDK::Drawing::GRAYCOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
			if (T6SDK::Drawing::DrawRectAbsolute((float)scrollbarX, (float)viewportY, (float)SCROLLBAR_WIDTH, (float)viewportHeight, T6SDK::Drawing::T_BLACKCOLOR, T6SDK::AnchorPoint::TopLeft, &scrollBarRect))
			{
				if (T6SDK::Input::MousePosX() > (float)scrollBarRect.left && T6SDK::Input::MousePosX() < (float)scrollBarRect.right && T6SDK::Input::MousePosY() > (float)scrollBarRect.top && T6SDK::Input::MousePosY() < (float)scrollBarRect.bottom)
				{
					scrollbarHovered = true;
					if (T6SDK::Input::Keys::LMB.IsAnyPressState())
					{
						if (scrollbarClicked == false)
						{
							scrollbarClicked = true;
							int scrollbarLeft = viewportX + viewportWidth - SCROLLBAR_WIDTH;

							// Check if click is within scrollbar area
							if (T6SDK::Input::MousePosX() >= (float)scrollbarLeft && T6SDK::Input::MousePosX() <= (float)scrollbarLeft + (float)SCROLLBAR_WIDTH)
							{
								int rows = static_cast<int>(std::ceil(Common::LocalDemos.size() / static_cast<float>(ITEMS_PER_ROW)));
								int contentHeight = rows * (ITEM_HEIGHT + ITEM_MARGIN);

								if (contentHeight > visibleAreaHeight)
								{
									float thumbPosition = static_cast<float>(scrollY) / contentHeight * visibleAreaHeight;
									float thumbHeight = static_cast<float>(visibleAreaHeight) * (visibleAreaHeight / static_cast<float>(contentHeight));
									thumbHeight = max(thumbHeight, 30.0f);

									// Check if click is on the thumb
									if (T6SDK::Input::MousePosY() >= (float)viewportY + (float)thumbPosition && T6SDK::Input::MousePosY() <= (float)viewportY + thumbPosition + thumbHeight)
									{
										isDraggingScrollbar = true;
										dragStartY = T6SDK::Input::MousePosY();
										dragStartScrollY = scrollY;
									}
								}
							}
						}
					}
					else
					{
						scrollbarClicked = false;
					}

				}
				else
				{
					scrollbarHovered = false;
					scrollbarClicked = false;
				}
			}

			// Calculate scrollbar thumb
			float thumbHeight = static_cast<float>(viewportHeight) * (viewportHeight / static_cast<float>(contentHeight));
			thumbHeight = max(thumbHeight, 30.0f);

			float thumbPosition = static_cast<float>(scrollY) / contentHeight * viewportHeight;

			// Draw scrollbar thumb (different color when dragging)
			if(T6SDK::Drawing::DrawRectAbsolute((float)scrollbarX, (float)viewportY + thumbPosition, (float)SCROLLBAR_WIDTH, thumbHeight, (isDraggingScrollbar || scrollBarThumbHovered) ? T6SDK::Drawing::WHITECOLOR : T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, &scrollBarThumbRect))
			{
				if (T6SDK::Input::MousePosX() > (float)scrollBarThumbRect.left && T6SDK::Input::MousePosX() < (float)scrollBarThumbRect.right && T6SDK::Input::MousePosY() > (float)scrollBarThumbRect.top && T6SDK::Input::MousePosY() < (float)scrollBarThumbRect.bottom)
				{
					scrollBarThumbHovered = true;
					if (T6SDK::Input::Keys::LMB.IsAnyPressState())
					{
						scrollBarThumbClicked = true;
						isDraggingScrollbar = true;
					}
				}
				else
				{
					scrollBarThumbHovered = false;
				}
				if (!T6SDK::Input::Keys::LMB.IsAnyPressState())
				{
					isDraggingScrollbar = false;
					scrollBarThumbClicked = false;
				}
			}
		}
	}

	void handle_scroll(int delta) 
	{
		int rows = static_cast<int>(std::ceil(Common::LocalDemos.size() / static_cast<float>(ITEMS_PER_ROW)));
		int contentHeight = rows * (ITEM_HEIGHT + ITEM_MARGIN)+100;
		int maxScroll = contentHeight - visibleAreaHeight;

		scrollY += delta;
		scrollY = max(0, min(scrollY, maxScroll));
	}
	void OnMouseWheelDown()
	{
		handle_scroll(150);
	}
	void OnMouseWheelUp()
	{
		handle_scroll(-150);
	}

	static void Close()
	{
		T6SDK::InternalFunctions::UI_CloseAll();
		T6SDK::InternalFunctions::Key_SetCatcher(16);
		isShown = false;
	}

	void playDemo()
	{
		if (std::filesystem::exists(Common::LocalDemos[selectedDemoNumber].DemoPath.c_str()))
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
		}
		else
		{
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, false, "DEMO", "Selected demo file does not or no longer exist!");
			T6SDK::MAIN::UI_ShowNotification("DEMO", "Demo file does not exist!", 200);
		}
	}

	void OnKeyPressed(BYTE key)
	{
		if (T6SDK::Theater::IsInTheater())
			return;
		if(key == T6SDK::Input::Keys::ESCAPE.KeyCode)
		{
			Close();
		}
		if(key == T6SDK::Input::Keys::ENTER.KeyCode)
			playDemo();
		else if (key == T6SDK::Input::Keys::UP.KeyCode)
		{
			if (selectedDemoNumber >= 3)
			{
				selectedDemoNumber -= 3;
				handle_scroll(-50);
			}

		}
		else if (key == T6SDK::Input::Keys::DOWN.KeyCode)
		{
			int allowedSize = Common::LocalDemos.size() - 3;
			if (selectedDemoNumber < allowedSize)
			{
				selectedDemoNumber += 3;
				handle_scroll(50);
			}

		}
		else if(key == T6SDK::Input::Keys::LEFT.KeyCode)
		{
			if(selectedDemoNumber > 0)
				selectedDemoNumber -= 1;
			else
				selectedDemoNumber = Common::LocalDemos.size() - 1;
		}
		else if (key == T6SDK::Input::Keys::RIGHT.KeyCode)
		{
			if (selectedDemoNumber < Common::LocalDemos.size() - 1)
				selectedDemoNumber += 1;
			else
				selectedDemoNumber = 0;
		}
		//T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, false, "DEMO", "LocalDemos size: %i; selectedDemoNumber: %i", Common::LocalDemos.size(), selectedDemoNumber);
	}
	// Call this when mouse is moved (with button down)
	void handle_mouse_drag(int mouseY, int viewportY) 
	{
		if (isDraggingScrollbar) {
			int rows = static_cast<int>(std::ceil(Common::LocalDemos.size() / static_cast<float>(ITEMS_PER_ROW)));
			int contentHeight = rows * (ITEM_HEIGHT + ITEM_MARGIN)+100;
			int maxScroll = contentHeight - visibleAreaHeight;

			// Calculate new scroll position based on drag distance
			float deltaY = mouseY - dragStartY;
			float scrollRatio = deltaY / visibleAreaHeight;
			scrollY = dragStartScrollY + static_cast<int>(scrollRatio * contentHeight);

			// Clamp scroll position
			scrollY = max(0, min(scrollY, maxScroll));
		}
	}

	//End scrollable grid
	

	std::string selectedFolder{};
	void ChangeDemoDirectory()
	{
		// Open the folder dialog
		if (T6SDK::InternalFunctions::OpenFolderDialog(selectedFolder))
		{
			T6SDK::Dvars::SetString(CustomDvars::dvar_demos_directory, selectedFolder.c_str());
			CustomDvars::dvar_demos_directory->modified = true;
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, false, "DEMOBROWSER", "Selected folder: %s", CustomDvars::dvar_demos_directory->current.string);
			Common::LoadAllDemos();
			*UIControls::UI_SelectedDemoNumber.SelectedValue = 0;
			handle_scroll(-900);
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


	bool thubmnailClicked = false;
	bool thumbnailHoversoundPlayed = false;
	float fadingX = 0.0f;
	float* GetFadingColor(tColor color)
	{

		float A = 0.5f;
		float k = 1.6f;
		float w = 0.8f;
		float R = color[0];
		float G = color[1];
		float B = color[2];
		float coef = (A * tanh(k * sin(w * fadingX)) / tanh(k)) + 0.5f;
		float Alpha = 0.5f + (0.5f * coef);
		return tColor{ R, G, B, Alpha };
	}
	void DrawSelectedDemoThumbnail()
	{
		float scale = 1.0f / 1080.0f * static_cast<float>(T6SDK::Addresses::ScreenHeight.Value()); //Assuming that initial size is in 1920x1080			
		float h = (float)T6SDK::Drawing::normalFont->pixelHeight * scale;
		vec2_t coordsStart = T6SDK::Drawing::GetGridCellCoords(11, 13);
		vec2_t coordsEnd = T6SDK::Drawing::GetGridCellCoords(15, 23);
		float rowHeight = (coordsEnd.y - coordsStart.y)/10.0f + 5.0f; //5.0f is a margin
		float WIDTH = coordsEnd.x - coordsStart.x;
		float HEIGHT = WIDTH / 1.78f;
		coordsEnd.y = coordsStart.y + HEIGHT;
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
			
			//Make it dim if hovered
			if (T6SDK::Input::MousePosX() > coordsStart.x && T6SDK::Input::MousePosX() < coordsEnd.x && T6SDK::Input::MousePosY() > coordsStart.y && T6SDK::Input::MousePosY() < coordsEnd.y)
			{
				T6SDK::Drawing::DrawRectAbsolute(coordsStart.x, coordsStart.y, WIDTH, HEIGHT, T6SDK::Drawing::T_BLACKCOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
				T6SDK::Drawing::DrawTextAbsolute("PLAY DEMO", coordsStart.x + WIDTH / 2.0f, coordsStart.y + HEIGHT / 2.0f, 1.5f, T6SDK::Drawing::YELLOWCOLOR, T6SDK::AnchorPoint::Center, 0x00);
				if (!thumbnailHoversoundPlayed)
				{
					T6SDK::InternalFunctions::PlaySound("uin_unlock_window");
					thumbnailHoversoundPlayed = true;
				}
				if (T6SDK::Input::Keys::LMB.IsAnyPressState())
				{
					if (thubmnailClicked == false)
					{
						T6SDK::InternalFunctions::PlaySound("uin_main_pause");
						thubmnailClicked = true;
						playDemo();
					}
				}
				else
					thubmnailClicked = false;
				T6SDK::Drawing::ShowToolTip("Click here to ^5play ^7selected demo!");
			}
			else
			{
				T6SDK::Drawing::DrawTextAbsolute("PLAY DEMO", coordsStart.x + WIDTH / 2.0f, coordsStart.y + HEIGHT / 2.0f, 1.5f, GetFadingColor(T6SDK::Drawing::WHITECOLOR), T6SDK::AnchorPoint::Center, 0x00);
				thubmnailClicked = false;
				thumbnailHoversoundPlayed = false;
			}

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
			T6SDK::Drawing::DrawTextAbsolute(descBuffer, coordsStart.x, coordsEnd.y + rowHeight, 1.0f, T6SDK::Drawing::WHITECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);

			//Drawing creation date
			T6SDK::Drawing::DrawTextAbsolute(T6SDK::InternalFunctions::FormatUnixTime(Common::LocalDemos[selectedDemoNumber].CreateDate).c_str(), coordsStart.x, coordsEnd.y + rowHeight * 3.0f, 1.0f, T6SDK::Drawing::GRAYCOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
			fadingX += 0.05f;
			if (fadingX > 8.0f)
				fadingX = 0.0f;
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
		
		vec2_t scrollableGridStart = T6SDK::Drawing::GetGridCellCoords(1, 1);
		vec2_t scrollableGridEnd = T6SDK::Drawing::GetGridCellCoords(10, 37);
		vec2_t viewportBounds = scrollableGridEnd - scrollableGridStart;
		draw_scrollable_grid(scrollableGridStart.x, scrollableGridStart.y, viewportBounds.x, viewportBounds.y);
		RECT gridRect = { scrollableGridStart.x, scrollableGridStart.y, viewportBounds.x, viewportBounds.y };
		bool mouseIsInsideGrdid = T6SDK::Input::MousePosX() > (float)gridRect.left && T6SDK::Input::MousePosX() < (float)gridRect.right && T6SDK::Input::MousePosY() > (float)gridRect.top && T6SDK::Input::MousePosY() < (float)gridRect.bottom;
		if (!mouseIsInsideGrdid)
			hoveredItem = -1;

		handle_mouse_drag(static_cast<int>(T6SDK::Input::MousePosY()), static_cast<int>(scrollableGridStart.y));

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
		//UIControls::UI_PlayDemoButton.Draw();

		UIControls::UI_NavigateToDemoFile.Draw();

	}
	
	static void Init()
	{
		UIControls::UI_SelectedDemoNumber = T6SDK::Drawing::UI_EnumButton("Demo number", 0, Common::LocalDemos.size()-1, &selectedDemoNumber, 8, 37, T6SDK::AnchorPoint::Center, 0x00);
		UIControls::UI_PlayDemoButton = T6SDK::Drawing::UI_ClickableButton("PLAY DEMO", 13, 14, T6SDK::AnchorPoint::Center, true, (uintptr_t)&playDemo);
		UIControls::UI_CloseDemoSelectMenu = T6SDK::Drawing::UI_ClickableButton("^3ESC ^7Back", 3, 38, T6SDK::AnchorPoint::Center, (uintptr_t)&Close);
		UIControls::UI_NavigateToDemoFile = T6SDK::Drawing::UI_ClickableButton("SHOW IN EXPLORER", 14, 37, T6SDK::AnchorPoint::TopLeft, false, 0x00);
		UIControls::UI_NavigateToDemoFile.Size = 1.0f;
		UIControls::UI_DemosDirectoryButton = T6SDK::Drawing::UI_ClickableButton("CHANGE FOLDER", 14, 35, T6SDK::AnchorPoint::TopLeft, (uintptr_t)&ChangeDemoDirectory);
		UIControls::UI_DemosDirectoryButton.ToolTip = "Select the directory where demos are stored.";
		UIControls::UI_DemosDirectoryButton.Size = 1.0f;
	}
	static void Show()
	{
		CalculateConstants();
		T6SDK::InternalFunctions::Key_SetCatcher(15);
		Init();
		isShown = true;
	}
}