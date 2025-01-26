#pragma once
#include <StdInclude.h>
#include "Misc.h"
#include "UIControls.h"
#include "CustomDvars.h"

namespace MiscMenu
{
	static void Draw()
	{
		if(!*UIControls::MiscTabButton.isChecked)
			return;
		UIControls::UI_GreenScreen.Draw();
		char buffer[100];
		sprintf(buffer, "Players green screen: %s", *UIControls::UI_PlayersGreenScreen.SelectedValue == 0 ? "No" : *UIControls::UI_PlayersGreenScreen.SelectedValue == 1 ? "Selected player" : *UIControls::UI_PlayersGreenScreen.SelectedValue == 2 ? "Threshold" : "All");
		UIControls::UI_PlayersGreenScreen.Text = buffer;
		UIControls::UI_PlayersGreenScreen.Draw(*UIControls::UI_GreenScreen.isChecked);
		UIControls::UI_PlayersGreenScreenThreshold.Draw(*UIControls::UI_PlayersGreenScreen.SelectedValue == 2);
	}

	static void Init()
	{
		UIControls::UI_GreenScreen = T6SDK::Drawing::UI_CheckBoxButton("Green screen\t\t\t\tOFF", "Green screen\t\t\t\tON", 4, 5, T6SDK::AnchorPoint::TopLeft, &CustomDvars::dvar_greenScreen->current.enabled, (uintptr_t)&Misc::SwitchGreenScreenState);
		UIControls::UI_GreenScreen.ToolTip = "Turn on/off green screen for POV and players.";

		UIControls::UI_PlayersGreenScreen = T6SDK::Drawing::UI_EnumButton("Players green screen: ", 0, 3, &CustomDvars::dvar_greenScreenPlayers->current.integer, 4, 7, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_PlayersGreenScreen.ToolTip = "You can either green screen selected player, all players, or all players that are in certain distance.";

		UIControls::UI_PlayersGreenScreenThreshold = T6SDK::Drawing::UI_Slider("Players green screen threshold", &CustomDvars::dvar_greenScreenPlayersThreshold->current.value, 500.0f, 0.0f, 5000.0f, 4, 9, T6SDK::Drawing::ORANGECOLOR, T6SDK::AnchorPoint::TopLeft, 0x00);
		UIControls::UI_PlayersGreenScreenThreshold.ToolTip = "Distance threshold for green screen players.";
	}
}