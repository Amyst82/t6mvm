#pragma once
#include "StdInclude.h"
#include "UIControls.h"

namespace Common
{
	typedef class CustomDemoBookmark
	{
	public:
		int playerNum;
		int tick;
		std::string description;
	};
	inline static std::vector<CustomDemoBookmark> CustomBookmarks{};
	inline static T6SDK::Drawing::UI_TextBoxDialog UI_BookmarkDialog{};
	inline static std::string LoadedDemoTagsPath{};
	static void AddBookmark(int tick)
	{
		bool ShouldBeAdded = false;
		if (Common::CustomBookmarks.size() == 0)
		{
			ShouldBeAdded = true;
		}
		else
		{
			bool found = false;
			for (int i = 0; i < Common::CustomBookmarks.size(); i++)
			{
				if (Common::CustomBookmarks[i].tick == tick)
				{
					T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_WARNING, true, "UI_Timeline", "Bookmark already exists at %02i:%02i (%i)", tick / 60000, tick % 60000 / 1000, tick);
					T6SDK::MAIN::UI_ShowNotification("Timeline", "^3Bookmark already exists at this tick.", 200);
					found = true;
					return;
				}
			}
			if (!found)
			{
				ShouldBeAdded = true;
				found = false;
			}
		}
		if (ShouldBeAdded)
		{
			UI_BookmarkDialog.Show([](std::string e) {
				T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "UI_Timeline", "Bookmark description: %s", e.c_str());
				Common::CustomDemoBookmark bookMark{};
				bookMark.playerNum = T6SDK::Dvars::GetInt(*T6SDK::Dvars::DvarList::demo_client);
				int tick = T6SDK::Addresses::cg->Tick;
				bookMark.tick = tick;
				bookMark.description = std::string(e);
				Common::CustomBookmarks.push_back(bookMark);
				T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "UI_Timeline", "Custom bookmark added at %02i:%02i (%i)", tick / 60000, tick % 60000 / 1000, tick);
				T6SDK::MAIN::UI_ShowNotification("Timeline", "Custom bookmark added", 200);
			});
		}
	}
}