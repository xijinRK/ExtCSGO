#pragma once
#include "Engine.h"
#include "Features\Aimbot.h"

namespace ExtCSGO
{
	class Update : public Engine
	{
        Features::Aimbot* m_Aimbot;
        bool              m_Running;		
	public:
		Update(char* argv[]);
		~Update();
		int Run();
	private:
		friend void UpdateThread(Update* update);
	};
}