#include "Renderer.h"

#include "Helper.h"
#include "RacingToHell.h"

namespace Render {

void clearScreen(uint32_t color) {
    float red = ((color & 0xff000000) >> 24) / 255.0f;
    float green = ((color & 0x00ff0000) >> 16) / 255.0f;
    float blue = ((color & 0x0000ff00) >> 8) / 255.0f;
    glClearColor(red, green, blue, 1.0f);
}

/**
 * position: center of the texture
 * tileIndex: starts in the top left and goes to the right and then down
 * colorMode: 0 for texture colors, 1 for solid color with alpha from texture
 */
void texture(GameState *gameState, Texture *texture, glm::vec2 position,
             glm::vec2 size, glm::vec2 direction, int tileIndex,
             uint8_t colorMode, uint32_t color) {
    size = size * 0.5F;
    glm::vec2 bottomLeft = glm::vec2(-size.x, -size.y);
    glm::vec2 topLeft = glm::vec2(-size.x, size.y);
    glm::vec2 bottomRight = glm::vec2(size.x, -size.y);
    glm::vec2 topRight = glm::vec2(size.x, size.y);

    double angle = Math::angle(direction) - PI / 2;
    bottomLeft = Math::rotate(bottomLeft, angle) + position;
    topLeft = Math::rotate(topLeft, angle) + position;
    bottomRight = Math::rotate(bottomRight, angle) + position;
    topRight = Math::rotate(topRight, angle) + position;

    float r, g, b, a;
    if (colorMode == 1) {
        r = ((color & 0xff000000) >> 24) / 255.0f;
        g = ((color & 0x00ff0000) >> 16) / 255.0f;
        b = ((color & 0x0000ff00) >> 8) / 255.0f;
        a = (color & 0x000000ff) / 255.0f;
    }

    float xStride = 1.0f / (float)texture->xDivision;
    float yStride = 1.0f / (float)texture->yDivision;
    float x = ((float)(tileIndex % texture->xDivision)) * xStride;
    float y = ((float)((int)(tileIndex / texture->yDivision))) * xStride;
    glm::vec2 texTL = glm::vec2(x, y);
    glm::vec2 texTR = glm::vec2(x + xStride, y);
    glm::vec2 texBL = glm::vec2(x, y + yStride);
    glm::vec2 texBR = glm::vec2(x + xStride, y + yStride);

    // holds the screen coordinates with their associated texture coordinates
    const float coordinates[] = {
        //
        bottomLeft.x,  bottomLeft.y,  r, g, b, a, texBL.x, texBL.y, //
        topLeft.x,     topLeft.y,     r, g, b, a, texTL.x, texTL.y, //
        bottomRight.x, bottomRight.y, r, g, b, a, texBR.x, texBR.y, //
        topRight.x,    topRight.y,    r, g, b, a, texTR.x, texTR.y  //
    };

    static GLuint coordinatesBufferID;
    if (!coordinatesBufferID) {
        glGenBuffers(1, &coordinatesBufferID);
    }
    glBindBuffer(GL_ARRAY_BUFFER, coordinatesBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coordinates), coordinates,
                 GL_STATIC_DRAW);

    GLuint positionLocation =
        glGetAttribLocation(gameState->glProgram, "a_Position");
    GLuint textureCoordinatesLocation =
        glGetAttribLocation(gameState->glProgram, "a_TextureCoordinates");
    GLuint colorLocation = glGetAttribLocation(gameState->glProgram, "a_Color");
    GLuint colorSourceLocation =
        glGetUniformLocation(gameState->glProgram, "u_ColorSource");
    GLuint textureUnitLocation =
        glGetUniformLocation(gameState->glProgram, "u_TextureUnit");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glUniform1i(textureUnitLocation, 0);
    glUniform1i(colorSourceLocation, colorMode);

    GLuint stride = 8 * sizeof(GL_FLOAT);

