#include "RacingToHell.h"

#include <spdlog/spdlog.h>

#include "Demos.h"
#include "Helper.h"
#include "Init.h"
#include "Renderer.h"

/**
 * Retrieves the GameState from memory
 */
GameState *getGameState(GameMemory *memory) {
    if (!memory->isInitialized) {
        init(memory);
    }
    return (GameState *)(memory->permanent);
}

void update_and_render(Input *input, GameMemory *memory) {
    beginFrame(memory, input);

    Render::pushTriangle(getGameState(memory), {0,0}, {1,0}, {1,1}, 0xffffffff, 1.0F);
    //     textDemo(memory, input, AtomPlane::BACKGROUND);

    followingCarDemo(memory, input);

    animationDemo(memory, AtomPlane::MENU);

    Render::flushBuffer(memory);
}
