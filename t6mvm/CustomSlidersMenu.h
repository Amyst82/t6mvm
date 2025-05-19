#pragma once
#include <StdInclude.h>
#include "UIControls.h"

namespace CustomSlidersMenu
{
	std::string Path = "\\Plugins\\Sliders\\";
	class DvarSliderData
	{
	public:
		std::string friendlyName{};
		T6SDK::Drawing::UI_Slider slider{};
		dvar_s* dvar{};
	};
	std::vector<DvarSliderData> Sliders{};
	const int ITEMS_PER_ROW = 3;
	const int START_COLUMN = 4;
	const int COLUMN_SPACING = 4;
	const int START_ROW = 5;
	const int ROW_SPACING = 3;
	const int MAX_ITEMS = 20;
	inline static T6SDK::Drawing::UI_TextBoxDialog UI_CustomSliderDvarNameDialog{};


	void SetModifiedDvarFlag()
	{
		for (int i = 0; i < Sliders.size(); i++)
		{
			Sliders[i].dvar->modified = true;
		}
	}
	static void AddFoundDvarAsSlider(std::string friendlyName, dvar_s* foundDvar, bool shouldBeenAddedToFolder = false)
	{
		if(shouldBeenAddedToFolder == true)
		{
			std::filesystem::path filePath = Path + std::to_string(Sliders.size() + 1) + ") " + foundDvar->dvarName + ".json";
			// Create directories if they don't exist
			if (!std::filesystem::exists(filePath.parent_path()))
			{
				std::filesystem::create_directories(filePath.parent_path());
			}
			std::ofstream file(filePath);
			if (file)
			{
				//Creating the json file compatible with V4
				json data;
				data["type"] = foundDvar->type;
				data["name"] = foundDvar->dvarName;
				data["friendlyName"] = foundDvar->dvarName;
				data["offset"] = 0;
				data["intMin"] = foundDvar->domain.integer.min;
				data["intMax"] = foundDvar->domain.integer.max;
				data["fMin"] = foundDvar->domain.value.min;
				data["fMax"] = foundDvar->domain.value.max;
				data["intdefaultValue"] = foundDvar->latched.integer;
				data["fDefaultValue"] = foundDvar->latched.value;
				file << data.dump(2, 32, false, nlohmann::json::error_handler_t::replace);
			}
			file.close();
		}

		//Get row number
		int rows = static_cast<int>(floor(Sliders.size() / ITEMS_PER_ROW));
		int row = START_ROW + (rows * ROW_SPACING);
		//Get column number
		int column = START_COLUMN + (COLUMN_SPACING * (Sliders.size() % ITEMS_PER_ROW));
		T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "SLIDERS", "Slider added at column: %i, row: %i; Total sliders: %i", column, row, Sliders.size()+1);

