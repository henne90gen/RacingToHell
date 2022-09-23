#include "Collision.h"

namespace Collision {

/**
 * rect: position is top left, size extends to bottom right
 */
bool rectangle(Math::Rectangle rect, glm::vec2 pos) {
	if (pos.x > rect.position.x && pos.x < rect.position.x + rect.size.x) {
		if (pos.y < rect.position.y && pos.y > rect.position.y - rect.size.y) {
			return true;
		}
	}
	return false;
}

}
