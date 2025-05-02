#pragma once
#include <StdInclude.h>
#include "CustomDvars.h"
namespace HoldGun
{
	class PlayerHoldGunData
	{
	public:
		int playerindex = -1;
		int weaponindex = -1;
		int droppedWeaponEntIndex = -1;
		PlayerHoldGunData(int playerIndex)
		{
			playerindex = playerIndex;
			weaponindex = -1;
			droppedWeaponEntIndex = -1;
		}
		PlayerHoldGunData()
		{

		}
	};
	static std::map<int, PlayerHoldGunData> PlayersHoldGunMap = {
		{0, PlayerHoldGunData(0)},
		{1, PlayerHoldGunData(1)},
		{2, PlayerHoldGunData(2)},
		{3, PlayerHoldGunData(3)},
		{4, PlayerHoldGunData(4)},
		{5, PlayerHoldGunData(5)},
		{6, PlayerHoldGunData(6)},
		{7, PlayerHoldGunData(7)},
		{8, PlayerHoldGunData(8)},
		{9, PlayerHoldGunData(9)},
		{10, PlayerHoldGunData(10)},
		{11, PlayerHoldGunData(11)},
		{12, PlayerHoldGunData(12)},
		{13, PlayerHoldGunData(13)},
		{14, PlayerHoldGunData(14)},
		{15, PlayerHoldGunData(15)},
		{16, PlayerHoldGunData(16)},
		{17, PlayerHoldGunData(17)},
	};

	vec3_t getNormalizedVector(vec3_t point1, vec3_t point2) 
	{
		vec3_t direction;
		direction.x = point2.x - point1.x;
		direction.y = point2.y - point1.y;
		direction.z = point2.z - point1.z;
		double length = std::sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
		if (length > 0)
		{
			direction.x /= length;
			direction.y /= length;
			direction.z /= length;
		}
		return direction;
	}

	Matrix33_s reflectRotationMatrix(const Matrix33_s& rot, char axis)
	{
		Matrix33_s reflected = rot;

		switch (axis) {
		case 'x':
		case 'X':
			// Отражение по X: инвертируем Y и Z компоненты
			for (int i = 0; i < 3; ++i) 
			{
				reflected.m[1][i] *= -1;  // Вторая строка (Y)
				reflected.m[2][i] *= -1;  // Третья строка (Z)
			}
			break;

		case 'y':
		case 'Y':
			// Отражение по Y: инвертируем X и Z компоненты
			for (int i = 0; i < 3; ++i) 
			{
				reflected.m[0][i] *= -1;  // Первая строка (X)
				reflected.m[2][i] *= -1;  // Третья строка (Z)
			}
			break;

		case 'z':
		case 'Z':
			// Отражение по Z: инвертируем X и Y компоненты
			for (int i = 0; i < 3; ++i) 
			{
				reflected.m[0][i] *= -1;  // Первая строка (X)
				reflected.m[1][i] *= -1;  // Вторая строка (Y)
			}
			break;

		default:
			std::cerr << "Неверная ось. Используйте 'x', 'y' или 'z'." << std::endl;
		}

		return reflected;
	}


