#include "game.h"
#include "moves.h"
#include "interfaces.h"
#include <utility>
#include <exception>
#include "catch.hpp"
#include "turn.h"
#include "splayer.h"
#include <functional>

void Game::register_player(S_Player p){
        players.push_back(p);
}


void Game::start(){
        Turn_Outcome outcome(Turn_Outcome::normal);
        while(outcome != Turn_Outcome::victory){
                int doubles_count = 0;
                for(auto p : players){
                        if(p.cheated){
                                continue;
                        }
                        do{
                                if((outcome = p.do_turn(board)) == Turn_Outcome::victory)
                                        goto out;
                                doubles_count += Turn_Outcome::doubles == outcome ? 1 : 0;
                        } while(outcome == Turn_Outcome::doubles);
                }
        out:
                ;
        }
}


