#pragma once
#include "Singleton.h"

#include <unordered_map>
#include <string>
#include <functional>

namespace dae
{
	class NumbersTracker final : public Singleton<NumbersTracker>
	{
	public:
		void AddTracker(std::string name, std::function<int()> getter)
		{
			m_mapGetterNumbers[name] = getter;
		}
		
		void AddTracker(std::string name, int number)
		{
			m_mapNumbers[name] = number;
		}
		void AddToTracker(std::string name, int amount)
		{
			m_mapNumbers[name] += amount;
		}

		int GetGetterNumber(std::string name)
		{
			int number = m_mapGetterNumbers[name]();
			return number;
		}

		int GetNumber(std::string name)
		{
			return m_mapNumbers[name];
		}


	private:
		friend class Singleton<NumbersTracker>;
		NumbersTracker() = default;
			
		std::unordered_map<std::string, std::function<int()>> m_mapGetterNumbers;
		std::unordered_map<std::string, int> m_mapNumbers;




	};
}

