#pragma once
#include "StdInclude.h"
#include "UIControls.h"
#include "Settings.h"
#include "../../../../Downloads/json.hpp"
using json = nlohmann::json;
namespace Common
{
	#pragma region Defs
			typedef class CustomDemoBookmark
			{
			public:
				int player_index;
				std::string player;
				int tick;
				std::string description;
				NLOHMANN_DEFINE_TYPE_INTRUSIVE(CustomDemoBookmark, player_index, player, tick, description)
			};
		#pragma endregion
	#pragma region Helpers
	std::string escapeQuotes(std::string input) 
	{
		size_t pos = 0;
		while ((pos = input.find('"', pos)) != std::string::npos) {
			input.replace(pos, 1, "\\\"");
			pos += 2; // Skip past the inserted backslash
		}
		T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "DEMOMETADATA", "Escapd quuotes: %s", input.c_str());
		return input;
	}
	static std::string repairJson(const std::string& badJson) 
	{
		try 
		{
			auto json = nlohmann::json::parse(badJson); // Throws if invalid
			return json.dump(2); // Reconvert to well-formatted string
		}
		catch (const nlohmann::json::parse_error& e) 
		{
			std::cerr << "JSON error: " << e.what() << std::endl;
			return ""; // Return empty or handle error
		}
	}
