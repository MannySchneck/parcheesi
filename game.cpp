#include "game.h"
#include "moves.h"
#include "interfaces.h"
#include <cstdlib>
#include <utility>
#include <exception>

void Game::register_player(std::shared_ptr<IPlayer> p){
        players.push_back(p);
}

std::pair<int,int> Game::roll_dice(){
        return std::pair<int, int>((rand() % 6) + 1, (rand() % 6) + 1);
}

void Game::start(){
        bool is_winner;
        while(!is_winner){
                for(auto p : players){
                        bool turn_done(false);
                        Status status = Status::normal;

                        while(!turn_done){

                                std::vector<std::shared_ptr<IMove>> moves;

                                switch(status){
                                case (Status::normal):
                                        moves = p->doMove(board, roll_dice());
                                        break;
                                case (Status::bop_bonus):
                                        moves = p->doMove(board, std::pair<int,int>(20,0));
                                        break;
                                case (Status::home_bonus):
                                        moves = p->doMove(board, std::pair<int,int>(10,0));
                                        break;
                                }

                                auto moves_result = process_moves(moves);

                                switch(moves_result.first){
                                case (Status::bop_bonus):
                                        status = Status::bop_bonus;
                                        break;
                                case (Status::home_bonus):
                                        status = Status::home_bonus;
                                        break;
                                case (Status::cheated):
                                        status = Status::normal;
                                        break;
                                case (Status::normal):
                                        board = moves_result.second;
                                        turn_done = true;
                                        break;
                                default:
                                        throw new std::logic_error("can't happen");
                                }

                                //check_final_board(old_board, new_board);
                        }
                }
        }
}


std::pair<Status, Board> Game::process_moves(std::vector<std::shared_ptr<IMove>> moves){
        Board new_board = board;
        Status status;
        for(auto m : moves){
                status = m->update_board(new_board);
        }
        return std::pair<Status,Board> (status, new_board);
}