    glEnableVertexAttribArray(positionLocation);
    glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, stride,
                          BUFFER_OFFSET(0));

    glEnableVertexAttribArray(colorLocation);
    glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, stride,
                          BUFFER_OFFSET(2 * sizeof(GL_FLOAT)));

    glEnableVertexAttribArray(textureCoordinatesLocation);
    glVertexAttribPointer(textureCoordinatesLocation, 2, GL_FLOAT, GL_FALSE,
                          stride, BUFFER_OFFSET(6 * sizeof(GL_FLOAT)));

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableVertexAttribArray(positionLocation);
    glDisableVertexAttribArray(colorLocation);
    glDisableVertexAttribArray(textureCoordinatesLocation);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void character(GameState *gameState, char character, glm::vec2 position,
               FontSize fontSize, uint32_t color) {
    Render::Character *c = getCharacter(gameState, character, fontSize);

    position = position + c->bearing;
    position = position + (c->size * 0.5F);

    texture(gameState, &c->texture, position, c->size, glm::vec2(0.0, 1.0), 0,
            1, color);
}

/**
 * Renders the given text with the given color to the screen.
 * Position is the bottom left corner of the text.
 * fontSizeID needs to be one of the following: FontSizeSmall, FontSizeMedium or
 * FontSizeBig.
 */
void text(GameState *gameState, std::string text, glm::vec2 position,
          FontSize fontSize, uint32_t color) {
    if (text.size() == 0) {
        return;
    }

    for (unsigned characterIndex = 0; characterIndex < text.size();
         ++characterIndex) {
        Character *c = getCharacter(gameState, text[characterIndex], fontSize);

        character(gameState, text[characterIndex], position, fontSize, color);

        if (characterIndex < text.size() - 1) {
            position.x +=
                c->advance +
                c->kerning[text[characterIndex + 1] - Render::firstCharacter];
        }
    }
}

void triangle(GameState *gameState, glm::vec2 point1, glm::vec2 point2,
              glm::vec2 point3, uint32_t color) {
    float r = ((color & 0xff000000) >> 24) / 255.0f;
    float g = ((color & 0x00ff0000) >> 16) / 255.0f;
    float b = ((color & 0x0000ff00) >> 8) / 255.0f;
    float a = (color & 0x000000ff) / 255.0f;

    // holds the screen coordinates with their associated colors
    float coordinates[] = {
        point1.x, point1.y, r, g, b, a, //
        point2.x, point2.y, r, g, b, a, //
        point3.x, point3.y, r, g, b, a, //
    };

    static GLuint coordinatesBufferID;
    if (!coordinatesBufferID) {
        glGenBuffers(1, &coordinatesBufferID);
    }
    glBindBuffer(GL_ARRAY_BUFFER, coordinatesBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coordinates), coordinates,
                 GL_STATIC_DRAW);

    GLuint positionLocation =
        glGetAttribLocation(gameState->glProgram, "a_Position");
    GLuint colorLocation = glGetAttribLocation(gameState->glProgram, "a_Color");
    GLuint colorSourceLocation =
        glGetUniformLocation(gameState->glProgram, "u_ColorSource");

    // rectangle supplies it's own color
    glUniform1i(colorSourceLocation, 2);

    GLuint stride = 6 * sizeof(GL_FLOAT);

    glEnableVertexAttribArray(positionLocation);
    glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, stride,
                          BUFFER_OFFSET(0));

    glEnableVertexAttribArray(colorLocation);
    glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, stride,
                          BUFFER_OFFSET(2 * sizeof(GL_FLOAT)));

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(positionLocation);
    glDisableVertexAttribArray(colorLocation);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/**
 * Draws a rectangle to the screen with it's top left position being
 * rect.position and extending to the bottom right
 */
