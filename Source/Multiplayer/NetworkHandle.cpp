#include "stdafx.h"
#include "Multiplayer\NetworkHandle.h"

NetworkHandle::NetworkHandle() : _TickRate(128), _Relationship(NetworkType::None), _Tick(0)
{
}

bool NetworkHandle::connect(std::string ip, int port, float timeout)
{
	_Socket.setBlocking(true);

	return _Socket.connect(ip, port, sf::seconds(timeout)) == sf::Socket::Status::Done;
}

void NetworkHandle::disconnect()
{
	_Socket.disconnect();
}

void NetworkHandle::run()
{
	while (_Relationship != NetworkType::None)
	{
		_Socket.setBlocking(false);

		sf::Packet TestPacket, SendPacket;

		++_Tick;
		sf::sleep(sf::seconds(1.0f / (float)_TickRate));
	}
}
