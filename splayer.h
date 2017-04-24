#pragma once

#include "turn.h"
#include "moves.h"
#include "interfaces.h"

enum class Turn_Outcome{
        normal,
        victory,
        doubles
};

class S_Player {
public:
        S_Player(Color color, Board board);

        Turn_Outcome do_turn(Turn turn);

        bool cheated;

        Color color;
private:
        std::shared_ptr<IPlayer> player;
        Turn turn;
};
