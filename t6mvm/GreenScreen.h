#pragma once
#include "IMVMStream.h"
class Stream_GreenScreen : public IMVMStream
{
private:
	bool prevFxMarksDraw = true;
	float prevZNear = 4.0f;
	float prevZFar = 0.0f;
	bool prevFlareEffects = true;
	bool prevSunFlares = true;
	bool prevDrawGun = true;
	BYTE prevBaseTechType = 0x04;
	BYTE prevEmissiveTechType = 0x03;
public:
	void Enable() 
	{
		prevBaseTechType = T6SDK::Addresses::GfxDrawMethod.Value().baseTechType;
		prevEmissiveTechType = T6SDK::Addresses::GfxDrawMethod.Value().emissiveTechType;
		prevFxMarksDraw = T6SDK::Dvars::GetBool(*T6SDK::Dvars::DvarList::fx_marks_draw);
		prevZNear = T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_znear);
		prevFlareEffects = T6SDK::Dvars::GetBool(*T6SDK::Dvars::DvarList::flareDisableEffects);
		prevSunFlares = T6SDK::Dvars::GetBool(*T6SDK::Dvars::DvarList::r_superFlareDraw);
		prevDrawGun = T6SDK::Dvars::GetBool(*T6SDK::Dvars::DvarList::cg_drawGun);
		prevZFar = T6SDK::Dvars::GetFloat(*T6SDK::Dvars::DvarList::r_zfar);
		//T6SDK::Addresses::Patches::RemoveSkyBoxPatch.Patch();
		T6SDK::Addresses::GfxDrawMethod.Value().baseTechType = 0x04;
		T6SDK::Addresses::GfxDrawMethod.Value().emissiveTechType = 0x01;
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_skipPvs, true);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_bloomTweaks, true);

		float clrR = max(0.0f, min(1.0f, CustomDvars::dvar_greenScreenColor->current.vector[0]));
		(*T6SDK::Dvars::DvarList::r_clearColor)->current.color[0] = static_cast<unsigned char>(clrR * 255.0f + 0.5f);
		(*T6SDK::Dvars::DvarList::r_clearColor2)->current.color[0] = static_cast<unsigned char>(clrR * 255.0f + 0.5f);

		float clrG = max(0.0f, min(1.0f, CustomDvars::dvar_greenScreenColor->current.vector[1]));
		(*T6SDK::Dvars::DvarList::r_clearColor)->current.color[1] = static_cast<unsigned char>(clrG * 255.0f + 0.5f);
		(*T6SDK::Dvars::DvarList::r_clearColor2)->current.color[1] = static_cast<unsigned char>(clrG * 255.0f + 0.5f);

		float clrB = max(0.0f, min(1.0f, CustomDvars::dvar_greenScreenColor->current.vector[2]));
		(*T6SDK::Dvars::DvarList::r_clearColor)->current.color[2] = static_cast<unsigned char>(clrB * 255.0f + 0.5f);
		(*T6SDK::Dvars::DvarList::r_clearColor2)->current.color[2] = static_cast<unsigned char>(clrB * 255.0f + 0.5f);

		//T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_clearColor, 0x0000FF00);
		//T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_clearColor2, 0x0000FF00);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::fx_marks_draw, false);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_zfar, 1.0f);
		//T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_znear, 10000.0f);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::flareDisableEffects, true);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_superFlareDraw, false);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::cg_drawGun, true);
		Enabled = true;
	}
	void Disable()
	{
		//T6SDK::Addresses::Patches::RemoveSkyBoxPatch.UnPatch();
		
		T6SDK::Addresses::GfxDrawMethod.Value().baseTechType= prevBaseTechType;
		T6SDK::Addresses::GfxDrawMethod.Value().emissiveTechType = prevEmissiveTechType;

		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_skipPvs, false);
		T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_clearColor, 0x00);
		T6SDK::Dvars::SetInt(*T6SDK::Dvars::DvarList::r_clearColor2, 0x00);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::fx_marks_draw, prevFxMarksDraw);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_znear, prevZNear);
		T6SDK::Dvars::SetFloat(*T6SDK::Dvars::DvarList::r_zfar, prevZFar);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::flareDisableEffects, prevFlareEffects);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::r_superFlareDraw, prevSunFlares);
		T6SDK::Dvars::SetBool(*T6SDK::Dvars::DvarList::cg_drawGun, prevDrawGun);
		StreamsCommon::SetPreviousDof();
		Enabled = false;
	}
	void Init()
	{
		IMVMStream::Name = "GreenScreen";
		IMVMStream::toggle = T6SDK::Dvars::RegisterBool("mvm_streams_greenScreen", false, "Green screen stream.");
	}
};