#pragma once
#include "UIControls.h"
#include "CustomDvars.h"

namespace WeaponMenu
{
	vector<const char*> CamoTags = {"Don't change",
									"Default",
									"None",
									"DEVGRU",
									"A-TACS AU",
									"ERDL",
									"Choco",
									"Blue Tiger",
									"Bloodshot",
									"Ghostex: Delta 6",
									"Kryptek: Typhon",
									"Carbon Fiber",
									"Cherry Blossom",
									"Art of War",
									"Ronin",
									"Skulls",
									"Gold",
									"Diamond",
									"Elite",
									"Digital CE",
									"Jungle Warfare",
									"UK Punk",
									"Benjamins",
									"Dia De Muertos",
									"Graffiti",
									"Kawaii",
									"Party Rock",
									"Zombies",
									"Viper",
									"Bacon",
									"Ghosts DLC Camo",
									"Paladin",
									"Cyborg",
									"Dragon",
									"Comics",
									"Aqua",
									"Breach",
									"Coyote",
									"Glam",
									"Rogue",
									"Pack-a-Punch",
									"Dead Man's Hand",
									"Beast",
									"Octane",
									"Weaponised 115",
									"Afterlife" };
	static void Draw()
	{
		if (!*UIControls::WeaponTabButton.isChecked)
			return;
		UIControls::UI_GunXOffset.Draw();
		UIControls::UI_GunYOffset.Draw();
		UIControls::UI_GunZOffset.Draw();
		UIControls::UI_GunPitch.Draw();
		UIControls::UI_GunYaw.Draw();
		UIControls::UI_GunRoll.Draw();
		UIControls::UI_CamoChanging.Draw();

		char buffer[64];
		sprintf_s(buffer, "Primary camo: ^3%s", CamoTags[CustomDvars::dvar_primaryCamo->current.integer+1]);
		UIControls::UI_PrimaryCamo.Text = buffer;
		UIControls::UI_PrimaryCamo.Draw(CustomDvars::dvar_camoChanging->current.enabled);

		char buffer2[64];
		sprintf_s(buffer2, "Secondary camo: ^3%s", CamoTags[CustomDvars::dvar_secondaryCamo->current.integer + 1]);
		UIControls::UI_SecondaryCamo.Text = buffer2;
		UIControls::UI_SecondaryCamo.Draw(CustomDvars::dvar_camoChanging->current.enabled);
	}

	static void Init()
	{
		UIControls::UI_GunXOffset = T6SDK::Drawing::UI_Slider("X Offset", &(*T6SDK::Dvars::DvarList::cg_gun_x)->current.value, 0.0f, -5.0f, 5.0f, 4, 5, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_GunXOffset.ToolTip = "Weapon viemodel X position offset.";

		UIControls::UI_GunYOffset = T6SDK::Drawing::UI_Slider("Y Offset", &(*T6SDK::Dvars::DvarList::cg_gun_y)->current.value, 0.0f, -5.0f, 5.0f, 7, 5, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_GunYOffset.ToolTip = "Weapon viemodel Y position offset.";

		UIControls::UI_GunZOffset = T6SDK::Drawing::UI_Slider("Z Offset", &(*T6SDK::Dvars::DvarList::cg_gun_z)->current.value, 0.0f, -5.0f, 5.0f, 10, 5, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_GunZOffset.ToolTip = "Weapon viemodel Z position offset.";

		UIControls::UI_GunPitch = T6SDK::Drawing::UI_Slider("Viewmodel pitch", &(*T6SDK::Dvars::DvarList::cg_gun_rot_p)->current.value, 0.0f, -90.0f, 90.0f, 4, 8, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_GunPitch.ToolTip = "Weapon viemodel pitch rotation offset.";

		UIControls::UI_GunYaw = T6SDK::Drawing::UI_Slider("Viewmodel yaw", &(*T6SDK::Dvars::DvarList::cg_gun_rot_y)->current.value, 0.0f, -90.0f, 90.0f, 7, 8, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_GunYaw.ToolTip = "Weapon viemodel yaw rotation offset.";

		UIControls::UI_GunRoll = T6SDK::Drawing::UI_Slider("Viewmodel roll", &(*T6SDK::Dvars::DvarList::cg_gun_rot_r)->current.value, 0.0f, -90.0f, 90.0f, 10, 8, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_GunRoll.ToolTip = "Weapon viemodel roll rotation offset.";

		UIControls::UI_CamoChanging = T6SDK::Drawing::UI_CheckBoxButton("Enable camo changing", "Disable camo changing", 4, 11, T6SDK::AnchorPoint::TopLeft, &CustomDvars::dvar_camoChanging->current.enabled, 0x00);

		UIControls::UI_PrimaryCamo = T6SDK::Drawing::UI_EnumButton("Primary camo", -1, 44, &CustomDvars::dvar_primaryCamo->current.integer, 4, 13, T6SDK::AnchorPoint::TopLeft, 0x00);

		UIControls::UI_SecondaryCamo = T6SDK::Drawing::UI_EnumButton("Secondary camo", -1, 44, &CustomDvars::dvar_secondaryCamo->current.integer, 10, 13, T6SDK::AnchorPoint::TopLeft, 0x00);
	}
}