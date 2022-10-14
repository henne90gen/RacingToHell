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

#ifdef HOT_RELOAD
extern "C"
#if WIN32
    __declspec(dllexport)
#endif
#endif
        void update_and_render(Platform &platform) {
    beginFrame(platform);

    game(platform);

    //    textDemo(platform, AtomPlane::BACKGROUND);
    //    followingCarDemo(platform);
    //    animationDemo(platform, AtomPlane::MENU);

    Render::flushBuffer(platform);
}
