#pragma once


enum class NetworkRelation { NoRel, Host, Client};

enum class NetworkCommunication { NoNetComm, Authentication, WrongPassword, ConnectionFailed, ConnectionSuccesfull, Disconnect, Kick, StartGame,
									EndGame, Ready, SettingsChanged, CreateGameObject, DeleteGameObject, UpdateAICar, PlayerKeyPress, PlayerInformation, SynchroniseTick };

enum class NetworkState { NoNetState, Lobby, Ingame};
