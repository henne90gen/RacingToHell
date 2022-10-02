#include "RacingToHell.h"

#include "Demos.h"
#include "Helper.h"
#include "Init.h"
#include "Renderer.h"

/**
 * Retrieves the GameState from memory
 */
GameState *getGameState(Platform &platform) {
    if (!platform.memory.isInitialized) {
        init(platform);
    }
    return (GameState *)(platform.memory.permanent);
}

void update_and_render(Platform &platform) {
    beginFrame(platform);

    Render::pushTriangle(getGameState(platform), {0, 0}, {1, 0}, {1, 1}, 0xffffffff, 1.0F);
    textDemo(platform, AtomPlane::BACKGROUND);

    followingCarDemo(platform);

    animationDemo(platform, AtomPlane::MENU);

    Render::flushBuffer(platform);
}
