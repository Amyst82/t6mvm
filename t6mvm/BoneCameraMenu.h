#pragma once
#include <StdInclude.h>
#include "BoneCamera.h"
#include "UIControls.h"
#include "CustomDvars.h"
namespace BoneCameraMenu
{

	inline static void Init()
	{
		UIControls::UI_SelectedBoneButton = T6SDK::Drawing::UI_EnumButton("SELECTED BONE: ", 0, BoneCamera::Bones.size()-1, &BoneCamera::SelectedBone, 4, 5, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_AttachToBoneButton = T6SDK::Drawing::UI_CheckBoxButton("Status: unattached", "Status: attached", 4, 7, T6SDK::AnchorPoint::TopLeft, &BoneCamera::BoneAttached, (uintptr_t)&BoneCamera::SwitchBoneState);
		UIControls::UI_AttachToBoneButton.ValueReadOnly = true;
		UIControls::UI_ShowBonesButton = T6SDK::Drawing::UI_CheckBoxButton("Show selected bone", "Hide selected bone", 4, 9, T6SDK::AnchorPoint::TopLeft, &CustomDvars::dvar_showBone->current.enabled, 0x00);
		UIControls::UI_ShowBonesButton.ToolTip = "Show or hide selected bone marker in demo. However even if it's enabled, it will not be visible when the theater controls are hidden.";
	}
	void DrawAllAvailableBones()
	{
		vec3_t pos{};
		Matrix33_s rot{};
		int playerindex = T6SDK::Dvars::GetInt(*T6SDK::Dvars::DvarList::demo_client);

		for (int i = 0; i < BoneCamera::Bones.size(); i++)
		{
			if (T6SDK::InternalFunctions::CG_DObjGetWorldTagMatrix(T6SDK::InternalFunctions::CG_GetEntity(playerindex), BoneCamera::Bones[i].Index, &rot, &pos))
			{
				vec2_t screenPos{};
				if (T6SDK::InternalFunctions::WorldPosToScreenPos(&pos, &screenPos))
				{

					RECT rect{};
					if (T6SDK::Drawing::DrawRectAbsolute(screenPos.x, screenPos.y, 10.0f, 10.0f, BoneCamera::SelectedBone == i ? T6SDK::Drawing::REDCOLOR : T6SDK::Drawing::YELLOWCOLOR, T6SDK::AnchorPoint::Center, &rect))
					{
						if (T6SDK::Input::MousePosX() > (float)rect.left && T6SDK::Input::MousePosX() < (float)rect.right && T6SDK::Input::MousePosY() > (float)rect.top && T6SDK::Input::MousePosY() < (float)rect.bottom)
						{
							T6SDK::Drawing::DrawTextAbsolute(BoneCamera::Bones[i].Name, T6SDK::Input::MousePosX()+25.0f, T6SDK::Input::MousePosY()+10.0f, 1.0f, T6SDK::Drawing::WHITECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
							if (T6SDK::Input::Keys::LMB.Pressed())
								BoneCamera::SelectedBone = i;
						}
					}
				}
			}
		}
	}
	inline static void Draw()
	{
		if (*UIControls::BoneCameraTabButton.isChecked)
		{
			if(BoneCamera::IsBoneCamera())
				DrawAllAvailableBones();
			//Draw other controls here
			char buffer[100];
			sprintf(buffer, "Selected bone: %s", BoneCamera::Bones[BoneCamera::SelectedBone].Name);
			UIControls::UI_SelectedBoneButton.Text = buffer;
			UIControls::UI_SelectedBoneButton.Draw();
			UIControls::UI_AttachToBoneButton.Draw();
			UIControls::UI_ShowBonesButton.Draw();
		}
	}
}