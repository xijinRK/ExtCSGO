#include "Settings.h"
#include <stdio.h>
#include <string>

namespace ExtCSGO
{
	static FILE* OpenFile(const char* FileName);
	static void CloseFile(FILE* File);
	static bool ReadFile(FILE* File, char* Buffer, const size_t BufferSize);
	static const char* ReadString(FILE* File, const char* Name);

	static double ReadDouble(FILE* File, const char* Name);
	static int ReadInt(FILE* File, const char* Name);


	Settings::Settings()
	{

	}

	Settings::~Settings()
	{
		delete m_GamePath;
		delete m_LaunchOptions;
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
		m_AimKey = ReadInt(File, "AimKey=");
		m_AimFov = ReadDouble(File, "AimFov=");
		m_AimSmooth = ReadInt(File, "AimSmooth=");
		m_Sensitivity = ReadDouble(File, "Sensitivity=");
		m_TriggerKey = ReadInt(File, "TriggerKey=");
		m_TriggerBurst = ReadInt(File, "TriggerBurst=");


		CloseFile(File);

		delete File;

		return true;
	}

	Settings* Settings::GetSettings()
	{
		if (m_Settings == nullptr)
		{
			m_Settings = new Settings();
		}
		return m_Settings;
	}

	void Settings::DeleteSettings()
	{
		if (m_Settings > nullptr)
		{
			delete m_Settings;
			m_Settings = nullptr;
		}
	}

	static FILE* OpenFile(const char* FileName)
	{
		FILE		nFile;
		auto*		fPtr = &nFile;
		return (fopen_s(&fPtr, FileName, "r")) ? fPtr : nullptr;
	}

	static void CloseFile(FILE* File)
	{
		fclose(File);
	}

	static bool ReadFile(FILE* File, char* Buffer, const size_t BufferSize)
	{
		return fgets(Buffer, (int)BufferSize, File) > nullptr;
	}

	static const char* ReadString(FILE* File, const char* Name)
	{
		std::string String;
		{
			char str[260];	
			while (ReadFile(File, str, 260))
			{
				String = str;
				if (String.find(Name) != std::string::npos)
				{
					String.erase(String.begin(), String.begin() + strlen(Name));
					break;
				}
			}
		}
		auto StringLenght = (strlen(String.c_str()) + 1);
		if (StringLenght == 0)
			return nullptr;

		char* str = new char[StringLenght];

		strcpy_s(str, StringLenght, String.c_str());
		return str;
	}

	static double ReadDouble(FILE* File, const char* Name)
	{
		auto String = ReadString(File, Name);
		auto Result = std::stod(String);
		delete String;
		return Result;
	}

	static int ReadInt(FILE* File, const char* Name)
	{
		auto String = ReadString(File, Name);
		auto Result = std::stoi(String);
		delete String;
		return Result;
	}

	Settings* Settings::m_Settings;
}