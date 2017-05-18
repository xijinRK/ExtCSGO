#pragma once
#include "Engine.h"
#include "Settings.h"
#include "Features\Features.h"

namespace ExtCSGO
{
	class Update
	{
		bool			m_Enabled;
	public:
		Engine*			m_Engine;
	public:
		Update();
		~Update();
		bool			IsEnabled() const;
		void			SetEnabled(const bool & v);
		int				Run();
	};
}