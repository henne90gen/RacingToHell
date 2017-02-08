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

sf::SoundBuffer &GameObjectFactory::_AIShotSoundBuffer() {
    static sf::SoundBuffer *result = new sf::SoundBuffer();
    return *result;
}

sf::SoundBuffer &GameObjectFactory::_PlayerShotSoundBuffer() {
    static sf::SoundBuffer *result = new sf::SoundBuffer();
    return *result;
}

sf::SoundBuffer &GameObjectFactory::_JetSoundBuffer() {
    static sf::SoundBuffer *result = new sf::SoundBuffer();
    return *result;
}

sf::Uint32 GameObjectFactory::_CurrentGameObjectID;
sf::Uint32 GameObjectFactory::_DeltaID;

std::shared_ptr<PlayerCar> GameObjectFactory::getPlayerCar(PlayerCarIndex carSkin) {
    std::shared_ptr<PlayerCar> player(
            new PlayerCar(_DeltaID + _CurrentGameObjectID++, carSkin, (*_PlayerCarTextures().at(
                    (unsigned long) carSkin))));
    return player;
}

std::shared_ptr<BossCar> GameObjectFactory::getBossCar(int level, int diff, int hp) {
    switch (level) {
        case 0: {
            std::shared_ptr<Tank> boss(
                    new Tank(_DeltaID + _CurrentGameObjectID++, diff, hp, (*_BossCarTextures()[0]), _BulletTexture()));
            return boss;
        }
        case 1: {
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
    if (type == GameObjectType::BulletObjectAI || type == GameObjectType::BulletObjectBoss) {
        std::shared_ptr<Bullet> bullet(
                new Bullet(_DeltaID + _CurrentGameObjectID++, pos, dir, speed, type, _BulletTexture()));
        return bullet;
    } else if (type == GameObjectType::BulletObjectPlayer) {
        std::shared_ptr<Bullet> bullet(
                new Bullet(_DeltaID + _CurrentGameObjectID++, pos, dir, speed, type, _BulletTexture()));
        return bullet;
    }
    return NULL;
}


std::shared_ptr<Bullet> GameObjectFactory::getBullet(GameObjectType type, float speed) {
    std::shared_ptr<Bullet> bullet(new Bullet(type, _BulletTexture(), speed));
    return bullet;
}

std::shared_ptr<Bullet>
GameObjectFactory::getBullet(sf::Packet &packet, sf::Vector2f PlayerPosition, GameObjectType type, sf::Uint32 ID,
                             float Speed) {
    std::shared_ptr<Bullet> bullet(new Bullet(packet, PlayerPosition, type, _BulletTexture(), ID, Speed));
    return bullet;

}

std::shared_ptr<AICar> GameObjectFactory::getAICar(int hp, int roadSpeed) {
    std::shared_ptr<AICar> car(
            new AICar(_DeltaID + _CurrentGameObjectID++, hp, roadSpeed, (*_AICarTextures().at(
                    (unsigned long) (std::rand() % 7)))));
    return car;
}

std::shared_ptr<GameObject> GameObjectFactory::getToolbox(sf::Vector2f pos) {
    std::shared_ptr<GameObject> toolbox(
            new GameObject(_DeltaID + _CurrentGameObjectID++, pos, GameObjectType::Tools, _ToolboxTexture()));
    return toolbox;
}

std::shared_ptr<GameObject> GameObjectFactory::getCanister(sf::Vector2f pos) {
    std::shared_ptr<GameObject> canister(
            new GameObject(_DeltaID + _CurrentGameObjectID++, pos, GameObjectType::Canister, _EnergyCanisterTexture()));
    return canister;
}

std::shared_ptr<PlayerCar> GameObjectFactory::getPlayerCar(sf::Packet &packet) {
    std::shared_ptr<PlayerCar> player(new PlayerCar(packet, _PlayerCarTextures()));
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
    std::shared_ptr<AICar> car(new AICar(packet, (*_AICarTextures().at((unsigned long) (std::rand() % 7)))));
    return car;
}

std::shared_ptr<GameObject> GameObjectFactory::getToolbox(sf::Packet &packet) {
    std::shared_ptr<GameObject> toolbox(new GameObject(packet, GameObjectType::Tools, _ToolboxTexture()));
    return toolbox;
}

std::shared_ptr<GameObject> GameObjectFactory::getCanister(sf::Packet &packet) {
    std::shared_ptr<GameObject> canister(new GameObject(packet, GameObjectType::Canister, _EnergyCanisterTexture()));
    return canister;
}

void GameObjectFactory::scanPacketForGO(int level, sf::Packet &packet, std::vector<std::shared_ptr<GameObject>> &gos) {
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
            gos.push_back(getPlayerCar(packet));
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

    for (int i = 1; i <= 8; i++) {
        std::shared_ptr<sf::Texture> texture(new sf::Texture());
        (*texture).loadFromFile("Resources/Texture/TrafficCar/Traffic" + std::to_string(i) + ".png");
        _AICarTextures().push_back(texture);
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
    _AIShotSoundBuffer().loadFromFile("Resources/Sound/shotAI.wav");
    _PlayerShotSoundBuffer().loadFromFile("Resources/Sound/shotPlayer.wav");
    _JetSoundBuffer().loadFromFile("Resources/Sound/jetengine.wav");
}
