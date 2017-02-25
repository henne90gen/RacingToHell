#include "stdafx.h"
#include "GameObject/GameObjectFactory.h"

std::vector<std::shared_ptr<sf::Texture>> &GameObjectFactory::_PlayerCarTextures() {
    static std::vector<std::shared_ptr<sf::Texture>> *result = new std::vector<std::shared_ptr<sf::Texture>>();
    return *result;
}

std::vector<std::shared_ptr<sf::Texture>> &GameObjectFactory::_AICarTextures() {
    static std::vector<std::shared_ptr<sf::Texture>> *result = new std::vector<std::shared_ptr<sf::Texture>>();
    return *result;
}

sf::Texture &GameObjectFactory::_AICarTextureMap() {
    static sf::Texture *result = new sf::Texture();
    return *result;
}

std::vector<std::shared_ptr<sf::Texture>> &GameObjectFactory::_BossCarTextures() {
    static std::vector<std::shared_ptr<sf::Texture>> *result = new std::vector<std::shared_ptr<sf::Texture>>();
    return *result;
}

sf::Texture &GameObjectFactory::_ToolboxTexture() {
    static sf::Texture *result = new sf::Texture();
    return *result;
}

sf::Texture &GameObjectFactory::_EnergyCanisterTexture() {
    static sf::Texture *result = new sf::Texture();
    return *result;
}

sf::Texture &GameObjectFactory::_BulletTexture() {
    static sf::Texture *result = new sf::Texture();
    return *result;
}

sf::SoundBuffer &GameObjectFactory::_JetSoundBuffer() {
    static sf::SoundBuffer *result = new sf::SoundBuffer();
    return *result;
}

sf::Uint32 GameObjectFactory::_CurrentGameObjectID;
sf::Uint32 GameObjectFactory::_DeltaID;

std::shared_ptr<PlayerCar> GameObjectFactory::getPlayerCar(PlayerCarIndex carSkin, sf::Texture &explosionTexture) {
    std::shared_ptr<PlayerCar> player = std::make_shared<PlayerCar>(_DeltaID + _CurrentGameObjectID++, carSkin,
                                                                    (*_PlayerCarTextures().at(
                                                                            (unsigned long) carSkin)),
                                                                    explosionTexture);
    return player;
}

std::shared_ptr<BossCar> GameObjectFactory::getBossCar(int level, int diff, int hp) {
    switch (level) {
        case 0: {
            std::shared_ptr<Tank> boss = std::make_shared<Tank>(_DeltaID + _CurrentGameObjectID++, diff, hp,
                                                                (*_BossCarTextures()[0]), _BulletTexture());
            return boss;
        }
        case 1: {
//            TODO use std::make:shared instead of new-operator
            std::shared_ptr<Mech> boss(new Mech(_DeltaID + _CurrentGameObjectID++, diff, hp, (*_BossCarTextures()[2]),
                                                (*_BossCarTextures()[3]), _BulletTexture()));
            return boss;
        }
        case 2: {
            std::shared_ptr<Jet> boss(
                    new Jet(_DeltaID + _CurrentGameObjectID++, diff, hp, (*_BossCarTextures()[4]), _BulletTexture(),
                            _JetSoundBuffer()));
            return boss;
        }
        case 3: {
            std::shared_ptr<Carrier> boss(
                    new Carrier(_DeltaID + _CurrentGameObjectID++, diff, hp, (*_BossCarTextures()[1]),
                                _BulletTexture()));
            return boss;
        }
        default:
            return NULL;
    }
}

std::shared_ptr<Bullet>
GameObjectFactory::getBullet(sf::Vector2f pos, sf::Vector2f dir, int speed, GameObjectType type) {
    sf::IntRect textureRect(0, 0, _BulletTexture().getSize().x, _BulletTexture().getSize().y);
    return std::make_shared<Bullet>(_DeltaID + _CurrentGameObjectID++, pos, dir, speed, type, _BulletTexture());

//    if (type == GameObjectType::BulletObjectAI || type == GameObjectType::BulletObjectBoss) {
//        std::shared_ptr<Bullet> bullet(
//                new Bullet(_DeltaID + _CurrentGameObjectID++, pos, dir, speed, type, _BulletTexture(), sf::IntRect()));
//        return bullet;
//    } else if (type == GameObjectType::BulletObjectPlayer) {
//        std::shared_ptr<Bullet> bullet(
//                new Bullet(_DeltaID + _CurrentGameObjectID++, pos, dir, speed, type, _BulletTexture(), sf::IntRect()));
//        return bullet;
//    }
//    return NULL;
}