		//Pushing the slider data + dvar info
		DvarSliderData sliderData{};
		sliderData.friendlyName = friendlyName;
		sliderData.slider = T6SDK::Drawing::UI_Slider(sliderData.friendlyName, &foundDvar->current.value,
			foundDvar->latched.value, foundDvar->domain.value.min, foundDvar->domain.value.max,
			column, row, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, (uintptr_t)&SetModifiedDvarFlag);
		sliderData.dvar = foundDvar;
		Sliders.push_back(sliderData);
		T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "SLIDERS", "Dvar: %s with min: %.3f and max: %.3f; Default: %.3f", sliderData.friendlyName.c_str(), foundDvar->domain.value.min, foundDvar->domain.value.max, foundDvar->latched.value);
	}
	static void Refresh()
	{
		Sliders.clear();
		auto list = T6SDK::InternalFunctions::find_files_by_extension(Path, ".json");
		for (int i = 0; i < list.size(); i++)
		{
			std::ifstream file(list[i]);
			if (file)
			{
				json data1 = json::parse(file);
				json data = json::parse(data1.dump(2, 32, false, nlohmann::json::error_handler_t::replace));
				file.close();
				T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "SLIDERS", "Found json file: %s; contents: %s", list[i].string().c_str(), data.dump(4, 32, false, nlohmann::json::error_handler_t::replace).c_str());
				if(data.contains("type") && data.contains("name") && data.contains("friendlyName") && data.contains("offset") && data.contains("intMin") && data.contains("intMax") && data.contains("fMin") && data.contains("fMax") && data.contains("intdefaultValue") && data.contains("fDefaultValue"))
				{
					if (data.at("type").get<int>() != Dvar_Float)
					{
						T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_WARNING, true, "SLIDERS", "This is not a float dvar: %s", list[i].string().c_str());
						continue;
					}
					dvar_s* foundDvar = T6SDK::Dvars::FindDvar(data.at("name").get<std::string>().c_str());
					foundDvar->domain.integer.min = data.at("intMin").get<int>();
					foundDvar->domain.integer.max = data.at("intMax").get<int>();
					foundDvar->domain.value.min = data.at("fMin").get<float>();
					foundDvar->domain.value.max = data.at("fMax").get<float>();
					foundDvar->latched.integer = data.at("intdefaultValue").get<int>();
					foundDvar->latched.value = data.at("fDefaultValue").get<float>();
					AddFoundDvarAsSlider(data.at("friendlyName").get<std::string>(), foundDvar, false);
				}
				else
				{
					T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_WARNING, true, "SLIDERS", "Invalid or incomplete json file: %s", list[i].string().c_str());
				}
			}
			else
			{
				T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, true, "SLIDERS", "File not found: %s", list[i].string().c_str());
			}
		}
	}
	void AddSlider()
	{

		CustomSlidersMenu::UI_CustomSliderDvarNameDialog.Show([](std::string e)
		{
				std::string dvarName = e;
				dvar_s* dvarsFound = T6SDK::Dvars::FindDvar(dvarName.c_str());
				T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "SLIDERS", "Found dvar: 0x%X", dvarsFound);
				if (dvarsFound != 0)
				{
					if (dvarsFound->type != DvarType::Dvar_Float)
					{
						T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_WARNING, true, "SLIDERS", "Dvar cannot be added since it's type is not float.");
						T6SDK::MAIN::UI_ShowNotification("SLIDERS", "Can't add not float dvars", 200);
						return;
					}
					else
					{
						//GOT IT!
						AddFoundDvarAsSlider(std::string(dvarsFound->dvarName), dvarsFound, true);
					}
				}
				else
				{
					T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "SLIDERS", "T6MVM was not able to find this dvar :(");
					T6SDK::MAIN::UI_ShowNotification("SLIDERS", "Could not find dvar", 200);
					return;
				}
		});
	}


	static void Draw()
	{
		if (!*UIControls::SlidersTabButton.isChecked)
			return;
		UIControls::UI_SlidersAddSliderButton.Draw(Sliders.size() < MAX_ITEMS);
		UIControls::UI_SlidersRefreshButton.Draw();
		if (Sliders.size() == 0)
		{
			vec2_t coords = T6SDK::Drawing::GetGridCellCoords(8, 19);
			T6SDK::Drawing::DrawTextAbsolute("NO ^3SLIDERS ^7ADDED YET", coords.x, coords.y, 1.5f, T6SDK::Drawing::WHITECOLOR, T6SDK::AnchorPoint::Center, 0x00);
		}
		for (int i = 0; i < Sliders.size(); i++)
		{
			Sliders[i].slider.Draw();
		}
	}
	static void Init()
	{
		Path = T6SDK::Dvars::GetString(*T6SDK::Dvars::DvarList::fs_homepath) + std::string("\\Plugins\\Sliders\\");
		UIControls::UI_SlidersAddSliderButton = T6SDK::Drawing::UI_ClickableButton("ADD CUSTOM SLIDER", 8, 36, T6SDK::AnchorPoint::Center, (uintptr_t)&AddSlider);
		UIControls::UI_SlidersAddSliderButton.ToolTip = "Add a custom slider just like in BO2 Console V4 :)";
		UIControls::UI_SlidersRefreshButton = T6SDK::Drawing::UI_ClickableButton("REFRESH", 10, 36, T6SDK::AnchorPoint::CenterLeft, (uintptr_t)&Refresh, true);
		UIControls::UI_SlidersRefreshButton.ToolTip = "Refresh the list of sliders from the sliders folder.";
		CustomSlidersMenu::UI_CustomSliderDvarNameDialog = T6SDK::Drawing::UI_TextBoxDialog("Search for a ^3float ^7type dvar");
		Refresh();
	}
}