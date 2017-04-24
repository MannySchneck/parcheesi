#include "game.h"
#include "moves.h"
#include "interfaces.h"
#include <cstdlib>
#include <utility>
#include <exception>
#include "catch.hpp"
#include "turn.h"
#include "splayer.h"

void Game::register_player(S_Player p){
        players.push_back(p);
}

fuel Game::roll_dice(){
        return fuel{(rand() % 6) + 1, (rand() % 6) + 1};
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
                                fuel fuel = roll_dice();
                                Turn turn(board, p.color, fuel);
                                if((outcome = p.do_turn(turn)) == Turn_Outcome::victory)
                                        goto out;
                                doubles_count += Turn_Outcome::doubles == outcome ? 1 : 0;
                        } while(outcome == Turn_Outcome::doubles);
                }
        out:
                ;
        }
}
