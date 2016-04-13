#pragma once

enum class NetworkRelation {None, Host, Client};

enum class NetworkCommunication {	None, Authentication, WrongPassword, ConnectionFailed, ConnectionSuccesfull, Disconnect, Kick, StartGame, 
									EndGame, Ready, SettingsChanged, CreateGameObject, UpdateP2, SynchroniseTick };

enum class NetworkState {None, Lobby, Ingame};