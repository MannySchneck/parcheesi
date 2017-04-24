#pragma once

#include "interfaces.h"
#include "board.h"
#include "moves.h"

class MoveMain;
class MoveHome;
class EnterPiece;
class Move;

using mv_ptr = std::shared_ptr<IMove>;

class Rules_Checker {
 public:
        Rules_Checker(fuel fuel);

        bool did_bop(IMove*, Board &old_board);

        bool moved_home(Move*, Board&);

        bool validate_main_move(MoveMain* move, Board &old_board);

        bool validate_home_move(MoveHome* move, Board &old_board);

        bool validate_enter_move(EnterPiece* mv, Board &old_board);

        bool validate_turn(Board &old_board, Board &new_board, Color color);


        bool is_boppable(std::vector<Pawn> pawns, Color c);

 private:
#if TEST
 public:
#endif

        bool start_blockaded(EnterPiece* mv, Board &old_board);
        bool encounters_blockade(Move* mv, Board &old_board);
        bool tried_safety_bop(MoveMain* mv, Board &old_board);
        bool pawn_doesnt_exist(Move* mv, Board &old_board);
        bool moved_past_home(MoveHome* mv, Board &old_board);
        bool illegal_distance(IMove* mv, Board &old_board);

        bool moved_blockade_together(Board &old_board, Board &new_board);
        bool has_more_moves(Board &new_board, Color color);


        bool a_move_exists(Pawn p, int loc, bool home, Board &new_board);

        fuel fuel;
};