//std::shared_ptr<Bullet> GameObjectFactory::getBullet(GameObjectType type, float speed) {
//    std::shared_ptr<Bullet> bullet(new Bullet(type, _BulletTexture(), speed));
//    return bullet;
//}

std::shared_ptr<Bullet>
GameObjectFactory::getBullet(sf::Packet &packet, sf::Vector2f PlayerPosition, GameObjectType type, sf::Uint32 ID,
                             float Speed) {
    std::shared_ptr<Bullet> bullet(new Bullet(packet, PlayerPosition, type, _BulletTexture(), ID, Speed));
    return bullet;

}

std::shared_ptr<AICar> GameObjectFactory::getAICar(int hp, int roadSpeed) {
    unsigned long carIndex = (unsigned long) (std::rand() % 7);
    int x = carIndex;
    int y = 0;
    if (carIndex > 3) {
        y = 1;
        x = carIndex - 4;
    }
    const sf::IntRect &textureRect = sf::IntRect(x * 40, y * 80, 40, 80);
    std::shared_ptr<AICar> car(
            new AICar(_DeltaID + _CurrentGameObjectID++, hp, roadSpeed, _AICarTextureMap(), textureRect));
    return car;
}

std::shared_ptr<GameObject> GameObjectFactory::getToolbox(sf::Vector2f pos, float speed) {
    const sf::IntRect &textureRect = sf::IntRect(0, 0, _ToolboxTexture().getSize().x, _ToolboxTexture().getSize().y);
    std::shared_ptr<GameObject> toolbox(
            new GameObject(_DeltaID + _CurrentGameObjectID++, GameObjectType::Tools, pos, _ToolboxTexture(),
                           textureRect));
    toolbox->setMovement(sf::Vector2f(0, speed));
    return toolbox;
}

std::shared_ptr<GameObject> GameObjectFactory::getCanister(sf::Vector2f pos, float speed) {
    const sf::IntRect &textureRect = sf::IntRect(0, 0, _EnergyCanisterTexture().getSize().x,
                                                 _EnergyCanisterTexture().getSize().y);
    std::shared_ptr<GameObject> canister(
            new GameObject(_DeltaID + _CurrentGameObjectID++, GameObjectType::Canister, pos, _EnergyCanisterTexture(),
                           textureRect));
    canister->setMovement(sf::Vector2f(0, speed));
    return canister;
}

std::shared_ptr<PlayerCar> GameObjectFactory::getPlayerCar(sf::Packet &packet, sf::Texture &explosionTexture) {
    std::shared_ptr<PlayerCar> player = std::make_shared<PlayerCar>(packet, _PlayerCarTextures(), explosionTexture);
    return player;
}

std::shared_ptr<BossCar> GameObjectFactory::getBossCar(int level, sf::Packet &packet) {
    switch (level) {
        case 0: {
            std::shared_ptr<Tank> boss(
                    new Tank(packet, (*_BossCarTextures()[0]), _BulletTexture()));
            return boss;
        }
        case 1: {
            std::shared_ptr<Mech> boss(
                    new Mech(packet, (*_BossCarTextures()[2]), (*_BossCarTextures()[3]), _BulletTexture()));
            return boss;
        }
        case 2: {
            std::shared_ptr<Jet> boss(
                    new Jet(packet, (*_BossCarTextures()[4]), _BulletTexture(), _JetSoundBuffer()));
            return boss;
        }
        case 3: {
            std::shared_ptr<Carrier> boss(
                    new Carrier(packet, (*_BossCarTextures()[1]), _BulletTexture()));
            return boss;
        }
        default:
            return NULL;
    }
}

std::shared_ptr<Bullet> GameObjectFactory::getBullet(sf::Packet &packet, GameObjectType type) {
    if (type == GameObjectType::BulletObjectAI || type == GameObjectType::BulletObjectBoss) {
        std::shared_ptr<Bullet> bullet(
                new Bullet(packet, type, _BulletTexture()));
        return bullet;
    } else if (type == GameObjectType::BulletObjectPlayer) {
        std::shared_ptr<Bullet> bullet(
                new Bullet(packet, type, _BulletTexture()));
        return bullet;
    }

    return NULL;
}

