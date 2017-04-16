#include "interfaces.h"
#include "moves.h"

Status MoveMain::validate(Rules_Checker &rc, Board board){
        return rc.validate_main_move(this, board);
}

Status EnterPiece::validate(Rules_Checker &rc, Board board){
        return Status::cheated;
}

Status MoveHome::validate(Rules_Checker &rc, Board board){
        return Status::cheated;
}


IMove::IMove() = default;

EnterPiece::EnterPiece(Pawn pawn) :
        pawn(pawn)
{}

MoveMain::MoveMain(int start, int distance, Pawn pawn)
        : pawn(pawn)
{
        this->pawn=pawn;
        this->start=start;
        this->distance=distance;
}

MoveHome::MoveHome(Pawn pawn, int start, int distance) :
        pawn(pawn)
{
        this->start=start;
        this->distance=distance;
}



