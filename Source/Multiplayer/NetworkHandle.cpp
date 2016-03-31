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
	_Socket.setBlocking(false);
	_Listener.setBlocking(false);

	while (_Relationship != NetworkRelation::None)
	{
		//sends data
		while (_SendPackets.size() > 0)
		{
			sf::Packet TmpPacket;
			TmpPacket << _Tick << _SendPackets[0];

			_Socket.send(TmpPacket);

			_SendPackets.erase(_SendPackets.begin());
		}

		sf::Uin

		++_Tick;
		sf::sleep(sf::seconds(1.0f / (float)_TickRate));
	}
}
