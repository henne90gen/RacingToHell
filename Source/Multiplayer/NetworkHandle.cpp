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

void NetworkHandle::disconnect()
{
	_Socket.disconnect();
	_Authenticated = false;
}

void NetworkHandle::run()
{
	_Socket.setBlocking(false);
	_Listener.setBlocking(false);

	while (_Relationship != NetworkRelation::None)
	{
		//waits for incomming connections
		if (_State == NetworkState::Lobby && _Socket.getRemoteAddress() == sf::IpAddress::None)
		{
			_Listener.listen(_Port);
			_Listener.accept(_Socket);
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
						}
					}
				}
			}
			else
			{
				//sends data
				while (_SendPackets.size() > 0)
				{
					sf::Packet TmpPacket;

					std::lock_guard<std::mutex> lock(_Mutex);
					TmpPacket << _Tick << _SendPackets[0];

					_Socket.send(TmpPacket);

					_SendPackets.erase(_SendPackets.begin());
				}

				//receives data
			}
		}

		

		std::cout << "Tick:" << _Tick << std::endl;

		++_Tick;
		sf::sleep(sf::seconds(1.0f / (float)_TickRate)); 
	}
}
