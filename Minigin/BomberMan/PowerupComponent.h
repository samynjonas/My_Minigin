#pragma once
#include "Component.h"
#include "Observer.h"
#include "Subject.h"

#include <memory>

namespace dae
{
	class Powerup;

	class PowerupComponent final : public Component, public Observer, public Subject
	{
	public:
		PowerupComponent();
		~PowerupComponent() = default;

		PowerupComponent(const PowerupComponent& other) = delete;
		PowerupComponent(PowerupComponent&& other) = delete;
		PowerupComponent& operator=(const PowerupComponent& other) = delete;
		PowerupComponent& operator=(PowerupComponent&& other) = delete;

		void Initialize(std::shared_ptr<Powerup> powerupToApply);
		void Update() override;

		void Notify(Event currEvent, Subject* actor);

	private:
		std::shared_ptr<Powerup> m_pPowerup;

	};

	class GameObject;

	class Powerup
	{
	public:
		virtual void SpawnPowerup(GameObject* powerupGB) = 0;
		virtual bool ApplyPowerup(GameObject* player) = 0;

	private:
	};

	class DoorPowerup final : public Powerup
	{
	public:
		void SpawnPowerup(GameObject* powerupGB) override;
		bool ApplyPowerup(GameObject* player) override;

	private:
	};

	class ExtraBombPowerup final : public Powerup
	{
	public:
		void SpawnPowerup(GameObject* powerupGB) override;
		bool ApplyPowerup(GameObject* player) override;

	private:
	};

	class DetonatorPowerup final : public Powerup
	{
	public:
		void SpawnPowerup(GameObject* powerupGB) override;
		bool ApplyPowerup(GameObject* player) override;

	private:
	};

	class FlamesPowerup final : public Powerup
	{
	public:
		void SpawnPowerup(GameObject* powerupGB) override;
		bool ApplyPowerup(GameObject* player) override;

	private:
	};

	class NULLPowerup final : public Powerup
	{
	public:
		void SpawnPowerup(GameObject* powerupGB) override;
		bool ApplyPowerup(GameObject* player) override;

	private:
	};
}


