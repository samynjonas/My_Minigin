#include "CameraComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"

#include "RenderingManager.h"

dae::CameraComponent::CameraComponent()
	: Component()
{

}

void dae::CameraComponent::Initialize(std::unique_ptr<CameraBehaviour> cameraBehaviour, int screenWidth, int screenHeight)
{
	RenderingManager::GetInstance().SetActiveCamera(this);

	m_ScreenWidth = screenWidth;
	m_ScreenHeight = screenHeight;

	SetBehaviour(std::move(cameraBehaviour));
}

void dae::CameraComponent::SetBehaviour(std::unique_ptr<CameraBehaviour> cameraBehaviour)
{
	m_pCameraBehaviour = std::move(cameraBehaviour);
	
	m_pCameraBehaviour->SetScreenSize(m_ScreenWidth, m_ScreenHeight);
	m_pCameraBehaviour->SetCamera(this);
}

void dae::CameraComponent::SetCameraActive()
{
	RenderingManager::GetInstance().SetActiveCamera(this);
}

void dae::CameraComponent::UpdateViewMatrix()
{
	auto cameraPos = glm::vec3(GetOwner()->transform()->GetWorldPosition(), 0);

	glm::vec3 offset{};
	if (m_pCameraBehaviour)
	{
		auto pos = m_pCameraBehaviour->GetPosition();
		offset.x += pos.x;
		offset.y += pos.y;
	}
	
	m_ViewMatrix = glm::lookAt(cameraPos + offset, cameraPos + offset + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void dae::CameraComponent::Update()
{
	UpdateViewMatrix();
}

// ----------------------------------------------- Camera Behaviours ---------------------------------------------- \\

glm::vec2 dae::CameraBehaviour::worldToCameraSpace(glm::vec2 pos) const
{
	if (m_pCamera)
	{
		auto cameraGB = m_pCamera->GetOwner();
		if (cameraGB)
		{
			auto cameraPos = m_pCamera->GetOwner()->transform()->GetWorldPosition();
			glm::vec2 cameraSpace{ pos.x - cameraPos.x, pos.y - cameraPos.y };
			return cameraSpace;
		}
	}
	return { 0.f, 0.f };
}


// -- Following Camera -- \\

void dae::FollowingCamera::Update()
{
	if (m_TrackObject)
	{
		auto& objectPos = m_TrackObject->transform()->GetWorldPosition();
		m_LocalPosition.x = objectPos.x;
		m_LocalPosition.y = objectPos.y;
	}
}

void dae::FollowingCamera::SetTrackingObject(GameObject* trackObject)
{
	m_TrackObject = trackObject;
}

// -- Horizontal Following Camera -- \\

void dae::HorizontalFollowingCamera::Update()
{
	if (m_TrackObject)
	{
		if (m_TrackObject->IsMarkedForDead())
		{
			return;
		}

		auto& objectPos = m_TrackObject->transform()->GetWorldPosition();
		glm::vec2 offset{};
		
		float halfScreenWidth{ m_ScreenWidth / 2.f };
		offset.x = -halfScreenWidth;

		m_LocalPosition.x = objectPos.x + offset.x;
		if (m_LocalPosition.x < m_Levelboundaries_X)
		{
			m_LocalPosition.x = m_Levelboundaries_X;
		}
		else if (m_LocalPosition.x + m_ScreenWidth > m_Levelboundaries_X + m_Levelboundaries_Width)
		{
			m_LocalPosition.x = m_Levelboundaries_X + m_Levelboundaries_Width - m_ScreenWidth;
		}
	}
}

void dae::HorizontalFollowingCamera::SetTrackingObject(GameObject* trackObject)
{
	m_TrackObject = trackObject;
}

void dae::HorizontalFollowingCamera::SetLevelBoundaries(float x, float width)
{
	m_Levelboundaries_X = x;
	m_Levelboundaries_Width = width;
}

// -- Freeze Camera -- \\

void dae::FreezeCamera::Update()
{

}