#pragma once
#include "Observer.h"
#include <vector>

namespace dae
{
	struct ObserverData
	{
		Observer* pObserver;
		std::vector<Event> vecEvents; //List of events to observe
	};

	class Subject
	{
	public:
		void AddObserver(Observer* observer, std::vector<Event> events)
		{
			m_Observers.push_back({ observer, events });
		}

		void RemoveObserver(Observer* observer)
		{
			for (size_t index = 0; index < m_Observers.size(); index++)
			{
				if (m_Observers[index].pObserver == observer)
				{
					m_Observers.erase(m_Observers.begin() + index);
				}
			}
		}

	protected:
		void NotifyObservers(Event currEvent, Subject* pSubject)
		{
			for (auto observer : m_Observers)
			{
				for (const auto& _event : observer.vecEvents)
				{
					if (_event == currEvent)
					{
						observer.pObserver->Notify(currEvent, pSubject);
						continue;
					}
				}
			}
		}

	private:
		std::vector<ObserverData> m_Observers;

	};
}


