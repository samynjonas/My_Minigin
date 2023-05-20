#pragma once
#include <string>

namespace dae
{
	class Logger
	{
	public:
		virtual ~Logger() = default;
		virtual void Log(const std::string& text) = 0;
		virtual void LogWarning(const std::string& text) = 0;
		virtual void LogError(const std::string& text) = 0;

	};
}


