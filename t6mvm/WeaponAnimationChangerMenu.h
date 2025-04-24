#pragma once
#include "UIControls.h"
#include "WeapsListBoxControl.h"
#include "WeaponAnimations.h"
namespace WeaponAnimationChangerMenu
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
	static bool MenuOpened = false;
	static int weaponIndex1 = 0;
	static int weaponIndex2 = 0;
	static int animIndex1 = 0;
	static int animIndex2 = 0;
	std::string GetWeapNameForMaterial(int index)
	{
		std::string name(WeaponAnimations::Weapons[index].Name);
		std::string outputName{};
		if (strcmp(name.c_str(), "srm1216_mp") == 0)
			outputName = "srm";
		else if (strcmp(name.c_str(), "dsr50_mp") == 0)
			outputName = "dsr1";
		else if (strcmp(name.c_str(), "beretta93r_mp") == 0)
			outputName = "baretta";
		else if (strcmp(name.c_str(), "fiveseven_mp") == 0)
			outputName = "five_seven";
		else if (strcmp(name.c_str(), "peacekeeper_mp") == 0)
			outputName = "pm";
		else if (strcmp(name.c_str(), "vector_mp") == 0)
			outputName = "kriss";
		else if (strcmp(name.c_str(), "pdw57_mp") == 0)
			outputName = "ar57";
		else if (strcmp(name.c_str(), "qcw05_mp") == 0)
			outputName = "qcw";
		else
			outputName = name.substr(0, name.length() - 3);
		return "menu_mp_weapons_" + outputName;
	}
	static void Draw()
	{
		T6SDK::Drawing::DrawRectAbsolute(0.0f, 0.0f, T6SDK::Addresses::ScreenWidth.Value(), T6SDK::Addresses::ScreenHeight.Value(), T6SDK::Drawing::T_BLACKCOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_ReplaceWeaponAnim.Draw();
		UIControls::UI_ResetSelectedWeaponAnim.Draw();
		UIControls::UI_ResetAllWeaponAnims.Draw();
		UIControls::UI_CloseWeaponAnimMenu.Draw();
		UIControls::UI_WeapsLB1.Draw();
		UIControls::UI_WeapsLB2.Draw();
		UIControls::UI_WeapAnimsLB1.Draw();
		UIControls::UI_WeapAnimsLB2.Draw();

		T6SDK::Drawing::DrawTextAbsolute("^9TARGET", T6SDK::Drawing::GetGridCellCoords(3,3).x, T6SDK::Drawing::GetGridCellCoords(3,3).y, 1.0f, T6SDK::Drawing::T_WHITECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		T6SDK::Drawing::DrawTextAbsolute("^9SOURCE", T6SDK::Drawing::GetGridCellCoords(12, 3).x, T6SDK::Drawing::GetGridCellCoords(12, 3).y, 1.0f, T6SDK::Drawing::T_WHITECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);

		
		auto anim = get_element_by_index(WeaponAnimations::Weapons[weaponIndex1].AnimationsMap, animIndex1);
		if (anim->second.Replaced)
		{
			char buffer[256];
			sprintf(buffer, "^9Replaced with: ^7%s", anim->second.ReplacedWith);
			T6SDK::Drawing::DrawTextAbsolute(buffer, T6SDK::Drawing::GetGridCellCoords(1, 29).x, T6SDK::Drawing::GetGridCellCoords(1, 29).y, 1.0f, T6SDK::Drawing::T_WHITECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		}

		//Draw target weapon image	
		void* targetWeaponImage = T6SDK::InternalFunctions::Material_RegisterHandle(GetWeapNameForMaterial(weaponIndex1).c_str(), (int)T6SDK::XAssetType::MATERIAL);
		if(targetWeaponImage)
		{
			vec2_t targetWeaponImageCoords = T6SDK::Drawing::GetGridCellCoords(1, 29);
			T6SDK::Typedefs::R_AddCmdDrawStretchPicRotateXYInternal(targetWeaponImageCoords.x, targetWeaponImageCoords.y, 256.0f, 128.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, T6SDK::Drawing::WHITECOLOR, targetWeaponImage);
		}

		//Draw source weapon image
		void* sourceWeaponImage = T6SDK::InternalFunctions::Material_RegisterHandle(GetWeapNameForMaterial(weaponIndex2).c_str(), (int)T6SDK::XAssetType::MATERIAL);
		if(sourceWeaponImage)
		{
			vec2_t sourceWeaponImageCoords = T6SDK::Drawing::GetGridCellCoords(12, 29);
			T6SDK::Typedefs::R_AddCmdDrawStretchPicRotateXYInternal(sourceWeaponImageCoords.x, sourceWeaponImageCoords.y, 256.0f, 128.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, T6SDK::Drawing::WHITECOLOR, sourceWeaponImage);
		}

	}
	void CloseMenu()
	{
		*UIControls::UI_WeaponAnimChanging.isChecked = false;
	}
	void Replace()
	{
		const char* targetAnimName = get_element_by_index(WeaponAnimations::Weapons[weaponIndex1].AnimationsMap, animIndex1)->second.Name;
		const char* sourceAnimName = get_element_by_index(WeaponAnimations::Weapons[weaponIndex2].AnimationsMap, animIndex2)->second.Name;
		WeaponAnimations::Weapons[weaponIndex1].AnimationsMap[targetAnimName].Replace(&WeaponAnimations::Weapons[weaponIndex2].AnimationsMap[sourceAnimName]);
	}
	void Reset()
	{
		const char* targetAnimName = get_element_by_index(WeaponAnimations::Weapons[weaponIndex1].AnimationsMap, animIndex1)->second.Name;
		WeaponAnimations::Weapons[weaponIndex1].AnimationsMap[targetAnimName].Reset();
	}
	void ResetAll()
	{
		for (auto& weapon : WeaponAnimations::Weapons)
		{
			for (auto& anim : weapon.AnimationsMap)
			{
				anim.second.Reset();
			}
		}
	}
	static void Init()
	{
		UIControls::UI_ReplaceWeaponAnim = T6SDK::Drawing::UI_ClickableButton("REPLACE", 8, 19, T6SDK::AnchorPoint::Center, (uintptr_t)&Replace);
		UIControls::UI_ReplaceWeaponAnim.ToolTip = "^5NOTE: ^7changes will be applied after you skip a frame back.";
		UIControls::UI_ResetSelectedWeaponAnim = T6SDK::Drawing::UI_ClickableButton("RESET", 8, 22, T6SDK::AnchorPoint::Center, (uintptr_t)&Reset);
		UIControls::UI_ResetSelectedWeaponAnim.ToolTip = "Reset selected animation.";
		UIControls::UI_ResetAllWeaponAnims = T6SDK::Drawing::UI_ClickableButton("RESET ALL", 8, 25, T6SDK::AnchorPoint::Center, (uintptr_t)&ResetAll);
		UIControls::UI_ResetAllWeaponAnims.ToolTip = "Reset all replaced animations.";
		UIControls::UI_CloseWeaponAnimMenu = T6SDK::Drawing::UI_ClickableButton("CLOSE MENU", 8, 33, T6SDK::AnchorPoint::Center, (uintptr_t)&CloseMenu);
		UIControls::UI_CloseWeaponAnimMenu.ToolTip = "Back to ^5T6MVM ^7menu.";

		UIControls::UI_WeapsLB1 = Weapon::UI_WeapsListBox("TARGET", 1, 6, 3, 28, &weaponIndex1, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_WeapAnimsLB1 = Weapon::UI_WeapAnimListBox("ANIMATIONS", 4, 6, 7, 28, &animIndex1, &weaponIndex1, T6SDK::AnchorPoint::TopLeft, 0x00);

		UIControls::UI_WeapsLB2 = Weapon::UI_WeapsListBox("SOURCE", 9, 6, 11, 28, &weaponIndex2, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_WeapAnimsLB2 = Weapon::UI_WeapAnimListBox("ANIMATIONS", 12, 6, 15, 28, &animIndex2, &weaponIndex2, T6SDK::AnchorPoint::TopLeft, 0x00);
		//UI_WeapsLB1.OnSelectedItemChanged = (uintptr_t)&OnSelectedItemChanged;
	}
}