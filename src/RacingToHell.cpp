#include "RacingToHell.h"

#include "Demos.h"
#include "GameDemo.h"
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

    //    gameDemo(platform);

    auto rect = Math::Rectangle();
    rect.position = {0,0};
    rect.size = {1, 1};
    Render::pushRectangle(getGameState(platform), rect, 0xffffffff, 1.0F);

    textDemo(platform, AtomPlane::BACKGROUND);
    followingCarDemo(platform);
    animationDemo(platform, AtomPlane::MENU);

    Render::flushBuffer(platform);
}
