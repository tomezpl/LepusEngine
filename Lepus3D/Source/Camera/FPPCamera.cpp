#include "FPPCamera.h"
#include "LepusEngine/Source/Logger.h"

using namespace LepusEngine::Lepus3D;

FPPCamera::FPPCamera(GLFWwindow* wnd)
{
	this->SetWindow(wnd);
}

void FPPCamera::SetWindow(GLFWwindow* wnd)
{
	m_Wnd = wnd;
	m_LastX = 0.5f;
	m_LastY = 0.5f;
}

glm::mat4 FPPCamera::Run()
{
	return this->GetView();
}

void FPPCamera::ProcessInput(float deltaTime)
{
	const float lookSensitivity = 100.f;
	auto position = m_Transform.GetPosition().vec3();
	/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		position += m_Target.vec3() * deltaTime * 2.f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		position -= m_Target.vec3() * deltaTime * 2.f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		position -= glm::normalize(glm::cross(m_Target.vec3(), m_Up.vec3())) * 2.0f * deltaTime;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		position += glm::normalize(glm::cross(m_Target.vec3(), m_Up.vec3())) * 2.0f * deltaTime;
	}*/
	m_Transform.SetPosition(Vector3(position.x, position.y, position.z));
	Vector3 rotation = m_Transform.GetRotation();

	/*auto mouseInput = sf::Mouse::getPosition();
	Vector2 mouseInputF(mouseInput.x, mouseInput.y);
	auto wndSize = sf::Vector2i(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
	bool mouseGood = false;

		mouseGood = true;
		mouseInputF.x /= wndSize.x;
		mouseInputF.y /= wndSize.y;
		Vector2 mouseRot(mouseInputF.x - 0.5f, mouseInputF.y - 0.5f);
		rotation.y += mouseRot.x * lookSensitivity;
		if (rotation.y <= 0.0f)
			rotation.y += 360.0f; // quick fix for issue #2 (negative rotation not supported)
		rotation.x -= mouseRot.y * lookSensitivity;*/

	// quick fix for issue #2 (no negative rotation)
	if (rotation.x <= 0.0f)
		rotation.x = 360.0f;
	if (rotation.x > 449.9f)
		rotation.x = 449.9f;
	if (rotation.x < 360.0f - 89.9f)
		rotation.x = 360.0f - 89.9f; // I was lazy to do the maths lol
	m_Transform.SetRotation(rotation);

	Logger::LogInfo("FPPCamera", "ProcessInput", (char*)(m_Transform.ToString().c_str()));
	/*if (mouseGood)
	{
		m_LastX = mouseInputF.x;
		m_LastY = mouseInputF.y;
		if (m_Wnd->hasFocus())
		{
			sf::Mouse::setPosition(sf::Vector2i(wndSize.x / 2, wndSize.y / 2));
			m_Wnd->setMouseCursorGrabbed(true);
			m_Wnd->setMouseCursorVisible(false);
		}
	}*/
}