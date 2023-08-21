#pragma once
#include "Component.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

#include <memory>

namespace dae
{
	class GameObject;
	class CameraBehaviour;
	class TransfromComponent;

	class CameraComponent final : public Component
	{
	public:
		CameraComponent();
		~CameraComponent() = default;

		CameraComponent(const CameraComponent& other) = delete;
		CameraComponent(CameraComponent&& other) = delete;
		CameraComponent& operator=(const CameraComponent& other) = delete;
		CameraComponent& operator=(CameraComponent&& other) = delete;

		void Initialize(std::unique_ptr<CameraBehaviour> cameraBehaviour, int screenWidth, int screenHeight);
		
		void SetBehaviour(std::unique_ptr<CameraBehaviour> cameraBehaviour);

		void SetCameraActive();

		void Update() override;

		glm::mat4 GetViewMatrix() const
		{
			return m_ViewMatrix;
		}

	private:
		void UpdateViewMatrix();

		glm::vec3 m_Position{};
		glm::mat4 m_ViewMatrix{};

		int m_ScreenWidth{};
		int m_ScreenHeight{};

		std::unique_ptr<CameraBehaviour> m_pCameraBehaviour{ nullptr };
	};


	class CameraBehaviour
	{
	public:
		virtual ~CameraBehaviour() = default;
		
		virtual void Update() = 0;

		void SetScreenSize(int screenWidth, int screenHeight)
		{
			m_ScreenWidth = screenWidth;
			m_ScreenHeight = screenHeight;
		}
		void SetCamera(CameraComponent* pCamera)
		{
			m_pCamera = pCamera;
		}
		
		glm::vec2 worldToCameraSpace(glm::vec2 pos) const;

		glm::vec3 GetPosition()
		{
			Update();
			return m_LocalPosition;
		}
		
	protected:
		glm::vec3 m_LocalPosition{};

		CameraComponent* m_pCamera{ nullptr };

		int m_ScreenWidth{};
		int m_ScreenHeight{};

	private:

	};


	class FollowingCamera : public CameraBehaviour
	{
	public:
		~FollowingCamera() = default;

		void Update() override;
		void SetTrackingObject(GameObject* trackObject);

	private:
		GameObject* m_TrackObject{ nullptr };

	};

	class HorizontalFollowingCamera : public CameraBehaviour
	{
	public:
		~HorizontalFollowingCamera() = default;

		void Update() override;
		void SetTrackingObject(GameObject* trackObject);
		void SetLevelBoundaries(float x, float width);

	private:
		GameObject* m_TrackObject{ nullptr };

		float m_Levelboundaries_X{ 0 };
		float m_Levelboundaries_Width{ 10 };

	};

	class FreezeCamera : public CameraBehaviour
	{
	public:
		~FreezeCamera() = default;

		void Update() override;

	private:

	};


}

