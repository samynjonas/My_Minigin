#pragma once
#include "Singleton.h"
#include "Observer.h"
#include "NumbersTracker.h"

#include <iostream>

namespace dae
{
	class GameListener final : public Singleton<GameListener>, public Observer
	{
	public:

		void Notify(Event currEvent, Subject*)
		{
			if (currEvent == OpenDoor)
			{
				m_HasSuccesFullyOpenedDoor = true;
			}
			else if (currEvent == ObjectDied)
			{
				m_HasPlayerDied = true;

			}
			else if (currEvent == LiveLost)
			{
				m_HasLivesLeft = true;
			}
			else if (currEvent == PausedGame)
			{
				m_HasPausedGame = true;
			}
		}

		bool GetHasPlayerDied() const
		{
			return m_HasPlayerDied;
		}
		void SetHasPlayerDied(bool state)
		{
			m_HasPlayerDied = state;
		}

		bool GetHasLivesLeft() const
		{
			return m_HasLivesLeft;
		}
		void SetHasLivesLeft(bool state)
		{
			m_HasLivesLeft = state;
		}

		bool GetHasSuccesFullyOpenedDoor() const
		{
			return m_HasSuccesFullyOpenedDoor;
		}
		void SetHasSuccesFullyOpenedDoor(bool state)
		{
			m_HasSuccesFullyOpenedDoor = state;
		}

		bool GetHasPausedGame() const
		{
			return m_HasPausedGame;
		}
		void SetHasPausedGame(bool state)
		{
			m_HasPausedGame = state;
		}

	private:
		friend class Singleton<GameListener>;
		GameListener() = default;


		bool m_HasPlayerDied{ false };
		bool m_HasLivesLeft{ false };
		bool m_HasSuccesFullyOpenedDoor{ false };
		bool m_HasPausedGame{ false };


	};
}


