#pragma once
#include "StdInclude.h"
#include <algorithm>
#include "WeaponAnimations.h"
namespace Weapon
{
	template<typename Map>
	auto get_element_by_index(const Map& m, size_t index) -> typename Map::const_iterator 
	{
		if (index >= m.size()) {
			return m.end();
		}
		auto it = m.begin();
		std::advance(it, index);
		return it;
	}
	class UI_WeapAnimListBox
	{
		typedef void func(int);
	private:
		bool Hovered = false;
		bool Clicked = false;
		bool Pressed = false;
		bool IsEnabled = true;
		uintptr_t Function = 0;
		T6SDK::AnchorPoint anchorPoint{};
		RECT bgRect{};
		float X = 0.0f;
		float Y = 0.0f;
		float m_width = 0.0f;
		float m_height = 0.0f;
		bool DrawRelative = false;
		int GridColumn = -1;
		int GridRow = -1;
		int GridColumn2 = -1;
		int GridRow2 = -1;

		float m_scrollY = 0.0f;
		float m_scrollbarWidth = 10.0f;
		bool m_isDraggingScrollbar = false;
		int InternalSelected = 0;

	public:
		int* SelectedIndex = &InternalSelected;
		int* SelectedWeap = 0;
		const char* Title{};

		uintptr_t OnSelectedItemChanged = 0;
		/// <summary>
		/// Draw a list box with a title 
		/// </summary>
		/// <param name="title"></param>
		/// <param name="x">Horizontal position in pixels, otherwise if drawRelative is true, this is relative to the screen size [0.0f - 1.0f].</param>
		/// <param name="y">Vertical position in pixels, otherwise if drawRelative is true, this is relative to the screen size [0.0f - 1.0f].</param>
		/// <param name="drawRelative">Describes will x and y values will be treated as relative to the screen size.</param>
		/// <param name="items">Pointer to a list of strings that will be displayed in the list box.</param>
		/// <param name="anchorPoint"></param>
		/// <param name="function">Pointer to a function that will be called when the selected item is changed. void func(int)</param>
		UI_WeapAnimListBox(const char* title, float x, float y, float width, float height, bool drawRelative, int* selectedIndex, T6SDK::AnchorPoint anchorPoint, uintptr_t function)
		{
			InternalSelected = 0;
			Title = title;
			X = x;
			Y = y;
			m_width = width;
			m_height = height;
			Function = function;
			this->anchorPoint = anchorPoint;
			DrawRelative = drawRelative;
			if (selectedIndex)
				SelectedIndex = selectedIndex;
		}
		/// <summary>
		/// Draw a list box with a title 
		/// </summary>
		/// <param name="title"></param>
		/// <param name="gridColumn">Number of one of 16 columns to take X position from.</param>
		/// <param name="gridRow">Number of one of 40 rows to take Y position from.</param>
		/// <param name="items">Pointer to a list of strings that will be displayed in the list box.</param>
		/// <param name="anchorPoint"></param>
		/// <param name="function">Pointer to a function that will be called when the selected item is changed. void func(int)</param>
		UI_WeapAnimListBox(const char* title, int gridColumn, int gridRow, float width, float height, int* selectedIndex, int* selectedWeap, T6SDK::AnchorPoint anchorPoint, uintptr_t function)
		{
			Title = title;
			GridColumn = gridColumn;
			GridRow = gridRow;
			m_width = (float)width;
			m_height = (float)height;
			Function = function;
			this->anchorPoint = anchorPoint;
			this->InternalSelected = 0;
			if (selectedIndex)
				SelectedIndex = selectedIndex;
			if (selectedWeap)
				SelectedWeap = selectedWeap;
		}
		UI_WeapAnimListBox(const char* title, int gridColumn, int gridRow, int gridColumn2, int gridRow2, int* selectedIndex, int* selectedWeap, T6SDK::AnchorPoint anchorPoint, uintptr_t function)
		{
			Title = title;
			GridColumn = gridColumn;
			GridRow = gridRow;
			GridColumn2 = gridColumn2;
			GridRow2 = gridRow2;
			Function = function;
			this->anchorPoint = anchorPoint;
			this->InternalSelected = 0;
			if (selectedIndex)
				SelectedIndex = selectedIndex;
			if (selectedWeap)
				SelectedWeap = selectedWeap;
		}
		UI_WeapAnimListBox()
		{
			this->InternalSelected = 0;
		}