void rectangle(GameState *gameState, Math::Rectangle rect, uint32_t color) {
    glm::vec2 bottomLeft = glm::vec2(0, -rect.size.y);
    glm::vec2 topLeft = glm::vec2(0, 0);
    glm::vec2 bottomRight = glm::vec2(rect.size.x, -rect.size.y);
    glm::vec2 topRight = glm::vec2(rect.size.x, 0);

    bottomLeft = bottomLeft + rect.position;
    topLeft = topLeft + rect.position;
    bottomRight = bottomRight + rect.position;
    topRight = topRight + rect.position;

    float r = ((color & 0xff000000) >> 24) / 255.0f;
    float g = ((color & 0x00ff0000) >> 16) / 255.0f;
    float b = ((color & 0x0000ff00) >> 8) / 255.0f;
    float a = (color & 0x000000ff) / 255.0f;

    // holds the screen coordinates with their associated texture coordinates
    const float coordinates[] = {
        //
        bottomLeft.x,  bottomLeft.y,  r, g, b, a, //
        topLeft.x,     topLeft.y,     r, g, b, a, //
        bottomRight.x, bottomRight.y, r, g, b, a, //
        topRight.x,    topRight.y,    r, g, b, a, //
    };

    static GLuint coordinatesBufferID;
    if (!coordinatesBufferID) {
        glGenBuffers(1, &coordinatesBufferID);
    }
    glBindBuffer(GL_ARRAY_BUFFER, coordinatesBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coordinates), coordinates,
                 GL_STATIC_DRAW);

    GLuint positionLocation =
        glGetAttribLocation(gameState->glProgram, "a_Position");
    GLuint colorLocation = glGetAttribLocation(gameState->glProgram, "a_Color");
    GLuint colorSourceLocation =
        glGetUniformLocation(gameState->glProgram, "u_ColorSource");

    // rectangle supplies it's own color
    glUniform1i(colorSourceLocation, 2);

    GLuint stride = 6 * sizeof(GL_FLOAT);

    glEnableVertexAttribArray(positionLocation);
    glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, stride,
                          BUFFER_OFFSET(0));

    glEnableVertexAttribArray(colorLocation);
    glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, stride,
                          BUFFER_OFFSET(2 * sizeof(GL_FLOAT)));

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableVertexAttribArray(positionLocation);
    glDisableVertexAttribArray(colorLocation);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void circle(GameState *gameState, glm::vec2 position, float radius,
            uint32_t color) {
    texture(gameState, &gameState->resources.bulletTexture, position,
            glm::vec2(radius, radius), glm::vec2(1.0, 0.0), 0, 1, color);
}

RenderAtom *pushRenderAtom(GameState *gameState, AtomType type, float plane) {
    if (gameState->renderGroup.count + 1 ==
        sizeof(gameState->renderGroup.renderAtoms) / sizeof(RenderAtom)) {
        return 0;
    }
    RenderAtom *atom =
        gameState->renderGroup.renderAtoms + gameState->renderGroup.count++;
    atom->type = type;
    atom->plane = plane;
    return atom;
}

RenderAtom *pushRenderAtom(GameState *gameState, AtomType type,
                           AtomPlane plane) {
    return pushRenderAtom(gameState, type, (float)plane);
}

void pushEnableScaling(GameState *gameState, bool enable, float plane) {
    if (enable) {
        pushRenderAtom(gameState, AtomType::SCALE, plane);
    } else {
        pushRenderAtom(gameState, AtomType::NOSCALE, plane);
    }
}

void pushCircle(GameState *gameState, glm::vec2 position, float radius,
                uint32_t color, AtomPlane plane) {
    RenderAtom *atom = pushRenderAtom(gameState, AtomType::CIRCLE, plane);
    if (atom == nullptr) {
        return;
    }
    atom->content.circle.position = position;
    atom->content.circle.radius = radius;
    atom->content.circle.color = color;
}

void pushTexture(GameState *gameState, Texture *texture, glm::vec2 position,
                 glm::vec2 size, glm::vec2 direction, int tileIndex,
                 AtomPlane plane) {
    RenderAtom *atom = pushRenderAtom(gameState, AtomType::TEXTURE, plane);
    if (atom == nullptr) {
        return;
    }

    atom->content.textureRect.texture = *texture;
    Math::Rectangle dimensions = {};
    dimensions.position = position;
    dimensions.size = size;
    atom->content.textureRect.dimensions = dimensions;
    atom->content.textureRect.direction = direction;
    atom->content.textureRect.tileIndex = tileIndex;
}

