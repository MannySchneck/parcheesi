#pragma once
#include "mplayer.h"

/* To instantiate a player that tries to move its earliest pawn, give,
   Direction::increasing as the sort direction for the pawn queue.
   Direction::decreasing will give the opposite behavior.
 */


class Dumb_Player : M_Player{
 public:
        Dumb_Player(Color color, Direction direction);
 private:
#ifdef TEST
 public:
#endif
        Color color;
        Direction direction;

        std::optional<mv_ptr> construct_move(Board board, fuel fuel,  std::vector<mv_ptr> bad_moves) override;

};

