#include "Helper.h"

#include "Memory.h"
#include "Platform.h"
#include "Renderer.h"
#include "Resources.h"
#include "Shader.h"

#include <ft2build.h>
#include FT_FREETYPE_H

FT_Library fontLibrary;

Render::Character *getCharacter(GameState *gameState, char character, unsigned fontSizeIndex) {
    return &gameState->resources.characterMap[fontSizeIndex][character - Render::firstCharacter];
}

int getFontSize(GameState *gameState, int fontSizeIndex) {
    return gameState->resources.availableFontSizes[fontSizeIndex];
}

Math::Rectangle getBoundingBox(glm::vec2 position, unsigned width, unsigned height) {
    Math::Rectangle rect = {};
    rect.size = glm::vec2(width, height);
    rect.position = position - rect.size / 2.0F;
    return rect;
}

/**
 * This adds rect2 to rect1
 * Use the result and the position of rect2 for collision detection
 */
Math::Rectangle getCollisionBox(Math::Rectangle rect1, Math::Rectangle rect2) {
    rect1.size += rect2.size;
    rect1.position.x -= rect2.size.x / 2.0f;
    rect1.position.y += rect2.size.y / 2.0f;
    return rect1;
}

void importPixelData(void *src, void *dest, unsigned srcWidth, unsigned srcHeight, int offsetX, int offsetY,
                     unsigned destWidth, unsigned destHeight) {

    for (unsigned y = 0; y < destHeight; y++) {
        for (unsigned x = 0; x < destWidth; x++) {
            int srcIndex = (srcHeight - (y + offsetY) - 1) * srcWidth + x + offsetX;
            int destIndex = y * destWidth + x;
            uint32_t color = ((uint32_t *)(src))[srcIndex];
            uint8_t red = (color & 0xff000000) >> 24;
            uint8_t green = (color & 0x00ff0000) >> 16;
            uint8_t blue = (color & 0x0000ff00) >> 8;
            uint8_t alpha = color & 0x000000ff;
            color = (alpha << 24) + (blue << 16) + (green << 8) + red;
            ((uint32_t *)(dest))[destIndex] = color;
        }
    }
}

GLuint createVBO(const GLsizeiptr size, const GLvoid *data, const GLenum usage) {
    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vbo;
}

void resizeView(Platform &platform) {
    GameState *gameState = getGameState(platform);
    platform.memory.doResize = false;

    auto yScale = platform.memory.aspectRatio;
    GLfloat aspectRatioMatrix[16] = {
        1.0, 0,      0,   0,   //
        0,   yScale, 0,   0,   //
        0,   0,      1.0, 0,   //
        0,   0,      0,   1.0, //
    };
    auto aspectRatioMatrixLocation = glGetUniformLocation(gameState->glProgram, "u_AspectRatioMatrix");
    glUniformMatrix4fv(aspectRatioMatrixLocation, 1, GL_FALSE, &aspectRatioMatrix[0]);
}

void rotateView(GameState *gameState) {
    float a = cos(gameState->rotationAngle);
    float b = sin(gameState->rotationAngle);
    GLfloat rotationMatrix[16] = {
        a,  b, 0,   0, //
        -b, a, 0,   0, //
        0,  0, 1.0, 0, //
        0,  0, 0,   1.0,
    };
    GLint rotationMatrixLocation = glGetUniformLocation(gameState->glProgram, "u_RotationMatrix");
    glUniformMatrix4fv(rotationMatrixLocation, 1, GL_FALSE, rotationMatrix);
}

void scaleView(GameState *gameState) {
    float s = gameState->scale;
    GLfloat scaleMatrix[16] = {
        s, 0, 0,   0,   //
        0, s, 0,   0,   //
        0, 0, 1.0, 0,   //
        0, 0, 0,   1.0, //
    };

    auto scaleMatrixLocation = glGetUniformLocation(gameState->glProgram, "u_ScaleMatrix");
    glUniformMatrix4fv(scaleMatrixLocation, 1, GL_FALSE, &scaleMatrix[0]);
}

