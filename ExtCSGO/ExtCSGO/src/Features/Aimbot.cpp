#include "Features\Aimbot.h"
#include "Maths\Math.h"

namespace ExtCSGO::Features
{
	using namespace sdk;
	using namespace Maths;

	Aimbot::Aimbot(const int &AimKey, const double & AimFov, const DWORD & AimSmooth, const double & Sensitivity, const Engine* Engine) :
		m_AimKey(AimKey),
		m_AimFov(AimFov),
		m_AimSmooth(AimSmooth),
		m_Sensitivity(Sensitivity),
		m_Engine(Engine)
	{
		ResetTarget();
	}

	Aimbot::~Aimbot()
	{
	}

	void Aimbot::Run()
	{
		if (GetAsyncKeyState(m_AimKey))
		{
			if (!m_Engine->GetIVEngine(&m_IVEngine))
				return;

			if (!m_Engine->GetClient(&m_EntList))
				return;

			auto LocalPlayer = m_EntList->GetClientEntity(m_IVEngine->GetLocalPlayer());
			if (!LocalPlayer->IsValid())
				return;
			
			if (!GetBestTarget(LocalPlayer, m_IVEngine->GetViewAngles(), &m_BestTarget, &m_BestIndex, &m_BestFov))
			{
				ResetTarget();
				return;
			}

			auto & AimAngles = GetAimAngles(LocalPlayer, m_BestTarget, m_BestIndex);
			if (m_BestFov < (float)m_AimFov)
			{
				PixelMove(m_IVEngine->GetViewAngles(), AimAngles, (float)m_Sensitivity, m_AimSmooth);
			}		
		}
		else
		{
			ResetTarget();
		}

	}

	void Aimbot::ResetTarget()
	{
		m_BestTarget = nullptr;
		m_BestIndex = -1;
		m_BestFov = 180.f;
	}

	bool Aimbot::GetBestTarget (
		const sdk::Player*  LocalPlayer,
		const vec3 &        ViewAngles,
		sdk::Player**       Target,
		int*                Index,
		float*              BestFov) const
	{
		for (auto i = 1; i < 32; i++)
		{
            auto Enemy = m_EntList->GetClientEntity(i);
            if (!Enemy->IsValid())
				continue;

			if (Enemy->GetTeamNum() == LocalPlayer->GetTeamNum())
				continue;

			auto & AimAngles = GetAimAngles(LocalPlayer, Enemy, i);
			
			auto Fov = GetFov(ViewAngles, AimAngles);

			if (Fov < *BestFov)
			{
				*BestFov = Fov;
				*Target = Enemy;
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

	vec3 Aimbot::GetAimAngles(
		const Player*	Local,
		const Player*	Entity,
		const int &     Index) const
	{
		vec3 AimAngles, BoneCoords;
		BoneCoords = m_EntList->GetHeadBone(Index);
		BoneCoords -= Local->GetEyePosition();

		VectorNormalize(BoneCoords);
		VectorAngles(BoneCoords, AimAngles);

		if (Local->GetShotsFired() > 1)
		{
			AimAngles -= Local->GetVecPunch() * 2.0f;
		}
		ClampAngles(AimAngles);
		return AimAngles;
	}

	void Aimbot::PixelMove(
		const vec3 &	ViewAngles,
		const vec3 &	AimAngles,
		const float &	Sensitivity,
		const DWORD &	Smooth) const
	{
		vec3 PixelAngles = ConvertAngles(Sensitivity, AimAngles, ViewAngles);
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
