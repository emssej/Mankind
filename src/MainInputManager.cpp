#include "MainInputManager.hpp"
#include "Log.hpp"
#include <glm/glm.hpp>

void MainInputManager::OnKeyDown(char key)
{
	Camera& c = game.GetCamera();
	glm::vec3 lookat;
	lookat.x = sinf(c.yaw) * cosf(c.pitch);
	lookat.y = sinf(c.pitch);
	lookat.z = cosf(c.yaw) * cosf(c.pitch);

	switch (key) {
	case 'w':
		game.GetCamera().x += lookat.x * 2;
		game.GetCamera().y += lookat.y * 2;
		game.GetCamera().z += lookat.z * 2;
		break;
	case 's':
		game.GetCamera().x -= lookat.x * 2;
		game.GetCamera().y -= lookat.y * 2;
		game.GetCamera().z -= lookat.z * 2;
		break;
	case 'a':
		game.GetCamera().x -= 1;
		break;
	case 'd':
		game.GetCamera().x += 1;
		break;
	case 'q':
		game.GetCamera().y += 1;
		break;
	case 'e':
		game.GetCamera().y -= 1;
		break;
	}
}

void MainInputManager::OnKeyUp(char key)
{
}

void MainInputManager::OnMouseButtonDown(MouseButton button)
{
}

void MainInputManager::OnMouseButtonUp(MouseButton button)
{
}

void MainInputManager::OnMouseMotion(int x, int y, int dx, int dy)
{
	Camera& c = game.GetCamera();
	c.yaw -= dx / 500.;
	c.pitch -= dy / 500.;
}

void MainInputManager::OnQuit()
{
	game.Stop();
}

