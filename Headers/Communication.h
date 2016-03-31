#pragma once

enum class NetworkRelation {None, Host, Client};

enum class NetworkCommunication { None, Authentication, WrongPassword, ConnectionFailed, ConnectionSuccesfull};

enum class NetworkState {None, Lobby, Ingame};