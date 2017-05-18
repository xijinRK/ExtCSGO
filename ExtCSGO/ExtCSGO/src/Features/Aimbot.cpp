#include "Features\Features.h"
#include "Maths\Math.h"

namespace ExtCSGO::Features
{
	using namespace sdk;
	using namespace Maths;

	static bool GetBestTarget (
		const IClientEntityList*	EntityList,
		const Player*			LocalPlayer,
		const vec3&			ViewAngles,
		Player**			Target,
		int*				Index,
		float*				BestFov);

	static vec3 CalculateAngles (
		const Player*	Local,
		const Player*	Entity,
		vec3&		BonePosition);

	static void PixelMove (
		const vec3&	ViewAngles,
		const vec3&	AimAngles,
		const float&	Sensitivity,
		const DWORD&	Smooth);

	void Aimbot(const Engine* Engine)
	{
		static Player*		BestTarget = nullptr;
		static int		BestIndex = -1;
		static float		BestFov = 180.f;

		if (GetAsyncKeyState(Settings::GetSettings()->m_AimKey))
		{
			auto LocalPlayer = Engine->GetIClientEnt()->GetClientEntity(Engine->GetIVEngine()->GetLocalPlayer());
			if (!LocalPlayer->IsValid())
				return;

			if (!GetBestTarget(Engine->GetIClientEnt(), LocalPlayer, Engine->GetIVEngine()->GetViewAngles(), &BestTarget, &BestIndex, &BestFov))
			{
				BestTarget = nullptr;
				BestIndex = -1;
				BestFov = 180.f;
				return;
			}

			auto & AimAngles = CalculateAngles(LocalPlayer, BestTarget, Engine->GetIClientEnt()->GetHeadBone(BestIndex));
			if (BestFov < (float)Settings::GetSettings()->m_AimFov)
			{
				PixelMove
				(
					Engine->GetIVEngine()->GetViewAngles(),
					AimAngles,
					(float)Settings::GetSettings()->m_Sensitivity,
					Settings::GetSettings()->m_AimSmooth
				);
			}
		}
		else
		{
			BestTarget = nullptr;
			BestIndex = -1;
			BestFov = 180.f;
		}
	}

	static bool GetBestTarget (
		const IClientEntityList*	EntityList,
		const Player*			LocalPlayer,
		const vec3&			ViewAngles,
		Player**			Target,
		int*				Index,
		float*				BestFov)
	{
		for (auto i = 1; i < MaxEntityIndex; i++)
		{
            		auto targetPlayer = EntityList->GetClientEntity(i);
            		if (!targetPlayer->IsValid())
				continue;

			if (targetPlayer->GetTeamNum() == LocalPlayer->GetTeamNum())
				continue;

			auto & AimAngles = CalculateAngles(LocalPlayer, targetPlayer, EntityList->GetHeadBone(i));
			
			auto Fov = GetFov(ViewAngles, AimAngles);

			if (Fov < *BestFov)
			{
				*BestFov = Fov;
				*Target = targetPlayer;
				*Index = i;
			}		
		}
		auto & target = *Target;
		if (target != nullptr)
		{
			if (!target->IsValid())
			{
				return false;
			}
		}
		else
		{
			return false;
		}
		return true;
	}


	static vec3 CalculateAngles (
		const Player*	Local,
		const Player*	Entity,
		vec3&		BonePosition)
	{

		auto & BoneCoords = BonePosition;
		BoneCoords -= Local->GetEyePosition();

		vec3 AimAngles;
		VectorNormalize(BoneCoords);
		VectorAngles(BoneCoords, AimAngles);

		if (Local->GetShotsFired() > 1)
		{
			AimAngles -= Local->GetVecPunch() * 2.0f;
		}

		ClampAngles(AimAngles);
		return AimAngles;
	}

	static void PixelMove(
		const vec3&	ViewAngles,
		const vec3&	AimAngles,
		const float&	Sensitivity,
		const DWORD&	Smooth)
	{
		auto & PixelAngles = ConvertAngles(Sensitivity, AimAngles, ViewAngles);
		if (Smooth > 0)
		{
			vec3 Smoothed;
			if (Smoothed.x != PixelAngles.x && std::abs(PixelAngles.x) > std::abs(PixelAngles.y))
			{
				Smoothed.x = Smoothed.x < PixelAngles.x ? Smoothed.x += 1 : (Smoothed.x > PixelAngles.x ? Smoothed.x -= 1 : Smoothed.x = PixelAngles.x);
			}
			else if (Smoothed.y != PixelAngles.y)
			{
				Smoothed.y = Smoothed.y < PixelAngles.y ? Smoothed.y += 1 : (Smoothed.y > PixelAngles.y ? Smoothed.y -= 1 : Smoothed.y = PixelAngles.y);
			}

			static auto TickCount = GetTickCount();
			if (GetTickCount() - TickCount >= Smooth - 1)
			{
				TickCount = GetTickCount();
				mouse_event(MOUSEEVENTF_MOVE, (int)Smoothed.x, (int)Smoothed.y, 0, 0);
			}
			
		}
		else
		{
			static auto TickCount = GetTickCount();
			if (GetTickCount() - TickCount > 0)
			{
				TickCount = GetTickCount();
				mouse_event(MOUSEEVENTF_MOVE, (int)PixelAngles.x, (int)PixelAngles.y, 0, 0);
			}
		}
	}

}
