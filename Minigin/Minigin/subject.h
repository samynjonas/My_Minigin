#pragma once
#include "Observer.h"
#include <vector>

namespace dae
{
	class subject
	{
	public:
		void AddObserver(Observer* observer)
		{
			m_Observers.push_back(observer);
		}


		void RemoveObserver(Observer* observer)
		{
			for (auto it = m_Observers.begin(); it != m_Observers.end(); ++it)
			{
				if (*it == observer)
				{
					m_Observers.erase(it);
					return;
				}
			}

		}

	protected:
		void NotifyObservers(Event currEvent, subject* pSubject)
		{
			for (auto observer : m_Observers)
			{
				observer->Notify(currEvent, pSubject);
			}
		}


	private:
		std::vector<Observer*> m_Observers;

	};
}


