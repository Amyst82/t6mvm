#pragma once
#include "StdInclude.h"
#include "LocalAddresses.h"
namespace UI_TimelineNS
{
	class UI_Timeline
	{
		typedef void func(int);
	private:
		bool Hovered = false;
		bool Pressed = false;
		bool Clicked = false;
		bool IsEnabled = true;
		bool MouseHold = false;
		uintptr_t Function = 0;
		T6SDK::AnchorPoint anchorPoint{};
		bool DrawRelative = false;
		int GridColumn = -1;
		int GridRow = -1;
		RECT baseRect{};
		RECT sliderRect{};
		RECT textRect{};
		float X = 0.0f;
		float Y = 0.0f;
		float* clr;
		float GetCoordByValue(int value, float x, float y, float width, int min, int max)
		{
			float res = 0.0f;
			float coef = ((float)max - (float)min) / width;
			res = (((float)value - (float)min) / coef) + x;
			return res;
		}
		float defaultValue = 0.0f;
		int internalDefaultValue = 11000;
		int internalValue = 0;

		float WIDTH = 750.0f;
		float HEIGHT = 30.0f;
		const char* Text{};


	public:
		int* value = &internalValue;
		int Min = 0.0f;
		int Max = 1.0f;
		const char* ToolTip = "";
		void Reset()
		{
			*value = internalDefaultValue;
			defaultValue = GetCoordByValue(internalDefaultValue, X, Y, WIDTH, Min, Max);
		}

