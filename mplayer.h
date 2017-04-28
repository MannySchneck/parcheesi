#pragma once

#include "interfaces.h"
#include "moves.h"
#include <optional>


class M_Player : IPlayer{
 public:

        void startGame(Color color) override;

        std::vector<mv_ptr> doMove(Board board, fuel) override;

        void DoublesPenalty() override;


 protected:
        Color color;

        virtual std::optional<Posn> select_pawn(Board board) = 0;
        virtual std::optional<mv_ptr> construct_move(std::optional<Posn>, Board board, fuel fuel) = 0;
};
