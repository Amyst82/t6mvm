#include "pch.h"
#include "StdInclude.h"
#include "Settings.h"

using json = nlohmann::json;
namespace Settings
{
	void Settings::Save()
	{
		try 
		{
			std::ofstream file(Settings::SettingsPath);
			json j;
			j["AlertzyKey"] = Settings::AlertzyKey;
			j["DemosDirectoryMp"] = Settings::DemosDirectoryMp;
			j["DemosDirectoryZm"] = Settings::DemosDirectoryZm;
			j["KeyBinds"] = Settings::KeyBinds;
			j["PostStreamsAction"] = Settings::PostStreamsActions;
			j["StreamsDirectory"] = Settings::StreamsDirectory;
			file << j.dump(4, 32, false, nlohmann::json::error_handler_t::replace); // Pretty print with 4 spaces
			file.close();
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_SUCCESS, false, "T6MVM SETTINGS", "Settings saved!");
		}
		catch (const std::exception& e) 
		{
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, false, "T6MVM SETTINGS", "Coulnd not save settings.");
		}
	}
	void Settings::Load()
	{
		Settings::SettingsPath = std::string(T6SDK::Dvars::GetString(*T6SDK::Dvars::DvarList::fs_homepath)) + "\\Plugins\\t6mvm.json";
		if (!std::filesystem::exists(Settings::SettingsPath))
		{
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_WARNING, false, "T6MVM SETTINGS", "Settings JSON not found. Creating new file.");
			Save();
		}
		else
		{
			std::ifstream file(Settings::SettingsPath);
			if (!file.is_open())
			{
				T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, false, "T6MVM SETTINGS", "Could not open settings file.");
				return;
			}
			json data = json::parse(file);
			Settings::AlertzyKey = data.value("AlertzyKey", "");
			Settings::DemosDirectoryMp = data.value("DemosDirectoryMp", "");
			Settings::DemosDirectoryZm = data.value("DemosDirectoryZm", "");
			Settings::KeyBinds = data.value("KeyBinds", std::map<std::string, int> {
				{ std::string("CameraForward"), 0x76 }, //Default W
				{ std::string("CameraBack"), 0x72 }, //Default S
				{ std::string("CameraLeft"), 0x60 }, //Default A
				{ std::string("CameraRight"), 0x63 }, //Default D
				{ std::string("CameraUp"), 0x64 }, //Default E
				{ std::string("CameraDown"), 0x71 }, //Default Q
				{ std::string("GotoFirstMarker"), 0x6C }, //Default M
				{ std::string("StreamsStartStop"), 0xAA }, //Default F5
				{ std::string("StreamsAbort"), 0xAB } //Default F6
			});
			Settings::PostStreamsActions = data.value("PostStreamsAction", std::map<std::string, bool>{
				{std::string("Notification"), false},
				{std::string("OpenStreamsFolder"), true}
			});
			Settings::StreamsDirectory = data.value("StreamsDirectory", "");
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_SUCCESS, false, "T6MVM SETTINGS", "Settings loaded!");
		}
	}

	void Settings::SetDemosDirectory(std::string& path)
	{
		T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_INFO, false, "T6MVM SETTINGS", "Setting MP demos directory...");
		if (T6SDK::CrossVersion::GetGameVersion() == T6SDK::CrossVersion::GameVersion::V43)
			Settings::DemosDirectoryMp = path;
		else if (T6SDK::CrossVersion::GetGameVersion() == T6SDK::CrossVersion::GameVersion::V41)
			Settings::DemosDirectoryZm = path;
		Save();
	}
	std::string Settings::GetDemosDirectory()
	{
		if (T6SDK::CrossVersion::GetGameVersion() == T6SDK::CrossVersion::GameVersion::V43)
			return Settings::DemosDirectoryMp;
		else if (T6SDK::CrossVersion::GetGameVersion() == T6SDK::CrossVersion::GameVersion::V41)
			return Settings::DemosDirectoryZm;
	}
	void Settings::SetStreamsDirectory(std::string& path)
	{
		T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_INFO, false, "T6MVM SETTINGS", "Setting streams directory...");
		Settings::StreamsDirectory = path;
		Save();
	}
	void Settings::SetKeyBind(std::string& path, int& key)
	{
		T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_INFO, false, "T6MVM SETTINGS", "Setting keybinds...");
		Settings::KeyBinds[path] = key;
		Save();
	}
	void Settings::SetPostStreamsAction(std::string& action, bool& value)
	{
		T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_INFO, false, "T6MVM SETTINGS", "Setting post streams actions...");
		Settings::PostStreamsActions[action] = value;
		Save();
	}

}