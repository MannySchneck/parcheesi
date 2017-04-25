#pragma once

#include "turn.h"
#include "moves.h"
#include "interfaces.h"
#include <optional>

enum class Turn_Outcome{
        normal,
        victory,
        doubles
};

class S_Player{
public:
        S_Player(Color color, Board board);

        Turn_Outcome do_turn(Turn turn);

        bool cheated;

        Color color;
private:
        #ifdef TEST
 public:
        #endif
        std::shared_ptr<IPlayer> player;
        std::optional<Turn> turn;
};
