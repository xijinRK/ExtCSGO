#pragma once
#include <windows.h>

namespace ExtCSGO
{
	enum PROCESS_FLAGS_CREATE
	{
		PROCESS_HANDLE =	( 1 << 0),
		PROCESS_WINDOW =	( 1 << 2 )
	};

	class Process
	{
		const char*		m_ProcessPath;
		const char*		m_ProcessName;
		const char*		m_WindowClassName;
		char*			m_ProcessArguments;
		HANDLE			m_Process;
	public:
		Process(
			const char* ProcessPath,
			const char* ProcessName,
			const char* WindowClassName,
			const char* ProcessArguments);
		~Process();


		HANDLE GetHandle() const;

		bool   ReadMemory(LPCVOID Adr, LPVOID Buffer, SIZE_T Size) const;
		bool   Init();
		bool   IsValid(const int & PROCESS_FLAGS) const;
	};

	class Module
	{
		const char*		m_ModuleName;
		int				m_ListModules;
		HMODULE			m_BaseAddress;
	public:
		Module(const char* ModuleName, const int & LIST_MODULES);
		~Module();
		HMODULE		GetBaseAddress() const;
		DWORD		GetdwBaseAddress() const;

		void		Reset();

		bool		IsValid() const;
		bool		Init(const HANDLE & hProcess);
	};
}