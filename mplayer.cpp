#include "mplayer.h"
#include <optional>

void M_Player::startGame(Color color){
        this->color = color;
}

std::vector<mv_ptr> M_Player::doMove(Board board, fuel fuel){
        bool out_of_pawns;
        while(!out_of_pawns){
                auto p = select_pawn(board);
                if(p.has_value()){
                        auto mv = construct_move(p.value(), board, fuel);
                        if(mv.has_value()){
                                return {mv.value()};
                        }
                }

        }

        return std::vector<mv_ptr>{};
}

void M_Player::DoublesPenalty(){
        return;
}
