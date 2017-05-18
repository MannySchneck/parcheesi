#include "mplayer.h"
#include <optional>
#include <algorithm>
#include <string>
#include <set>
#include <iostream>

std::string M_Player::startGame(Color color){
        this->color = color;
        return "a machine player =)";
}

std::vector<mv_ptr> M_Player::doMove(Board board, fuel fuel){
        std::vector<mv_ptr> moves;
        std::vector<mv_ptr> bad_moves;
        std::optional<mv_ptr> mv;



        turn = Turn(board, color, fuel);
        while((mv = construct_move(turn->get_new_board(), fuel, bad_moves))){

                std:: cout << "This fucker gets called right??\n\n";

                if(Status::cheated  == turn->update_cur_board(mv.value())){
                        bad_moves.push_back(mv.value());
                        std::cout << "add ing to the bad moves\n";
                } else{
                        bad_moves = {};
                        std::cout << "add ing to the good moves\n";
                        moves.push_back(mv.value());
                }
        }
        return moves;
}

void M_Player::DoublesPenalty(){
        return;
}

