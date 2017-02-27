#pragma once

#include "GameObject/GameObjectType.h"
#include "Collision.h"
#include "Serializable.h"

class GameObject : public Serializable {
public:
    GameObject() {}

    /**
     * Any object that can be on the screen
     * @param pos Position of the GameObject
     * @param type Type of the GameObject
     * @param texture Texture that is going to be used for the Sprite
     */
    GameObject(unsigned int id, GameObjectType type, sf::Vector2f pos, sf::Texture &texture, sf::IntRect textureRect);

    /**
     * Any object that can be on the screen
     * @param stream Input stream that contains all the other information needed to make a GameObject
     * @param type Type of the GameObject
     * @param texture Texture that is going to be used for the Sprite
     */
    GameObject(sf::Packet &packet, GameObjectType type, sf::Texture &texture, sf::IntRect textureRect);

    /**
     * Any object that can be on the screen
     * Use this constructor with extrem caution, no texture is being specified
     * To fully use the GameObject one has to specifiy a texture!
     * @param stream Input stream that contains all the other information needed to make a GameObject
     * @param type Type of the GameObject
     */
    GameObject(sf::Packet &packet, GameObjectType type);

    ~GameObject() {}

    /**
     * Renders the GameObject to the specified RenderWindow
     * @param window Window to draw to
     */
    virtual void render(sf::RenderWindow &window);

    /**
     * Handles events for GameObject
     * @param newEvent Event to be handled
     */
    virtual void handleEvent(sf::Event &newEvent) {}

    /**
     * Updates the GameObject with the given frame time
     * @param frameTime Time that has passed since the last update
     * @param roadSpeed Velocity of the road
     */
    virtual void update(float frameTime);

    /**
     * @return GameObjectTyoe The type of the GameObject
     */
    GameObjectType getType() { return _Type; }

    /**
     * @return Vectorf2f The position of the GameObject on the screen
     */
    virtual sf::Vector2f getPosition() { return _Position; }

    /**
     * @return float The Width of the sprite of the GameObject
     */
    virtual float getWidth() { return _Sprite.getLocalBounds().width; }

    /**
     * @return float The Height of the sprite of the GameObject
     */
    virtual float getHeight() { return _Sprite.getLocalBounds().height; }

    /**
     * @return float The Sprite of the GameObject
     */
    virtual sf::Sprite &getSprite() { return _Sprite; }

    /**
     * Sets the sprite of the GameObject
     * @param sprite New sprite for this GameObject
     */
    void setSprite(sf::Sprite &sprite) { _Sprite = sprite; }

    /**
     * Sets the position of the GameObject
     * @param pos New position for this GameObject
     */
    virtual void setPos(sf::Vector2f pos) {
        _Position = pos;
        _Sprite.setPosition(pos);
    }

    /**
     * Sets the texture of the GameObject
     * @param texture New texture for this GameObject
     */
    void setTexture(sf::Texture &texture);

    /**
     * Changes the color of the sprite
     * @param color New color for the sprite
     */
    void setSpriteColor(sf::Color color) { _Sprite.setColor(color); }

    /**
     * Compares this GameObject with another one to see if they are colliding
     * @param go GameObject to compare this one to
     * @return bool True if the two GameObjects collide
     */
    bool checkForCollision(GameObject &go);

    /**
     * Writes the necessary data for a gameobject to a packet
     */
    virtual void operator>>(sf::Packet &packet);

    /**
     * Reads the necessary data for a gameobject from a packet
     */
    virtual void operator<<(sf::Packet &packet);

    /**
     * Initializes the texture for this GameObject
     */
    void initTexture(sf::Texture &texture, sf::IntRect textureRect);

    sf::Uint32 getID() { return (sf::Uint32) _ID; }

    void setID(sf::Uint32 id) { _ID = id; }

    void setMovement(sf::Vector2f movement) { _Movement = movement; }

protected:
    sf::Uint32 _ID;
    GameObjectType _Type;
    sf::Vector2f _Position, _Movement;
private:
    sf::Sprite _Sprite;
    sf::Texture _Texture;
};

