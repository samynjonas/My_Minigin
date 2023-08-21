#pragma once
#include <memory>
#include <string>

namespace dae
{
	class Scene;
	class GameState
	{
	public:
		virtual ~GameState() = default;

		virtual void Enter() = 0;
		virtual void Exit() = 0;
		virtual std::unique_ptr<GameState> Update() = 0;

		void Initialize(const std::string& CurrentScene, const std::string& NextScene, const std::string& StartScene, const std::string& LeaderbordScene);

	protected:
		std::string m_CurrentScene{};
		std::string m_NextScene{};
		std::string m_StartScene{};
		std::string m_LeaderbordScene{};

	private:
		

	};

	class Loading final : public GameState
	{
	public:
		~Loading() = default;

		void Enter() override;
		void Exit() override;

		std::unique_ptr<GameState> Update() override;

	private:
		const float LOAD_TIME{ 3.f };
		float m_ElapsedTime{};
	};

	class InGame final : public GameState
	{
	public:
		~InGame() = default;

		void Enter() override;
		void Exit() override;

		std::unique_ptr<GameState> Update() override;

	private:




	};

	class Paused final : public GameState
	{
	public:
		~Paused() = default;

		void Enter() override;
		void Exit() override;

		std::unique_ptr<GameState> Update() override;

	private:


	};

}

