#include "mplayer.h"
#include <optional>
#include <algorithm>
#include <string>
#include <set>

std::string M_Player::startGame(Color color){
        this->color = color;
        return "a machine player =)";
}

std::vector<mv_ptr> M_Player::doMove(Board board, fuel fuel){
        std::vector<mv_ptr> moves;
        std::optional<mv_ptr> mv;

        turn = Turn(board, color, fuel);
        while((mv = construct_move(board, fuel))){
                turn->update_cur_board(mv.value());
                moves.push_back(mv.value());
        }

        return moves;
}

void M_Player::DoublesPenalty(){
        return;
}