void pushRectangle(GameState *gameState, Math::Rectangle dimensions,
                   uint32_t color, float plane) {
    RenderAtom *atom = pushRenderAtom(gameState, AtomType::RECTANGLE, plane);
    if (atom == nullptr) {
        return;
    }

    atom->content.rect.dimensions = dimensions;
    atom->content.rect.color = color;
}

void pushRectangle(GameState *gameState, Math::Rectangle dimensions,
                   uint32_t color, AtomPlane plane) {
    pushRectangle(gameState, dimensions, color, (float)plane);
}

void pushTriangle(GameState *gameState, glm::vec2 point1, glm::vec2 point2,
                  glm::vec2 point3, uint32_t color, float plane) {
    RenderAtom *atom = pushRenderAtom(gameState, AtomType::TRIANGLE, plane);
    if (atom == nullptr) {
        return;
    }

    atom->content.triangle.p1 = point1;
    atom->content.triangle.p2 = point2;
    atom->content.triangle.p3 = point3;
    atom->content.triangle.color = color;
}

void pushText(GameState *gameState, std::string text, glm::vec2 position,
              FontSize fontSize, uint32_t color, AtomPlane plane) {
    while (true) {
        RenderAtom *atom = pushRenderAtom(gameState, AtomType::TEXT, plane);
        if (atom == nullptr) {
            return;
        }

        unsigned numChars = 50;
        if (text.size() < numChars) {
            numChars = text.size() + 1;
        }
        for (unsigned i = 0; i < numChars - 1; i++) {
            atom->content.text.characters[i] = text[i];
        }
        atom->content.text.characters[numChars - 1] = '\0';

        atom->content.text.position = position;
        atom->content.text.fontSize = fontSize;
        atom->content.text.color = color;
        if (text.size() < 50) {
            return;
        }

        std::string tmp = text.substr(0, 49);
        position.x += calculateTextLength(gameState, tmp, fontSize);
        text = text.substr(49);
    }
}

void pushAnimation(GameState *gameState, Texture *texture, glm::vec2 position,
                   glm::vec2 size, unsigned *tileIndex, AtomPlane plane,
                   int timing) {
    pushTexture(gameState, texture, position, size, glm::vec2(0, 1), *tileIndex,
                plane);
    if (gameState->frameCounter % timing == 0) {
        *tileIndex += 1;

        if ((int)*tileIndex >= texture->xDivision * texture->yDivision) {
            *tileIndex = 0;
        }
    }
}

void sortRenderAtoms(GameState *gameState, int left = -1, int right = -1) {
    if (left == -1 && right == -1) {
        left = 0;
        right = gameState->renderGroup.count - 1;
    }

    if (left >= right) {
        return;
    }

    RenderAtom *pivot =
        &gameState->renderGroup.renderAtoms[(int)((left + right) / 2)];
    int i = left;
    int j = right;

    do {
        while (gameState->renderGroup.renderAtoms[i].plane < pivot->plane) {
            i++;
        }
        while (gameState->renderGroup.renderAtoms[j].plane > pivot->plane) {
            j--;
        }
        if (i <= j) {
            RenderAtom tmp = {};
            std::memcpy(&tmp, &gameState->renderGroup.renderAtoms[i],
                        sizeof(RenderAtom));
            std::memcpy(&gameState->renderGroup.renderAtoms[i],
                        &gameState->renderGroup.renderAtoms[j],
                        sizeof(RenderAtom));
            std::memcpy(&gameState->renderGroup.renderAtoms[j], &tmp,
                        sizeof(RenderAtom));
            i++;
            j--;
        }
    } while (i <= j);

    if (left < j) {
        sortRenderAtoms(gameState, left, j);
    }
    if (right > i) {
        sortRenderAtoms(gameState, i, right);
    }
}

