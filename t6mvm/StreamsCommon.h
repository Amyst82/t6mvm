#pragma once
#include <StdInclude.h>
namespace StreamsCommon
{
	inline static bool prevBloomTweaks = false;
	inline static int prevClearColor = 0;
	inline static int prevClearColor2 = 0;
	inline static float prevDOFBias = 0.5f;
	inline static int prevDOFHDR = 2;
	inline static bool prevDOFTweak = false;
	inline static bool prevDOFEnable = false;
	inline static float prevDOFFarBlur = 1.0f;
	inline static float prevDOFFarStart = 1000.0f;
	inline static float prevDOFFarEnd = 7000.0f;
	inline static float prevDOFNearBlur = 1.0f;
	inline static float prevDOFNearStart = 100.0f;
	inline static float prevDOFNearEnd = 2000.0f;
	inline static float prevDOFViewModelStart = 0.5f;
	inline static float prevDOFViewModelEnd = 0.5f;

	inline static bool prevSsaoEnabled = false;
	inline static float prevSsaoScale = 0.0f;
	inline static float prevSsaoRadius = 0.0f;
	inline static float prevSsaoBias = 0.0f;
	inline static float prevSsaoIntensity = 0.0f;



	static void CapturePreviousDof()
	{
		prevBloomTweaks = T6SDK::Dvars::GetBool(*T6SDK::Dvars::DvarList::r_bloomTweaks);
		prevClearColor = T6SDK::Dvars::GetInt(*T6SDK::Dvars::DvarList::r_clearColor);
		prevClearColor2 = T6SDK::Dvars::GetInt(*T6SDK::Dvars::DvarList::r_clearColor2);
		prevDOFBias = T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_dof_bias);
		prevDOFTweak = T6SDK::Dvars::GetBool(*T6SDK::Dvars::DvarList::r_dof_tweak);
		prevDOFEnable = T6SDK::Dvars::GetBool(*T6SDK::Dvars::DvarList::r_dof_enable);
		prevDOFHDR = T6SDK::Dvars::GetInt(*T6SDK::Dvars::DvarList::r_dofHDR);
		prevDOFFarBlur = T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_dof_farBlur);
		prevDOFFarStart = T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_dof_farStart);
		prevDOFFarEnd = T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_dof_farEnd);
		prevDOFNearBlur = T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_dof_nearBlur);
		prevDOFNearStart = T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_dof_nearStart);
		prevDOFNearEnd = T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_dof_nearEnd);
		prevDOFViewModelStart = T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_dof_viewModelStart);
		prevDOFViewModelEnd = T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_dof_viewModelEnd);
		//T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, false, "STREAMS", "Previous DOF settings captured!");
	}
	static void SetPreviousDof()
	{
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_bloomTweaks, prevBloomTweaks);
		T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_clearColor, prevClearColor);
		T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_clearColor2, prevClearColor2);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_bias, prevDOFBias);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_dof_tweak, prevDOFTweak);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_dof_enable, prevDOFEnable);
		T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_dofHDR, prevDOFHDR);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_farBlur, prevDOFFarBlur);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_farStart, prevDOFFarStart);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_farEnd, prevDOFFarEnd);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_nearBlur, prevDOFNearBlur);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_nearStart, prevDOFNearStart);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_nearEnd, prevDOFNearEnd);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_viewModelStart, prevDOFViewModelStart);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_dof_viewModelEnd, prevDOFViewModelEnd);
		//T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_DEBUG, false, "STREAMS", "Previous DOF settings restored.");
	}

	static void CapturePreviousSsao()
	{
		prevSsaoEnabled = T6SDK::Dvars::GetBool(*T6SDK::Dvars::DvarList::r_ssao);
		prevSsaoScale = T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_ssaoScale);
		prevSsaoRadius = T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_ssaoRadius);
		prevSsaoBias = T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_ssaoBias);
		prevSsaoIntensity = T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_ssaoIntensity);
	}

	static void SetPreviousSsao()
	{
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_ssao, prevSsaoEnabled);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_ssaoScale, prevSsaoScale);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_ssaoRadius, prevSsaoRadius);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_ssaoBias, prevSsaoBias);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_ssaoIntensity, prevSsaoIntensity);
	}
}