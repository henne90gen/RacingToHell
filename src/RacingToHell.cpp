#include "RacingToHell.h"

#include "Helper.cpp"
#include "MyMath.cpp"
#include "Memory.cpp"
#include "Collision.cpp"
#include "Renderer.cpp"
#include "Sound.cpp"
#include "GameMenu.cpp"
#include "Boss.cpp"
#include "Init.cpp"
#include "Demos.cpp"

/**
 * Retrieves the GameState from memory
 */
GameState *getGameState(GameMemory *memory) {
	if (!memory->isInitialized) {
		init(memory);
	}
	return (GameState *) (memory->permanent);
}

extern "C"
UPDATE_AND_RENDER(updateAndRender) {
	beginFrame(memory, input);

	textDemo(memory, input, AtomPlane::BACKGROUND);

	followingCarDemo(memory, input);

	animationDemo(memory, AtomPlane::MENU);

	Render::flushBuffer(memory);
}
