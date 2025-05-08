#pragma once
#include <StdInclude.h>
#include "CustomDvars.h"
namespace HoldGun
{
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


	std::array<int, 18> weaponIndices{};
	std::array<entity_t*, 18> corpseIndices{};
	std::array<entity_t*, 18> itemIndices{};

	vec3_t holdGunPos{};
	vec3_t holdGunAngles{};
	void AttachWeap(int clientNumindex)
	{
		vec3_t pos{};
		Matrix33_s rot{};
		if (T6SDK::InternalFunctions::CG_DObjGetWorldTagMatrix(corpseIndices[clientNumindex], T6SDK::Dvars::GetBool(CustomDvars::dvar_holdgunWrist) ? 204 : 212, &rot, &pos)) //right wrist
		{
			//T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "HOLDGUN", "Corpse right wrist bone x: %.3f; y: %.3f; z: %.3f;", pos.x, pos.y, pos.z);
			vec3_t angles{};
			if (T6SDK::Dvars::GetBool(CustomDvars::dvar_holdgunWrist)) //attach to wrist to lock weapon in a hand
			{
				//Getting vector from wrist to middle finger to adjust weapon pos along that vector
				vec3_t middleRingPos{};
				Matrix33_s middleRingRot{};
				T6SDK::InternalFunctions::CG_DObjGetWorldTagMatrix(corpseIndices[clientNumindex], 507, &middleRingRot, &middleRingPos); //get ring matrices
				vec3_t dirVector = getNormalizedVector(pos, middleRingPos);
				//Getting vector from wrist to thumb to adjust weapon pos orthogonally to wrist-middleFinger vector (big brain moment)
				vec3_t thumb3Pos{};
				Matrix33_s thumb3Rot{};
				T6SDK::InternalFunctions::CG_DObjGetWorldTagMatrix(corpseIndices[clientNumindex], 534, &thumb3Rot, &thumb3Pos); //get thumb matrices
				vec3_t dirVector2 = getNormalizedVector(pos, thumb3Pos);
				//Adjusting weapon position
				itemIndices[clientNumindex]->pose.origin = pos + (12.0f * dirVector) + (5.0f * dirVector2); //12.0f and 5.0f were chosen experimentally 
				
				Matrix33_s newRotationMatrix{};
				newRotationMatrix = reflectRotationMatrix(rot, 'Z');
				//Had to reflect wrist bone rotation by Z axis since it's has wrong direction
				T6SDK::InternalFunctions::AxisToAnglesCustom(&newRotationMatrix, &angles);
				itemIndices[clientNumindex]->pose.angles = angles;
			}
			else //attach to tag_weapon_right to allow weapon move as deveropers wanted tag_weapon_right to move
			{
				itemIndices[clientNumindex]->pose.origin = pos;
				T6SDK::InternalFunctions::AxisToAnglesCustom(&rot, &angles);
				itemIndices[clientNumindex]->pose.angles = angles;

			}
		}
		else
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, true, "HOLDGUN", "Corpse right wrist bone not found!");
	}
	inline static void Update(entity_t* cEntity)
	{
		if (!T6SDK::Theater::IsInTheater())
			return;
		if (!T6SDK::Dvars::GetBool(CustomDvars::dvar_holdgun))
			return;
		if (cEntity->nextState.eType == (int)T6SDK::EntityType::PLAYERENTITY)
		{
			if (static_cast<std::size_t>(cEntity->nextState.clientNum) < weaponIndices.size() && cEntity->nextState.WeaponID != 0)
				weaponIndices[(int)cEntity->nextState.clientNum] = cEntity->nextState.WeaponID;
		}
		else if (cEntity->nextState.eType == (BYTE)T6SDK::EntityType::PLAYERCORPSEENTITY)
		{
			if (static_cast<std::size_t>(cEntity->nextState.clientNum) < corpseIndices.size())
			{
				corpseIndices[cEntity->nextState.clientNum] = cEntity;
			}
		}
		else if (cEntity->nextState.eType == (BYTE)T6SDK::EntityType::ITEMENTITY)
		{
			if (static_cast<std::size_t>(cEntity->nextState.clientNum) < itemIndices.size() && cEntity->nextState.WeaponID != 0)
			{
				itemIndices[cEntity->nextState.clientNum] = cEntity;
			}
		}
	}

	inline static void UpdateItem(entity_t* cEntity)
	{
		if (!T6SDK::Theater::IsInTheater())
			return;
		if (!T6SDK::Dvars::GetBool(CustomDvars::dvar_holdgun))
			return;
		
		if (static_cast<std::size_t>(cEntity->nextState.clientNum) < itemIndices.size() && itemIndices[cEntity->nextState.clientNum] != 0 && 
			static_cast<std::size_t>(cEntity->nextState.clientNum) < corpseIndices.size() && corpseIndices[cEntity->nextState.clientNum] != 0)
		{
			AttachWeap(cEntity->nextState.clientNum);
		}
		else
		{
			//Item->clientnum equals 18 somehow so find corpse and its item by ourself. Usually happens when a weapon should be already on the ground.
			for (int i = 0; i < 18; i++)
			{
				if (static_cast<std::size_t>(i) < itemIndices.size() && itemIndices[i] != 0 && static_cast<std::size_t>(i) < corpseIndices.size() && corpseIndices[i] != 0)
				{
					AttachWeap(i);
				}
			}
		}	
	}

	//DEBUG PURPOSES ONLY
	inline static void OnEndScene()
	{
		if (!T6SDK::Theater::IsInTheater())
			return;
		if (!T6SDK::Dvars::GetBool(CustomDvars::dvar_holdgun))
			return;
		//Item->clientnum equals 18 somehow so find corpse and its item by ourself. Usually happens when a weapon should be already on the ground.
		for (int i = 0; i < 18; i++)
		{
			if (static_cast<std::size_t>(i) < itemIndices.size() && itemIndices[i] != 0 && static_cast<std::size_t>(i) < corpseIndices.size() && corpseIndices[i] != 0)
			{
				vec3_t pos{};
				Matrix33_s rot{};
				if (T6SDK::InternalFunctions::CG_DObjGetWorldTagMatrix(corpseIndices[i], T6SDK::Dvars::GetBool(CustomDvars::dvar_holdgunWrist) ? 204 : 212, &rot, &pos)) //right wrist
				{
					vec2_t screenPos{};
					if (T6SDK::InternalFunctions::WorldPosToScreenPos(&pos, &screenPos))
					{
						RECT rect{};
						if (T6SDK::Drawing::DrawRectAbsolute(screenPos.x, screenPos.y, 20.0f, 20.0f, T6SDK::Drawing::YELLOWCOLOR, T6SDK::AnchorPoint::Center, &rect))
						{
							vec3_t angles{};
							T6SDK::InternalFunctions::AxisToAngles(&rot, &angles);

							vec3_t angles2{};
							T6SDK::InternalFunctions::AxisToAnglesCustom(&rot, &angles2);
							char buffer[512];
							//sprintf(buffer, "%i) Corpse: 0x%X; Item: 0x%X;\nx: %.3f; y: %.3f; z: %.3f\nP: %.3f; Y: %.3f; R: %.3f;\nbX: %.3f; bY: %.3f; bZ: %.3f;\nbP: %.3f; bY: %.3f; bR: %.3f;\n[%.3f] [%.3f] [%.3f]\n[%.3f] [%.3f] [%.3f]\n[%.3f] [%.3f] [%.3f]\n", i, corpseIndices[i], itemIndices[i],
							//	itemIndices[i]->pose.origin.x, itemIndices[i]->pose.origin.y, itemIndices[i]->pose.origin.z,
							//	itemIndices[i]->pose.angles.x, itemIndices[i]->pose.angles.y, itemIndices[i]->pose.angles.z,
							//	pos.x, pos.y, pos.z,
							//	rot.m[0][0], rot.m[0][1], rot.m[0][2], 
							//	rot.m[1][0], rot.m[1][1], rot.m[1][2], 
							//	rot.m[2][0], rot.m[2][1], rot.m[2][2]);
							sprintf(buffer, "^3Expected: ^7[%.3f] [%.3f] [%.3f];\n^3Yours: ^7[%.3f] [%.3f] [%.3f];\n^3Matrix was:\n^7[%.3f] [%.3f] [%.3f]\n[%.3f] [%.3f] [%.3f]\n[%.3f] [%.3f] [%.3f]",
								angles.x, angles.y, angles.z, angles2.x, angles2.y, angles2.z,
								rot.m[0][0], rot.m[0][1], rot.m[0][2],
								rot.m[1][0], rot.m[1][1], rot.m[1][2],
								rot.m[2][0], rot.m[2][1], rot.m[2][2]);

							T6SDK::Drawing::DrawTextAbsolute(buffer, screenPos.x+50.0f, screenPos.y - 200.0f, 1.0f, T6SDK::Drawing::WHITECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
							if (T6SDK::Input::MousePosX() > (float)rect.left && T6SDK::Input::MousePosX() < (float)rect.right && T6SDK::Input::MousePosY() > (float)rect.top && T6SDK::Input::MousePosY() < (float)rect.bottom)
							{
								
							}
						}
					}
				}
			}
		}
		
	}
}