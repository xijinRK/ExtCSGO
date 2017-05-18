#include "Update.h"

namespace ExtCSGO
{
	static void UpdateThread(Update* update);
	Update::Update() :
		m_Enabled(false)
	{
		if (Settings::GetSettings()->LoadSettings())
		{
			m_Engine = new Engine();
		}
		else
		{
			exit(0);
		}
	}

	Update::~Update()
	{
		delete m_Engine;
		Settings::DeleteSettings();
	}

	bool Update::IsEnabled() const
	{
		return m_Enabled;
	}

	void Update::SetEnabled(const bool & v)
	{
		m_Enabled = v;
	}

	int Update::Run()
	{
		auto hUpdate = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)UpdateThread, (Update*)this, 0, 0);
		while (true)
		{
			Sleep(1);
			if (this->IsEnabled())
			{
				if (m_Engine->IsValid())
				{
					Features::Aimbot(m_Engine);
					Features::Triggerbot(m_Engine);
				}
			}
		}
		CloseHandle(hUpdate);
		return 0;
	}

	static void UpdateThread(Update* update)
	{
		while (true)
		{
			Sleep(1);
			if (update->IsEnabled())
			{ 
				if (update->m_Engine->IsValid())
				{
					update->m_Engine->UpdateEvents();
				}
				else
				{
					#ifdef _DEBUG
					#else
					Sleep(5000);
					#endif
					update->m_Engine->Update();
				}
			}
			if (GetAsyncKeyState(VK_INSERT) & 1)
			{
				update->SetEnabled(!update->IsEnabled());
			}
		}
		return ExitThread(0);
	}
}