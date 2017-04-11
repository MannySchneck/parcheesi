#pragma once

#include "interfaces.h"
#include "board.h"


// represents a move where a player enters a piece
class EnterPiece : IMove{
        Pawn pawn;
        EnterPiece(Pawn pawn) :
                pawn(pawn)
        {}

        Status update_board(Board &b) override;
};

// represents a move that starts on the main ring
// (but does not have to end up there)
class MoveMain : IMove{
        Pawn pawn;
        int start;
        int distance;
        MoveMain(Pawn pawn, int start, int distance)
                : pawn(pawn)
        {
                this->pawn=pawn;
                this->start=start;
                this->distance=distance;
        }

        Status update_board(Board &b) override;
};

// represents a move that starts on one of the home rows
class MoveHome : IMove{
        Pawn pawn;
        int start;
        int distance;
        MoveHome(Pawn pawn, int start, int distance) :
                pawn(pawn)
        {
                this->start=start;
                this->distance=distance;
        }

        Status update_board(Board &b) override;
};
