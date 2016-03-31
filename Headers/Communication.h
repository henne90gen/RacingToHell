#pragma once

enum class NetworkRelation {None, Host, Client};

enum class NetworkCommunication { None, Authentication, WrongPassword, ConnectionFailed, ConnectionSuccesfull, Disconnect};

enum class NetworkState {None, Lobby, Ingame};