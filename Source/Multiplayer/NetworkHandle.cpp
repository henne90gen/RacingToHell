#include "stdafx.h"
#include "Multiplayer/NetworkHandle.h"

NetworkHandle::NetworkHandle() : _TickRate(32), _UpdateIntervall(2), _Delay(0.1f * _TickRate),
                                 _Relationship(NetworkRelation::NoRel), _Tick(0), _Authenticated(false) {
    _MPGOCServer = std::make_shared<MPGameObjectContainer>();
    _MPGOCServer->setNetworkHandle(this, true);
}

void NetworkHandle::connect(std::string ip, std::string password, std::string name, int port, float timeout) {
    _Socket.setBlocking(true);

    std::cout << "Connecting..." << std::endl;

    if (_Socket.connect(ip, port, sf::seconds(timeout)) == sf::Socket::Status::Done) {
        sf::Packet AuthPacket;
        AuthPacket << (sf::Uint8) (NetworkCommunication::Authentication) << password << name;

        sf::Clock SyncClock;
        _Socket.send(AuthPacket);

        AuthPacket.clear();

        _Socket.receive(AuthPacket);
        float TimePassed = SyncClock.getElapsedTime().asSeconds() / 2.0f;

        sf::Uint8 Response;
        AuthPacket >> Response;
        if (Response == (sf::Uint8) (NetworkCommunication::ConnectionSuccesfull)) {
            // std::lock_guard<std::mutex> lock(_Mutex);
            _Authenticated = true;
            _LastResponse = std::make_pair(NetworkCommunication::ConnectionSuccesfull, 0);
            _MyName = name;
            AuthPacket >> _MemberName >> _Tick;
            _Tick += (sf::Uint32) (_TickRate * TimePassed);
            std::cout << "Connected with " << _MemberName << std::endl;
        } else if (Response == (sf::Uint8) (NetworkCommunication::WrongPassword)) {
            _LastResponse = std::make_pair(NetworkCommunication::WrongPassword, 0);
            _Socket.disconnect();
            std::cout << "Connection failed due to wrong password" << std::endl;
        } else {
            _LastResponse = std::make_pair(NetworkCommunication::ConnectionFailed, 0);
            _Socket.disconnect();
            std::cout << "Host giving back bad type: " << (int) Response << std::endl;
        }
    } else {
        _LastResponse = std::make_pair(NetworkCommunication::ConnectionFailed, 0);
        _Socket.disconnect();
        std::cout << "Connection failed for some reason" << std::endl;
    }
}

bool NetworkHandle::create(std::string name, std::string password, int port) {
    _MPGOCServer->resetGameObjects(0);

    _Listener.setBlocking(true);

    if (_Listener.listen(port) == sf::Socket::Status::Done) {
        _Port = port;
        _MyName = name;
        _Password = password;
        return true;
    }

    _Listener.close();
    return false;
}

void NetworkHandle::disconnect(bool self) {
    if (_Relationship == NetworkRelation::Host && self) {
        _Socket.disconnect();
        _Authenticated = false;
        _Listener.close();
        _SendPackets.clear();
        _ReceivedPackets.clear();
        _Relationship = NetworkRelation::NoRel;
        _MyName = "";
        _Password = "";
        _Tick = 0;
        std::cout << "You closed the lobby." << std::endl;
    } else if (_Relationship == NetworkRelation::Host && !self) {
        _Socket.disconnect();
        _Authenticated = false;
        _SendPackets.clear();
        _ReceivedPackets.clear();
        std::cout << "The other player left the lobby." << std::endl;
    } else if (_Relationship == NetworkRelation::Client && self) {
        _Socket.disconnect();
        _Authenticated = false;
        _SendPackets.clear();
        _ReceivedPackets.clear();
        _Relationship = NetworkRelation::NoRel;
        _MyName = "";
        std::cout << "You left the lobby." << std::endl;
        _Password = "";
    } else if (_Relationship == NetworkRelation::Client && !self) {
        _Socket.disconnect();
        _Authenticated = false;
        _SendPackets.clear();
        _ReceivedPackets.clear();
        _Relationship = NetworkRelation::NoRel;
        if (_LastResponse.first != NetworkCommunication::Kick) {
            std::cout << "Lobby closed by the host." << std::endl;
        } else {
            std::cout << "You have been kicked by the host." << std::endl;
        }
    }

    _MemberName = "";
}

