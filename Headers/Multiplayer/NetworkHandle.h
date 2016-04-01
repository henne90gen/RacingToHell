#pragma once

#include <SFML\Network.hpp>
#include <mutex>
#include "Communication.h"

class NetworkHandle
{
public:
	NetworkHandle();
	~NetworkHandle() {}

	void connect(std::string ip, std::string password, std::string name, int port, float timeout);
	bool create(std::string name, std::string password, int port);
	void disconnect(bool self);

	void run();

	NetworkRelation getRelationship() { return _Relationship; }
	NetworkCommunication getLastResponse();
	NetworkState getState() { return _State; }

	void addPacket(NetworkCommunication Type, sf::Packet newPacket);

	bool getAuthenticated() { return _Authenticated; }
	std::string getMyName() { return _MyName; }
	std::string getMemeberName() { return _MemberName; }

	void setState(NetworkState state) { _State = state; }
	void setPort(unsigned int port) { _Port = port; }
	void setPassword(std::string pw) { _Password = pw; }
	void setAuthenticated(bool Authenticated) { _Authenticated = Authenticated; }
	void setRelation(NetworkRelation relation) { _Relationship = relation; }
private:
	sf::TcpSocket _Socket;
	sf::TcpListener _Listener;

	std::vector<sf::Packet> _ReceivedPackets;
	std::vector < std::pair<NetworkCommunication, sf::Packet>> _SendPackets;
	std::mutex _Mutex;

	unsigned int _TickRate, _Port, _Authenticated;
	sf::Uint32 _Tick;
	std::string _Password;

	NetworkRelation _Relationship;
	NetworkState _State;
	NetworkCommunication _LastResponse;

	std::string _MyName, _MemberName;
};