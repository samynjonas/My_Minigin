#pragma once
#include <string>

namespace dae
{
	class Scene;
	class GameStateManagerComponent;

	class GameModes
	{
	public:
		GameModes() {};
		virtual ~GameModes() = default;
		GameModes(const GameModes& other) = delete;
		GameModes(GameModes&& other) = delete;
		GameModes& operator=(const GameModes& other) = delete;
		GameModes& operator=(GameModes&& other) = delete;


		virtual void InitializeScene();

		virtual void SpawnMap(Scene* pScene);
		virtual void SetGameState(Scene* pScene, std::string NextScene, GameStateManagerComponent* pGameStateManagerComponent);
		virtual void SpawnPlayers(Scene*) {};
		virtual void SpawnEnemies(Scene*, int) {};

	protected:
		GameStateManagerComponent* m_pGameStateManager_1{ nullptr };
		GameStateManagerComponent* m_pGameStateManager_2{ nullptr };
		GameStateManagerComponent* m_pGameStateManager_3{ nullptr };
	private:

	};

	class GameMode_SinglePlayer final : public GameModes
	{
	public:
		GameMode_SinglePlayer() {};
		~GameMode_SinglePlayer() = default;
		GameMode_SinglePlayer(const GameMode_SinglePlayer& other) = delete;
		GameMode_SinglePlayer(GameMode_SinglePlayer&& other) = delete;
		GameMode_SinglePlayer& operator=(const GameMode_SinglePlayer& other) = delete;
		GameMode_SinglePlayer& operator=(GameMode_SinglePlayer&& other) = delete;

		void SpawnPlayers(Scene* pScene) override;
		void SpawnEnemies(Scene* pScene, int level) override;

	private:
	};

	class GameMode_Coop final : public GameModes
	{
	public:
		~GameMode_Coop() = default;

		void SpawnPlayers(Scene* pScene) override;
		void SpawnEnemies(Scene* pScene, int level) override;

	private:
	};

	class GameMode_Versus final : public GameModes
	{
	public:
		~GameMode_Versus() = default;

		void SpawnPlayers(Scene* pScene) override;
		void SpawnEnemies(Scene* pScene, int level) override;

	private:
	};

}

