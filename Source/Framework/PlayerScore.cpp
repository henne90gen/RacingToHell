//
// Created by henne on 24.01.17.
//
#include "stdafx.h"
#include "Framework/PlayerScore.h"

PlayerScore::PlayerScore(std::string name, int level, int score) : _Name(name), _Level(level), _Score(score) {}

bool PlayerScore::operator<(PlayerScore &Player2) const {
    return _Score < Player2.getScore();
}

void PlayerScore::operator<<(std::istream &stream) {
    read(stream, _Rank);
    std::cout << "Rank: " << _Rank << std::endl;
    read(stream, _Name);
    std::cout << "Name: " << _Name << std::endl;
    read(stream, _Level);
    std::cout << "Level: " << _Level << std::endl;
    read(stream, _Score);
    std::cout << "Score: " << _Score << std::endl;
}

void PlayerScore::operator>>(std::ostream &stream) {
    write(stream, _Rank);
    std::cout << _Rank << std::endl;
    write(stream, _Name);
    std::cout << _Name << std::endl;
    write(stream, _Level);
    std::cout << _Level << std::endl;
    write(stream, _Score);
    std::cout << _Score << std::endl;
}
