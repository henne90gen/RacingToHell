#pragma once

#include <SFML\Network.hpp>
#include <mutex>
#include "Communication.h"

class NetworkHandle
{
public:
	NetworkHandle();
	~NetworkHandle() {}

	void connect(std::string ip, std::string password, int port, float timeout);
	void disconnect();

	void run();

	NetworkRelation getRelationship() { return _Relationship; }
	NetworkCommunication getLastResponse() { return _LastResponse; }

	bool getAuthenticated() { return _Authenticated; }

	void setState(NetworkState state) { _State = state; }
	void setPort(unsigned int port) { _Port = port; }
	void setPassword(std::string pw) { _Password = pw; }
	void setAuthenticated(bool Authenticated) { _Authenticated = Authenticated; }
	void setRelation(NetworkRelation relation) { _Relationship = relation; }
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
	NetworkCommunication _LastResponse;
};