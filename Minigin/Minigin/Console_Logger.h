#pragma once
#include "Logger.h"
#include <iostream>

#include <Windows.h>

namespace dae
{
	class Console_Logger final : public Logger
	{
    public:
        Console_Logger()
        {
            m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            GetConsoleScreenBufferInfo(m_hConsole, &m_ConsoleInfo);
            m_OriginalAttribute = m_ConsoleInfo.wAttributes;
        }

        //TODO fix this to use stringstream
		void Log(const std::string& text)
		{
            SetConsoleTextAttribute(m_hConsole, m_OriginalAttribute);
			std::cout << "LOGGER: " << text << std::endl;
		}

        //TODO fix this to use stringstream
        void LogWarning(const std::string& text)
        {
            SetConsoleTextAttribute(m_hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            std::cout << "WARNING: " << text << std::endl;

            //Resetting color
            SetConsoleTextAttribute(m_hConsole, m_OriginalAttribute);
        }

        //TODO fix this to use stringstream
        void LogError(const std::string& text)
        {
            SetConsoleTextAttribute(m_hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
            std::cout << "ERROR: " << text << std::endl;

            //Resetting color
            SetConsoleTextAttribute(m_hConsole, m_OriginalAttribute);
        }

    private:
        HANDLE m_hConsole{};
        CONSOLE_SCREEN_BUFFER_INFO m_ConsoleInfo{};
        WORD m_OriginalAttribute{};

	};
}


