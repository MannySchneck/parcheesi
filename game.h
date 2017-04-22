#pragma once

#include "interfaces.h"
#include "board.h"

class Game : IGame {
 public:
        void register_player(std::shared_ptr<IPlayer> p) override;

        void start() override;

 private:
#ifdef TEST
 public:
 Game(Board board) :
        board(board)
        {}
#endif
        int doubles_counter;

        std::vector<std::shared_ptr<IPlayer>> players;
        Board board;

        std::pair<int,int> roll_dice();


        std::pair<Status, Board> process_moves(std::vector<std::shared_ptr<IMove>> moves);

};