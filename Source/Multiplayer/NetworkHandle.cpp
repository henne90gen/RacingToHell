#include "stdafx.h"
#include "Multiplayer\NetworkHandle.h"

NetworkHandle::NetworkHandle() : _TickRate(128), _Relationship(NetworkRelation::None), _Tick(0), _Authenticated(false), _Port(12345)
{
}

void NetworkHandle::connect(std::string ip, std::string password, int port, float timeout)
{
	_Socket.setBlocking(true);

	if (_Socket.connect(ip, port, sf::seconds(timeout)) == sf::Socket::Status::Done)
	{
		sf::Packet AuthentificationPacket;
		AuthentificationPacket << (sf::Uint8)(NetworkCommunication::Authentication) << password;

		_Socket.send(AuthentificationPacket);

		AuthentificationPacket.clear();

		_Socket.receive(AuthentificationPacket);

		sf::Uint8 Response;
		AuthentificationPacket >> Response;

		if (Response == (sf::Uint8)(NetworkCommunication::ConnectionSuccesfull))
		{
			std::lock_guard<std::mutex> lock(_Mutex);
			_Authenticated = true;
			_LastResponse = NetworkCommunication::ConnectionSuccesfull;
			std::cout << "Connected" << std::endl;
		}
		else if (Response == (sf::Uint8)(NetworkCommunication::WrongPassword))
		{
			_LastResponse = NetworkCommunication::WrongPassword;
		}
		else
		{
			_LastResponse = NetworkCommunication::ConnectionFailed;
		}
	}
	else
	{
		_LastResponse = NetworkCommunication::ConnectionFailed;
	}
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
		std::cout << "You left the lobby." << std::endl;
	}
	else if (_Relationship == NetworkRelation::Client && !self) {
		_Socket.disconnect();
		_Authenticated = false;
		_SendPackets.clear();
		_ReceivedPackets.clear();
		_Relationship = NetworkRelation::None;
		_LastResponse = NetworkCommunication::Disconnect;
		std::cout << "Lobby closed." << std::endl;
	}
}

void NetworkHandle::run()
{
	std::cout << "Starting network thread" << std::endl;

	_Socket.setBlocking(false);
	_Listener.setBlocking(false);

	if (_Relationship == NetworkRelation::Host) {
		_Listener.listen(_Port);
	}

	while (_Relationship != NetworkRelation::None)
	{
		//waits for incomming connections
		if (_State == NetworkState::Lobby && _Socket.getRemoteAddress() == sf::IpAddress::None && _Relationship == NetworkRelation::Host)
		{
			_Listener.accept(_Socket);
			if (_Socket.getRemoteAddress() != sf::IpAddress::None) {
				std::cout << "New player connected" << std::endl;
			}
		}
		
		if (_Socket.getRemoteAddress() != sf::IpAddress::None)
		{
			sf::Packet IncommingPacket;
			_Socket.receive(IncommingPacket);

			if (!_Authenticated)
			{
				if (_Relationship == NetworkRelation::Host)
				{
					sf::Uint8 Type;

					IncommingPacket >> Type;

					if (Type == (sf::Uint8)NetworkCommunication::Authentication)
					{
						std::string password;
						IncommingPacket >> password;

						sf::Packet AuthPacket;
						if (_Password == password)
						{
							AuthPacket << (sf::Uint8)(NetworkCommunication::ConnectionSuccesfull);
							_Socket.send(AuthPacket);
							_Authenticated = true;
						}
						else
						{
							AuthPacket << (sf::Uint8)(NetworkCommunication::WrongPassword);
							_Socket.send(AuthPacket);
							_Socket.disconnect();
							_SendPackets.clear();
							_ReceivedPackets.clear();
							_Authenticated = false;
						}
					}
				}
			}
			else
			{
				//receives data
				if (IncommingPacket.getDataSize() > 0)
				{
					sf::Packet TmpPacket = IncommingPacket;
					sf::Uint8 Type;

					TmpPacket >> Type;

					switch ((NetworkCommunication)Type)
					{
					case NetworkCommunication::Disconnect:
						disconnect(false);
						break;
					default:
						std::cout << "WTF" << std::endl;
						break;
					}
				}

				//sends data
				while (_SendPackets.size() > 0)
				{
					std::lock_guard<std::mutex> lock(_Mutex);
					sf::Packet TmpPacket;
					TmpPacket << sf::Uint8(_SendPackets[0].first) << _Tick << _SendPackets[0].second;

					_Socket.send(TmpPacket);

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
