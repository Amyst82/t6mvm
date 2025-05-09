#pragma once
#include "UIControls.h"
#include "CustomDvars.h"
namespace DofMenu
{
	static string GetCFG()
	{
		std::string cfg =
			"//DOF\n" +
			std::string("r_dof_tweak ") + std::to_string(T6SDK::Dvars::GetBool(*T6SDK::Dvars::DvarList::r_dof_enable) ? 1 : 0) + ";\n" +
			std::string("r_dof_enable ") + std::to_string(T6SDK::Dvars::GetBool(*T6SDK::Dvars::DvarList::r_dof_tweak) ? 1 : 0) + ";\n" +
			std::string("r_dof_farBlur ") + std::to_string(T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_dof_farBlur)) + ";\n" +
			std::string("r_dof_farStart ") + std::to_string(T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_dof_farStart)) + ";\n" +
			std::string("r_dof_farEnd ") + std::to_string(T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_dof_farEnd)) + ";\n" +
			std::string("r_dof_nearBlur ") + std::to_string(T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_dof_nearBlur)) + ";\n" +
			std::string("r_dof_nearStart ") + std::to_string(T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_dof_nearStart)) + ";\n" +
			std::string("r_dof_nearEnd ") + std::to_string(T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_dof_nearEnd)) + ";\n" +
			std::string("r_dof_viewModelStart ") + std::to_string(T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_dof_viewModelStart)) + ";\n" +
			std::string("r_dof_viewModelEnd ") + std::to_string(T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_dof_viewModelEnd)) + ";\n" +
			std::string("r_dof_farEnd ") + std::to_string(T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_dof_farEnd)) + ";\n" +
			std::string("r_dof_bias ") + std::to_string(T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_dof_bias)) + ";\n" +
			std::string("r_dofHDR ") + std::to_string(T6SDK::Dvars::GetInt(*T6SDK::Dvars::DvarList::r_dofHDR)) + ";\n";
		return cfg;
	}
	void ResetDof()
	{
		UIControls::UI_DofFarBlur.Reset();
		UIControls::UI_DofFarStart.Reset();
		UIControls::UI_DofFarEnd.Reset();
		UIControls::UI_DofNearBlur.Reset();
		UIControls::UI_DofNearStart.Reset();
		UIControls::UI_DofNearEnd.Reset();
		UIControls::UI_DofViewmodelStart.Reset();
		UIControls::UI_DofViewmodelEnd.Reset();
		UIControls::UI_DofBias.Reset();
		*UIControls::UI_DofHDR.SelectedValue = 2;
	}
	void ToggleDOF(T6SDK::Drawing::UI_CheckBoxButton* control)
	{
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_dof_enable, T6SDK::Dvars::GetBool(*T6SDK::Dvars::DvarList::r_dof_tweak));
	}
	static void Init()
	{
		UIControls::UI_DofToggle = T6SDK::Drawing::UI_CheckBoxButton("DOF OFF", "DOF ON", 4, 5, T6SDK::AnchorPoint::TopLeft, &(*T6SDK::Dvars::DvarList::r_dof_tweak)->current.enabled, (uintptr_t)&ToggleDOF);
		UIControls::UI_DofToggle.ToolTip = "Use dvars to set the depth of field effect; overrides ^5r_dof_enable.";

		UIControls::UI_DofFarBlur = T6SDK::Drawing::UI_Slider("FAR BLUR", &(*T6SDK::Dvars::DvarList::r_dof_farBlur)->current.value, 1.8f, 0.0f, 10.0f, 4, 8, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_DofFarBlur.ToolTip = "Sets the radius of the gaussian blur used by depth of field.";

		UIControls::UI_DofFarStart = T6SDK::Drawing::UI_Slider("FAR START", &(*T6SDK::Dvars::DvarList::r_dof_farStart)->current.value, 1000.0f, 0.0f, 1000.0f, 7, 8, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_DofFarStart.ToolTip = "Depth of field far start distance, in inches.";

		UIControls::UI_DofFarEnd = T6SDK::Drawing::UI_Slider("FAR END", &(*T6SDK::Dvars::DvarList::r_dof_farEnd)->current.value, 7000.0f, 0.0f, 7000.0f, 10, 8, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_DofFarEnd.ToolTip = "Depth of field far end distance, in inches.";

		UIControls::UI_DofNearBlur = T6SDK::Drawing::UI_Slider("NEAR BLUR", &(*T6SDK::Dvars::DvarList::r_dof_nearBlur)->current.value, 6.0f, 0.0f, 10.0f, 4, 12, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_DofNearBlur.ToolTip = "Sets the radius of the gaussian blur used by depth of field.";

		UIControls::UI_DofNearStart = T6SDK::Drawing::UI_Slider("NEAR START", &(*T6SDK::Dvars::DvarList::r_dof_nearStart)->current.value, 10.0f, 0.0f, 1000.0f, 7, 12, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_DofNearStart.ToolTip = "Depth of field near start distance, in inches.";

		UIControls::UI_DofNearEnd = T6SDK::Drawing::UI_Slider("NEAR END", &(*T6SDK::Dvars::DvarList::r_dof_nearEnd)->current.value, 60.0f, 0.0f, 2000.0f, 10, 12, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_DofNearEnd.ToolTip = "Depth of field near end distance, in inches.";

		UIControls::UI_DofViewmodelStart = T6SDK::Drawing::UI_Slider("VIEWMODEL START", &(*T6SDK::Dvars::DvarList::r_dof_viewModelStart)->current.value, 2.0f, 0.0f, 50.0f, 4, 16, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_DofViewmodelStart.ToolTip = "Depth of field viewmodel start distance, in inches.";

		UIControls::UI_DofViewmodelEnd = T6SDK::Drawing::UI_Slider("VIEWMODEL END", &(*T6SDK::Dvars::DvarList::r_dof_viewModelEnd)->current.value, 8.0f, 0.0f, 50.0f, 7, 16, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_DofViewmodelEnd.ToolTip = "Depth of field viewmodel end distance, in inches.";

		UIControls::UI_DofBias = T6SDK::Drawing::UI_Slider("BIAS", &(*T6SDK::Dvars::DvarList::r_dof_bias)->current.value, 0.5f, 0.0f, 1.0f, 10, 16, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_DofBias.ToolTip = "Depth of field bias as a power function (like gamma); less than ^51 ^7is sharper.";

		UIControls::UI_DofHDR = T6SDK::Drawing::UI_EnumButton("QUIALITY", 0, 2, &(*T6SDK::Dvars::DvarList::r_dofHDR)->current.integer, 4, 19, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_DofHDR.ToolTip = "DOF mode, 0 = legacy 1 = HDR low, 2 = HDR high.";

		UIControls::UI_DofReset = T6SDK::Drawing::UI_ClickableButton("RESET DOF", 8, 32, T6SDK::AnchorPoint::Center, (uintptr_t)&ResetDof);
		UIControls::UI_DofReset.ToolTip = "Reset depth of field settings.";
	}
	static void Draw()
	{
		if (!T6SDK::Theater::IsInTheater())
			return;
		if (!*UIControls::DofTabButton.isChecked)
			return;

		UIControls::UI_DofToggle.Draw();
		UIControls::UI_DofFarBlur.Draw(*UIControls::UI_DofToggle.isChecked);
		UIControls::UI_DofFarStart.Draw(*UIControls::UI_DofToggle.isChecked);
		UIControls::UI_DofFarEnd.Draw(*UIControls::UI_DofToggle.isChecked);
		UIControls::UI_DofNearBlur.Draw(*UIControls::UI_DofToggle.isChecked);
		UIControls::UI_DofNearStart.Draw(*UIControls::UI_DofToggle.isChecked);
		UIControls::UI_DofNearEnd.Draw(*UIControls::UI_DofToggle.isChecked);
		UIControls::UI_DofViewmodelStart.Draw(*UIControls::UI_DofToggle.isChecked);
		UIControls::UI_DofViewmodelEnd.Draw(*UIControls::UI_DofToggle.isChecked);
		UIControls::UI_DofBias.Draw(*UIControls::UI_DofToggle.isChecked);

		switch (T6SDK::Dvars::GetInt(*T6SDK::Dvars::DvarList::r_dofHDR))
		{
		case 0:
			UIControls::UI_DofHDR.Text = "QUIALITY: LEGACY";
			break;
		case 1:
			UIControls::UI_DofHDR.Text = "QUIALITY: LOW";
			break;
		case 2:
			UIControls::UI_DofHDR.Text = "QUIALITY: HIGH";
			break;
		default:
			UIControls::UI_DofHDR.Text = "QUIALITY: LEGACY";
			break;
		}
		UIControls::UI_DofHDR.Draw(*UIControls::UI_DofToggle.isChecked);
		UIControls::UI_DofReset.Draw(*UIControls::UI_DofToggle.isChecked);
	}
}