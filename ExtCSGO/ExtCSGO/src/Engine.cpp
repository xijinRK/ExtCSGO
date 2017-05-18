#include "Engine.h"
#include "Settings.h"

#define LIST_MODULES_32BIT 0x1
#define LIST_MODULES_64BIT 0x2
#define LIST_MODULES_ALL 0x3

namespace ExtCSGO
{
	using namespace sdk;
	Engine::Engine(const Settings* settings) :
		m_Process
		(
			new Process
			(
				settings->m_GamePath.c_str(),
				"csgo.exe",
				"Valve001",
				settings->m_LaunchOptions.c_str()
			)
		),
		m_ClientDLL(new Module("client.dll", LIST_MODULES_32BIT)),
		m_EngineDLL(new Module("engine.dll", LIST_MODULES_32BIT)),
		m_IVEngineClient(new IVEngineClient()),
		m_IClientEntity(new IClientEntityList())
	{
	}

	Engine::~Engine()
	{
		delete m_IClientEntity;
		delete m_IVEngineClient;
		delete m_EngineDLL;
		delete m_ClientDLL;
		delete m_Process;
	}

	Process * Engine::GetProcess() const
	{
		return m_Process;
	}

	Module * Engine::GetClientDLL() const
	{
		return m_ClientDLL;
	}

	Module * Engine::GetEngineDLL() const
	{
		return m_EngineDLL;
	}

	IClientEntityList *Engine::GetIClientEnt() const
	{
		return m_IClientEntity;
	}

	IVEngineClient *Engine::GetIVEngine() const
	{
		return m_IVEngineClient;
	}

	void Engine::Update() const
	{
		if (!IsHandleValid())
		{
			GetClientDLL()->Reset();
			GetEngineDLL()->Reset();

			GetProcess()->Init();
		}
		else
		{
			GetClientDLL()->Init(GetProcess()->GetHandle());
			GetEngineDLL()->Init(GetProcess()->GetHandle());

			if (IsValid())
			{
				std::cout << "Process Found! Handle:" << m_Process->GetHandle() << std::endl;
			}
		}
	}

	void Engine::UpdateEvents()
	{
		m_IClientEntity->Update(this);
		m_IVEngineClient->Update(this);
	}

	bool Engine::IsValid() const
	{
		return (IsHandleValid() && IsModuleValid());
	}

	bool Engine::IsHandleValid() const
	{
		return GetProcess()->IsValid(PROCESS_HANDLE);
	}

	bool Engine::IsModuleValid() const
	{
		return GetClientDLL()->IsValid() &&
			   GetEngineDLL()->IsValid();
	}
}