std::shared_ptr<AICar> GameObjectFactory::getAICar(sf::Packet &packet) {
    unsigned long carIndex = (unsigned long) (std::rand() % 7);
    int x = carIndex;
    int y = 0;
    if (carIndex > 3) {
        y = 1;
        x = carIndex - 4;
    }
    const sf::IntRect &textureRect = sf::IntRect(x * 40, y * 80, (x + 1) * 40, (y + 1) * 80);
    std::shared_ptr<AICar> car(
            new AICar(packet, _AICarTextureMap(), textureRect));
    return car;
}

std::shared_ptr<GameObject> GameObjectFactory::getToolbox(sf::Packet &packet) {
    const sf::IntRect &textureRect = sf::IntRect(0, 0, _ToolboxTexture().getSize().x, _ToolboxTexture().getSize().y);
    std::shared_ptr<GameObject> toolbox(
            new GameObject(packet, GameObjectType::Tools, _ToolboxTexture(), textureRect));
    return toolbox;
}

std::shared_ptr<GameObject> GameObjectFactory::getCanister(sf::Packet &packet) {
    const sf::IntRect &textureRect = sf::IntRect(0, 0, _EnergyCanisterTexture().getSize().x,
                                                 _EnergyCanisterTexture().getSize().y);
    std::shared_ptr<GameObject> canister(
            new GameObject(packet, GameObjectType::Canister, _EnergyCanisterTexture(), textureRect));
    return canister;
}

void GameObjectFactory::scanPacketForGO(int level, sf::Packet &packet, std::vector<std::shared_ptr<GameObject>> &gos,
                                        sf::Texture &explosionTexture) {
    sf::Uint8 type;
    packet >> type;
    switch ((GameObjectType) type) {
        case GameObjectType::AI:
            gos.push_back(getAICar(packet));
            break;
        case GameObjectType::BulletObjectAI:
            gos.push_back(getBullet(packet, (GameObjectType) type));
            break;
        case GameObjectType::BulletObjectBoss:
            gos.push_back(getBullet(packet, (GameObjectType) type));
            break;
        case GameObjectType::BulletObjectPlayer:
            gos.push_back(getBullet(packet, (GameObjectType) type));
            break;
        case GameObjectType::Tools:
            gos.push_back(getToolbox(packet));
            break;
        case GameObjectType::Canister:
            gos.push_back(getCanister(packet));
            break;
        case GameObjectType::Player:
            gos.push_back(getPlayerCar(packet, explosionTexture));
            break;
        case GameObjectType::Boss:
            gos.push_back(getBossCar(level, packet));
            break;
    }
}

void GameObjectFactory::load() {
    // Loading Textures
    for (unsigned int i = 1; i < 7; i++) {
        sf::Texture texture;
        if (texture.loadFromFile("Resources/Texture/PlayerCar/playercar" + std::to_string(i) + ".png")) {
            texture.setSmooth(true);
            _PlayerCarTextures().push_back(std::make_shared<sf::Texture>(texture));
        }
    }

    if (!_AICarTextureMap().loadFromFile("Resources/Texture/TrafficCar/TrafficTextureMap.png")) {
        std::cout << "Couldn't load car texture!" << std::endl;
    }

    std::string bossTextures[] =
            {
                    "Tank",
                    "Carrier",
                    "mech-top", "mech-legs",
                    "Jet"
            };

    for (int i = 0; i < sizeof(bossTextures) / sizeof(std::string); i++) {
        std::shared_ptr<sf::Texture> texture(new sf::Texture());
        texture->loadFromFile("Resources/Texture/BossCar/" + bossTextures[i] + ".png");
        _BossCarTextures().push_back(texture);
    }

    _BulletTexture().loadFromFile("Resources/Texture/Object/Bullet.png");
    _ToolboxTexture().loadFromFile("Resources/Texture/Object/toolbox.png");
    _EnergyCanisterTexture().loadFromFile("Resources/Texture/Object/canister.png");

    // Loading sounds
    _JetSoundBuffer().loadFromFile("Resources/Sound/jetengine.wav");
}
