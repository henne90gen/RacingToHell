#include "Renderer.h"
#include "RacingToHell.h"

namespace Render {

void healthBar(VideoBuffer *buffer, Math::Vector2f position, uint8_t health, uint32_t color) {
	Math::Rectangle healthBar = { };
	healthBar.position = {position.x - health / 2, (float) position.y};
	healthBar.width = health;
	healthBar.height = 8;
	Render::rectangle(buffer, healthBar, 0xff0000ff);
}

void rectangle(VideoBuffer * buffer, Math::Rectangle rect, uint32_t color) {
	int offsetX = rect.position.x;
	int offsetY = rect.position.y;
	unsigned width = rect.width;
	unsigned height = rect.height;

	int32_t *currentBufferPixel = (int32_t *) buffer->content
			+ offsetY * (int) buffer->width + offsetX;

	int32_t nextLine = buffer->width - width;

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

			uint8_t *currentBufferPixel8 = (uint8_t *) currentBufferPixel;

			uint8_t textureB = (color & 0xff000000) >> 24;
			uint8_t textureG = (color & 0x00ff0000) >> 16;
			uint8_t textureR = (color & 0x0000ff00) >> 8;
			uint8_t textureA = color & 0x000000ff;

			if (textureA == 255) {
				color = (textureB << 24) + (textureG << 16) + (textureR << 8)
						+ textureA;
				*currentBufferPixel++ = color;
			} else if (textureA == 0) {
				currentBufferPixel++;
			} else {
				uint8_t bufferA = *currentBufferPixel8;

				float textureAlpha = textureA / 255.0f;
				float bufferAlpha = bufferA / 255.0f;

				float resultAlpha = textureAlpha
						+ bufferAlpha * (1.0f - textureAlpha);

				if (resultAlpha == 0.0f) {
					*currentBufferPixel = 0;
				} else {
					float newB = ((bufferAlpha * (1.0f - textureAlpha)
							* *currentBufferPixel8) + (textureAlpha * textureB))
							/ resultAlpha;
					*currentBufferPixel8++ = (uint8_t) newB;

					float newG = ((bufferAlpha * (1.0f - textureAlpha)
							* *currentBufferPixel8) + (textureAlpha * textureG))
							/ resultAlpha;
					*currentBufferPixel8++ = (uint8_t) newG;

					float newR = ((bufferAlpha * (1.0f - textureAlpha)
							* *currentBufferPixel8) + (textureAlpha * textureR))
							/ resultAlpha;
					*currentBufferPixel8++ = (uint8_t) newR;

					float newA = resultAlpha * 255.0f;
					*currentBufferPixel8++ = (uint8_t) newA;
				}

				currentBufferPixel++;
			}
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

void clearScreen(VideoBuffer *buffer, int color) {
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
	int32_t *currentBufferPixel = (int32_t *) buffer->content
			+ offsetY * (int) buffer->width + offsetX;
	int32_t *currentTexturePixel = (int32_t *) texture->content;

	int32_t nextLine = buffer->width - texture->width;

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

			uint8_t *currentBufferPixel8 = (uint8_t *) currentBufferPixel;
			uint8_t *currentTexturePixel8 = (uint8_t *) currentTexturePixel;

			uint8_t textureB = *currentTexturePixel8++;
			uint8_t textureG = *currentTexturePixel8++;
			uint8_t textureR = *currentTexturePixel8++;
			uint8_t textureA = *currentTexturePixel8++;

			if (textureA == 255) {
				*currentBufferPixel++ = *currentTexturePixel++;
			} else if (textureA == 0) {
				currentBufferPixel++;
				currentTexturePixel++;
			} else {
				uint8_t bufferA = *currentBufferPixel8;

				float textureAlpha = textureA / 255.0f;
				float bufferAlpha = bufferA / 255.0f;

				float resultAlpha = textureAlpha
						+ bufferAlpha * (1.0f - textureAlpha);

				if (resultAlpha == 0.0f) {
					*currentBufferPixel = 0;
				} else {
					float newB = ((bufferAlpha * (1.0f - textureAlpha)
							* *currentBufferPixel8) + (textureAlpha * textureB))
							/ resultAlpha;
					*currentBufferPixel8++ = (uint8_t) newB;

					float newG = ((bufferAlpha * (1.0f - textureAlpha)
							* *currentBufferPixel8) + (textureAlpha * textureG))
							/ resultAlpha;
					*currentBufferPixel8++ = (uint8_t) newG;

					float newR = ((bufferAlpha * (1.0f - textureAlpha)
							* *currentBufferPixel8) + (textureAlpha * textureR))
							/ resultAlpha;
					*currentBufferPixel8++ = (uint8_t) newR;

					float newA = resultAlpha * 255.0f;
					*currentBufferPixel8++ = (uint8_t) newA;
				}

				currentBufferPixel++;
				currentTexturePixel++;
			}
		}

		currentBufferPixel += nextLine;
	}
}

}
