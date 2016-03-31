#pragma once

#include <SFML\Network.hpp>
#include "Communication.h"

class NetworkHandle
{
public:
	NetworkHandle();
	~NetworkHandle() {}

	bool connect(std::string ip, int port, float timeout);
	void disconnect();

	void run();

	NetworkRelation getRelationship() { return _Relationship; }

	void setState(NetworkState state) { _State = state; }
	void setPort(unsigned int port) { _Port = port; }
private:
	sf::TcpSocket _Socket;
	sf::TcpListener _Listener;

	std::vector<sf::Packet> _SendPackets, _ReveivedPackets;

	unsigned int _TickRate, _Port;
	sf::Uint32 _Tick;
	
	NetworkRelation _Relationship;
	NetworkState _State;
};