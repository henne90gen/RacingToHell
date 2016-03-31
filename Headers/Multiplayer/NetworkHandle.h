#pragma once

#include <SFML\Network.hpp>
#include <mutex>
#include "Communication.h"

class NetworkHandle
{
public:
	NetworkHandle();
	~NetworkHandle() {}

	NetworkCommunication connect(std::string ip, std::string password, int port, float timeout);
	void disconnect();

	void run();

	NetworkRelation getRelationship() { return _Relationship; }

	void setState(NetworkState state) { _State = state; }
	void setPort(unsigned int port) { _Port = port; }
	void setPassword(std::string pw) { _Password = pw; }
private:
	sf::TcpSocket _Socket;
	sf::TcpListener _Listener;

	std::vector<sf::Packet> _SendPackets, _ReveivedPackets;
	std::mutex _Mutex;

	unsigned int _TickRate, _Port, _Authenticated;
	sf::Uint32 _Tick;
	std::string _Password;

	NetworkRelation _Relationship;
	NetworkState _State;
};