void NetworkHandle::run() {
    std::cout << "Starting network thread" << std::endl;

    _Socket.setBlocking(false);
    _Listener.setBlocking(false);

    _SyncTimer.restart();

    float Difference = 0;

    while (_Relationship != NetworkRelation::NoRel) {
        sf::Clock beginningTime;
        checkForConnection();

        if (_Socket.getRemoteAddress() != sf::IpAddress::None) {
            if (_Authenticated && _Relationship == NetworkRelation::Host) {
                //_MPGOCServer->update(1 / (float)_TickRate + Difference, 0);
            }

            sf::Packet incommingPacket;
            while (_Socket.receive(incommingPacket) == sf::Socket::Status::Done) {
                if (!_Authenticated) {
                    authenticatePlayer(incommingPacket);
                } else {
                    //receiveData(incommingPacket);
                }
            }

            if (_Authenticated) {
                //sendData();
            }
        } else {
            while (_SendPackets.size() > 0) {
                if (_SendPackets[0].first == NetworkCommunication::Disconnect) {
                    disconnect(true);
                    break;
                } else {
                    _SendPackets.erase(_SendPackets.begin());
                }
            }
        }

        std::cout << _Tick << std::endl;

        if (_Relationship == NetworkRelation::Client && _SyncTimer.getElapsedTime().asSeconds() > 2.0f) {
            synchroniseTick();
            _SyncTimer.restart();
        }

        ++_Tick;

        if (beginningTime.getElapsedTime().asSeconds() < 1.0f / (float) _TickRate) {
            //std::cout << beginningTime.getElapsedTime().asSeconds() << std::endl;
            Difference = 0;
            sf::sleep(sf::seconds((1.0f / (float) _TickRate) - beginningTime.getElapsedTime().asSeconds()));
        } else {
            Difference = beginningTime.getElapsedTime().asSeconds() - (1.0f / (float) _TickRate);
            std::cout << Difference << std::endl;
        }
    }
    _State = NetworkState::NoNetState;
}

std::pair<NetworkCommunication, int> NetworkHandle::getLastResponse() {
    // std::lock_guard<std::mutex> lock(_Mutex);
    std::pair<NetworkCommunication, int> tmp = _LastResponse;
    _LastResponse = std::make_pair(NetworkCommunication::NoNetComm, 0);
    return tmp;
}

void NetworkHandle::addPacket(NetworkCommunication Type, sf::Packet newPacket) {
    // std::lock_guard<std::mutex> lock(_Mutex);
    std::pair<NetworkCommunication, sf::Packet> p;
    p = std::make_pair(Type, newPacket);
//	std::cout << "Packet is being added" << std::endl;
    _SendPackets.push_back(std::make_pair(Type, newPacket));
}

void NetworkHandle::addReceivedPacket(NetworkCommunication Type, sf::Packet newPacket) {
    // std::lock_guard<std::mutex> lock(_Mutex);
    sf::Packet TmpPacket;
    TmpPacket << sf::Uint8(Type) << _Tick;

    const void *data = newPacket.getData();
    size_t len = newPacket.getDataSize();
    TmpPacket.append(data, len);

    _ReceivedPackets.push_back(TmpPacket);
}

void NetworkHandle::setReceivedPackets(std::vector<sf::Packet> packets) {
    // std::lock_guard<std::mutex> lock(_Mutex);
    _ReceivedPackets = packets;
}

void NetworkHandle::checkForConnection() {
    if (_State == NetworkState::Lobby && _Socket.getRemoteAddress() == sf::IpAddress::None &&
        _Relationship == NetworkRelation::Host) {
        _Listener.accept(_Socket);
        if (_Socket.getRemoteAddress() != sf::IpAddress::None) {
            std::cout << "New player is trying to connect" << std::endl;
        }
    }
}

void NetworkHandle::authenticatePlayer(sf::Packet &packet) {
    if (_Relationship == NetworkRelation::Host) {
        sf::Uint8 Type;

        packet >> Type;

        if (Type == (sf::Uint8) NetworkCommunication::Authentication) {
            std::string password;
            packet >> password;

            sf::Packet AuthPacket;
            if (_Password == password) {
                AuthPacket << (sf::Uint8) (NetworkCommunication::ConnectionSuccesfull) << _MyName << _Tick;
                _Socket.send(AuthPacket);
                _Authenticated = true;
                packet >> _MemberName;
                _LastResponse = std::make_pair(NetworkCommunication::ConnectionSuccesfull, 0);
                std::cout << "Connection with " << _MemberName << " accepted" << std::endl;
            } else {
                AuthPacket << (sf::Uint8) (NetworkCommunication::WrongPassword);
                _Socket.send(AuthPacket);
                disconnect(false);
                std::cout << "Connection dropped due to wrong password" << std::endl;
            }
        }
    }
}

