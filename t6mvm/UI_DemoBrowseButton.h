#pragma once
#include "StdInclude.h"
#include <ocidl.h>

namespace UIDemoBrowseButtonNS
{
	static bool DemoBrowserChecked = false;
	class UI_DemoBrowseButton
	{
		typedef void func(UI_DemoBrowseButton*);
	private:
		float fadingX = 0.0f;
		bool hoverSoundPlayed = false;

		bool Hovered = false;
		bool Pressed = false;
		bool Clicked = false;
		bool IsEnabled = true;
		uintptr_t Function = 0;
		RECT iconRect{};
		RECT baseRect{};
		T6SDK::AnchorPoint anchorPoint{};
		bool DrawRelative = false;
		int GridColumn = -1;
		int GridRow = -1;
		float X = 0.0f;
		float Y = 0.0f;

		float WIDTH = 300.0f;
		float HEIGHT = 60.0f;
		bool internalCheckedValue = false;

	public:
		bool* isChecked = &internalCheckedValue;
		const char* UnCheckedText{};
		const char* CheckedText{};
		bool ValueReadOnly = false;
		const char* ToolTip = "";
		UI_DemoBrowseButton()
		{

		}
		/// <summary>
		/// Check box.
		/// </summary>
		/// <param name="uncheckedtext">Text to display when in unchecked state.</param>
		/// <param name="checkedtext">Text to display when in checked state.</param>
		/// <param name="x">Horizontal position in pixels, otherwise relative to screen size if drawRelative is true.</param>
		/// <param name="y">Vertical position in pixels, otherwise relative to screen size if drawRelative is true.</param>
		/// <param name="drawRelative">Describes will x and y values will be treated as relative to the screen size.</param>
		/// <param name="anchorPoint"></param>
		/// <param name="value">Pointer to a bool value.</param>
		/// <param name="function"> Function to call when checked state is changed. void func(UI_DemoBrowseButton*)</param>

		UI_DemoBrowseButton(float x, float y, bool drawRelative, T6SDK::AnchorPoint anchorPoint)
		{
			UnCheckedText = "BROWSE DEMOS";
			CheckedText = "BROWSING...";
			X = x;
			Y = y;
			this->anchorPoint = anchorPoint;
			isChecked = &DemoBrowserChecked;
			DrawRelative = drawRelative;
		}
		/// <summary>
		/// Check box.
		/// </summary>
		/// <param name="uncheckedtext">Text to display when in unchecked state.</param>
		/// <param name="checkedtext">Text to display when in checked state.</param>
		/// <param name="gridColumn">Number of one of 8 columns that the check box will take X position from.</param>
		/// <param name="gridRow">Number of one of 21 rows that the check box will take Y position from.</param>
		/// <param name="anchorPoint"></param>
		/// <param name="value">Pointer to a bool value.</param>
		/// <param name="function"> Function to call when checked state is changed. void func(UI_DemoBrowseButton*)</param>
		UI_DemoBrowseButton(int gridColumn, int gridRow, T6SDK::AnchorPoint anchorPoint)
		{
			UnCheckedText = "LOAD DEMO FILE";
			CheckedText = "BROWSING...";

			GridColumn = gridColumn;
			GridRow = gridRow;
			this->anchorPoint = anchorPoint;
			isChecked = &DemoBrowserChecked;
		}

