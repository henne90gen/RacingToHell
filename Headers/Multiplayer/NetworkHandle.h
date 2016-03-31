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

	NetworkType getRelationship() { return _Relationship; }
private:
	sf::TcpSocket _Socket;
	sf::TcpListener _Listener;

	unsigned int _TickRate;
	sf::Uint32 _Tick;
	
	NetworkType _Relationship;
};