#pragma once

#include "turn.h"
#include "moves.h"
#include "interfaces.h"
#include <optional>
#include <utility>

enum class Turn_Outcome{
        normal,
                victory,
                doubles,
                cheated
                };

class S_Player{
 public:
        S_Player(Color color);

        std::pair<Turn_Outcome, Board> do_turn(Board board, int doubles_count);

        Board get_final_board();

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