		void Draw()
		{
			if (*SelectedIndex == -1 || WeaponAnimations::Weapons[*SelectedWeap].AnimationsMap.size() == 0)
				return;
			if (!IsEnabled)
			{
				Clicked = false;
				Pressed = false;
			}
			bool successDraw = false;
			if (DrawRelative)
				successDraw = T6SDK::Drawing::DrawRectCoordsRelative(X, Y, m_width, m_height, T6SDK::Drawing::T_BLACKCOLOR, anchorPoint, &bgRect);
			else if (GridColumn != -1 && GridRow != -1 && GridColumn2 != -1 && GridRow2 != -1)
			{
				vec2_t coords = T6SDK::Drawing::GetGridCellCoords(GridColumn, GridRow);
				m_width = (T6SDK::Drawing::GetGridCellCoords(GridColumn2, GridRow2) - coords).x;
				m_height = (T6SDK::Drawing::GetGridCellCoords(GridColumn2, GridRow2) - coords).y;
				successDraw = T6SDK::Drawing::DrawRectAbsolute(coords.x, coords.y, m_width, m_height, T6SDK::Drawing::T_BLACKCOLOR, anchorPoint, &bgRect);
			}
			else
				successDraw = T6SDK::Drawing::DrawRectAbsolute(X, Y, m_width, m_height, T6SDK::Drawing::T_BLACKCOLOR, anchorPoint, &bgRect);

			if (!successDraw)
				return;
			float scale = 1.0f / 1080.0f * static_cast<float>(T6SDK::Addresses::ScreenHeight.Value());

			// Calculate the number of visible items
			float itemHeight = (float)T6SDK::Drawing::normalFont->pixelHeight * scale;
			int visibleItemCount = m_height / itemHeight;

			// Adjust the visible items based on the scroll position
			int startIndex = m_scrollY / itemHeight;
			int endIndex = min(startIndex + visibleItemCount, static_cast<int>(WeaponAnimations::Weapons[*SelectedWeap].AnimationsMap.size()));

			// Draw the visible items
			for (int i = startIndex; i < endIndex; ++i)
			{
				int itemY = bgRect.top + (i - startIndex) * itemHeight;

				// Highlight the selected item
				if (i == *SelectedIndex)
				{
					T6SDK::Drawing::DrawRectAbsolute(bgRect.left, itemY, m_width - m_scrollbarWidth, itemHeight, T6SDK::Drawing::T_WHITECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
				}
				RECT itemRect{ bgRect.left, itemY, bgRect.left + m_width - m_scrollbarWidth, itemY + itemHeight };
				Hovered = false;
				if (T6SDK::Input::MousePosX() > (float)itemRect.left && T6SDK::Input::MousePosX() < (float)itemRect.right && T6SDK::Input::MousePosY() > (float)itemRect.top && T6SDK::Input::MousePosY() < (float)itemRect.bottom)
				{
					if (IsEnabled)
						Hovered = true;
				}
				// Draw the text
				RECT textRect{};
				auto anim = get_element_by_index(WeaponAnimations::Weapons[*SelectedWeap].AnimationsMap, i);
				const char* animName = anim->second.Name;
				T6SDK::Drawing::DrawTextAbsolute(animName, bgRect.left + 5, itemY + 0, 1.0f, !IsEnabled ? T6SDK::Drawing::GRAYCOLOR : Hovered ? T6SDK::Drawing::ORANGECOLOR : anim->second.Replaced ? T6SDK::Drawing::GREENCOLOR : T6SDK::Drawing::WHITECOLOR, T6SDK::AnchorPoint::TopLeft, &textRect);
			}
			// Draw the scrollbar
			drawScrollbar(&bgRect);

			//Handle input
			if (!IsEnabled)
				return;
			if (T6SDK::Input::Keys::LMB.Pressed())
				handleMouseClick(T6SDK::Input::MousePosX(), T6SDK::Input::MousePosY(), &bgRect);
			handleMouseDrag(T6SDK::Input::MousePosY(), &bgRect);
			if (!T6SDK::Input::Keys::LMB.IsAnyPressState())
				handleMouseRelease();
		}//

		void Draw(bool isEnabled)
		{
			IsEnabled = isEnabled;
			Draw();
		}
		void handleMouseClick(float mouseX, float mouseY, RECT* rect)
		{
			// Check if the click is on the scrollbar
			if (mouseX >= (float)((float)rect->left + m_width - m_scrollbarWidth) && mouseX <= (float)(rect->left) + m_width)
			{
				m_isDraggingScrollbar = true;
				updateScrollPositionFromMouse(mouseY, rect);
			}
			else
			{
				if (mouseX >= (float)rect->left && mouseX <= (float)(rect->left + m_width - m_scrollbarWidth))
				{
					// Handle item selection
					float scale = 1.0f / 1080.0f * static_cast<float>(T6SDK::Addresses::ScreenHeight.Value());
					float itemHeight = (float)T6SDK::Drawing::normalFont->pixelHeight * scale;
					int startIndex = m_scrollY / itemHeight;
					int clickedIndex = startIndex + (mouseY - (float)rect->top) / itemHeight;

					if (clickedIndex >= 0 && clickedIndex < WeaponAnimations::Weapons[*SelectedWeap].AnimationsMap.size())
					{
						*SelectedIndex = clickedIndex;
						if (OnSelectedItemChanged != 0)
						{
							func* f = (func*)OnSelectedItemChanged;
							if (f)
								f(*SelectedIndex);
						}
					}
				}
				else
					return;
			}
		}

		void handleMouseDrag(float mouseY, RECT* rect)
		{
			if (m_isDraggingScrollbar)
			{
				updateScrollPositionFromMouse(mouseY, rect);
			}
		}

		void handleMouseRelease()
		{
			m_isDraggingScrollbar = false;
		}

		// Draw the scrollbar
		void drawScrollbar(RECT* rect)
		{
			// Draw the scrollbar background
			T6SDK::Drawing::DrawRectAbsolute((float)rect->left + m_width - m_scrollbarWidth, (float)rect->top, m_scrollbarWidth, m_height, T6SDK::Drawing::T_BLACKCOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);

			// Calculate the scrollbar handle size and position
			float scale = 1.0f / 1080.0f * static_cast<float>(T6SDK::Addresses::ScreenHeight.Value());
			float itemHeight = (float)T6SDK::Drawing::normalFont->pixelHeight * scale;
			int totalContentHeight = WeaponAnimations::Weapons.size() * itemHeight; // Total height of all items
			int scrollbarHandleHeight = (int)(m_height * m_height) / totalContentHeight;
			scrollbarHandleHeight = min(m_height, max(scrollbarHandleHeight, 10)); // Minimum handle height

			int scrollbarHandleY = (float)rect->top + (m_scrollY * (m_height - scrollbarHandleHeight)) / (totalContentHeight - m_height);
			if ((float)scrollbarHandleHeight == m_height)
				return;
			// Draw the scrollbar handle
			T6SDK::Drawing::DrawRectAbsolute((float)rect->left + m_width - m_scrollbarWidth, (float)scrollbarHandleY, m_scrollbarWidth, (float)scrollbarHandleHeight, T6SDK::Drawing::T_WHITECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		}

		// Update the scroll position based on mouse Y position
		void updateScrollPositionFromMouse(int mouseY, RECT* rect)
		{
			float scale = 1.0f / 1080.0f * static_cast<float>(T6SDK::Addresses::ScreenHeight.Value());
			float itemHeight = (float)T6SDK::Drawing::normalFont->pixelHeight * scale;
			int totalContentHeight = WeaponAnimations::Weapons.size() * itemHeight; // Total height of all items
			int scrollbarHandleHeight = (m_height * m_height) / totalContentHeight;
			scrollbarHandleHeight = min(m_height, max(scrollbarHandleHeight, 10)); // Minimum handle height

			// Calculate the new scroll position
			m_scrollY = ((mouseY - (float)rect->top) * ((float)totalContentHeight - m_height)) / (m_height - (float)scrollbarHandleHeight);
			m_scrollY = max(0, min(m_scrollY, (float)totalContentHeight - m_height));
		}
	};
}