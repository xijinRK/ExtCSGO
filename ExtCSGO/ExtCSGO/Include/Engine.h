#pragma once
#include "Settings.h"

#include "Engine\Memory.h"
#include "sdk\Player.h"
#include "sdk\IVEngineClient.h"
#include "sdk\IClientEntityList.h"

namespace ExtCSGO
{
	class Engine
	{
		Process*                    m_Process;
		Module*                     m_ClientDLL;
		Module*                     m_EngineDLL;
		sdk::IClientEntityList*     m_IClientEntity;
		sdk::IVEngineClient*        m_IVEngineClient;
	public:
		Engine(const Settings* settings);
		~Engine();
		Process*                    GetProcess() const;
		Module*                     GetClientDLL() const;
		Module*                     GetEngineDLL() const;

		sdk::IClientEntityList*     GetIClientEnt() const;
		sdk::IVEngineClient*        GetIVEngine() const;
		void                        Update() const;
		void                        UpdateEvents();
		bool                        IsValid() const;

	private:
		bool                        IsHandleValid() const;
		bool                        IsModuleValid() const;
	};
}