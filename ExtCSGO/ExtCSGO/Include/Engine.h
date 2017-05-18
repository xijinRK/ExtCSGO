#pragma once
#include "Engine\Memory.h"

#include "sdk\Player.h"
#include "sdk\IVEngineClient.h"
#include "sdk\IClientEntityList.h"

namespace ExtCSGO
{
	class Engine
	{
		Process*           m_Process;
sdk::   IClientEntityList* m_IClientEntity;
sdk::   IVEngineClient*    m_IVEngineClient;
		Module*            m_ClientDLL;
		Module*            m_EngineDLL;

	public:
		Engine();
		~Engine();

		Process*					GetProcess() const;
		Module*						GetClientDLL() const;
		Module*						GetEngineDLL() const;

		sdk::IClientEntityList*		GetIClientEnt() const;
		sdk::IVEngineClient*		GetIVEngine() const;

		void						Update() const;
		void						UpdateEvents();
		bool						IsValid() const;

	private:
		bool						IsHandleValid() const;
		bool						IsModuleValid() const;
	};
}