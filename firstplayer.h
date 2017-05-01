#pragma once
#include "mplayer.h"

class First_Player : M_Player{

 public:
        First_Player(Color color);
 private:
        #ifdef TEST
 public:
        #endif
        Color color;
        //std::optional<Posn> select_pawn(Board board) override;

        std::optional<mv_ptr> construct_move(Board board, fuel fuel) override;

};
