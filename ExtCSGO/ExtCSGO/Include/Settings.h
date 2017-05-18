#pragma once

namespace ExtCSGO
{
	class Settings
	{
		static Settings*	m_Settings;
	public:
		Settings();
		~Settings();
		const char*		m_GamePath;
		const char*		m_LaunchOptions;
		int			m_AimKey;
		double			m_AimFov;
		int			m_AimSmooth;
		double			m_Sensitivity;
		int			m_TriggerKey;
		int			m_TriggerBurst;
	public:
		bool			LoadSettings();
		static Settings*	GetSettings();
		static void		DeleteSettings();
	};

	
}