void initOpenGL(Platform &platform, Resource *vertex_shader, Resource *fragment_shader) {
    GameState *gameState = getGameState(platform);

    gameState->glProgram = buildProgram(platform, vertex_shader, fragment_shader);
    glUseProgram(gameState->glProgram);

    glDepthMask(GL_TRUE);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    resizeView(platform);
    scaleView(gameState);
    rotateView(gameState);
}

void checkShaders(Platform &platform) {
    auto vertex_opt = get_resource(platform, "res/shaders/vertex.glsl");
    if (!vertex_opt.has_value()) {
        platform.abort("Failed to load vertex shader resource");
        return;
    }

    auto fragment_opt = get_resource(platform, "res/shaders/fragment.glsl");
    if (!fragment_opt.has_value()) {
        platform.abort("Failed to load fragment shader resource");
        return;
    }

    if (getGameState(platform)->glProgram == 0 ||    //
        vertex_opt.value()->has_changed(platform) || //
        fragment_opt.value()->has_changed(platform)) {
        initOpenGL(platform, vertex_opt.value(), fragment_opt.value());
    }
}

GameState *beginFrame(Platform &platform) {
    static unsigned int VAO;
    if (!VAO) {
        glGenVertexArrays(1, &VAO);
    }
    glBindVertexArray(VAO);

    GameState *gameState = getGameState(platform);
    gameState->frameCounter++;

    checkShaders(platform);

    if (platform.memory.doResize) {
        resizeView(platform);
    }

    Render::clearScreen(0);
    gameState->renderGroup.count = 0;

    if (platform.input.plusKeyPressed) {
        gameState->scale += 0.01;
    } else if (platform.input.minusKeyPressed) {
        gameState->scale -= 0.01;
    }

    return gameState;
}

/**
 * Moves content to graphics memory.
 */
void loadTextureToGraphicsMemory(Render::Texture *texture, void *content) {
    if (!texture->id) {
        glGenTextures(1, &texture->id);
    }

    glBindTexture(GL_TEXTURE_2D, texture->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, content);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
}

/**
 * Load the texture and kerning information for the specified character
 */
void loadCharacter(Platform &platform, FT_Face face, char loadCharacter, int fontSize, bool hasKerning) {
    float scale = 0.002f; // this is a constant to scale from pixel sizes to
                          // coordinate sizes

    int currentGlyphIndex = FT_Get_Char_Index(face, loadCharacter);
    int error = FT_Load_Glyph(face, currentGlyphIndex, FT_LOAD_RENDER);
    if (error) {
        platform.abort("Couldn't load glyph for " + std::to_string(loadCharacter));
    }

    GameState *gameState = getGameState(platform);

    Render::Character newCharacter =
        gameState->resources.characterMap[fontSize][loadCharacter - Render::firstCharacter];

    newCharacter.value = loadCharacter;
    newCharacter.hasKerning = hasKerning;

    newCharacter.texture.width = face->glyph->bitmap.width;
    newCharacter.texture.height = face->glyph->bitmap.rows;
    newCharacter.texture.xDivision = 1;
    newCharacter.texture.yDivision = 1;

    newCharacter.size = glm::vec2(newCharacter.texture.width, newCharacter.texture.height) * scale;

    newCharacter.bearing =
        glm::vec2(face->glyph->bitmap_left, (newCharacter.texture.height - face->glyph->bitmap_top)) * scale;
    newCharacter.bearing.y *= -1;

    newCharacter.advance = (face->glyph->advance.x >> 6) * scale;

    if (hasKerning) {
        for (char nextChar = Render::firstCharacter; nextChar < Render::lastCharacter; nextChar++) {
            int nextGlyphIndex = FT_Get_Char_Index(face, nextChar);

            FT_Vector kerning;
            FT_Get_Kerning(face, currentGlyphIndex, nextGlyphIndex, FT_KERNING_DEFAULT, &kerning);

            newCharacter.kerning[nextChar - Render::firstCharacter] = (kerning.x >> 6) * scale;
        }
    }

    unsigned bitmapSizeInPixel = newCharacter.texture.width * newCharacter.texture.height;

    void *content = reserveTemporaryMemory(platform, bitmapSizeInPixel * 4);
    uint8_t *src = face->glyph->bitmap.buffer;
    uint32_t *dest = (uint32_t *)content;

    for (unsigned i = 0; i < bitmapSizeInPixel; i++) {
        *dest = (*src << 24) + 0x00ffffff;
        src++;
        dest++;
    }

    loadTextureToGraphicsMemory(&newCharacter.texture, content);

    freeTemporaryMemory(platform);

    gameState->resources.characterMap[fontSize][loadCharacter - Render::firstCharacter] = newCharacter;
}

