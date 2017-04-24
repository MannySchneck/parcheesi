#pragma once

#include "interfaces.h"
#include "board.h"
#include "splayer.h"

class Game : IGame {
 public:
        void register_player(S_Player p) override;

        void start() override;

 private:
#ifdef TEST
 public:
 Game(Board board) :
        board(board)
        {}
#endif
        int doubles_counter;

        Board board;

        fuel roll_dice();

        std::vector<S_Player> players;

        std::pair<Status, Board> process_moves(std::vector<std::shared_ptr<IMove>> moves);

};
