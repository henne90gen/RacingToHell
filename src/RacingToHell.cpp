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

    // Render::pushTriangle(getGameState(platform), {0, 0}, {1, 0}, {1, 1}, 0xffffffff, 1.0F);
    // Render::pushTriangle(getGameState(platform), {0, 0}, {platform.memory.aspectRatio, 0},
    //                      {platform.memory.aspectRatio, 1}, 0x00ffffff, 1.0F);
    auto rect = Math::Rectangle{};
    rect.position = glm::vec2(0, 0);
    rect.size = glm::vec2(1, 1);
    Render::pushRectangle(getGameState(platform), rect, 0x00ffffff, 1.0F);

    rect.position = glm::vec2(-1, 1);
    rect.size = glm::vec2(2, 2);
    Render::pushRectangle(getGameState(platform), rect, 0xff00ffff, 0.5F);

    rect.position = platform.input.mousePosition;
    rect.size = glm::vec2(0.1, 0.1);
    Render::pushRectangle(getGameState(platform), rect, 0xffff00ff, 1.5F);

    // textDemo(platform, AtomPlane::BACKGROUND);

    followingCarDemo(platform);

    // animationDemo(platform, AtomPlane::MENU);

    Render::flushBuffer(platform);
}
