#pragma once
#include "Observer.h"
#include <memory>

namespace dae
{
	class GameState
	{
	public:
		virtual ~GameState() = default;

		virtual void Enter() = 0;
		virtual void Exit() = 0;

		virtual std::unique_ptr<GameState> GameStateNotify(Event currEvent) = 0;

	private:


	};

	class MainMenuState final : public GameState
	{
	public:
		~MainMenuState() = default;

		void Enter() override;
		void Exit() override;

		std::unique_ptr<GameState> GameStateNotify(Event currEvent) override;

	private:

	};

	class SinglePlayerState : public GameState
	{
	public:
		virtual ~SinglePlayerState() = default;

		virtual void Enter() override;
		virtual void Exit() override;

		virtual std::unique_ptr<GameState> GameStateNotify(Event currEvent) override;

	private:

	};


	class Singleplayer_MAP1 final : public SinglePlayerState
	{
	public:
		~Singleplayer_MAP1() = default;

		void Enter() override;
		void Exit() override;

		std::unique_ptr<GameState> GameStateNotify(Event currEvent) override;

	private:

	};

	class Singleplayer_MAP2 final : public SinglePlayerState
	{
	public:
		~Singleplayer_MAP2() = default;

		void Enter() override;
		void Exit() override;

		std::unique_ptr<GameState> GameStateNotify(Event currEvent) override;

	private:

	};

	class Singleplayer_MAP3 final : public SinglePlayerState
	{
	public:
		~Singleplayer_MAP3() = default;

		void Enter() override;
		void Exit() override;

		std::unique_ptr<GameState> GameStateNotify(Event currEvent) override;

	private:

	};


	class CoopState : public GameState
	{
	public:
		virtual ~CoopState() = default;

		virtual void Enter() override;
		virtual void Exit() override;

		virtual std::unique_ptr<GameState> GameStateNotify(Event currEvent) override;

	private:

	};

	class CoopState_MAP1 final : public CoopState
	{
	public:
		~CoopState_MAP1() = default;

		void Enter() override;
		void Exit() override;

		std::unique_ptr<GameState> GameStateNotify(Event currEvent) override;

	private:

	};

	class CoopState_MAP2 final : public CoopState
	{
	public:
		~CoopState_MAP2() = default;

		void Enter() override;
		void Exit() override;

		std::unique_ptr<GameState> GameStateNotify(Event currEvent) override;

	private:

	};

	class CoopState_MAP3 final : public CoopState
	{
	public:
		~CoopState_MAP3() = default;

		void Enter() override;
		void Exit() override;

		std::unique_ptr<GameState> GameStateNotify(Event currEvent) override;

	private:

	};


	class VersusState final : public GameState
	{
	public:
		~VersusState() = default;

		void Enter() override;
		void Exit() override;

		std::unique_ptr<GameState> GameStateNotify(Event currEvent) override;

	private:

	};

	class HighscoreMenuState final : public GameState
	{
	public:
		~HighscoreMenuState() = default;

		void Enter() override;
		void Exit() override;

		std::unique_ptr<GameState> GameStateNotify(Event currEvent) override;

	private:

	};

}


