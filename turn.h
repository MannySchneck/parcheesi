#pragma once

#include "interfaces.h"
#include "board.h"
#include <memory>

class Turn{
 public:
        Turn(Board old_board, Color color, fuel fuel);

        Turn(Board old_board, Board new_board, Color color, fuel fuel);

        Status update_cur_board(std::shared_ptr<IMove> mv);

        bool validate();

        Board get_old_board();
        Board get_new_board();

        void add_gas(int gallons);
        void guzzle_gas(int gallons);
 private:
        #ifdef TEST
 public:
        #endif
        Board old_board;
        Board cur_board;
        Color color;
        fuel fuel;
};
