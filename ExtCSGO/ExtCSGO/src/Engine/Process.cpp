#include "Engine\Memory.h"

namespace ExtCSGO
{
	static bool HandleIsValid(const HANDLE & ProcessHandle);

	static bool WindowExists(const char * WindowClassName);

	static void CloseProcess(const char * ProcessName, const HANDLE & hProcess);

	static bool NewProcess(
		const char* ProcessPath,
		const char* ProcessName,
		const char* Arguments,
		PHANDLE     ProcessHandle);

	static bool DebugNewProcess(const char* WindowClassName, PHANDLE ProcessHandle);

	Process::Process(
		const char* ProcessPath,
		const char* ProcessName,
		const char* WindowClassName,
		const char* ProcessArguments) :
			m_ProcessPath(ProcessPath), 
			m_ProcessName(ProcessName),
			m_WindowClassName(WindowClassName),
			m_ProcessArguments(ProcessArguments)
	{
		m_Process = nullptr;
	}

	Process::~Process()
	{
	}

	HANDLE Process::GetHandle() const
	{
		return m_Process;
	}

	bool Process::ReadMemory(LPCVOID Adr, LPVOID Buffer, SIZE_T Size) const
	{
		return (ReadProcessMemory(m_Process, Adr, Buffer, Size, 0) > FALSE);
	}

	bool Process::Init()
	{
		#ifdef _DEBUG
		return DebugNewProcess(m_WindowClassName, &m_Process);
		#else
		if (IsValid(PROCESS_WINDOW))
		{
			CloseProcess(m_ProcessName, m_Process);
			m_Process = nullptr;
			Sleep(5000);
		}
		else
		{
			m_Process = nullptr;
		}

		return NewProcess(m_ProcessPath, m_ProcessName, m_ProcessArguments, &m_Process);
		#endif
	}

	bool Process::IsValid(const int & PROCESS_FLAGS) const
	{
		bool status = false;
		switch (PROCESS_FLAGS)
		{
			case PROCESS_WINDOW:
			{
				status = WindowExists(m_WindowClassName);
				break;
			}

			case PROCESS_HANDLE:
			{
				status = HandleIsValid(m_Process);
				break;
			}
		}
		return status;
	}

	static bool HandleIsValid(const HANDLE & ProcessHandle)
	{
		if (ProcessHandle == nullptr)
		{
			return false;
		}

		DWORD ExitCode;
		GetExitCodeProcess(ProcessHandle, &ExitCode);
		return (ExitCode == STILL_ACTIVE);
	}

	static bool WindowExists(const char * WindowClassName)
	{
		return (FindWindowA(WindowClassName, nullptr) > nullptr);
	}

	static void CloseProcess(const char * ProcessName, const HANDLE & hProcess)
	{
		std::string CmdString = 
		(
			"Taskkill /IM" + std::string(" ") + ProcessName	+ " /F"
		);

		system(CmdString.c_str());
		if (hProcess > nullptr)
		{
			CloseHandle(hProcess);
		}
	}

	static bool NewProcess(
		const char* ProcessPath,
		const char* ProcessName,
		const char* Arguments,
		PHANDLE	    ProcessHandle)
	{
		PROCESS_INFORMATION             pi = { 0 };
		STARTUPINFOA                    si = { 0 };
		                                si.cb = sizeof(STARTUPINFO);
		                                si.dwFlags = STARTF_USESHOWWINDOW;
		                                si.wShowWindow = SW_HIDE;


		std::string Path = ProcessPath;
		Path.erase(Path.end() - 1);

		std::string FullPath
		(
			Path + std::string(ProcessName + std::string(" ") + "-steam " + Arguments)
		);

		if (!CreateProcessA (
			nullptr, (LPSTR)FullPath.c_str(),
			nullptr, nullptr, FALSE, FALSE,
			nullptr, nullptr, &si, &pi))
		{
			return false;
		}

		CloseHandle(pi.hThread);

		*ProcessHandle = pi.hProcess;
		return true;
	}

	static bool DebugNewProcess(const char* WindowClassName, PHANDLE ProcessHandle)
	{
		DWORD ProcessId = FALSE;
		GetWindowThreadProcessId(FindWindowA(WindowClassName, nullptr), &ProcessId);
		if (ProcessId == FALSE)
		{
			return false;
		}
		*ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, false, ProcessId);
		return (*ProcessHandle > nullptr);
	}
}


