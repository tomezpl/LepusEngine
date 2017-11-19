#include "FPPCamera.h"
#include "LepusEngine/Source/Logger.h"

using namespace LepusEngine::Lepus3D;

FPPCamera::FPPCamera(Window* wnd)
{
	this->SetWindow(wnd);
}

void FPPCamera::SetWindow(Window* wnd)
{
	m_Wnd = wnd;
	m_LastX = 0.5f;
	m_LastY = 0.5f;
	m_Wnd->setMouseCursorGrabbed(true);
	m_Wnd->setMouseCursorVisible(false);
}

glm::mat4 FPPCamera::Run()
{
	return this->GetView();
}

void FPPCamera::ProcessInput(float deltaTime)
{
	const float lookSensitivity = 250.f;
	auto position = m_Transform.GetPosition().vec3();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		//m_Transform.Move(Vector3(0.f, 0.f, 2.f * deltaTime));
		position += m_Target.vec3() * deltaTime * 2.f;
		//m_Target.z += 0.01f * deltaTime;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		//m_Transform.Move(Vector3(0.f, 0.f, -2.f * deltaTime));
		position -= m_Target.vec3() * deltaTime * 2.f;
		//m_Target.z -= 0.01f * deltaTime;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		//m_Transform.Move(Vector3(2.0f * deltaTime, 0.f, 0.f));
		position -= glm::normalize(glm::cross(m_Target.vec3(), m_Up.vec3())) * 2.0f * deltaTime;
		//m_Target.x += 0.01f * deltaTime;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		//m_Transform.Move(Vector3(-2.f * deltaTime, 0.f, 0.f));
		position += glm::normalize(glm::cross(m_Target.vec3(), m_Up.vec3())) * 2.0f * deltaTime;
		//m_Target.x -= 0.01f * deltaTime;
	}
	m_Transform.SetPosition(Vector3(position.x, position.y, position.z));
	Vector3 rotation = m_Transform.GetRotation();
	/* LEGACY CODE FOR KEYBOARD ROTATION
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		rotation.x += 0.25f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		rotation.x -= 0.25f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		rotation.y -= 0.25f;
		if (rotation.y <= 0.0f)
			rotation.y += 360.0f; // quick fix for issue #2 (negative rotation not supported)
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		rotation.y += 0.25f;
	}
	*/
	auto mouseInput = sf::Mouse::getPosition(*m_Wnd);
	Vector2 mouseInputF(mouseInput.x, mouseInput.y);
	auto wndSize = m_Wnd->getSize();
	bool mouseGood = false;
	if (mouseInput.x >= 0 && mouseInput.x <= wndSize.x && mouseInput.y >= 0 && mouseInput.y <= wndSize.y)
	{
		mouseGood = true;
		mouseInputF.x /= wndSize.x;
		mouseInputF.y /= wndSize.y;
		Vector2 mouseRot(mouseInputF.x - m_LastX, mouseInputF.y - m_LastY);
		rotation.y += mouseRot.x * lookSensitivity;
		if (rotation.y <= 0.0f)
			rotation.y += 360.0f; // quick fix for issue #2 (negative rotation not supported)
		rotation.x -= mouseRot.y * lookSensitivity;
	}
	// quick fix for issue #2 (no negative rotation)
	if (rotation.x <= 0.0f)
		rotation.x = 360.0f;
	if (rotation.x > 449.9f)
		rotation.x = 449.9f;
	if (rotation.x < 360.0f - 89.9f)
		rotation.x = 360.0f - 89.9f; // I was lazy to do the maths lol
	m_Transform.SetRotation(rotation);
	//m_Target = m_Transform.GetPosition();
	//m_Target.z += 1.0f;
	Logger::LogInfo("FPPCamera", "ProcessInput", (char*)(m_Transform.ToString().c_str()));
	if (mouseGood)
	{
		m_LastX = mouseInputF.x;
		m_LastY = mouseInputF.y;
	}
}