		void LoadDemoFromFile()
		{
			char openedFileName[MAX_PATH];
			const TCHAR* FilterSpec = (const TCHAR*)"BO2 Demo (*.DEMO)\0*.DEMO*\0";
			const TCHAR* Title = (const TCHAR*)"Load demo from file";
			if (T6SDK::InternalFunctions::OpenFileDialog((TCHAR*)openedFileName, false, FilterSpec, Title))
			{
				string str(openedFileName);
				ofstream ImportFile(str, ios_base::in);
				if (ImportFile.good())
				{
					ifstream fileopen(str);
					int counter = 0;
					bool successStatus = false;
					if (fileopen.is_open())
					{
						try
						{
							stringstream buffer;
							//buffer << fileopen.rdbuf();
							T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, false, "DEMO", "Demo file opened!");
							successStatus = true;
						}
						catch (exception ex)
						{
							{
								fileopen.close();
								T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, false, "DEMO", "Error during loading demo file!");
								T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_WARNING, false, "DEMO", ex.what());
								successStatus = false;
								*isChecked = false;
								return;
							}

						}
						if (successStatus)
						{
							fileopen.close();
							T6SDK::DemoHandler::LoadDemoFromFile(str.c_str());
							*isChecked = false;
						}
					}
				}
			}
			else
				*isChecked = false;
		}
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
		bool skipFrame = false;
		void Draw()
		{
			if (!IsEnabled)
			{
				Hovered = false;
				Pressed = false;
				Clicked = false;
			}
			bool successDraw = false;
			if (DrawRelative)
			{
				float scale = 1.0f / 1080.0f * static_cast<float>(T6SDK::Addresses::ScreenHeight.Value()); //Assuming that initial size is in 1920x1080			
				float h = (float)T6SDK::Drawing::normalFont->pixelHeight * scale;
				successDraw = T6SDK::Drawing::DrawRectCoordsRelative(X, Y, WIDTH*scale, HEIGHT*scale, T6SDK::Drawing::T_BLACKCOLOR, anchorPoint, &baseRect);
			}
			else if (GridColumn != -1 && GridRow != -1)
			{
				float scale = 1.0f / 1080.0f * static_cast<float>(T6SDK::Addresses::ScreenHeight.Value()); //Assuming that initial size is in 1920x1080			
				float h = (float)T6SDK::Drawing::normalFont->pixelHeight * scale;
				vec2_t coords = T6SDK::Drawing::GetGridCellCoords(GridColumn, GridRow);
				successDraw = T6SDK::Drawing::DrawRectAbsolute(coords.x, coords.y, WIDTH * scale, HEIGHT * scale, T6SDK::Drawing::T_BLACKCOLOR, anchorPoint, &baseRect);
			}
			else
			{
				float scale = 1.0f / 1080.0f * static_cast<float>(T6SDK::Addresses::ScreenHeight.Value()); //Assuming that initial size is in 1920x1080			
				float h = (float)T6SDK::Drawing::normalFont->pixelHeight * scale;
				vec2_t coords = T6SDK::Drawing::GetGridCellCoords(GridColumn, GridRow);
				successDraw = T6SDK::Drawing::DrawRectAbsolute(X, Y, WIDTH * scale, HEIGHT * scale, T6SDK::Drawing::T_BLACKCOLOR, anchorPoint, &baseRect);
			}
			//If we got the common bounds
			if (successDraw)
			{
				//Drawing icon
				if (T6SDK::Drawing::DrawRectAbsolute((float)baseRect.left, (float)baseRect.top, (float)baseRect.bottom - (float)baseRect.top, (float)baseRect.bottom - (float)baseRect.top, *isChecked ? T6SDK::Drawing::ORANGECOLOR : T6SDK::Drawing::REDCOLOR, T6SDK::AnchorPoint::TopLeft, &iconRect))
				{
					float w = (float)baseRect.bottom - (float)baseRect.top;
					vec2_t coords = T6SDK::Drawing::EvaluateAnchorPoint(T6SDK::AnchorPoint::Center, (float)baseRect.left + w/2.0f, (float)baseRect.top + w/2.0f, w, w);
					T6SDK::Typedefs::R_AddCmdDrawStretchPicRotateXYInternal(coords.x+2.0f, coords.y+2.0f, 0.9f*w, 0.9f*w, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, T6SDK::Drawing::WHITECOLOR, T6SDK::InternalFunctions::DB_FindXAssetHeader(T6SDK::XAssetType::MATERIAL, "menu_mp_lobby_icon_film"));
				}
				//Drawing text
				float textX = (float)iconRect.right + (((float)baseRect.right - (float)iconRect.right)/2.0f);
				float textY = (float)baseRect.top + (((float)baseRect.bottom - (float)baseRect.top)/2.0f);
				T6SDK::Drawing::DrawTextAbsolute(*isChecked ? CheckedText : UnCheckedText, textX, textY, 1.2f, !IsEnabled ? T6SDK::Drawing::GRAYCOLOR : *isChecked ? T6SDK::Drawing::ORANGECOLOR : Hovered ? T6SDK::Drawing::YELLOWCOLOR : GetFadingColor(T6SDK::Drawing::WHITECOLOR), T6SDK::AnchorPoint::Center , 0x00);
				//If mouse pointer is inside the whole button
				if (T6SDK::Input::MousePosX() > (float)baseRect.left && T6SDK::Input::MousePosX() < (float)baseRect.right && T6SDK::Input::MousePosY() > (float)baseRect.top && T6SDK::Input::MousePosY() < (float)baseRect.bottom)
				{
					//T6SDK::ConsoleLog::Log("Hovered");
					Hovered = true;
					if (hoverSoundPlayed == false)
					{
						T6SDK::InternalFunctions::PlaySound("uin_unlock_window");
						//T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, false, "CHECKBOX", "Hover sound played!");
						hoverSoundPlayed = true;
					}
					if (T6SDK::Input::Keys::LMB.IsAnyPressState())
					{
						Pressed = true;
						if (Clicked == false)
						{
							//T6SDK::ConsoleLog::Log("Clicked!");
							T6SDK::InternalFunctions::PlaySound("uin_main_pause");
							Clicked = true;
							*isChecked = true;
							return;
						}
					}
					else
					{
						Pressed = false;
						Clicked = false;
					}
					if (strlen(ToolTip) > 1)
					{
						T6SDK::Drawing::ShowToolTip(ToolTip);
					}
				}
				else
				{
					hoverSoundPlayed = false;
					Hovered = false;
					Pressed = false;
					Clicked = false;
				}
				if (*isChecked == true && skipFrame == false)
				{
					skipFrame = true;
					return;
				}
				if(skipFrame)
				{
					LoadDemoFromFile();
					skipFrame = false;
				}
				fadingX += 0.05f;
				if (fadingX > 8.0f)
					fadingX = 0.0f;
			}
		}
		void Draw(bool isEnabled)
		{
			IsEnabled = isEnabled;
			Draw();
		}
	};
}
