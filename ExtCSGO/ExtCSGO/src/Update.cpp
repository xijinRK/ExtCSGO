#include "Update.h"

#include <iostream>
namespace ExtCSGO
{
	using namespace Features;
	Update::Update(char* argv[]):
		m_Aimbot(new Aimbot(std::atoi(argv[1]), std::atof(argv[2]), std::atoi(argv[3]), std::atof(argv[4]), this)),
		m_Running(false)
	{
	}

	Update::~Update()
	{
		delete m_Aimbot;
	}

	int Update::Run()
	{
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)UpdateThread, (Update*)this, 0, 0);
		while (true)
		{
			Sleep(1);
			if (m_Running == true)
			{
				if (Engine::IsValid())
				{
					m_Aimbot->Run();		
				}
			}
		}
		return 0;
	}

	void UpdateThread(Update* update)
	{
		while (true)
		{
			Sleep(1);
			if (update->m_Running == true)
			{ 
				if (update->Engine::IsValid())
				{
					update->Engine::UpdateEvents();
				}
				else
				{
					#ifdef _DEBUG
					#else
					Sleep(5000);
					#endif
					update->Engine::Update();
				}
			}
			if (GetAsyncKeyState(VK_INSERT) & 1)
			{
				update->m_Running = !update->m_Running;
			}
		}
	}
}