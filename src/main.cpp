#include <iostream>
#include <config.h>
#include <memory>
#include <cstdlib>
#include <ctime>

#include "Renderer.hpp"
#include "Game.hpp"
#include "MainInputManager.hpp"
#include "InputSystem.hpp"

int main()
{
	std::cout << PACKAGE_STRING << std::endl;
	Renderer renderer;
	Game g;

	renderer.OpenWindow();

	InputSystem input_system;
	input_system.SetInputManager(std::make_unique<MainInputManager>(g));

	int seed = time(0);

	g.GetWorld().Generate(seed);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	unsigned int last_time(0), current_time;
	const unsigned int MS_PER_TICK = 15;
	unsigned int accumulator;
	float dt = MS_PER_TICK / 1000.f;

	while (g.KeepGoing()) {
		current_time = SDL_GetTicks();
		accumulator += current_time - last_time;
		last_time = current_time;

		while (accumulator >= MS_PER_TICK) {
			accumulator -= MS_PER_TICK;
			input_system.ProcessEvents();
			g.GetWorld().Update(dt);
		}

		renderer.Render(g.GetWorld(), g.GetCamera());
	}

	return 0;
}

