#pragma once
#include "Settings.h"
#include "Engine.h"
#include "Features\Features.h"

namespace ExtCSGO
{
	class Update
	{
		Settings*       m_Settings;
		Engine*         m_Engine;
		bool            m_Enabled;
	public:
		Update();
		~Update();
		Settings*       GetSettings() const;
		Engine*         GetEngine() const;
		bool            IsEnabled() const;
		void            SetEnabled(const bool & v);
		int             Run();
	};
}