#pragma once
#include "Component.h"
#include "CSVParser.h"
#include "glm/glm.hpp"

#include <string>

namespace dae 
{
	class Scene;
	class MapGeneratorComponent final : public Component
	{
	public:
		enum class MapPiece //Depending on the parsed vector value a block will be placed
		{
			WallNoCollider		= 0,
			Wall				= 1,
			Floor				= 2,
			Diamond				= 3,
			BlueTankSpawner		= 4,
			RecognizerSpawner	= 5,
			PlayerSpawner		= 6,
		};
		
		MapGeneratorComponent();

		~MapGeneratorComponent() = default;

		MapGeneratorComponent(const MapGeneratorComponent& other) = delete;
		MapGeneratorComponent(MapGeneratorComponent&& other) = delete;
		MapGeneratorComponent& operator=(const MapGeneratorComponent& other) = delete;
		MapGeneratorComponent& operator=(MapGeneratorComponent&& other) = delete;

		void Initialize(const std::string& jsonMapFile, const float& block_size);
		void Update() override {}

	private:
		const CSVParser m_CSVParser{};

		float m_BlockSize{};

		Scene* m_pScene{};
		glm::vec2 m_ParentPos{};

		void CreateWall(int row, int coll);
		void CreateFloor(int row, int coll);
		void CreateBlueTank(int row, int coll);
		void CreatePlayer(int row, int coll);
		void CreateTeleporter(int row, int coll);

	};
}