#pragma endregion
	inline static std::vector<CustomDemoBookmark> CustomBookmarks{};
	inline static T6SDK::Drawing::UI_TextBoxDialog UI_BookmarkDialog{};
	inline static std::vector<T6SDK::DemoBriefData> LocalDemos{};
	inline static T6SDK::DemoBriefData CurrentLoadedDemo{};
	static void AddBookmark(int tick)
	{
		if (tick <= 0)
			return;
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
				bookMark.player_index = T6SDK::Dvars::GetInt(*T6SDK::Dvars::DvarList::demo_client);
				bookMark.player = std::string(T6SDK::Addresses::cg->client[T6SDK::Dvars::GetInt(*T6SDK::Dvars::DvarList::demo_client)].szName);
				int tick = T6SDK::Addresses::cg->Tick;
				bookMark.tick = tick;
				bookMark.description = std::string(e);
				Common::CustomBookmarks.push_back(bookMark);
				T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "UI_Timeline", "Custom bookmark added at %02i:%02i (%i)", tick / 60000, tick % 60000 / 1000, tick);
				T6SDK::MAIN::UI_ShowNotification("Timeline", "Custom bookmark added", 200);
			});
		}
	}

	static void AddBookmarkFromJson(std::string jsonData)
	{
		try 
		{
			json data = json::parse(repairJson(jsonData));
			//Firstly check if it has metadata from BO2 Console V4
			if (data.contains("start_tick"))
			{
				std::string tick = data.at("start_tick").get<std::string>();		
				T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "DEMOMETADATA", "Metadata from V4. tick = %s int -> %i", tick.c_str(), std::stoi(tick));
				int player_index = 0;
				std::string desc = "^1N/A";
				if (data.contains("player_index"))
				{
					std::string string_player_index = data.at("player_index").get<std::string>();
					T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "DEMOMETADATA", "Metadata from V4. pIndex = %s int -> %i", string_player_index.c_str(), std::stoi(string_player_index));
					player_index = std::stoi(string_player_index);
				}
				if (data.contains("description"))
				{
					desc = data.at("description").get<std::string>();
					T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "DEMOMETADATA", "Metadata from V4. desc = %s", desc.c_str());
				}
				if (std::stoi(tick) > 0)
				{
					T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "DEMOMETADATA", "Start tick above 0. Adding a bookmark...");
					Common::CustomDemoBookmark bkmrk{};
					T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "DEMOMETADATA", "1) Empty bookmark created");
					bkmrk.tick = std::stoi(tick);
					T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "DEMOMETADATA", "2) Tick set");
					//bkmrk.playerNum = player_index;
					T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "DEMOMETADATA", "3) Player index set");
					bkmrk.description = std::string(desc);
					T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "DEMOMETADATA", "4) Desc set! Pushing back...");
					Common::CustomBookmarks.push_back(bkmrk);
					T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "DEMOMETADATA", "Bookmark added! Bookmarks : %i; tick: %i, pplayer %s, desc: %s", Common::CustomBookmarks.size(), bkmrk.tick, bkmrk.player.c_str(), bkmrk.description.c_str());
				}
			}
			//Now check if it has metadata from T6MVM
			else if (data.contains("Bookmarks"))
			{
				CustomBookmarks = data["Bookmarks"].get<std::vector<CustomDemoBookmark>>();
			}
			else
				T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_WARNING, true, "DEMOMETADATA", "Demo custom metadata doesn't contain any known keys!");
		}
		catch (const json::parse_error& e) 
		{
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, true, "DEMOMETADATA", "JSON parse error: %s", e.what());
		}
	}

	static void ExportBookmarksToJson()
	{
		//If the path is not set(e.g.loaded with v4 or from local storage) :
		//1) get the demo name
		//2) look for that demo in local storage
		//2.1) if it's still not found -> ask user to specify .tags file
		//3) check.thumbnail file if there is a create time value
		//3.1) if ot->put this value inside)
		if (Common::CurrentLoadedDemo.Inited)
		{
			//Writing custom bookmarks
			json j;
			j["Bookmarks"] = CustomBookmarks;
			std::string json_str = j.dump(4); // Pretty print with 4-space indentation
			if (T6SDK::DemoHandler::WriteTagsMetadata(Common::CurrentLoadedDemo.DemoPath.c_str(), json_str, 0x19D314BB2FD8D972))
			{
				T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_SUCCESS, true, "DEMOMETADATA", "Bookmarks written to tags file -> %s -> %s", Common::CurrentLoadedDemo.DemoPath.c_str(), json_str.c_str());
				T6SDK::MAIN::UI_ShowNotification("DEMO", "Bookmarks written to tags file!", 200);
			}
			else
			{
				T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, true, "DEMOMETADATA", "Failed to write bookmarks to tags file!");
				T6SDK::MAIN::UI_ShowNotification("DEMO", "^1Failed to write bookmarks to tags file!", 200);
			}
		}
		else
		{
			bool existsInLocalStorage = false;
			//1) Get demo name
			std::string demoFullName(T6SDK::Addresses::DemoName);
			//2) Look for that demo in local storage
			std::string redactedPath{};
			if (T6SDK::CrossVersion::GetGameVersion() == T6SDK::CrossVersion::GameVersion::V43)
				redactedPath = std::string(T6SDK::Dvars::GetString(*T6SDK::Dvars::DvarList::fs_homepath)) + "\\Plugins\\OpenNetStorage\\Cloud\\FILM_MP\\";
			else if (T6SDK::CrossVersion::GetGameVersion() == T6SDK::CrossVersion::GameVersion::V41)
				redactedPath = std::string(T6SDK::Dvars::GetString(*T6SDK::Dvars::DvarList::fs_homepath)) + "\\Plugins\\OpenNetStorage\\Cloud\\FILM_ZM\\";
			if (std::filesystem::exists(redactedPath + demoFullName + ".tags"))
				existsInLocalStorage = true;

			//2.1) If it's still not found -> ask user to specify .tags file
			if (!existsInLocalStorage)
			{
				T6SDK::MAIN::UI_ShowNotification("DEMO", "Couldn't find demo in local storage. Please specify the .tags file manually.", 200);
				//TODO: Open file dialog
			}
			else
			{
				//Write to tags in local storage
				json j;
				j["Bookmarks"] = CustomBookmarks;
				std::string json_str = j.dump(4); // Pretty print with 4-space indentation
				if (T6SDK::DemoHandler::WriteTagsMetadata(std::string(redactedPath + demoFullName + ".tags").c_str(), json_str, 0x19D314BB2FD8D972))
				{
					T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_SUCCESS, true, "DEMOMETADATA", "Bookmarks written to tags file!");
					T6SDK::MAIN::UI_ShowNotification("DEMO", "Bookmarks written to tags file!", 200);
				}
				else
				{
					T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, true, "DEMOMETADATA", "Failed to write bookmarks to tags file!");
					T6SDK::MAIN::UI_ShowNotification("DEMO", "^1Failed to write bookmarks to tags file!", 200);
				}
			}
		}
	}

	static void LoadAllDemos()
	{
		std::string redactedPath;
		if (T6SDK::CrossVersion::GetGameVersion() == T6SDK::CrossVersion::GameVersion::V43)
			redactedPath = Settings::Settings::DemosDirectoryMp;
		else if(T6SDK::CrossVersion::GetGameVersion() == T6SDK::CrossVersion::GameVersion::V41)
			redactedPath = Settings::Settings::DemosDirectoryZm;

		auto list = T6SDK::InternalFunctions::find_files_by_extension(redactedPath, ".demo");
		T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_INFO, false, "T6MVM", "Found demos: %i", list.size());
		Common::LocalDemos.clear();
		for (auto& file : list)
		{
			T6SDK::DemoBriefData data = T6SDK::DemoHandler::GetDemoBriefData(file.string().c_str());
			Common::LocalDemos.push_back(data);
		}
		if(Common::LocalDemos.size() > 0)
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_SUCCESS, false, "T6MVM", "Cached demos: %i / %i", Common::LocalDemos.size(), list.size());
		else
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_WARNING, false, "T6MVM", "No demos were cached!");
	}
}