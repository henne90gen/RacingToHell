#include "stdafx.h"
#include "Multiplayer\NetworkHandle.h"

NetworkHandle::NetworkHandle() : _TickRate(64), _Relationship(NetworkRelation::None), _Tick(0), _Authenticated(false)
{}

void NetworkHandle::connect(std::string ip, std::string password, std::string name,int port, float timeout)
{
	_Socket.setBlocking(true);

	std::cout << "Connecting..." << std::endl;

	if (_Socket.connect(ip, port, sf::seconds(timeout)) == sf::Socket::Status::Done)
	{
		sf::Packet AuthPacket;
		AuthPacket << (sf::Uint8)(NetworkCommunication::Authentication) << password << name;

		sf::Clock SyncClock;
		_Socket.send(AuthPacket);

		AuthPacket.clear();

		_Socket.receive(AuthPacket);
		float TimePassed = SyncClock.getElapsedTime().asSeconds() / 2.0f;

		sf::Uint8 Response;
		AuthPacket >> Response;
		if (Response == (sf::Uint8)(NetworkCommunication::ConnectionSuccesfull))
		{
			std::lock_guard<std::mutex> lock(_Mutex);
			_Authenticated = true;
			_LastResponse = std::make_pair(NetworkCommunication::ConnectionSuccesfull, 0);
			_MyName = name;
			AuthPacket >> _MemberName >> _Tick;
			_Tick += (sf::Uint32)(_TickRate * TimePassed);
			std::cout << "Connected with " << _MemberName << std::endl;
		}
		else if (Response == (sf::Uint8)(NetworkCommunication::WrongPassword))
		{
			_LastResponse = std::make_pair(NetworkCommunication::WrongPassword, 0);
			_Socket.disconnect();
			std::cout << "Connection failed due to wrong password" << std::endl;
		}
		else
		{
			_LastResponse = std::make_pair(NetworkCommunication::ConnectionFailed, 0);
			_Socket.disconnect();
			std::cout << "Host giving back bad type: " << (int)Response << std::endl;
		}
	}
	else
	{
		_LastResponse = std::make_pair(NetworkCommunication::ConnectionFailed, 0);
		_Socket.disconnect();
		std::cout << "Connection failed for some reason" << std::endl;
	}
}

bool NetworkHandle::create(std::string name, std::string password, int port)
{
	_Listener.setBlocking(true);

	if (_Listener.listen(port) == sf::Socket::Status::Done)
	{
		_Port = port;
		_MyName = name;
		_Password = password;
		return true;
	}

	_Listener.close();
	return false;
}

void NetworkHandle::disconnect(bool self)
{
	if (_Relationship == NetworkRelation::Host && self) {
		_Socket.disconnect();
		_Authenticated = false;
		_Listener.close();
		_SendPackets.clear();
		_ReceivedPackets.clear();
		_Relationship = NetworkRelation::None;
		_MyName = "";
		_Password = "";
		std::cout << "You closed the lobby." << std::endl;
	}
	else if (_Relationship == NetworkRelation::Host && !self) {
		_Socket.disconnect();
		_Authenticated = false;
		_SendPackets.clear();
		_ReceivedPackets.clear();
		std::cout << "The other player left the lobby." << std::endl;
	}
	else if (_Relationship == NetworkRelation::Client && self) {
		_Socket.disconnect();
		_Authenticated = false;
		_SendPackets.clear();
		_ReceivedPackets.clear();
		_Relationship = NetworkRelation::None;
		_MyName = "";
		std::cout << "You left the lobby." << std::endl;
		_Password = "";
	}
	else if (_Relationship == NetworkRelation::Client && !self) {
		_Socket.disconnect();
		_Authenticated = false;
		_SendPackets.clear();
		_ReceivedPackets.clear();
		_Relationship = NetworkRelation::None;
		if (_LastResponse.first != NetworkCommunication::Kick)
		{
			std::cout << "Lobby closed by the host." << std::endl;
	}
		else
		{
			std::cout << "You have been kicked by the host." << std::endl;
		}
	}

	_MemberName = "";
}

void NetworkHandle::run()
{
	std::cout << "Starting network thread" << std::endl;

	_Socket.setBlocking(false);
	_Listener.setBlocking(false);

	while (_Relationship != NetworkRelation::None)
	{
		sf::Clock beginningTime;
		checkForConnection();

		if (_Socket.getRemoteAddress() != sf::IpAddress::None)
		{
			sf::Packet incommingPacket;
			_Socket.receive(incommingPacket);

			if (!_Authenticated)
			{
				authenticatePlayer(incommingPacket);
			}
			else
			{
				receiveData(incommingPacket);
				
				sendData();
			}
		}
		else {
			while (_SendPackets.size() > 0)
			{
				if (_SendPackets[0].first == NetworkCommunication::Disconnect)
				{
					disconnect(true);
					break;
				}
				else
				{
					_SendPackets.erase(_SendPackets.begin());
				}
			}
		}

		//std::cout << _Authenticated << _Tick << std::endl;

		if (_Relationship == NetworkRelation::Client && _Tick % (sf::Uint32)256 == 0) {
			synchroniseTick();
		}

		++_Tick;
		if (beginningTime.getElapsedTime().asSeconds() < 1.0f / (float)_TickRate) {
			sf::sleep(sf::seconds((1.0f / (float)_TickRate) - beginningTime.getElapsedTime().asSeconds()));
		}
	}
	_State = NetworkState::None;
}

