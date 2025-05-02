#pragma once
#include <StdInclude.h>

namespace BoneCamera
{
#pragma region Defenitions

	class BoneData
	{
	public:
		int Index;
		const char* Name;
		BoneData(const char* name, int index) : Index(index), Name(name) {}
	};

	inline static T6SDK::Drawing::UI_KeyReactiveText AttachToBoneButton{};

	inline static bool BoneAttached = false;
	inline static int SelectedBone = 0;
	inline static bool IsTransformMatrixSet = false;

	static T6SDK::Theater::CustomCameraMode BONECAMMODE("BONE CAMERA");
	inline static vector<BoneData> Bones
	{
		BoneData("tag_origin", 189),
		BoneData("j_mainroot", 190),
		BoneData("j_spine4", 193),
		BoneData("j_neck", 194),
		BoneData("j_head", 195),
		BoneData("j_clavicle_le", 196),
		BoneData("j_shoulder_le", 197),
		BoneData("j_elbow_le", 198),
		BoneData("j_wrist_le", 199),
		BoneData("j_wristtwist_le", 200),
		BoneData("j_clavicle_ri", 201),
		BoneData("j_shoulder_ri", 202),
		BoneData("j_elbow_ri", 203),
		BoneData("j_wrist_ri", 204),
		BoneData("j_wristtwist_ri", 205),
		BoneData("j_hip_le", 206),
		BoneData("j_knee_le", 207),
		BoneData("j_ankle_le", 208),
		BoneData("j_hip_ri", 209),
		BoneData("j_knee_ri", 210),
		BoneData("j_ankle_ri", 211),
		BoneData("tag_weapon_right", 212),
		BoneData("j_ball_le", 459),
		BoneData("j_ball_ri", 460),
		BoneData("j_jaw", 470),
		BoneData("j_lip_top_le", 471),
		BoneData("j_lip_top_ri", 472),
		BoneData("j_elbow_bulge_le", 499),
		BoneData("j_elbow_bulge_ri", 500),
		BoneData("j_shouldertwist_le", 501),
		BoneData("j_shouldertwist_ri", 502),
		BoneData("j_index_le_1", 504),
		BoneData("j_index_ri_1", 505),
		BoneData("j_mid_le_1", 506),
		BoneData("j_mid_ri_1", 507),
		BoneData("j_pinky_le_1", 508),
		BoneData("j_pinky_ri_1", 509),
		BoneData("j_ring_le_1", 510),
		BoneData("j_ring_ri_1", 511),
		BoneData("j_thumb_le_1", 512),
		BoneData("j_thumb_ri_1", 513),
		BoneData("tag_weapon_left", 514),
		BoneData("j_index_le_2", 515),
		BoneData("j_index_ri_2", 516),
		BoneData("j_mid_le_2", 517),
		BoneData("j_mid_ri_2", 518),
		BoneData("j_pinky_le_2", 519),
		BoneData("j_pinky_ri_2", 520),
		BoneData("j_ring_le_2", 521),
		BoneData("j_ring_ri_2", 522),
		BoneData("j_thumb_le_2", 523),
		BoneData("j_thumb_ri_2", 524),
		BoneData("j_index_le_3", 525),
		BoneData("j_index_ri_3", 526),
		BoneData("j_mid_le_3", 527),
		BoneData("j_mid_ri_3", 528),
		BoneData("j_pinky_le_3", 529),
		BoneData("j_pinky_ri_3", 530),
		BoneData("j_ring_le_3", 531),
		BoneData("j_ring_ri_3", 532),
		BoneData("j_thumb_le_3", 533),
		BoneData("j_thumb_ri_3", 534),
		BoneData("j_head_end", 999),
		BoneData("torso_stabilizer", 1128),
		BoneData("j_cheek_le", 1147),
		BoneData("j_cheek_ri", 1148),
		BoneData("j_chin_skinroll", 1151),
		BoneData("j_levator_le", 1160),
		BoneData("j_levator_ri", 1161),
		BoneData("j_mouth_le", 1172),
		BoneData("j_mouth_ri", 1173),
		BoneData("pelvis", 3027),
		BoneData("back_low", 3028),
		BoneData("back_mid", 3029)
	};
	inline static bool IsBoneCamera()
	{
		return T6SDK::Theater::IsInTheater() && T6SDK::Addresses::DemoPlayback.Value()->CameraMode == (T6SDK::DemoCameraMode)0x03;
	}
#pragma endregion
	Matrix44_s storedTransformation{};
	inline static void Update()
	{
		if (!T6SDK::Theater::IsInTheater())
			return;
		if (!T6SDK::Addresses::DemoPlayback.IsValid())
			return;
		if (!IsBoneCamera())
			return;
		if (!BoneAttached)
			return;
		//Big brain code here
		vec3_t pos{};
		Matrix33_s rot{};
		int playerindex = T6SDK::Dvars::GetInt(*T6SDK::Dvars::DvarList::demo_client);
		if (T6SDK::InternalFunctions::CG_GetEntity(playerindex)->pose.physUserBody == 0)
		{
			for (int i = 0; i < 48; i++)
			{
				if (T6SDK::InternalFunctions::CG_GetEntity(i)->nextState.clientNum == playerindex && T6SDK::InternalFunctions::CG_GetEntity(i)->pose.eType == (BYTE)T6SDK::EntityType::PLAYERCORPSEENTITY && i != playerindex)
				{
					playerindex = i;
					break;
				}
			}
			//playerindex = (*(playerState_t*)(T6SDK::Addresses::PlayerStates + (playerindex * 0x2A08))).corpseIndex;
			//T6SDK::ConsoleLog::LogErrorFormatted("Player is DEAD! Switching to corpse index: %i;", playerindex);
		}
		if (T6SDK::InternalFunctions::CG_DObjGetWorldTagMatrix(T6SDK::InternalFunctions::CG_GetEntity(playerindex), Bones[SelectedBone].Index, &rot, &pos))
		{
			Matrix44_s boneMatrix{};
			T6SDK::InternalFunctions::MatrixSet44(&boneMatrix, &pos, &rot, 1.0f);
			Matrix44_s inversedBoneMatrix{};
			if (!T6SDK::InternalFunctions::MatrixInverse44(&boneMatrix, &inversedBoneMatrix))
			{
				T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, false, "BONECAMERA", "Bone inversed matrix failed!");
				return;
			}
			Matrix44_s cameraMatrix{};
			T6SDK::InternalFunctions::MatrixSet44(&cameraMatrix, &T6SDK::Addresses::DemoPlayback.Value()->FreeRoamCamera.Origin, &T6SDK::Addresses::cg->RefDef.viewAxis, 1.0f);
			if (IsTransformMatrixSet == false)
			{
				T6SDK::InternalFunctions::MatrixMultiply44(&inversedBoneMatrix, &cameraMatrix, &storedTransformation);
				IsTransformMatrixSet = true;
			}
			Matrix44_s resultMatrix{};
			T6SDK::InternalFunctions::MatrixMultiply44(&boneMatrix, &storedTransformation, &resultMatrix);
			Matrix33_s rotationMatrix = T6SDK::InternalFunctions::GetRotation33FromMatrix44(&resultMatrix);
			T6SDK::Addresses::cg->RefDef.viewAxis = rotationMatrix;
			T6SDK::Addresses::DemoPlayback.Value()->FreeRoamCamera.Origin = T6SDK::InternalFunctions::GetOriginFromMatrix44(&resultMatrix);
			
			entity_t* corpseEntity = T6SDK::InternalFunctions::CG_GetEntity(playerindex);
			//T6SDK::ConsoleLog::LogSameLineFormatted("ent pos: %.3f %.3f %3f", corpseEntity->pose.origin.x, corpseEntity->pose.origin.y, corpseEntity->pose.origin.z);

			//T6SDK::InternalFunctions::AxisToAngles(&T6SDK::Addresses::cg->RefDef.viewAxis, &T6SDK::Addresses::DemoPlayback.Value()->FreeRoamCamera.Angles);
		}
	}

	static void DrawBoneCameraControls()
	{
		if(!IsBoneCamera())
			return;
		AttachToBoneButton.Text = BoneAttached ? "^7Press ^3F ^7to detach from selected bone" : "^7Press ^3F ^7to attach to selected bone";
		AttachToBoneButton.Draw();
		BONECAMMODE.FreeCameraModes[0] = Bones[SelectedBone].Name;

		//DrawSelectedBone
		if (!CustomDvars::dvar_showBone->current.enabled)
			return;
		vec3_t pos{};
		Matrix33_s rot{};
		int playerindex = T6SDK::Dvars::GetInt(*T6SDK::Dvars::DvarList::demo_client);
		if (T6SDK::InternalFunctions::CG_GetEntity(playerindex)->pose.physUserBody == 0)
		{
			for (int i = 0; i < 48; i++)
			{
				if (T6SDK::InternalFunctions::CG_GetEntity(i)->nextState.clientNum == playerindex && T6SDK::InternalFunctions::CG_GetEntity(i)->pose.eType == (BYTE)T6SDK::EntityType::PLAYERCORPSEENTITY && i != playerindex)
				{
					playerindex = i;
					break;
				}
			}
		}
		if (T6SDK::InternalFunctions::CG_DObjGetWorldTagMatrix(T6SDK::InternalFunctions::CG_GetEntity(playerindex), Bones[SelectedBone].Index, &rot, &pos))
		{
			vec2_t screenPos{};
			if(T6SDK::InternalFunctions::WorldPosToScreenPos(&pos, &screenPos))
				T6SDK::Drawing::DrawRectAbsolute(screenPos.x, screenPos.y, 10.0f, 10.0f, T6SDK::Drawing::REDCOLOR, T6SDK::AnchorPoint::Center, 0x00);
		}
	}
	static void DetachBone()
	{
		try
		{
			BoneAttached = false;
			IsTransformMatrixSet = false;
			T6SDK::Addresses::IsDemoPaused.Value() = true;
			T6SDK::InternalFunctions::GrabRefDefViewAxisMatrix(false);
			T6SDK::Theater::GrabFreeCameraAngles = false;
			T6SDK::Addresses::DemoPlayback.Value()->FreeRoamCamera.Angles.z = 0.0f;
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_SUCCESS, false, "BONECAMERA", "BONE CAMERA DETACHED!");
		}
		catch (const char* e)
		{
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, false, "BONECAMERA", e);
		}
	}
	static void AttachBone()
	{
		try
		{
			T6SDK::Theater::GrabFreeCameraAngles = true;
			T6SDK::InternalFunctions::GrabRefDefViewAxisMatrix(true);
			BoneAttached = true;
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_SUCCESS, false, "BONECAMERA", "BONE CAMERA ATTACHED TO %s!", Bones[SelectedBone].Name);
		}
		catch (const char* e)
		{
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, false, "BONECAMERA", e);
		}
	}
	static void SwitchBoneState()
	{
		if (!IsBoneCamera())
			return;
		if (!T6SDK::Theater::IsInTheater())
			return;
		if(BoneAttached)
			DetachBone();
		else
			AttachBone();
	}
	void IncreaseSelectedBoneNumber()
	{
		if(T6SDK::Input::IsInputLocked())
			return;
		if(!IsBoneCamera())
			return;
		if(SelectedBone == Bones.size() - 1)
			SelectedBone = 0;
		else
			SelectedBone++;
	}
	void DecreaseSelectedBoneNumber()
	{
		if (T6SDK::Input::IsInputLocked())
			return;
		if (!IsBoneCamera())
			return;
		if(SelectedBone == 0)
			SelectedBone = Bones.size() - 1;
		else
			SelectedBone--;
	}
	static void OnCameraModeChanged(int mode)
	{
		if(BoneAttached)
			DetachBone();
	}
	inline static void Init()
	{
		T6SDK::Theater::RegisterCameraMode(&BONECAMMODE);
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnTheaterControlsDrawn, (uintptr_t)&DrawBoneCameraControls);
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnMouseRightButtonClicked, (uintptr_t)&IncreaseSelectedBoneNumber);
		T6SDK::Events::RegisterListener(T6SDK::EventType::OnMouseLeftButtonClicked, (uintptr_t)&DecreaseSelectedBoneNumber);
		AttachToBoneButton = T6SDK::Drawing::UI_KeyReactiveText("^7Press ^3F ^7to attach to selected bone", 8, 33, &T6SDK::Input::Keys::F, 0x00, T6SDK::AnchorPoint::BottomCenter, (uintptr_t)&BoneCamera::SwitchBoneState);
	}
}