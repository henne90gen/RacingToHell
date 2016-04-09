#pragma once

enum class NetworkRelation {None, Host, Client};

enum class NetworkCommunication {	None, Authentication, WrongPassword, ConnectionFailed, ConnectionSuccesfull, Disconnect, Kick, StartGame, 
									EndGame, Ready, SettingsChanged, CreateGameObject, UpdateGameObject, SynchroniseTick };

enum class NetworkState {None, Lobby, Ingame};