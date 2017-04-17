#include "interfaces.h"
#include "moves.h"

Status MoveMain::validate(Rules_Checker &rc, Board board){
        return rc.validate_main_move(this, board);
}

Status EnterPiece::validate(Rules_Checker &rc, Board board){
        return Status::cheated;
}

Status MoveHome::validate(Rules_Checker &rc, Board board){
        return rc.validate_home_move(this, board);
}


IMove::IMove() = default;

EnterPiece::EnterPiece(Pawn pawn) :
        pawn(pawn)
{}

Move::Move(int start, int distance, Pawn pawn)
        : pawn(pawn),
          start(start),
          distance(distance){}

Pawn Move::get_pawn(){
        return pawn;
}

int Move::get_start(){
        return start;
}

int Move::get_distance(){
        return distance;
}


MoveMain::MoveMain(int start, int distance, Pawn pawn)
        : Move(start,distance,pawn){}

MoveHome::MoveHome(Pawn pawn, int start, int distance)
        : Move(start,distance,pawn){}