/**
 * Load the specified font file to be used as game font
 */
void loadFont(Platform &platform, const std::string &fontFileName) {
    int error;

    // setting up font system
    static bool fontLibraryLoaded = false;
    if (!fontLibraryLoaded) {
        error = FT_Init_FreeType(&fontLibrary);
        if (error) {
            platform.abort("Couldn't initialize font library.");
        }
        fontLibraryLoaded = true;
    }

    GameState *gameState = getGameState(platform);

    auto resource_opt = get_resource(platform, fontFileName);
    if (!resource_opt.has_value()) {
        platform.abort("Failed to load font file " + fontFileName);
    }

    auto content = resource_opt.value()->get_content(platform);
    FT_Face face = {};
    error = FT_New_Memory_Face(fontLibrary, (const FT_Byte *)content.data(), (FT_Long)content.size(), 0, &face);
    if (error) {
        platform.abort("Couldn't load font " + fontFileName + ". Errorcode: " + std::to_string(error));
    }

    extractFileName(fontFileName, ".ttf", gameState->resources.fontName);

    bool hasKerning = FT_HAS_KERNING(face);

    for (unsigned fontSizeIndex = 0; fontSizeIndex < sizeof(gameState->resources.availableFontSizes) / 4;
         fontSizeIndex++) {
        int fontSize = getFontSize(gameState, fontSizeIndex);
        int error = FT_Set_Pixel_Sizes(face, fontSize * 5, 0);
        if (error) {
            std::string message = "Couldn't set pixel size to " + std::to_string(fontSize) + ".";
            platform.abort(message);
        }

        for (char currentChar = Render::firstCharacter; currentChar < Render::lastCharacter; currentChar++) {
            loadCharacter(platform, face, currentChar, fontSizeIndex, hasKerning);
        }
    }
}

void extractFileName(std::string fileName, const std::string &fileExtension, char result[]) {
    fileName = fileName.substr(0, fileName.size() - fileExtension.size());

    int index = 0;
    for (char i : fileName) {
        if (i == '/') {
            index = 0;
            continue;
        }
        result[index] = i;
        index++;
    }
    result[index] = '\0';
}

void checkPlayerTileCollision(Player *player, Tile *tile) {
    Math::Rectangle playerRect = {};
    playerRect.position = player->position;
    playerRect.size = player->size;
    //	Math::Rectangle collisionBox = getCollisionBox(tile->rect, playerRect);
    // TODO use actual size of car instead of just the position
    if (Collision::rectangle(tile->rect, player->position)) {
        //		memory->log("Collision!");
    }
}

glm::vec2 calculateTextSize(GameState *gameState, std::string text, Render::FontSize fontSize) {
    if (text.empty()) {
        return {};
    }

    auto result = glm::vec2();

    for (unsigned characterIndex = 0; characterIndex < text.size(); ++characterIndex) {
        Render::Character *c = getCharacter(gameState, text[characterIndex], fontSize);

        result.x += c->advance + c->kerning[text[characterIndex + 1] - Render::firstCharacter];
        result.y = std::max(c->size.y, result.y);
    }

    return result;
}
