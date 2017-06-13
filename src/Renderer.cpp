#include "Renderer.h"
#include "RacingToHell.h"

namespace Render {

void blendColor(uint32_t color, uint32_t* currentBufferPixel) {
	uint8_t *currentBufferPixel8 = (uint8_t *) currentBufferPixel;
	uint8_t *currentColorPointer8 = (uint8_t *) &color;

	uint8_t colorR = *currentColorPointer8++;
	uint8_t colorG = *currentColorPointer8++;
	uint8_t colorB = *currentColorPointer8++;
	uint8_t colorA = *currentColorPointer8++;

	if (color == 0x80010203) {
		int x = 10;
	}

	if (colorA == 255) {
		*currentBufferPixel = color;
	} else if (colorA != 0) {
		uint8_t bufferA = *currentBufferPixel8;

		float textureAlpha = colorA / 255.0f;
		float bufferAlpha = bufferA / 255.0f;

		float resultAlpha = textureAlpha + bufferAlpha * (1.0f - textureAlpha);

		if (resultAlpha == 0.0f) {
			*currentBufferPixel = 0;
		} else {
			float newR = ((bufferAlpha * (1.0f - textureAlpha)
					* *currentBufferPixel8) + (textureAlpha * colorR))
					/ resultAlpha;
			*currentBufferPixel8++ = (uint8_t) newR;

			float newG = ((bufferAlpha * (1.0f - textureAlpha)
					* *currentBufferPixel8) + (textureAlpha * colorG))
					/ resultAlpha;
			*currentBufferPixel8++ = (uint8_t) newG;

			float newB = ((bufferAlpha * (1.0f - textureAlpha)
					* *currentBufferPixel8) + (textureAlpha * colorB))
					/ resultAlpha;
			*currentBufferPixel8++ = (uint8_t) newB;

			float newA = resultAlpha * 255.0f;
			*currentBufferPixel8++ = (uint8_t) newA;
		}
	}
}

void switchPoint(Math::Vector2f *point1, Math::Vector2f *point2) {
	Math::Vector2f tmp = *point1;
	*point1 = *point2;
	*point2 = tmp;
}

void bar(VideoBuffer *buffer, Math::Vector2f position, uint8_t length,
		uint32_t color) {
	Math::Rectangle healthBar = { };
	healthBar.position = {position.x - length / 2, (float) position.y};
	healthBar.width = length;
	healthBar.height = 8;
	Render::rectangle(buffer, healthBar, color);
}

// render bottom flat triangle
void triangleBottom(VideoBuffer *buffer, uint32_t color, Math::Vector2f point1,
		Math::Vector2f point2, Math::Vector2f point3) {

	if (point2.x > point3.x) {
		switchPoint(&point2, &point3);
	}

	float invslope1 = (point2.x - point1.x) / (point2.y - point1.y);
	float invslope2 = (point3.x - point1.x) / (point3.y - point1.y);

	float curx1 = point1.x;
	float curx2 = point1.x;

	for (int scanlineY = point1.y; scanlineY <= point2.y; scanlineY++) {
		for (int x = curx1; x < curx2; x++) {
			uint32_t *currentBufferPixel = ((uint32_t *) buffer->content)
					+ (scanlineY * (int) buffer->width + x);
			blendColor(color, currentBufferPixel);
		}
		curx1 += invslope1;
		curx2 += invslope2;
	}
}

// render top flat triangle
void triangleTop(VideoBuffer *buffer, uint32_t color, Math::Vector2f point1,
		Math::Vector2f point2, Math::Vector2f point3) {

	if (point1.x > point2.x) {
		switchPoint(&point1, &point2);
	}

	float invslope1 = (point3.x - point1.x) / (point3.y - point1.y);
	float invslope2 = (point3.x - point2.x) / (point3.y - point2.y);

	float curx1 = point3.x;
	float curx2 = point3.x;

	for (int scanlineY = point3.y; scanlineY > point1.y; scanlineY--) {
		for (int x = curx1; x < curx2; x++) {
			uint32_t *currentBufferPixel = (uint32_t *) buffer->content
					+ scanlineY * (int) buffer->width + x;
			blendColor(color, currentBufferPixel);
		}
		curx1 -= invslope1;
		curx2 -= invslope2;
	}
}

void triangle(VideoBuffer *buffer, uint32_t color, Math::Vector2f point1,
		Math::Vector2f point2, Math::Vector2f point3) {
	if (point1.y > point2.y) {
		switchPoint(&point1, &point2);
	}
	if (point2.y > point3.y) {
		switchPoint(&point2, &point3);
	}
	if (point1.y > point2.y) {
		switchPoint(&point1, &point2);
	}

	if (point2.y == point3.y) {
		triangleBottom(buffer, color, point1, point2, point3);
	} else if (point1.y == point2.y) {
		triangleTop(buffer, color, point1, point2, point3);
	} else {
		float x = point1.x
				+ ((point2.y - point1.y) / (point3.y - point1.y))
						* (point3.x - point1.x);
		Math::Vector2f point4 = Math::Vector2f { x, point2.y };
		triangleBottom(buffer, color, point1, point2, point4);
		triangleTop(buffer, color, point2, point4, point3);
	}
}

void rectangle(VideoBuffer *buffer, Math::Rectangle rect, uint32_t color) {
	int offsetX = rect.position.x;
	int offsetY = rect.position.y;
	unsigned width = rect.width;
	unsigned height = rect.height;

	uint32_t *currentBufferPixel = (uint32_t *) buffer->content
			+ offsetY * (int) buffer->width + offsetX;

	uint32_t nextLine = buffer->width - width;

	for (int y = 0; y < (int) height; ++y) {
		if (offsetY + y < 0) {
			currentBufferPixel += buffer->width;
			continue;
		} else if (offsetY + y >= (int) buffer->height) {
			break;
		}

		for (int x = 0; x < (int) width; ++x) {
			if (offsetX + x < 0 || offsetX + x >= (int) buffer->width) {
				currentBufferPixel++;
				continue;
			}
			blendColor(color, currentBufferPixel++);

		}
		currentBufferPixel += nextLine;
	}
}

void circle(VideoBuffer* buffer, Math::Vector2f pos, unsigned radius,
		uint32_t color) {
// prevent array index issues
	int x = pos.x;
	int y = pos.y;

	if (x + radius < 0 || x - (int) radius > WINDOW_WIDTH) {
		return;
	}
	if (y + radius < 0 || y - (int) radius > WINDOW_HEIGHT) {
		return;
	}

	for (int i = -1 * (int) radius; i < (int) radius; i++) {
		if (y + i >= WINDOW_HEIGHT) {
			break;
		} else if (y + i < 0) {
			continue;
		}
		float angle = asin(i / (float) radius);
		int xRange = abs(cos(angle) * (float) radius);
		for (int j = -1 * xRange; j < xRange; j++) {
			if (x + j >= WINDOW_WIDTH) {
				break;
			} else if (x + j < 0) {
				continue;
			}
			((uint32_t*) buffer->content)[(y + i) * buffer->width + (x + j)] =
					color;
		}
	}
}

void explosion(VideoBuffer *buffer, GameState *gameState, int x, int y,
		unsigned *explosionIndex) {
	if (gameState->frameCounter % 3 == 0) {
		std::size_t explosionSize = sizeof(gameState->resources.explosion);
		explosionSize /= sizeof(Texture);
		explosionSize--;
		if ((*explosionIndex)++ >= explosionSize) {
			*explosionIndex = 0;
		}
	}
	textureAlpha(buffer, &gameState->resources.explosion[*explosionIndex], x,
			y);
}

void debugInformation(VideoBuffer *buffer, Input *input, GameState *gameState) {
	std::string text = "Player 1: "
			+ std::to_string(gameState->player.position.x) + ", "
			+ std::to_string(gameState->player.position.y);
	Text::renderText(buffer, text, 20, 100, 7);
}

void clearScreen(VideoBuffer *buffer, uint32_t color) {
	for (unsigned int y = 0; y < buffer->height; y++) {
		for (unsigned int x = 0; x < buffer->width; x++) {
			((uint32_t*) buffer->content)[y * buffer->width + x] = color
					+ (255 << 24);
		}
	}
}

void texture(VideoBuffer *buffer, Texture* texture, int offsetX, int offsetY) {
	unsigned startY = 0;
	if (offsetY < 0) {
		startY = 0 - offsetY;
		printf("setting new startY");
	} else if (offsetY >= (int) buffer->height) {
		return;
	}

	unsigned startX = 0;
	if (offsetX < 0) {
		startX = 0 - offsetX;
	} else if (offsetX >= (int) buffer->width) {
		return;
	}

	for (unsigned y = startY; y < texture->height; y++) {
		if (offsetY + y >= buffer->height) {
			continue;
		}
		for (unsigned x = startX; x < texture->width; x++) {
			if (offsetX + x >= buffer->width) {
				continue;
			}
			int bufferIndex = buffer->width * (offsetY + y) + offsetX + x;
			int textureIndex = y * texture->width + x;
			((int*) buffer->content)[bufferIndex] =
					((int*) (texture->content))[textureIndex];
		}
	}
}

#define MIN(a, b) ((a < b) ? a : b)
#define MAX(a, b) ((a > b) ? a : b)
#define ABS(a) ((a < 0) ? -a : a)

void backgroundTexture(VideoBuffer *buffer, Texture* texture, int offsetY) {
	if (offsetY < -(int32_t) texture->height
			|| offsetY > (int32_t) texture->height) {
		return;
	}

	uint32_t *currentTexturePixel = (uint32_t *) texture->content
			- MIN(offsetY * (int32_t )texture->width, 0);
	uint32_t *currentBufferPixel = (uint32_t *) buffer->content
			+ MAX(0, offsetY * (int32_t )buffer->width);

	uint32_t nextLine = buffer->width - texture->width;
	unsigned yMax = 0;

	if (offsetY >= 0) {
		yMax = buffer->height - offsetY;
	} else if (offsetY < 0) {
		yMax = texture->height - ABS(offsetY);
	}

	for (unsigned y = 0; y < yMax; ++y) {
		for (unsigned x = 0; x < texture->width; ++x) {
			*currentBufferPixel++ = *currentTexturePixel++;
		}
		currentBufferPixel += nextLine;
	}
}

void textureAlpha(VideoBuffer *buffer, Texture* texture, int offsetX,
		int offsetY) {
	uint32_t *currentBufferPixel = (uint32_t *) buffer->content
			+ offsetY * (int) buffer->width + offsetX;
	uint32_t *currentTexturePixel = (uint32_t *) texture->content;

	uint32_t nextLine = buffer->width - texture->width;

	for (int y = 0; y < (int) texture->height; ++y) {
		if (offsetY + y < 0) {
			currentBufferPixel += buffer->width;
			currentTexturePixel += texture->width;
			continue;
		} else if (offsetY + y >= (int) buffer->height) {
			break;
		}

		for (int x = 0; x < (int) texture->width; ++x) {
			if (offsetX + x < 0 || offsetX + x >= (int) buffer->width) {
				currentBufferPixel++;
				currentTexturePixel++;
				continue;
			}

			blendColor(*currentTexturePixel, currentBufferPixel);
			currentBufferPixel++;
			currentTexturePixel++;
		}
		currentBufferPixel += nextLine;
	}
}

}
