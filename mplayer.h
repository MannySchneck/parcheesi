#pragma once

#include "interfaces.h"
#include "moves.h"
#include <optional>
#include "turn.h"
#include <string>

class M_Player : IPlayer{
 public:

        std::string startGame(Color color) override;

        std::vector<mv_ptr> doMove(Board board, fuel) override;

        void DoublesPenalty() override;


 protected:
        Color color;

        std::optional<Turn> turn;

        virtual std::optional<mv_ptr> construct_move(Board board, fuel fuel,  std::vector<mv_ptr> bad_moves) = 0;
};
