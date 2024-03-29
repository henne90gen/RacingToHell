#include "Demos.h"

#include "Helper.h"
#include "Renderer.h"

#include <array>
#include <fmt/core.h>

void textDemo(Platform &platform, AtomPlane plane) {
    static std::array<std::string, 3> fontFileNames = {
        "res/font/DejaVuSansMono.ttf",
        "res/font/ComicSans.ttf",
        "res/font/Arial.ttf",
    };
    static int fontIndex = 0;

    if (platform.input.enterKeyClicked) {
        loadFont(platform, fontFileNames[fontIndex]);
        fontIndex++;
        if (fontIndex >= 3) {
            fontIndex = 0;
        }
    }

    GameState *gameState = getGameState(platform);

    Math::Rectangle rect = {};
    rect.position = glm::vec2(-platform.memory.aspectRatio, 0.5);
    rect.size = glm::vec2(platform.memory.aspectRatio * 2.0f, 1.0);
    Render::pushRectangle(gameState, rect, glm::vec4(0, 1, 0, 1), plane - 1);

    std::string text = "The quick brown fox jumps over the lazy dog.";
    glm::vec2 position = glm::vec2(-1.7f, 0.3);
    const auto color = glm::vec4(1, 0, 1, 1);
    Render::pushText(gameState, text, position, Render::FontSize::Small, color, plane);

    position = glm::vec2(-1.7f, 0);
    Render::pushText(gameState, text, position, Render::FontSize::Medium, color, plane);

    position = glm::vec2(-1.7f, -0.35);
    Render::pushText(gameState, text, position, Render::FontSize::Large, color, plane);
}

void performanceDemo(Platform &platform, AtomType type) {
    GameState *gameState = getGameState(platform);

    AtomPlane plane = AtomPlane::AI;

    switch (type) {
    case AtomType::RECTANGLE: {
        Math::Rectangle dim = {};
        dim.position = glm::vec2(0, 0);
        dim.size = glm::vec2(1, 1);
        Render::pushRectangle(gameState, dim, glm::vec4(1, 0, 0, 1), plane);
    } break;
    case AtomType::TEXTURE: {
        Render::Texture *texture = &gameState->resources.playerCarTextures[0];
        glm::vec2 pos = glm::vec2(0, 0);
        glm::vec2 size = glm::vec2(1, 1);
        glm::vec2 dir = glm::vec2(1, 1);
        Render::pushTexture(gameState, texture, pos, size, dir, plane);
    } break;
    case AtomType::TRIANGLE: {
        glm::vec2 p1 = glm::vec2(1, 1);
        glm::vec2 p2 = glm::vec2(-1, 1);
        glm::vec2 p3 = glm::vec2();
        Render::pushTriangle(gameState, p1, p2, p3, glm::vec4(1, 0, 0, 1), plane);
    } break;
    case AtomType::CIRCLE: {
        glm::vec2 position = glm::vec2();
        Render::pushCircle(gameState, position, 1, glm::vec4(1, 0, 0, 1), plane);
    } break;
    case AtomType::TEXT: {
        glm::vec2 position = glm::vec2(-1.7f, 0.7);
        std::string msg = "The quick brown fox jumps over the lazy dog";
        Render::pushText(gameState, msg, position, Render::FontSize::Medium, glm::vec4(1, 0, 0, 1), plane);
    } break;
    }
}

void performanceDemo(Platform &platform, AtomType type, int n) {
    for (int i = 0; i < n; i++) {
        performanceDemo(platform, type);
    }
}

void animationDemo(Platform &platform, AtomPlane plane) {
    GameState *gameState = getGameState(platform);
    Render::Texture *explosionTexture = &gameState->resources.explosion;

    static unsigned int explosionIndex3 = 0;
    glm::vec2 position = glm::vec2(1, 0);
    glm::vec2 size = glm::vec2(1, 1);
    Render::pushAnimation(gameState, explosionTexture, position, size, &explosionIndex3, plane, 3);

    static unsigned int explosionIndex2 = 0;
    if (explosionIndex2 >= explosionIndex3) {
        position = glm::vec2();
        size = glm::vec2(1, 1);
        Render::pushAnimation(gameState, explosionTexture, position, size, &explosionIndex2, plane, 2);
    }

    static unsigned int explosionIndex1 = 0;
    if (explosionIndex1 >= explosionIndex3) {
        position = glm::vec2(-1, 0);
        size = glm::vec2(1, 1);
        Render::pushAnimation(gameState, explosionTexture, position, size, &explosionIndex1, plane);
    }
}

void followingCarDemo(Platform &platform) {
    GameState *gameState = getGameState(platform);

    if (platform.input.shootKeyPressed &&
        gameState->agentCount < (int)sizeof(gameState->agents) / (int)sizeof(Player)) {
        platform.log(fmt::format("Spawning agent {}", gameState->agentCount));
        gameState->agents[gameState->agentCount++] = Agent();
        auto &agent = gameState->agents[gameState->agentCount - 1];
        agent.position = platform.input.mousePosition;
        agent.size = glm::vec2(0.05f, 0.10f);
        agent.maxSpeed = PLAYER_SPEED;
        agent.carIndex = std::rand() % NUM_PLAYER_TEXTURES;
    }

    for (int i = 0; i < gameState->agentCount; i++) {
        auto &agent = gameState->agents[i];

        Render::Texture *texture = &gameState->resources.playerCarTextures[agent.carIndex];

        glm::vec2 desired = platform.input.mousePosition - agent.position;
        desired = glm::normalize(desired);
        desired = desired * static_cast<float>(agent.maxSpeed);

        glm::vec2 steering = desired - agent.velocity;
        steering = glm::normalize(steering);
        steering = steering * 0.00005f;

        agent.acceleration = agent.acceleration + steering;

        agent.velocity = agent.velocity + agent.acceleration;
        if (glm::length(agent.velocity) > agent.maxSpeed) {
            agent.velocity = glm::normalize(agent.velocity) * static_cast<float>(agent.maxSpeed);
        }
        agent.position = agent.position + agent.velocity;

        if (agent.velocity.x != 0.0f && agent.velocity.y != 0.0f) {
            agent.direction = agent.velocity;
        }

        Render::pushTexture(gameState, texture, agent.position, agent.size, agent.direction, 0, AtomPlane::PLAYER);
    }
}
