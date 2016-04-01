#include "stdafx.h"
#include "Multiplayer\NetworkHandle.h"

NetworkHandle::NetworkHandle() : _TickRate(128), _Relationship(NetworkRelation::None), _Tick(0), _Authenticated(false)
{}

void NetworkHandle::connect(std::string ip, std::string password, std::string name,int port, float timeout)
{
	_Socket.setBlocking(true);

	std::cout << "Connecting..." << std::endl;

	if (_Socket.connect(ip, port, sf::seconds(timeout)) == sf::Socket::Status::Done)
	{
		sf::Packet AuthPacket;
		AuthPacket << (sf::Uint8)(NetworkCommunication::Authentication) << password << name;

		_Socket.send(AuthPacket);

		AuthPacket.clear();

		_Socket.receive(AuthPacket);

		sf::Uint8 Response;
		AuthPacket >> Response;
		if (Response == (sf::Uint8)(NetworkCommunication::ConnectionSuccesfull))
		{
			std::lock_guard<std::mutex> lock(_Mutex);
			_Authenticated = true;
			_LastResponse = NetworkCommunication::ConnectionSuccesfull;
			_MyName = name;
			AuthPacket >> _MemberName;
			std::cout << "Connected with " << _MemberName << std::endl;
		}
		else if (Response == (sf::Uint8)(NetworkCommunication::WrongPassword))
		{
			_LastResponse = NetworkCommunication::WrongPassword;
			_Socket.disconnect();
			std::cout << "Connection failed due to wrong password" << std::endl;
		}
		else
		{
			_LastResponse = NetworkCommunication::ConnectionFailed;
			_Socket.disconnect();
			std::cout << "Host giving back bad type: " << (int)Response << std::endl;
		}
	}
	else
	{
		_LastResponse = NetworkCommunication::ConnectionFailed;
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
		if (_LastResponse != NetworkCommunication::Kick)
		{
			_LastResponse = NetworkCommunication::Disconnect;
			std::cout << "Lobby closed." << std::endl;
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

		++_Tick;
		sf::sleep(sf::seconds(1.0f / (float)_TickRate)); 
	}

	_State = NetworkState::None;
}

NetworkCommunication NetworkHandle::getLastResponse()
{
	std::lock_guard<std::mutex> lock(_Mutex);
	NetworkCommunication tmp = _LastResponse;
	_LastResponse = NetworkCommunication::None;
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

void NetworkHandle::authenticatePlayer(sf::Packet packet)
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
				AuthPacket << (sf::Uint8)(NetworkCommunication::ConnectionSuccesfull) << _MyName;
				_Socket.send(AuthPacket);
				_Authenticated = true;
				packet >> _MemberName;
				_LastResponse = NetworkCommunication::ConnectionSuccesfull;
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

void NetworkHandle::receiveData(sf::Packet packet)
{
	if (packet.getDataSize() > 0)
	{
		sf::Packet TmpPacket = packet;
		sf::Uint8 Type;

		TmpPacket >> Type;

		switch ((NetworkCommunication)Type)
		{
		case NetworkCommunication::Disconnect:
			if (_Relationship == NetworkRelation::Host)
			{
				_LastResponse = NetworkCommunication::Disconnect;

			}
			disconnect(false);
			break;
		case NetworkCommunication::StartGame:
			_LastResponse = NetworkCommunication::StartGame;
			_State = NetworkState::Ingame;
			break;
		case NetworkCommunication::EndGame:
			_LastResponse = NetworkCommunication::EndGame;
			_State = NetworkState::Lobby;
			break;
		case NetworkCommunication::Kick:
			if (_Relationship == NetworkRelation::Client)
			{
				_LastResponse = NetworkCommunication::Kick;
				disconnect(false);
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
		TmpPacket << sf::Uint8(_SendPackets[0].first) << _Tick << _SendPackets[0].second;

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
			if (_Relationship == NetworkRelation::Host)
			{
				disconnect(false);
			}
			break;
		default:
			_SendPackets.erase(_SendPackets.begin());
			break;
		}
	}
}