	public:
		UI_Timeline()
		{

		}
		/// <summary>
		/// Slider.
		/// </summary>
		/// <param name="text">Text to be displayed in format "Text: %.3f".</param>
		/// <param name="_value">Pointer to a float value.</param>
		/// <param name="_defaultValue">Default value to reset the slider to.</param>
		/// <param name="min">Minimum value.</param>
		/// <param name="max">Maximum value.</param>
		/// <param name="x">Horizontal position in pixels, otherwise if drawRelative is true, this is relative to the screen size [0.0f - 1.0f].</param>
		/// <param name="y">Vertical position in pixels, otherwise if drawRelative is true, this is relative to the screen size [0.0f - 1.0f].</param>
		/// <param name="drawRelative">Describes will x and y values will be treated as relative to the screen size.</param>
		/// <param name="color">tColor struct with four [0.0 - 1.0] values</param>
		/// <param name="anchorPoint"></param>
		/// <param name="function">Function to be called when the slider is dragged.</param>
		UI_Timeline(int max, float x, float y, bool drawRelative, tColor color, T6SDK::AnchorPoint anchorPoint)
		{
			X = x;
			Y = y;
			this->anchorPoint = anchorPoint;
			value = &T6SDK::Addresses::cg->Tick;
			Min = 11000;
			Max = max;
			clr = color;
			internalDefaultValue = 11000;
			defaultValue = GetCoordByValue(T6SDK::Addresses::cg->activeSnapshots[0].serverTime, X, Y, WIDTH, Min, max);
			Text = "00:00/00:00";
			DrawRelative = drawRelative;
		}
		/// <summary>
		/// Slider.
		/// </summary>
		/// <param name="text">Text to be displayed in format "Text: %.3f".</param>
		/// <param name="_value">Pointer to a float value.</param>
		/// <param name="_defaultValue">Default value to reset the slider to.</param>
		/// <param name="min">Minimum value.</param>
		/// <param name="max">Maximum value.</param>
		/// <param name="gridColumn">Number of one of 8 columns that the slider will take X position from.</param>
		/// <param name="gridRow">Number of one of 21 rows that the slider will take Y position from.</param>
		/// <param name="color">tColor struct with four [0.0 - 1.0] values</param>
		/// <param name="anchorPoint"></param>
		/// <param name="function">Function to be called when the slider is dragged.</param>
		UI_Timeline(int max, int GridColumn, int GridRow, tColor color, T6SDK::AnchorPoint anchorPoint)
		{
			this->anchorPoint = anchorPoint;
			value = &T6SDK::Addresses::cg->Tick;
			Min = 11000;
			Max = max;
			clr = color;
			internalDefaultValue = 11000;
			defaultValue = GetCoordByValue(T6SDK::Addresses::cg->activeSnapshots[0].serverTime, X, Y, WIDTH, Min, max);
			Text = "00:00/00:00";
			this->GridColumn = GridColumn;
			this->GridRow = GridRow;
		}
		bool gameUnpaused = false;
		void Draw()
		{
			if(gameUnpaused)
			{
				T6SDK::Addresses::IsDemoPaused.SetValueSafe(1);
				gameUnpaused = false;
			}
			if (!T6SDK::Drawing::CheckResources())
				return;
			float scale2 = 1.0f / 720.0f * static_cast<float>(T6SDK::Addresses::ScreenHeight.Value());
			//Getting the base rect bounds (combined rect for text + slider underneath)
			bool successDraw = false;
			if (DrawRelative)
			{
				float scale = 1.0f / 1080.0f * static_cast<float>(T6SDK::Addresses::ScreenHeight.Value()); //Assuming that initial size is in 1920x1080			
				float h = (float)T6SDK::Drawing::normalFont->pixelHeight * scale;
				successDraw = T6SDK::Drawing::DrawRectCoordsRelative(X, Y, WIDTH * scale2, 2.0f + HEIGHT + h, T6SDK::Drawing::TRANSPARENTCOLOR, anchorPoint, &baseRect);
			}
			else if (GridColumn != -1 && GridRow != -1)
			{
				float scale = 1.0f / 1080.0f * static_cast<float>(T6SDK::Addresses::ScreenHeight.Value()); //Assuming that initial size is in 1920x1080			
				float h = (float)T6SDK::Drawing::normalFont->pixelHeight * scale;
				vec2_t coords = T6SDK::Drawing::GetGridCellCoords(GridColumn, GridRow);
				successDraw = T6SDK::Drawing::DrawRectAbsolute(coords.x, coords.y, WIDTH*scale2, 2.0f + HEIGHT + h, T6SDK::Drawing::TRANSPARENTCOLOR, anchorPoint, &baseRect);
			}
			else
			{
				float scale = 1.0f / 1080.0f * static_cast<float>(T6SDK::Addresses::ScreenHeight.Value()); //Assuming that initial size is in 1920x1080			
				float h = (float)T6SDK::Drawing::normalFont->pixelHeight * scale;
				vec2_t coords = T6SDK::Drawing::GetGridCellCoords(GridColumn, GridRow);
				successDraw = T6SDK::Drawing::DrawRectAbsolute(X, Y, WIDTH * scale2, 2.0f + HEIGHT + h, T6SDK::Drawing::TRANSPARENTCOLOR, anchorPoint, &baseRect);
			}
			//If we got the common bounds
			if (successDraw)
			{
#pragma region Slider text

				tColor* color{};
				if (Pressed)
					color = &T6SDK::Drawing::ORANGECOLOR;
				else if (Hovered)
					color = &T6SDK::Drawing::YELLOWCOLOR;
				else if (!IsEnabled)
					color = &T6SDK::Drawing::GRAYCOLOR;
				else
					color = &T6SDK::Drawing::WHITECOLOR;

				//Draw the slider text
				char buffer[100];
				sprintf_s(buffer, 100, "%02i:%02i/%02i:%02i", T6SDK::Addresses::cg->Tick / 60000, T6SDK::Addresses::cg->Tick % 60000 / 1000, Max / 60000, Max % 60000 / 1000);
				if (T6SDK::Drawing::DrawTextAbsolute(buffer, (float)baseRect.left, (float)baseRect.top, 1.0f, *color, T6SDK::AnchorPoint::TopLeft, &textRect))
				{
					if (IsEnabled && T6SDK::Input::MousePosX() > (float)textRect.left && T6SDK::Input::MousePosX() < (float)textRect.right && T6SDK::Input::MousePosY() > (float)textRect.top && T6SDK::Input::MousePosY() < (float)textRect.bottom)
					{
						//T6SDK::ConsoleLog::Log("Hovered");
						Hovered = true;
						if (T6SDK::Input::Keys::LMB.IsAnyPressState() && MouseHold == false)
						{
							Pressed = true;
							if (Clicked == false)
							{
								//T6SDK::ConsoleLog::Log("Clicked!");
								Clicked = true;
								//Reset();
								//TODO ADD BOOKMARK
							}
						}
						else
						{
							Pressed = false;
							Clicked = false;
						}
						char buffer[128];
						sprintf(buffer, "Click to add a bookmark at tick: %i", *value);
						T6SDK::Drawing::ShowToolTip(buffer);
					}
					else
					{
						Hovered = false;
						Pressed = false;
						Clicked = false;
					}
				}

#pragma endregion
				//Drawing the slider
				if (T6SDK::Drawing::DrawRectAbsolute((float)baseRect.left, (float)baseRect.bottom - HEIGHT, WIDTH * scale2, HEIGHT, T6SDK::Drawing::T_BLACKCOLOR, T6SDK::AnchorPoint::TopLeft, &sliderRect)) //drawing the background of slider
				{
					defaultValue = GetCoordByValue(*value, (float)sliderRect.left, (float)sliderRect.top, WIDTH * scale2, Min, Max);
					float clickx = defaultValue - T6SDK::Input::MousePosX();
					float moveto = defaultValue - clickx;
					if (T6SDK::Input::MousePosX() > (float)sliderRect.left && T6SDK::Input::MousePosX() < (float)sliderRect.right && T6SDK::Input::MousePosY() > (float)sliderRect.top && T6SDK::Input::MousePosY() < (float)sliderRect.bottom)
					{
						if (T6SDK::Input::Keys::LMB.IsAnyPressState())
						{
							MouseHold = true;
						}
						if (strlen(ToolTip) > 1)
						{
							T6SDK::Drawing::ShowToolTip(ToolTip);
						}
					}
					else
					{
						MouseHold = false;
					}
					if (!T6SDK::Input::Keys::LMB.IsAnyPressState())
					{
						if (MouseHold == true && IsEnabled)
						{
							//RELEASED
							defaultValue = moveto;
							float coef = ((float)Max - (float)Min) / (WIDTH * scale2);
							if ((int)((float)Min + coef * (defaultValue - (float)sliderRect.left)) < 11000)
							{
								*value = 11000;
							}
							else
							{
								*value = (int)((float)Min + coef * (defaultValue - (float)sliderRect.left));
							}

							T6SDK::Theater::GoToTick(*value);

							T6SDK::Addresses::IsDemoPaused.SetValueSafe(0);
							gameUnpaused = true;
							//T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "UI_Timeline", "Released!");
							//T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "UI_Timeline", "value set: %i", *value);
						}

						MouseHold = false;
					}
					if (MouseHold == true && IsEnabled)
					{
						defaultValue = moveto;
						float coef = ((float)Max - (float)Min) / (WIDTH * scale2);
						int val = (int)((float)Min + coef * (defaultValue - (float)sliderRect.left));
						char buffer1[100];
						sprintf_s(buffer1, 100, "%02i:%02i (%i)", val / 60000, val % 60000 / 1000, val);
						T6SDK::Drawing::DrawTextAbsolute(buffer1, defaultValue, (float)baseRect.top-10.0f, 1.0f, *color, T6SDK::AnchorPoint::Center, 0x00);
						//T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "UI_Timeline", "Tick set!");

					}
					if (defaultValue < (float)sliderRect.left + 5.0f)
					{
						defaultValue = (float)sliderRect.left;
						*value = Min;
					}
					if (defaultValue > (float)sliderRect.right - 5.0f)
					{
						defaultValue = (float)sliderRect.right;
						*value = Max;
					}
					//Drawing the slider
					T6SDK::Drawing::DrawRectAbsolute((float)sliderRect.left, (float)sliderRect.top, defaultValue - (float)sliderRect.left, HEIGHT, !IsEnabled ? T6SDK::Drawing::GRAYCOLOR : T6SDK::Drawing::GRAYCOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);

					//Draw kills and deaths bookmarks
					int kdBookmarkType = 0;
					int kdBookmarkTime = 0;
					vec4_t kdBookmarkColor{};
					int KDbookmarkIndex = -1;
					for (int i = -1; i < T6SDK::Addresses::NumDefaultBookmarks.Value(); i++)
					{
						KDbookmarkIndex = T6SDK::InternalFunctions::Demo_GetNextDefaultBookmarkForPlayer(i, &kdBookmarkType, &kdBookmarkTime, &kdBookmarkColor);
						if (KDbookmarkIndex > -1)
						{
							T6SDK::Drawing::DrawRectAbsolute(GetCoordByValue(kdBookmarkTime, (float)sliderRect.left, (float)sliderRect.top, (WIDTH * scale2), Min, Max), (float)sliderRect.top, 2.0f, HEIGHT, (float*)&kdBookmarkColor, T6SDK::AnchorPoint::TopCenter, 0x00);
						}
					}
					
				}
			}
		}
		void Draw(bool isEnabled)
		{
			IsEnabled = isEnabled;
			Draw();
		}
	};
}