void setScaleToIdentity(GameState *gameState) {
    GLfloat scaleMatrix[16] = {1.0, 0,   0,   0, //
                               0,   1.0, 0,   0, //
                               0,   0,   1.0, 0, //
                               0,   0,   0,   1.0};

    GLuint scaleMatrixLocation =
        glGetUniformLocation(gameState->glProgram, "u_ScaleMatrix");
    glUniformMatrix4fv(scaleMatrixLocation, 1, GL_FALSE, &scaleMatrix[0]);
}

void flushBuffer(GameMemory *memory) {
    GameState *gameState = getGameState(memory);

    sortRenderAtoms(gameState);

#if RENDER_DEBUG
    int rects = 0, tris = 0, texts = 0, textures = 0, circles = 0, scales = 0,
        noscales = 0;
    std::string ordering = "";
#endif

    for (unsigned i = 0; i < gameState->renderGroup.count; i++) {
        RenderAtom *atom = &gameState->renderGroup.renderAtoms[i];
        switch (atom->type) {
        case AtomType::RECTANGLE: {
#if RENDER_DEBUG
            rects++;
            ordering += " re";
#endif
            Rectangle *r = &atom->content.rect;
            Render::rectangle(gameState, r->dimensions, r->color);
        } break;
        case AtomType::TRIANGLE: {
#if RENDER_DEBUG
            tris++;
            ordering += " tr";
#endif
            Triangle *t = &atom->content.triangle;
            Render::triangle(gameState, t->p1, t->p2, t->p3, t->color);
        } break;
        case AtomType::TEXT: {
#if RENDER_DEBUG
            texts++;
            ordering += " te";
#endif
            Text *t = &atom->content.text;
            std::string textString = std::string(t->characters);
            Render::text(gameState, textString, t->position, t->fontSize,
                         t->color);
        } break;
        case AtomType::TEXTURE: {
#if RENDER_DEBUG
            textures++;
            ordering += " tx";
#endif
            TextureRectangle tr = atom->content.textureRect;
            Render::texture(gameState, &tr.texture, tr.dimensions.position,
                            tr.dimensions.size, tr.direction, tr.tileIndex, 0,
                            0);
        } break;
        case AtomType::CIRCLE: {
#if RENDER_DEBUG
            circles++;
            ordering += " ci";
#endif
            Circle c = atom->content.circle;
            Render::circle(gameState, c.position, c.radius, c.color);
        } break;
        case AtomType::SCALE:
#if RENDER_DEBUG
            scales++;
            ordering += " sc";
#endif
            scaleView(gameState);
            break;
        case AtomType::NOSCALE:
#if RENDER_DEBUG
            noscales++;
            ordering += " ns";
#endif
            setScaleToIdentity(gameState);
            break;
        }
    }

#if RENDER_DEBUG
    memory->log("RenderAtoms: " + std::to_string(gameState->renderGroup.count));
    memory->log("Ordering:" + ordering);
    memory->log("Statistics: ");
    memory->log("\tRectangles: " + std::to_string(rects));
    memory->log("\tTriangles: " + std::to_string(tris));
    memory->log("\tCirlces: " + std::to_string(circles));
    memory->log("\tTexts: " + std::to_string(texts));
    memory->log("\tTextures: " + std::to_string(textures));
    memory->log("\tScales: " + std::to_string(scales));
    memory->log("\tNoScales: " + std::to_string(noscales));
    memory->log("");

    std::string msg;
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        switch (err) {
        case GL_INVALID_OPERATION:
            msg = "INVALID_OPERATION";
            break;
        case GL_INVALID_ENUM:
            msg = "INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            msg = "INVALID_VALUE";
            break;
        case GL_OUT_OF_MEMORY:
            msg = "OUT_OF_MEMORY";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            msg = "INVALID_FRAMEBUFFER_OPERATION";
            break;
        }
        memory->abort("OpenGL error: " + msg);
    }
#endif
}

} // namespace Render
