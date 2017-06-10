#include "Collision.h"

namespace Collision {

/**
 * rectX, rectY specify the top left position of the rectangle
 */
bool rectangle(Math::Rectangle rect, Math::Vector2f pos) {
	if (pos.x > rect.position.x && pos.x < rect.position.x + rect.width) {
		if (pos.y > rect.position.y && pos.y < rect.position.y + rect.height) {
			return true;
		}
	}
	return false;
}

bool rectangle(Math::Rectangle rect1, Math::Rectangle rect2) {
//	width1 += width2;
//	height1 += height2;
//	return rectangle(rect1, width1, height1, rect2);
	return false;
}

}
