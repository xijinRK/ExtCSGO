#include "sdk\IVEngineClient.h"
#include "Engine.h"

namespace ExtCSGO::sdk
{
	int IVEngineClient::GetLocalPlayer() const
	{
		static int Angles;
		GetArrayData(m_IVEngine, m_GetLocalPlayer, &Angles);
		return Angles;
	}

	vec3 IVEngineClient::GetViewAngles() const
	{
		static vec3 Angles;
		GetArrayData(m_IVEngine, m_ViewAngles, &Angles);
		return Angles;
	}

	void IVEngineClient::Update(const Engine* engine) const
	{
		static DWORD Ptr = 0;
		if (!engine->GetProcess()->ReadMemory
		(
			(PVOID)((DWORD64)engine->GetEngineDLL()->GetdwBaseAddress() + m_ClientState),
			&Ptr, sizeof(vec3)
		))
		{
			return;
		}

		if (!engine->GetProcess()->ReadMemory
		(
			(PVOID)(DWORD64)(Ptr),
			(LPVOID)this, sizeof(IVEngineClient)
		))
		{
			return;
		}
	}
}