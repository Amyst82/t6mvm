#pragma once
#include "StdInclude.h"
#include "../../../../Downloads/json.hpp"
#include "CustomDvars.h"
namespace Settings
{
	class Settings
	{
	private:
		inline static std::string SettingsPath = std::string("\\Plugins\\t6mvm.json");
	public:
		//properties
		inline static std::string AlertzyKey{};
		inline static std::string DemosDirectoryMp{};
		inline static std::string DemosDirectoryZm{};
		inline static std::map<std::string, int> KeyBinds
		{ 
			{std::string("CameraForward"), 0x76}, //Default W
			{std::string("CameraBack"), 0x72}, //Default S
			{std::string("CameraLeft"), 0x60}, //Default A
			{std::string("CameraRight"), 0x63}, //Default D
			{std::string("CameraUp"), 0x64}, //Default E
			{std::string("CameraDown"), 0x71}, //Default Q
			{std::string("GotoFirstMarker"), 0x6C}, //Default M
			{std::string("StreamsStartStop"), 0xAA}, //Default F5
			{std::string("StreamsAbort"), 0xAB} //Default F6
		};
		inline static std::map<std::string, bool> PostStreamsActions
		{ 
			{std::string("Notification"), false}, 
			{std::string("OpenStreamsFolder"), true} 
		};
		inline static std::string StreamsDirectory{};

		//methods
		static void Load();
		static void Save();
		static void SetDemosDirectory(std::string& directory);
		static std::string GetDemosDirectory();
		static void SetStreamsDirectory(std::string& directory);
		static void SetKeyBind(std::string& key, int& bind);
		static void SetPostStreamsAction(std::string& action, bool& value);

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
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(Settings, AlertzyKey, DemosDirectoryMp, DemosDirectoryZm, KeyBinds, PostStreamsActions, StreamsDirectory)
	};
}