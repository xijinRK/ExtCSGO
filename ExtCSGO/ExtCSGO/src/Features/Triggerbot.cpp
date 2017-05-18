#include "Features\Features.h"

namespace ExtCSGO::Features
{
	using namespace sdk;


	enum ButtonState : bool
	{
		Button_Press = true,
		Button_Release = false
	};

	static bool GetTarget(const Engine* engine);
	static void ClickLeftButton(const bool & Flags, const int & ClickBurst);

	void Triggerbot(const Engine* engine, const Settings* settings)
	{
		if (GetAsyncKeyState(settings->m_TriggerKey))
		{
			if (GetTarget(engine))
			{
				ClickLeftButton(Button_Press, settings->m_TriggerBurst);
			}

			else
			{
				ClickLeftButton(Button_Release, settings->m_TriggerBurst);
			}
		}
		else
		{
			ClickLeftButton(Button_Release, settings->m_TriggerBurst);
		}
	}

	static bool GetTarget(const Engine* engine)
	{
		auto LocalPlayer = engine->GetIClientEnt()->GetClientEntity(engine->GetIVEngine()->GetLocalPlayer());

		int EntityIndex = (LocalPlayer->GetCrosshairId() - 1);

		if (EntityIndex > MaxEntityIndex || EntityIndex < 0)
			return false;

		auto TargetPlayer = engine->GetIClientEnt()->GetClientEntity(EntityIndex);

		return (TargetPlayer->GetTeamNum() != LocalPlayer->GetTeamNum() && TargetPlayer->GetHealth() > 0);
	}


	static void ClickLeftButton(const bool & Flags, const int & ClickBurst)
	{
		static bool Click = false;
		switch (Flags)
		{
			case Button_Press:
			{
				if (Click == false)
				{
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					Click = true;
				}
				break;
			}

			case Button_Release:
			{
				if (Click == true)
				{
					bool Release = false;
					if (ClickBurst)
					{
						static auto TickCount = GetTickCount();
						if (GetTickCount() - TickCount > (unsigned int)ClickBurst)
						{
							TickCount = GetTickCount();
							Release = true;
						}
					}
					else
					{
						Release = true;
					}

					if (Release == true)
					{
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
						Click = false;
					}
				}
				break;
			}
		}
	}
}