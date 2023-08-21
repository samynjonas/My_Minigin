#pragma once
#include "Component.h"
#include "CSVParser.h"
#include "glm/glm.hpp"

#include <memory>

#include "Observer.h"

#include <string>

namespace dae
{
	class Scene;
	class Powerup;

	class MapGeneratorComponent final : public Component, public Observer
	{
	public:
		enum class MapPiece //Depending on the parsed vector value a block will be placed
		{
			Wall = 1,
			Floor = 2
		};

		MapGeneratorComponent();

		~MapGeneratorComponent() = default;

		MapGeneratorComponent(const MapGeneratorComponent& other) = delete;
		MapGeneratorComponent(MapGeneratorComponent&& other) = delete;
		MapGeneratorComponent& operator=(const MapGeneratorComponent& other) = delete;
		MapGeneratorComponent& operator=(MapGeneratorComponent&& other) = delete;

		void Initialize(const std::string& jsonMapFile, int block_size);
		void Update() override {}

		void Notify(Event currEvent, Subject* actor) override;
		void LoadMap();
		void UnloadMap();

	private:
		const CSVParser m_CSVParser{};

		std::string m_JsonMapFile;

		int m_BlockSize{};

		Scene* m_pScene{};
		glm::vec2 m_ParentPos{};
		int m_Width{};
		int m_Height{};

		int m_WallCount;

		const int MAX_BOMB_POWERUPS{ 2 };
		int m_BombPowerupCount{};

		const int MAX_EXPLOSIONRANGE_POWERUPS{ 1 };
		int m_ExplosionPowerupCount{};

		const int MAX_DOORS{ 1 };
		int m_DoorCount{};

		const int MAX_DETONATORS{ 1 };
		int m_DetonatorCount{};

		const int POWERUP_COUNT{ 5 };

		std::shared_ptr<Powerup> AddPowerup();

		std::vector<glm::vec2> m_VecBreakablePositions;

		void CreateWall(int row, int coll);
		void CreateFloor(int row, int coll);
		void CreateBreakableWall(int row, int coll);

	};
}
