#pragma once
#include "Engine.h"
#include "sdk\Player.h"


namespace ExtCSGO::Features
{
	class Aimbot
	{
		int					m_AimKey;
		double				m_AimFov;
		DWORD				m_AimSmooth;
		double				m_Sensitivity;

	    const Engine*		m_Engine;
sdk::   IClientEntityList*	m_EntList;
sdk::   IVEngineClient*		m_IVEngine;

sdk::   Player*             m_BestTarget;
        int                 m_BestIndex;
		float               m_BestFov;
	public:
		Aimbot(const int &AimKey, const double & AimFov, const DWORD & AimSmooth, const double & Sensitivity, const Engine* Engine);
		~Aimbot();

		void Run();
	private:
		void ResetTarget();


		bool GetBestTarget (
			const sdk::Player*  LocalPlayer,
			const vec3 &		ViewAngles,
			sdk::Player**       Target,
			int*                Index,
			float*              BestFov) const;

		vec3 GetAimAngles (
			const sdk::Player*	Local,
			const sdk::Player*	Entity,
			const int &         Index) const;

		void PixelMove(
			const vec3 &		ViewAngles,
			const vec3 &		AimAngles,
			const float &       Sensitivity,
			const DWORD &       Smooth) const;
	};
}