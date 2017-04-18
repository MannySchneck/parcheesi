#include "interfaces.h"
#include "moves.h"

Status MoveMain::inspect(Rules_Checker &rc, Board board){
  if(!rc.validate_main_move(this, board)){
      return Status::cheated;
    }
    return Status::normal;
}

Status EnterPiece::inspect(Rules_Checker &rc, Board board){

  if(!rc.validate_enter_move(this, board)){
    return Status::cheated;
  }
  return Status::normal;
}

Pawn EnterPiece::get_pawn(){
  return pawn;
}

Status MoveHome::inspect(Rules_Checker &rc, Board board){
  if(!rc.validate_home_move(this, board)){
    return Status::cheated;
  }
  return Status::normal;
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

MoveHome::MoveHome(int start, int distance, Pawn pawn)
        : Move(start,distance,pawn){}
