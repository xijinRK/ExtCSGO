#pragma once
#include <string>
namespace ExtCSGO
{
	class Settings
	{
	public:
		Settings();
		~Settings();
		std::string        m_GamePath;
		std::string        m_LaunchOptions;
		int                m_AimKey;
		double             m_AimFov;
		int                m_AimSmooth;
		double             m_Sensitivity;
		int                m_TriggerKey;
		int                m_TriggerBurst;
		bool               LoadSettings();
	};

	
}