#pragma once
#include "Observer.h"
#include "Singleton.h"
#include <memory>

#include "GameState.h"

namespace dae
{
	class Gamestatemachine final : public Observer, public Singleton<Gamestatemachine>
	{
	public:
		~Gamestatemachine() = default;

		Gamestatemachine(const Gamestatemachine& other) = delete;
		Gamestatemachine(Gamestatemachine&& other) = delete;
		Gamestatemachine& operator=(const Gamestatemachine& other) = delete;
		Gamestatemachine& operator=(Gamestatemachine&& other) = delete;

		void Update();
		void Notify(Event event, Subject* pSubject) override;

		void Start();

	private:
		friend class Singleton<Gamestatemachine>;
		Gamestatemachine();

		std::unique_ptr<GameState> m_State{};
	};
}