void NetworkHandle::synchroniseTick() {
    sf::Packet syncPacket;
    syncPacket << (sf::Uint8) NetworkCommunication::SynchroniseTick << _Tick;
    _PackageTravelTimer.restart();
    _Socket.send(syncPacket);
}

void NetworkHandle::receiveData(sf::Packet &packet) {
    if (packet.getDataSize() > 0) {
        // std::lock_guard<std::mutex> lock(_Mutex);
        sf::Packet tmp = packet;
        sf::Uint8 Type;
        sf::Uint32 Tick;

        tmp >> Type >> Tick;

        switch ((NetworkCommunication) Type) {
            case NetworkCommunication::Disconnect:
                _LastResponse = std::make_pair(NetworkCommunication::Disconnect,
                                               (int) (_Relationship == NetworkRelation::Host));
                disconnect(false);
                break;
            case NetworkCommunication::StartGame:
                _LastResponse = std::make_pair(NetworkCommunication::StartGame, (int) (_Tick - Tick));
                _State = NetworkState::Ingame;
                break;
            case NetworkCommunication::ShotFired:
            case NetworkCommunication::SpawnPlayerBullet:
            case NetworkCommunication::PlayerInformation:
            case NetworkCommunication::PlayerKeyPress:
                _ReceivedPackets.push_back(packet);
                break;
            case NetworkCommunication::EndGame:
                _LastResponse = std::make_pair(NetworkCommunication::EndGame, 0);
                _State = NetworkState::Lobby;
                break;
            case NetworkCommunication::Kick:
                if (_Relationship == NetworkRelation::Client) {
                    _LastResponse = std::make_pair(NetworkCommunication::Kick, 0);
                    disconnect(false);
                }
                break;
            case NetworkCommunication::Ready: {
                sf::Uint8 OnOff;
                tmp >> OnOff;

                _LastResponse = std::make_pair(NetworkCommunication::Ready, (int) OnOff);
                break;
            }
            case NetworkCommunication::SynchroniseTick:
                if (_Relationship == NetworkRelation::Host) {
                    sf::Packet responsePacket;
                    responsePacket << (sf::Uint8) NetworkCommunication::SynchroniseTick << _Tick;
                    _Socket.send(responsePacket);
                } else if (_Relationship == NetworkRelation::Client) {
                    sf::Uint32 newTick =
                            Tick + (sf::Uint32) (_TickRate * _PackageTravelTimer.restart().asSeconds() / 2.0f);

                    std::cout << (int) (newTick - _Tick) << std::endl;

                    _Tick = newTick;
                }
                //std::cout << "Tick: " << _Tick << std::endl;
                //std::cout << "Packetsize: " << _ReceivedPackets.size() << std::endl;
                break;
            default:
                std::cout << "Unexpected communication type: " << (int) Type << std::endl;
                break;
        }
    }
}

void NetworkHandle::sendData() {
    while (_SendPackets.size() > 0) {
        // std::lock_guard<std::mutex> lock(_Mutex);
        sf::Packet TmpPacket;
        TmpPacket << sf::Uint8(_SendPackets[0].first) << _Tick;

        const void *data = _SendPackets[0].second.getData();
        size_t len = _SendPackets[0].second.getDataSize();
        TmpPacket.append(data, len);

        _Socket.send(TmpPacket);

        switch (_SendPackets[0].first) {
            case NetworkCommunication::Disconnect:
                disconnect(true);
                break;
            case NetworkCommunication::StartGame:
                std::cout << "Starting game at tick " << _Tick << std::endl;
                _State = NetworkState::Ingame;
                break;
            case NetworkCommunication::EndGame:
                _State = NetworkState::Lobby;
                break;
            case NetworkCommunication::Kick:
                if (_Relationship == NetworkRelation::Host) {
                    disconnect(false);
                }
                break;
            default:
                break;
        }

        if (_SendPackets.size() > 0) {
            _SendPackets.erase(_SendPackets.begin());
        }
    }
}