std::pair<NetworkCommunication, int> NetworkHandle::getLastResponse()
{
	std::lock_guard<std::mutex> lock(_Mutex);
	std::pair<NetworkCommunication, int> tmp = _LastResponse;
	_LastResponse = std::make_pair(NetworkCommunication::None, 0);
	return tmp;
}

void NetworkHandle::addPacket(NetworkCommunication Type, sf::Packet newPacket)
{
	std::lock_guard<std::mutex> lock(_Mutex);
	_SendPackets.push_back(std::make_pair(Type, newPacket));
}

void NetworkHandle::checkForConnection()
{
	if (_State == NetworkState::Lobby && _Socket.getRemoteAddress() == sf::IpAddress::None && _Relationship == NetworkRelation::Host)
	{
		_Listener.accept(_Socket);
		if (_Socket.getRemoteAddress() != sf::IpAddress::None) {
			std::cout << "New player is trying to connect" << std::endl;
		}
	}
}

void NetworkHandle::authenticatePlayer(sf::Packet& packet)
{
	if (_Relationship == NetworkRelation::Host)
	{
		sf::Uint8 Type;

		packet >> Type;

		if (Type == (sf::Uint8)NetworkCommunication::Authentication)
		{
			std::string password;
			packet >> password;

			sf::Packet AuthPacket;
			if (_Password == password)
			{
				AuthPacket << (sf::Uint8)(NetworkCommunication::ConnectionSuccesfull) << _MyName << _Tick;
				_Socket.send(AuthPacket);
				_Authenticated = true;
				packet >> _MemberName;
				_LastResponse = std::make_pair(NetworkCommunication::ConnectionSuccesfull, 0);
				std::cout << "Connection with " << _MemberName << " accepted" << std::endl;
			}
			else
			{
				AuthPacket << (sf::Uint8)(NetworkCommunication::WrongPassword);
				_Socket.send(AuthPacket);
				disconnect(false);
				std::cout << "Connection dropped due to wrong password" << std::endl;
			}
		}
	}
}

void NetworkHandle::synchroniseTick()
{
	sf::Packet syncPacket;
	syncPacket << (sf::Uint8)NetworkCommunication::SynchroniseTick << _Tick;
	_SynchronisationTimer.restart();
	_Socket.send(syncPacket);
}

void NetworkHandle::receiveData(sf::Packet& packet)
{
	if (packet.getDataSize() > 0)
	{
		sf::Packet tmp = packet;
		sf::Uint8 Type;
		sf::Uint32 Tick;

		tmp >> Type >> Tick;

		switch ((NetworkCommunication)Type)
		{
		case NetworkCommunication::Disconnect:
			_LastResponse = std::make_pair(NetworkCommunication::Disconnect, (int)(_Relationship == NetworkRelation::Host));
			disconnect(false);
			break;
		case NetworkCommunication::StartGame:
			_LastResponse = std::make_pair(NetworkCommunication::StartGame, 0);
			_State = NetworkState::Ingame;
			break;
		case NetworkCommunication::CreateGameObject:
			_ReceivedPackets.push_back(packet);
			break;
		case NetworkCommunication::EndGame:
			_LastResponse = std::make_pair(NetworkCommunication::EndGame, 0);
			_State = NetworkState::Lobby;
			break;
		case NetworkCommunication::Kick:
			if (_Relationship == NetworkRelation::Client)
			{
				_LastResponse = std::make_pair(NetworkCommunication::Kick, 0);
				disconnect(false);
			}
			break;
		case NetworkCommunication::Ready:
		{
			sf::Uint8 OnOff;
			tmp >> OnOff;

			_LastResponse = std::make_pair(NetworkCommunication::Ready, (int)OnOff);
			break;
		}
		case NetworkCommunication::SynchroniseTick:
			if (_Relationship == NetworkRelation::Host) {
				sf::Packet responsePacket;
				responsePacket << (sf::Uint8)NetworkCommunication::SynchroniseTick << _Tick;
				_Socket.send(responsePacket);
				std::cout << "Sending SyncPacket with tick = " << _Tick << std::endl;
			}
			else if (_Relationship == NetworkRelation::Client) {
				std::cout << "Old Tick: " << _Tick << std::endl;
				_Tick = Tick + (sf::Uint32)(_TickRate * _SynchronisationTimer.restart().asSeconds() / 2.0f);
				std::cout << "Received SyncPacket. Tick is now: " << _Tick << " " << (float)(_Tick / 256.0f) << std::endl;
			}
			break;
		default:
			std::cout << "Unexpected communicationtype" << std::endl;
			break;
		}
	}
}

void NetworkHandle::sendData()
{
	while (_SendPackets.size() > 0)
	{
		std::lock_guard<std::mutex> lock(_Mutex);
		sf::Packet TmpPacket;
		TmpPacket << sf::Uint8(_SendPackets[0].first) << _Tick;

		const void* data = _SendPackets[0].second.getData();
		size_t len = _SendPackets[0].second.getDataSize();
		TmpPacket.append(data, len);

		_Socket.send(TmpPacket);

		switch (_SendPackets[0].first) {
		case NetworkCommunication::Disconnect:
			disconnect(true);
			break;
		case NetworkCommunication::StartGame:
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
