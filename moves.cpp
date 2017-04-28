#include "interfaces.h"
#include "moves.h"

Status Move::do_move(Board &board){
        return board.move_pawn(start, distance, pawn);
}

Status MoveMain::inspect(Rules_Checker &rc, Board board){
        if(!rc.validate_main_move(this, board)){
                return Status::cheated;
        }

        if(rc.did_bop(this, board)){
                return Status::bop_bonus;
        }

        if(rc.moved_home(this, board)){
                return Status::home_bonus;
        }

        return Status::normal;
}

bool Move::operator==(const Move& rhs){
        return start == rhs.start && distance == rhs.distance && pawn == rhs.pawn;
}

bool MoveMain::operator==(const MoveMain& rhs){
        return Move::operator==(rhs);
}

bool MoveHome::operator==(const MoveHome& rhs){
        return Move::operator==(rhs);
}

Status EnterPiece::do_move(Board &board){
        return board.enter_pawn(pawn);
}

bool EnterPiece::operator==(const EnterPiece& rhs){
        return pawn == rhs.pawn;
}

Status EnterPiece::inspect(Rules_Checker &rc, Board board){

        if(!rc.validate_enter_move(this, board)){
                return Status::cheated;
        }

        if(rc.did_bop(this, board)){
                return Status::bop_bonus;
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

        if(rc.moved_home(this, board)) return Status::home_bonus;

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
