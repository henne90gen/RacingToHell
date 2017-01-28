//
// Created by henne on 24.01.17.
//
#include "stdafx.h"
#include "Framework/PlayerScore.h"

//PlayerScore::PlayerScore() : PlayerScore("", 0 , 0) {
//
//}

PlayerScore::PlayerScore(std::string name, int level, int score) : _Name(name), _Level(level),
                                                                   _Score(score) {

}

void PlayerScore::operator<<(std::istream &stream) {
    read(stream, _Rank);
    read(stream, _Name);
    read(stream, _Level);
    read(stream, _Score);
}

void PlayerScore::operator>>(std::ostream &stream) {
    write(stream, _Rank);
    write(stream, _Name);
    write(stream, _Level);
    write(stream, _Score);
}
