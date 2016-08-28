#pragma once


enum class NetworkRelation { NoRel, Host, Client};

enum class NetworkCommunication { NoNetComm, Authentication, WrongPassword, ConnectionFailed, ConnectionSuccesfull, Disconnect, Kick, StartGame,
									EndGame, Ready, SettingsChanged, CreateGameObject, DeleteGameObject,PlayerKeyPress, PlayerInformation, SynchroniseTick, ShotFired, SpawnPlayerBullet };

enum class NetworkState { NoNetState, Lobby, Ingame};