	void HoldGunForEachPlayer(int playerIndex, entity_t* itemEntity)
	{
		int playerindex = playerIndex;
		int corpseIndex = -1;
		//If player info is nor valid
		if (!*(int*)&T6SDK::Addresses::cg->client[playerIndex])
			return;
		entity_t* playerEntity = T6SDK::InternalFunctions::CG_GetEntity(playerindex);
		
		if (T6SDK::InternalFunctions::CG_GetEntity(playerindex)->pose.physUserBody == 0)
		{
			for (int i = 0; i < 128; i++)
			{
				if (T6SDK::InternalFunctions::CG_GetEntity(i)->nextState.clientNum == playerindex && T6SDK::InternalFunctions::CG_GetEntity(i)->pose.eType == (BYTE)T6SDK::EntityType::PLAYERCORPSEENTITY && i != playerindex)
				{
					corpseIndex = i;
					//T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "HOLDGUN", "Selected player dead, corpse index: %i; playerEnt -> 0x%X; corpseEnt -> 0x%X", corpseIndex, playerEntity, T6SDK::InternalFunctions::CG_GetEntity(corpseIndex));
					break;
				}
			}
		}
		else
		{
			PlayersHoldGunMap[playerIndex].weaponindex = playerEntity->nextState.WeaponID;
			//T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "HOLDGUN", "Player is holding weapon by index: %i", weaponindex);
		}
		entity_t* corpseEntity = 0x00;

		if (corpseIndex > -1)
		{
			corpseEntity = T6SDK::InternalFunctions::CG_GetEntity(corpseIndex);

			//T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "HOLDGUN", "Player was holding weapon by index: %i", weaponindex);
			//T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "HOLDGUN", "Current item ent weap index: %i", itemEntity->nextState.WeaponID);
			if (itemEntity->nextState.WeaponID == PlayersHoldGunMap[playerIndex].weaponindex)
			{
				//Check if dropped weapon was around
				float searchRadius = T6SDK::Dvars::GetFloat(CustomDvars::dvar_holdgunSearchRadius);
				if (itemEntity->pose.origin.x - searchRadius < corpseEntity->pose.origin.x && itemEntity->pose.origin.x + searchRadius > corpseEntity->pose.origin.x &&
					itemEntity->pose.origin.y - searchRadius < corpseEntity->pose.origin.y && itemEntity->pose.origin.y + searchRadius > corpseEntity->pose.origin.y)
				{
					PlayersHoldGunMap[playerIndex].droppedWeaponEntIndex = itemEntity->nextState.number;
				}
				//T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "HOLDGUN", "Corpse x: %.3f; y: %.3f; z: %.3f;", corpseEntity->pose.origin.x, corpseEntity->pose.origin.y, corpseEntity->pose.origin.z );
				//T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "HOLDGUN", "Found dropped weapon! Entity number: %i; x: %.3f; y: %.3f; z: %.3f;", itemEntity->nextState.number, itemEntity->pose.origin.x, itemEntity->pose.origin.y, itemEntity->pose.origin.z);
				//If item entity is the entity we want
				if (itemEntity->nextState.number == PlayersHoldGunMap[playerIndex].droppedWeaponEntIndex)
				{
					//T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "HOLDGUN", "Corpse x: %.3f; y: %.3f; z: %.3f;", corpseEntity->pose.origin.x, corpseEntity->pose.origin.y, corpseEntity->pose.origin.z);
					//T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "HOLDGUN", "Found dropped weapon! Entity number: %i; x: %.3f; y: %.3f; z: %.3f;", itemEntity->nextState.number, itemEntity->pose.origin.x, itemEntity->pose.origin.y, itemEntity->pose.origin.z);
					vec3_t pos{};
					Matrix33_s rot{};
					if (T6SDK::InternalFunctions::CG_DObjGetWorldTagMatrix(corpseEntity, T6SDK::Dvars::GetBool(CustomDvars::dvar_holdgunWrist) ? 204 : 212, &rot, &pos)) //right wrist
					{
						//T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "HOLDGUN", "Corpse right wrist bone x: %.3f; y: %.3f; z: %.3f;", pos.x, pos.y, pos.z);
						vec3_t angles{};
						if (T6SDK::Dvars::GetBool(CustomDvars::dvar_holdgunWrist)) //attach to wrist to lock weapon in a hand
						{
							//Getting vector from wrist to middle finger to adjust weapon pos along that vector
							vec3_t middleRingPos{};
							Matrix33_s middleRingRot{};
							T6SDK::InternalFunctions::CG_DObjGetWorldTagMatrix(corpseEntity, 507, &middleRingRot, &middleRingPos); //get ring matrices
							vec3_t dirVector = getNormalizedVector(pos, middleRingPos);
							//Getting vector from wrist to thumb to adjust weapon pos orthogonally to wrist-middleFinger vector (big brain moment)
							vec3_t thumb3Pos{};
							Matrix33_s thumb3Rot{};
							T6SDK::InternalFunctions::CG_DObjGetWorldTagMatrix(corpseEntity, 534, &thumb3Rot, &thumb3Pos); //get thumb matrices
							vec3_t dirVector2 = getNormalizedVector(pos, thumb3Pos);
							//Adjusting weapon position
							itemEntity->pose.origin = pos + (12.0f * dirVector) + (5.0f * dirVector2); //12.0f and 5.0f were chosen experimentally 
							Matrix33_s newRotationMatrix{};
							newRotationMatrix = reflectRotationMatrix(rot, 'Z');
							//Had to reflect wrist bone rotation by Z axis since it's has wrong direction
							T6SDK::InternalFunctions::AxisToAngles(&newRotationMatrix, &angles);
							itemEntity->pose.angles = angles;
						}
						else //attach to tag_weapon_right to allow weapon move as deveropers wanted tag_weapon_right to move
						{
							itemEntity->pose.origin = pos;
							T6SDK::InternalFunctions::AxisToAngles(&rot, &angles);
							itemEntity->pose.angles = angles;
						}
					}
				}
			}
		}
	}
	inline static void Update(entity_t* itemEntity)
	{
		if (!CustomDvars::dvar_holdgun->current.enabled)
			return;
		for (int i = 0; i < 18; i++)
		{
			HoldGunForEachPlayer(i, itemEntity);
		}
	}
}