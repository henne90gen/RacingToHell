#pragma once

#include <fstream>
#include <sstream>
#include <algorithm>

#include <Serializable.h>

class PlayerScore : public Serializable {
public:

    PlayerScore(std::string name = "", int level = 0, int score = 0);

    bool operator<(PlayerScore &Player2) const;

    void operator>>(std::ostream &stream);

    void operator<<(std::istream &stream);

    int getScore() { return _Score; }

    int getRank() { return _Rank; }

    void setRank(int rank) { _Rank = rank; }

    std::string getName() { return _Name; }

    int getLevel() { return _Level; }

private:
    int _Rank;
    std::string _Name;
    int _Score;
    int _Level;
};


