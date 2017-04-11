#include "interfaces.h"
#include "moves.h"

Status EnterPiece::update_board(Board &b){
        return b.enter_pawn(pawn);
}

Status MoveMain::update_board(Board &b){
        return b.move_pawn(start, distance, pawn);
}
