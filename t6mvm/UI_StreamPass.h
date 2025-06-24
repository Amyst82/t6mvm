#pragma once
#include "StdInclude.h"
#include "IMVMStream.h"

namespace UI_StreamPassNS
{
	class UI_StreamPass
	{
	private:
		string Name = "Default";
		string PreviewStringOn = "PREVIEWING...";
		string PreviewStringOff = "PREVIEW";
		string IgnoreReshadeStringOn = "IGNORE RESHADE ON";
		string IgnoreReshadeStringOff = "IGNORE RESHADE OFF";
		IMVMStream* Stream = nullptr;

		T6SDK::AnchorPoint anchorPoint{};
		int GridColumn = -1;
		int GridRow = -1;


		std::vector<IMVMStream*>* StreamsList = nullptr;
		//Properties
		RECT btnRect1{};
		bool hoverSoundPlayed1 = false;
		bool Hovered1 = false;
		bool Pressed1 = false;
		bool Clicked1 = false;

		RECT btnRect2{};
		bool hoverSoundPlayed2 = false;
		bool Hovered2 = false;
		bool Pressed2 = false;
		bool Clicked2 = false;

		RECT btnRect3{};
		bool hoverSoundPlayed3 = false;
		bool Hovered3 = false;
		bool Pressed3 = false;
		bool Clicked3 = false;

	public:

		bool IsEnabled = false;
	public:
		UI_StreamPass()
		{
		}
		
		UI_StreamPass(std::vector<IMVMStream*>* streamsList, IMVMStream* stream, const std::string& name, int gridColumn, int gridRow, T6SDK::AnchorPoint anchorPoint)
		{
			Stream = stream;
			Name = name;
			GridColumn = gridColumn;
			GridRow = gridRow;
			this->anchorPoint = anchorPoint;
			StreamsList = streamsList;
		}

		void DrawCheckBox(bool isEnabled, bool* isChecked, const char* CheckedText, const char* UnCheckedText, const char* ToolTip, float size, int GridColumn, int GridRow, bool* Hovered, bool* Pressed, bool* Clicked, bool* hoverSoundPlayed, RECT* btnRect, T6SDK::AnchorPoint anchorPoint_, std::function<void(bool)> OkFunc)
		{
			if (T6SDK::Input::InputLockedByTextBoxDialog)
				return;
			if (!IsEnabled)
			{
				*Hovered = false;
				*Pressed = false;
				*Clicked = false;
			}
			bool successDraw = false;
			vec2_t coords = T6SDK::Drawing::GetGridCellCoords(GridColumn, GridRow);
			successDraw = T6SDK::Drawing::DrawTextAbsolute(*isChecked ? CheckedText : UnCheckedText, coords.x, coords.y, size, !IsEnabled ? T6SDK::Drawing::GRAYCOLOR : *isChecked ? T6SDK::Drawing::ORANGECOLOR : *Hovered ? T6SDK::Drawing::YELLOWCOLOR : T6SDK::Drawing::WHITECOLOR, anchorPoint_, btnRect);
			if (successDraw && IsEnabled)
			{
				if (T6SDK::Input::MousePosX() > (float)btnRect->left && T6SDK::Input::MousePosX() < (float)btnRect->right && T6SDK::Input::MousePosY() > (float)btnRect->top && T6SDK::Input::MousePosY() < (float)btnRect->bottom)
				{
					//T6SDK::ConsoleLog::Log("Hovered");
					*Hovered = true;
					if (*hoverSoundPlayed == false)
					{
						T6SDK::InternalFunctions::PlaySound("uin_unlock_window");
						*hoverSoundPlayed = true;
					}
					if (T6SDK::Input::Keys::LMB.IsAnyPressState())
					{
						*Pressed = true;
						if (*Clicked == false)
						{
							//T6SDK::ConsoleLog::Log("Clicked!");
							*Clicked = true;
							T6SDK::InternalFunctions::PlaySound("uin_main_pause");
							*isChecked = !*isChecked;
							if (OkFunc)
								OkFunc(*isChecked);

						}
					}
					else
					{
						*Pressed = false;
						*Clicked = false;
					}
					if (strlen(ToolTip) > 1)
					{
						T6SDK::Drawing::ShowToolTip(ToolTip);
					}
				}
				else
				{
					*hoverSoundPlayed = false;

					*Hovered = false;
					*Pressed = false;
					*Clicked = false;
				}
			}
		}

		void Draw()
		{
			//Drawing toggle button
			DrawCheckBox(true, &Stream->toggle->current.enabled, Name.c_str(), Name.c_str(), Stream->toggle->dvarDesc, 1.25f, GridColumn, GridRow, &Hovered1, &Pressed1, &Clicked1, &hoverSoundPlayed1, &btnRect1, T6SDK::AnchorPoint::CenterLeft, 0x00);

			//Drawing preview button
			DrawCheckBox(Stream->toggle->current.enabled , &Stream->preview->current.enabled, PreviewStringOn.c_str(), PreviewStringOff.c_str(), Stream->preview->dvarDesc, 1.0f, GridColumn, GridRow+1, &Hovered2, &Pressed2, &Clicked2, &hoverSoundPlayed2, &btnRect2, T6SDK::AnchorPoint::TopLeft, [&](bool e)
				{
					for (auto& stream : *StreamsList)
					{
						stream->Disable();
						stream->preview->current.enabled = false;
					}
					if (e == true)
					{
						Stream->Enable();
						Stream->preview->current.enabled = true;
					}
				});

			//Drawing ignore reshade button
			DrawCheckBox(Stream->toggle->current.enabled , &Stream->IgnoreReshade->current.enabled, IgnoreReshadeStringOn.c_str(), IgnoreReshadeStringOff.c_str(), Stream->IgnoreReshade->dvarDesc, 1.0f, GridColumn + 1, GridRow + 1, &Hovered3, &Pressed3, &Clicked3, &hoverSoundPlayed3, &btnRect3, T6SDK::AnchorPoint::TopLeft, 0x00);
		}
		void Draw(bool isEnabled)
		{
			IsEnabled = isEnabled;
			Draw();
		}
	};
}