#pragma once

#include "interfaces.h"
#include "board.h"
#include "moves.h"

class MoveMain;
class MoveHome;
class Move;

using mv_ptr = std::shared_ptr<IMove>;

class Rules_Checker {
 public:
        Rules_Checker(fuel fuel);

        Status validate_main_move(MoveMain* move, const Board &old_board);

        Status validate_home_move(MoveHome* move, const Board &old_board);

 private:
#if TEST
 public:
#endif
        bool encounters_blockade(Move* mv, const Board &old_board);
        bool tried_safety_bop(MoveMain* mv, const Board &old_board);
        bool pawn_doesnt_exist(Move* mv, const Board &old_board);
        bool moved_past_home(MoveHome* mv, const Board &old_board);
        fuel fuel;
};

