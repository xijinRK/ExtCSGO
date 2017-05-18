#include "Settings.h"
#include <iostream>

namespace ExtCSGO
{
	static FILE*        OpenFile(const char* FileName);
	static void         CloseFile(FILE* File);
	static bool         ReadFile(FILE* File, char* Buffer, const size_t BufferSize);
	static std::string  ReadString(FILE* File, const char* Name);

	Settings::Settings()
	{
		if (!LoadSettings())
		{
			std::cout << "[error]settings" << std::endl;;
			exit(0);
		}
	}

	Settings::~Settings()
	{
	}

	bool Settings::LoadSettings()
	{
		auto File = OpenFile("config.cfg");
		if(File == nullptr)
		{
			return false;
		}
	
		m_GamePath = ReadString(File, "GamePath=");
		m_LaunchOptions = ReadString(File, "LaunchOptions=");
		m_AimKey = std::stoi(ReadString(File, "AimKey="));
		m_AimFov = std::stod(ReadString(File, "AimFov="));
		m_AimSmooth = std::stoi(ReadString(File, "AimSmooth="));
		m_Sensitivity = std::stod(ReadString(File, "Sensitivity="));
		m_TriggerKey = std::stoi(ReadString(File, "TriggerKey="));
		m_TriggerBurst = std::stoi(ReadString(File, "TriggerBurst="));
		
		CloseFile(File);
		delete File;
		return true;
	}

	static FILE* OpenFile(const char* FileName)
	{
		FILE		File;
		auto*		FilePtr = &File;
		return 		!(fopen_s(&FilePtr, FileName, "r")) ? FilePtr : nullptr;
	}

	static void CloseFile(FILE* File)
	{
		fclose(File);
	}

	static bool ReadFile(FILE* File, char* Buffer, const size_t BufferSize)
	{
		return fgets(Buffer, (int)BufferSize, File) > nullptr;
	}

	static std::string ReadString(FILE* File, const char* Name)
	{
		char str[260];
		std::string StrString;
		while (ReadFile(File, str, 260))
		{
			StrString = str;
			if (StrString.find(Name) != std::string::npos)
			{
				StrString.erase(StrString.begin(), StrString.begin() + strlen(Name));
				break;
			}
		}

		return StrString;
	}
}