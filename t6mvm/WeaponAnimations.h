#pragma once
#include <StdInclude.h>
namespace WeaponAnimations
{
	class WeaponAnimationData
	{
	public:
		const char* Name = "";
		bool Replaced = false;
		const char* ReplacedWith = "";
		uintptr_t Address1 = 0;
		uintptr_t Address2 = 0;
		int defaultValue1 = 0;
		int defaultValue2 = 0;
		void Init(const char* name, uintptr_t address)
		{
			Name = name;
			Address1 = address;
			defaultValue1 = *(int*)address;
			Address2 = (uintptr_t)T6SDK::InternalFunctions::DB_FindXAssetHeader(T6SDK::XAssetType::XANIMPARTS, name);
			defaultValue2 = *(int*)Address2;
			//T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, false, "WEAPONANIMS", "Animation %s: 0x%X, 0x%X; Defaults: 0x%X, 0x%X", name, Address1, Address2, defaultValue1, defaultValue2);
		}
		void Reset()
		{
			*(int*)Address1 = defaultValue1;
			*(int*)Address2 = defaultValue2;
			Replaced = false;
			ReplacedWith = "";
		}
		void Replace(WeaponAnimationData* newAnim)
		{
			*(int*)Address1 = *(int*)newAnim->Address1;
			*(int*)Address2 = *(int*)newAnim->Address2;
			Replaced = true;
			ReplacedWith = newAnim->Name;
		}
	};
	inline static map<std::string, std::string> WeaponNames = 
	{
		//SMG
		{"mp7_mp", "MP7"},
		{"pdw57_mp", "PDW-57"},
		{"vector_mp", "Vector K10"},
		{"insas_mp", "MSMC"},
		{"qcw05_mp", "Chicom CQB"},
		{"evoskorpion_mp", "Skorpion EVO"},
		{"peacekeeper_mp", "Peacekeeper"},
		//ASSAULT RIFLES
		{"tar21_mp", "MTAR"},
		{"type95_mp", "Type 25"},
		{"sig556_mp", "SWAT-556"},
		{"sa58_mp", "FAL OSW"},
		{"hk416_mp", "M27"},
		{"scar_mp", "SCAR-H"},
		{"srm1216_mp", "SMR"},
		{"xm8_mp", "M8A1"},
		{"an94_mp", "AN94"},
		//SHOTGUNS
		{"870mcs_mp", "R870 MCS"},
		{"saiga12_mp", "S12"},
		{"ksg_mp", "KSG"},
		{"srm1216_mp", "M1216"},
		//LMG
		{"mk48_mp", "Mk 48"},
		{"qbb95_mp", "QBB LSW"},
		{"lsat_mp", "LSAT"},
		{"hamr_mp", "HAMR"},
		//SNIPERS
		{"svu_mp", "SVU-AS"},
		{"dsr50_mp", "DSR 50"},
		{"ballista_mp", "Ballista"},
		{"as50_mp", "XPR-50"},
		//PISTOLS
		{"fiveseven_mp", "Five-seven"},
		{"fnp45_mp", "Tac-45"},
		{"beretta93r_mp", "B23R"},
		{"judge_mp", "Executioner"},
		{"kard_mp", "KAP-40"}
	};
	class WeaponData
	{
	public:
		const char* FriendlyName = "";
		const char* Name = "";
		std::map<std::string, WeaponAnimationData> AnimationsMap{};
		void Init(const char* name, const char* friendlyName)
		{	
			uintptr_t weapAddress = (uintptr_t)T6SDK::InternalFunctions::DB_FindXAssetHeader(T6SDK::XAssetType::WEAPON, name);	
			if (weapAddress == 0x00)
			{
				T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, false, "WEAPONANIMS", "Weapon %s not found!", name);
				return;
			}
			Name = name;//*(reinterpret_cast<const char**>(weapAddress + 0x0C)); //DISPLAY NAME
			FriendlyName = friendlyName;
			//T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, false, "WEAPONANIMS", "Loading %s", Name);
			uintptr_t weapAnimsAddress = *(int*)(weapAddress + 0x20); //szXAnims
			int animsCount = 87;
			for (int i = 0; i < animsCount; i++)
			{
				if (*(int*)weapAnimsAddress > 0x00)
				{

					const char* animName = *(reinterpret_cast<const char**>(weapAnimsAddress));
					if (strlen(animName) > 0)
					{
						if (AnimationsMap[animName].Name == "")
						{
							WeaponAnimationData anim{};
							anim.Init(animName, weapAnimsAddress);
							AnimationsMap[animName] = anim;
							//T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, false, "WEAPONANIMS", "Animation %s: 0x%X, 0x%X; Defaults: 0x%X, 0x%X", anim.Name, anim.Address1, anim.Address2, anim.defaultValue1, anim.defaultValue2);
						}
						else
						{
							//T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_WARNING, false, "WEAPONANIMS", "Animation %s already exists!", animName);
						}
					}		
				}
				weapAnimsAddress += 0x4;
			}
			
		}
	};
	inline static vector<WeaponData> Weapons{};
	inline static bool Inited = false;
	inline static void Init()
	{
		if (!(T6SDK::CrossVersion::GetGameVersion() == T6SDK::CrossVersion::V43 || T6SDK::CrossVersion::GetGameVersion() == T6SDK::CrossVersion::MP))
		{
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_WARNING, false, "WEAPONANIMS", "This feature is only available in BO2 MP!");
			Inited = false;
			return;
		}
		for (auto& weapon : WeaponNames)
		{
			WeaponData weap{};
			weap.Init(weapon.first.c_str(), weapon.second.c_str());
			Weapons.push_back(weap);
		}
		Inited = true;
		T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, false, "WEAPONANIMS", "Loaded %i weapons!", Weapons.size());
	}
}