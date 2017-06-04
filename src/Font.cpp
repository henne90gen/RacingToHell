#include <ft2build.h>
#include FT_FREETYPE_H
#include "Font.h"

namespace Text {

FT_Library fontLibrary;
FT_Face face;

Character characterMap[3][100];
unsigned availableFontSizes[] = { 7, 10, 20 };

void loadCharacter(GameMemory* memory, char loadCharacter, int fontSize) {
	int glyphIndex = FT_Get_Char_Index(face, loadCharacter);
	int error = FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);
	if (error) {
		abort("Couldn't load glyph for " + std::to_string(loadCharacter));
	}
	if (face->glyph->format != FT_GLYPH_FORMAT_BITMAP) {
		error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
		if (error) {
			abort(
					"Couldn't render character " + std::to_string(loadCharacter)
							+ " as bitmap.");
		}
	}

	Character newCharacter = { };
	newCharacter.value = loadCharacter;
	newCharacter.width = face->glyph->bitmap.width;
	newCharacter.height = face->glyph->bitmap.rows;

    unsigned bitmapSizeInPixel = newCharacter.width * newCharacter.height;

    newCharacter.bitmap = reservePermanentMemory(memory, bitmapSizeInPixel);

	memcpy(newCharacter.bitmap, face->glyph->bitmap.buffer, bitmapSizeInPixel);
	characterMap[fontSize][loadCharacter - ' '] = newCharacter;
}

int getFontSize(int fontSizeIndex) {
	return availableFontSizes[fontSizeIndex];
}

void loadFont(GameMemory* memory, std::string fontFileName) {
	int error;
	error = FT_Init_FreeType(&fontLibrary);
	if (error) {
		abort("Couldn't initialize font library.");
	}

    File fontFile = readFile(fontFileName);
	error = FT_New_Memory_Face(fontLibrary, (const FT_Byte *) fontFile.content, fontFile.size, 0, &face);
	if (error) {
		abort("Couldn't load font " + fontFileName + ".");
	}

	for (unsigned fontSizeIndex = 0;
			fontSizeIndex < sizeof(availableFontSizes) / 4; fontSizeIndex++) {
		int fontSize = getFontSize(fontSizeIndex);
//		int error = FT_Set_Char_Size(face, 0, fontSize * 64, WINDOW_WIDTH, WINDOW_HEIGHT);
		int error = FT_Set_Pixel_Sizes(face, fontSize*2, 0);
		if (error) {
			std::string message = "Couldn't set pixel size to "
					+ std::to_string(fontSize) + ".";
			abort(message);
		}


		for (char currentChar = ' '; currentChar < '~'; currentChar++) {
			loadCharacter(memory, currentChar, fontSizeIndex);
		}
	}
}

Character getCharacter(char character, int fontSizeIndex) {
	return characterMap[fontSizeIndex][character - ' '];
}

int getFontSizeIndex(unsigned fontSize) {
	for (unsigned i = 0; i < sizeof(availableFontSizes) / 4; i++) {
		if (availableFontSizes[i] == fontSize) {
			return i;
		}
	}
	abort("Font size " + std::to_string(fontSize) + " not available.");
	return -1;
}

void renderText(VideoBuffer* buffer, std::string text, int posX, int posY,
		unsigned fontSize) {
	Character character;
	int fontSizeIndex = getFontSizeIndex(fontSize);
	for (unsigned i = 0; i < text.size(); i++) {
		character = getCharacter(text[i], fontSizeIndex);

		for (int y = character.height - 1; y >= 0; y--) {
			int yIndex = posY - (character.height - y);
			if (yIndex >= (int) buffer->height || yIndex < 0) {
				continue;
			}

			for (unsigned x = 0; x < character.width; x++) {
				int xIndex = posX + x;
				if (xIndex >= (int) buffer->width || xIndex < 0) {
					continue;
				}

				int bufferIndex = yIndex * buffer->width + (xIndex);
				int glyphIndex = y * character.width + x;
				((uint32_t*) buffer->content)[bufferIndex] |=
						character.bitmap[glyphIndex];
			}
		}
		posX += character.width + 10;
	}
}

void renderCharacterAlpha(VideoBuffer *buffer, char c, int offsetX,
    int offsetY, uint8_t r, uint8_t g, uint8_t b, unsigned fontSize) {
    uint32_t characterColor = (r << 16) | (g << 8) | (b);

    int fontSizeIndex = getFontSizeIndex(fontSize);
    Character character = getCharacter(c, fontSizeIndex);

    int32_t *currentBufferPixel = (int32_t *)buffer->content
        + offsetY * (int)buffer->width + offsetX;
    uint8_t *currentTexturePixel = (uint8_t *)character.bitmap;

    int32_t nextLine = buffer->width - character.width;

    for (int y = 0; y < (int)character.height; ++y) {
        if (offsetY + y < 0) {
            currentBufferPixel += buffer->width;
            currentTexturePixel += character.width;
            continue;
        }
        else if (offsetY + y >= buffer->height) {
            break;
        }

        for (int x = 0; x < (int)character.width; ++x) {
            if (offsetX + x < 0 || offsetX + x >= buffer->width) {
                currentBufferPixel++;
                currentTexturePixel++;
                continue;
            }

            uint8_t *currentBufferPixel8 = (uint8_t *)currentBufferPixel;

            uint8_t textureValue = *currentTexturePixel++;

            if (textureValue == 255) {
                currentTexturePixel++;
                *currentBufferPixel++ = (255 << 24) | characterColor;
            }
            else if (textureValue < 255) {
                currentBufferPixel++;
                currentTexturePixel++;
            }
            else {
                uint8_t bufferA = *currentBufferPixel8;

                float textureAlpha = textureValue / 255.0f;
                float bufferAlpha = bufferA / 255.0f;

                float resultAlpha = textureAlpha
                    + bufferAlpha * (1.0f - textureAlpha);

                if (resultAlpha == 0.0f) {
                    *currentBufferPixel = 0;
                }
                else {
                    float newB = ((bufferAlpha * (1.0f - textureAlpha)
                        * *currentBufferPixel8) + (textureAlpha * b))
                        / resultAlpha;
                    *currentBufferPixel8++ = (uint8_t)newB;

                    float newG = ((bufferAlpha * (1.0f - textureAlpha)
                        * *currentBufferPixel8) + (textureAlpha * g))
                        / resultAlpha;
                    *currentBufferPixel8++ = (uint8_t)newG;

                    float newR = ((bufferAlpha * (1.0f - textureAlpha)
                        * *currentBufferPixel8) + (textureAlpha * r))
                        / resultAlpha;
                    *currentBufferPixel8++ = (uint8_t)newR;

                    float newA = resultAlpha * 255.0f;
                    *currentBufferPixel8++ = (uint8_t)newA;
                }

                currentBufferPixel++;
                currentTexturePixel++;
            }
        }

        currentBufferPixel += nextLine;
    }
}

void renderTextAlpha(VideoBuffer *buffer, Texture* texture, int offsetX,
    int offsetY) {

}

}
