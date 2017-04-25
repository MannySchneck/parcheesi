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
        S_Player(Color color);

        Turn_Outcome do_turn(Board board);

        fuel roll_dice(Color, std::function<int()>, Board board);

        bool cheated;

        Color color;
private:
        #ifdef TEST
 public:
        #endif
        std::shared_ptr<IPlayer> player;
        std::optional<Turn> turn;
};
