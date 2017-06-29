#include "Renderer.h"
#include "RacingToHell.h"

namespace Render {

void blendColor(uint32_t color, uint32_t* currentBufferPixel) {
	uint8_t *currentBufferPixel8 = (uint8_t *) currentBufferPixel;
	uint8_t *currentColorPixel8 = (uint8_t *) &color;

	uint8_t colorR = *currentColorPixel8++;
	uint8_t colorG = *currentColorPixel8++;
	uint8_t colorB = *currentColorPixel8++;
	uint8_t colorA = *currentColorPixel8++;

	if (colorA == 255) {
		*currentBufferPixel = color;
	} else if (colorA != 0) {
		uint8_t bufferA = *currentBufferPixel8;

		float textureAlpha = colorA / 255.0f;
		float bufferAlpha = (bufferA / 255.0f) * (1.0f - textureAlpha);

		float resultAlpha = textureAlpha + bufferAlpha;

		if (resultAlpha == 0.0f) {
			*currentBufferPixel = 0;
		} else {
			float newR = ((bufferAlpha * *currentBufferPixel8)
					+ (textureAlpha * colorR)) / resultAlpha;
			*currentBufferPixel8++ = (uint8_t) newR;

			float newG = ((bufferAlpha * *currentBufferPixel8)
					+ (textureAlpha * colorG)) / resultAlpha;
			*currentBufferPixel8++ = (uint8_t) newG;

			float newB = ((bufferAlpha * *currentBufferPixel8)
					+ (textureAlpha * colorB)) / resultAlpha;
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

/**
 * point2 and point3 are on the same height and point1 is above the two
 */
void triangleBottom(VideoBuffer *buffer, uint32_t color, Math::Vector2f point1,
		Math::Vector2f point2, Math::Vector2f point3) {

	if (point2.x > point3.x) {
		switchPoint(&point2, &point3);
	}

	float invslope1 = (point2.x - point1.x) / (point2.y - point1.y);
	float invslope2 = (point3.x - point1.x) / (point3.y - point1.y);

	float curx1 = point1.x;
	float curx2 = point1.x;

	for (int scanlineY = point1.y; scanlineY <= point2.y - 1; scanlineY++) {
		for (int x = curx1; x < curx2; x++) {
			uint32_t *currentBufferPixel = ((uint32_t *) buffer->content)
					+ (scanlineY * (int) buffer->width + x);
			blendColor(color, currentBufferPixel);
		}
		curx1 += invslope1;
		curx2 += invslope2;
	}
}

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

// FIXME gap appears when not rendering just top or bottom flat triangle
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

uint16_t &getUShortElement(__m128i *ptr, int index) {
	int bigIndex = index >> 3;
	int smallIndex = index - (bigIndex << 3);
//    return ptr[bigIndex].m128i_u16[smallIndex];
}

__m128i &get128BitElement(__m128i* ptr, int bigIndex, int bigSize, int smallRemainder)
{
	if (bigIndex != bigSize - 1)
	return ptr[bigIndex];
	else if (smallRemainder == 0)
	return ptr[bigIndex];
	else
	{
		for (int i = smallRemainder; i<8; ++i) {
//            ptr[bigIndex].m128i_u16[i] = 1;
		}
		return ptr[bigIndex];
	}
}

uint16_t &getUShortElement256(__m256i *ptr, int index) {
	int bigIndex = index >> 4;
	int smallIndex = index - (bigIndex << 4);
//	return ptr[bigIndex].m256i_u16[smallIndex];
}

__m256i &get256BitElement(__m256i* ptr, int bigIndex, int bigSize, int smallRemainder)
{
	if (bigIndex != bigSize - 1)
	return ptr[bigIndex];
	else if (smallRemainder == 0)
	return ptr[bigIndex];
	else
	{
		for (int i = smallRemainder; i < 16; ++i) {
//		ptr[bigIndex].m256i_u16[i] = 1;
		}
		return ptr[bigIndex];
	}
}

void rectangleSSE256(VideoBuffer *buffer, Math::Rectangle rect,
		uint32_t color) {
	int fullSize = rect.width * rect.height;
	int arraySize = fullSize >> 4;
	int remainder = fullSize % 16;

	if (remainder != 0) {
		++arraySize;
	}

	fullSize = arraySize * 16 * 2;

	__m256i *sourceR = (__m256i *) (_mm_malloc((size_t) (fullSize), 16));
	__m256i *sourceG = (__m256i *) (_mm_malloc((size_t) (fullSize), 16));
	__m256i *sourceB = (__m256i *) (_mm_malloc((size_t) (fullSize), 16));

	uint32_t *bufferContent = (uint32_t *) buffer->content;

	int index = 0;
	int bufferIndex = rect.position.y * buffer->width + rect.position.x;
	int line = buffer->width - rect.width;

	for (int rowIndex = 0; rowIndex < rect.height; ++rowIndex) {
		for (int columnIndex = 0; columnIndex < rect.width; ++columnIndex) {
			uint32_t r = (bufferContent[bufferIndex] & 0x000000ff);
			uint32_t g = (bufferContent[bufferIndex] & 0x0000ff00) >> 8;
			uint32_t b = (bufferContent[bufferIndex++] & 0x00ff0000) >> 16;

			getUShortElement256(sourceR, index) = r;
			getUShortElement256(sourceG, index) = g;
			getUShortElement256(sourceB, index++) = b;
		}

		bufferIndex += line;
	}

	uint32_t rectColorR = (color & 0x000000ff);
	uint32_t rectColorG = (color & 0x0000ff00) >> 8;
	uint32_t rectColorB = (color & 0x00ff0000) >> 16;
	uint32_t rectColorA = (color & 0xff000000) >> 24;

	__m256i rectR = _mm256_set1_epi16(rectColorR * rectColorA);
	__m256i rectG = _mm256_set1_epi16(rectColorG * rectColorA);
	__m256i rectB = _mm256_set1_epi16(rectColorB * rectColorA);

	__m256i sourceA = _mm256_set1_epi16(255 - rectColorA);

	__m256i tmpR = _mm256_set1_epi16(0);
	__m256i tmpG = _mm256_set1_epi16(0);
	__m256i tmpB = _mm256_set1_epi16(0);

	__m256i finalR = _mm256_set1_epi16(0);
	__m256i finalG = _mm256_set1_epi16(0);
	__m256i finalB = _mm256_set1_epi16(0);

	bufferIndex = rect.position.y * buffer->width + rect.position.x;
	int widthCounter = 0;

	for (int i = 0; i < arraySize; ++i) {
		tmpR = _mm256_mullo_epi16(
				get256BitElement(sourceR, i, arraySize, remainder), sourceA);
		tmpG = _mm256_mullo_epi16(
				get256BitElement(sourceG, i, arraySize, remainder), sourceA);
		tmpB = _mm256_mullo_epi16(
				get256BitElement(sourceB, i, arraySize, remainder), sourceA);

		finalR = _mm256_add_epi16(tmpR, rectR);
		finalG = _mm256_add_epi16(tmpG, rectG);
		finalB = _mm256_add_epi16(tmpB, rectB);

		finalR = _mm256_srli_epi16(finalR, 8);
		finalG = _mm256_srli_epi16(finalG, 8);
		finalB = _mm256_srli_epi16(finalB, 8);

		if (i != arraySize - 1 || (i == arraySize && remainder == 0)) {
			for (int j = 0; j < 16; j++) {
//				bufferContent[bufferIndex++] = 0xff000000
//						| finalB.m256i_u16[j] << 16 | finalG.m256i_u16[j] << 8
//						| finalR.m256i_u16[j];

				if (++widthCounter % rect.width == 0) {
					bufferIndex += line;
				}
			}
		} else if (remainder != 0) {
			for (int j = 0; j < remainder; j++) {
//				bufferContent[bufferIndex++] = 0xff000000
//						| finalB.m256i_u16[j] << 16 | finalG.m256i_u16[j] << 8
//						| finalR.m256i_u16[j];
			}

			if (++widthCounter % rect.width == 0) {
				bufferIndex += line;
			}
		}
	}

	if (sourceR) {
		_mm_free(sourceR);
	}

	if (sourceG) {
		_mm_free(sourceG);
	}

	if (sourceB) {
		_mm_free(sourceB);
	}
}

void rectangleSSE(VideoBuffer *buffer, Math::Rectangle rect, uint32_t color) {
	int fullSize = rect.width * rect.height;
	int arraySize = fullSize >> 3;
	int remainder = fullSize % 8;

	if (remainder != 0) {
		++arraySize;
	}

	fullSize = arraySize * 8 * 2;

	__m128i *sourceR = (__m128i *) (_mm_malloc((size_t) (fullSize), 16));
	__m128i *sourceG = (__m128i *) (_mm_malloc((size_t) (fullSize), 16));
	__m128i *sourceB = (__m128i *) (_mm_malloc((size_t) (fullSize), 16));

	uint32_t *bufferContent = (uint32_t *) buffer->content;

	int index = 0;
	int bufferIndex = rect.position.y * buffer->width + rect.position.x;
	int line = buffer->width - rect.width;

	for (int rowIndex = 0; rowIndex < rect.height; ++rowIndex) {
		for (int columnIndex = 0; columnIndex < rect.width; ++columnIndex) {
			uint32_t r = (bufferContent[bufferIndex] & 0x000000ff);
			uint32_t g = (bufferContent[bufferIndex] & 0x0000ff00) >> 8;
			uint32_t b = (bufferContent[bufferIndex++] & 0x00ff0000) >> 16;

			getUShortElement(sourceR, index) = r;
			getUShortElement(sourceG, index) = g;
			getUShortElement(sourceB, index++) = b;
		}

		bufferIndex += line;
	}

	uint32_t rectColorR = (color & 0x000000ff);
	uint32_t rectColorG = (color & 0x0000ff00) >> 8;
	uint32_t rectColorB = (color & 0x00ff0000) >> 16;
	uint32_t rectColorA = (color & 0xff000000) >> 24;

	__m128i rectR = _mm_set1_epi16(rectColorR * rectColorA);
	__m128i rectG = _mm_set1_epi16(rectColorG * rectColorA);
	__m128i rectB = _mm_set1_epi16(rectColorB * rectColorA);

	__m128i sourceA = _mm_set1_epi16(255 - rectColorA);

	__m128i tmpR = _mm_set1_epi16(0);
	__m128i tmpG = _mm_set1_epi16(0);
	__m128i tmpB = _mm_set1_epi16(0);

	__m128i finalR = _mm_set1_epi16(0);
	__m128i finalG = _mm_set1_epi16(0);
	__m128i finalB = _mm_set1_epi16(0);

	bufferIndex = rect.position.y * buffer->width + rect.position.x;
	int widthCounter = 0;

	for (int i = 0; i < arraySize; ++i) {
		tmpR = _mm_mullo_epi16(
				get128BitElement(sourceR, i, arraySize, remainder), sourceA);
		tmpG = _mm_mullo_epi16(
				get128BitElement(sourceG, i, arraySize, remainder), sourceA);
		tmpB = _mm_mullo_epi16(
				get128BitElement(sourceB, i, arraySize, remainder), sourceA);

		finalR = _mm_add_epi16(tmpR, rectR);
		finalG = _mm_add_epi16(tmpG, rectG);
		finalB = _mm_add_epi16(tmpB, rectB);

		finalR = _mm_srli_epi16(finalR, 8);
		finalG = _mm_srli_epi16(finalG, 8);
		finalB = _mm_srli_epi16(finalB, 8);

		if (i != arraySize - 1 || (i == arraySize && remainder == 0)) {
			for (int j = 0; j < 8; j++) {
//				bufferContent[bufferIndex++] = 0xff000000
//						| finalB.m128i_u16[j] << 16 | finalG.m128i_u16[j] << 8
//						| finalR.m128i_u16[j];

				if (++widthCounter % rect.width == 0) {
					bufferIndex += line;
				}
			}
		} else if (remainder != 0) {
			for (int j = 0; j < remainder; j++) {
//				bufferContent[bufferIndex++] = 0xff000000
//						| finalB.m128i_u16[j] << 16 | finalG.m128i_u16[j] << 8
//						| finalR.m128i_u16[j];
			}

			if (++widthCounter % rect.width == 0) {
				bufferIndex += line;
			}
		}
	}

	if (sourceR) {
		_mm_free(sourceR);
	}

	if (sourceG) {
		_mm_free(sourceG);
	}

	if (sourceB) {
		_mm_free(sourceB);
	}
}

void rectangle(VideoBuffer *buffer, Math::Rectangle rect, uint32_t color) {
#if _WIN32
	if (rect.position.y < 0)
	{
		rect.position.y = 0;
	}

	if (rect.position.x < 0)
	{
		rect.position.x = 0;
	}

	if (rect.position.y + rect.height > buffer->height)
	{
		rect.height = buffer->height - rect.position.y;
	}

	if (rect.position.x + rect.width > buffer->width)
	{
		rect.width = buffer->width - rect.position.x;
	}

	rectangleSSE(buffer, rect, color);
	return;
#endif

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

//void debugInformation(GameMemory *memory, VideoBuffer *buffer, Input *input, GameState *gameState) {
//	std::string text = "Player 1: "
//			+ std::to_string(gameState->player.position.x) + ", "
//			+ std::to_string(gameState->player.position.y);
//	Text::renderText(memory, buffer, text, {20, 100}, 7);
//}

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

