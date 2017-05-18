#pragma once
#include "..\Maths\Math.h"
#include "..\Netvars.h"

namespace ExtCSGO
{
	class Engine;
}

namespace ExtCSGO::sdk
{
	class IVEngineClient
	{
		char 	m_IVEngine[m_ViewAngles + 0xC];
	public:
		int 	GetLocalPlayer() const;
		vec3 	GetViewAngles() const;
		void 	Update(const Engine* engine) const;
	};
}