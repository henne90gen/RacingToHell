#pragma once

#include <mutex>
#include "Multiplayer/Communication.h"
#include "Multiplayer/MPGameObjectContainer.h"

class MPGameObjectContainer;

class NetworkHandle
{
public:
	NetworkHandle();
	~NetworkHandle() {}

	void connect(std::string ip, std::string password, std::string name, int port, float timeout);
	bool create(std::string name, std::string password, int port);
	void disconnect(bool self);

	void run();

	/*
		@return NetworkRelation The part of the network relationship this handle is in
	*/
	NetworkRelation getRelation() { return _Relationship; }
	std::pair<NetworkCommunication, int> getLastResponse();
	NetworkState getState() { return _State; }

	void addPacket(NetworkCommunication Type, sf::Packet newPacket);
	void addReceivedPacket(NetworkCommunication Type, sf::Packet newPacket);
	void setReceivedPackets(std::vector<sf::Packet> packets);

	bool getAuthenticated() { return _Authenticated; }
	std::string getMyName() { return _MyName; }
	std::string getMemberName() { return _MemberName; }
	sf::Uint32 getTick() { return _Tick; }
	int getTickRate() { return _TickRate; }

	std::vector<sf::Packet> getReceivedPackets() { /*std::lock_guard<std::mutex> lock(_Mutex);*/  return _ReceivedPackets; }
	sf::Packet getNextPacket() { sf::Packet p = _ReceivedPackets.back(); _ReceivedPackets.pop_back(); return p; }
	std::vector<std::pair<NetworkCommunication, sf::Packet>>& getSendPackets() { return _SendPackets; }

	int getDelay() { return _Delay; }

	void setState(NetworkState state) { _State = state; }
	void setPort(unsigned int port) { _Port = port; }
	void setPassword(std::string pw) { _Password = pw; }
	void setAuthenticated(bool Authenticated) { _Authenticated = Authenticated; }
	void setRelation(NetworkRelation relation) { _Relationship = relation; }

private:
	void checkForConnection();
	void authenticatePlayer(sf::Packet& packet);
	void synchroniseTick();

	void receiveData(sf::Packet& packet);
	void sendData();

	sf::TcpSocket _Socket;
	sf::TcpListener _Listener;

	std::shared_ptr<MPGameObjectContainer>  _MPGOCServer;

	std::vector<sf::Packet> _ReceivedPackets;
	std::vector<std::pair<NetworkCommunication, sf::Packet>> _SendPackets;
	// std::mutex _Mutex;

	unsigned int _TickRate, _UpdateIntervall, _Port, _Authenticated;
	int _Delay;
	sf::Uint32 _Tick;
	std::string _Password;

	NetworkRelation _Relationship;
	NetworkState _State;
	std::pair<NetworkCommunication, int> _LastResponse;

	std::string _MyName, _MemberName;

	sf::Clock _PackageTravelTimer, _SyncTimer;
};
