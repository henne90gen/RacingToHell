
#include "stdafx.h"
#include "Framework/PlayerScore.h"

PlayerScore::PlayerScore(std::string name, int level, int score) : _Name(name), _Level(level), _Score(score) {}

bool PlayerScore::operator<(PlayerScore &Player2) const {
    return _Score < Player2.getScore();
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
