#include "game.h"
#include "moves.h"
#include "interfaces.h"
#include <cstdlib>
#include <utility>
#include <exception>
#include "catch.hpp"
#include "turn.h"
#include "splayer.h"
#include <functional>

void Game::register_player(S_Player p){
        players.push_back(p);
}

fuel Game::roll_dice(Color color, std::function<int()> die){
        fuel fuel{(die() % 6) + 1, (die() % 6) + 1};
        if(board.get_nest_count(color) == 0 && fuel[0] == fuel[1]) {
                fuel.push_back(7 - fuel[0]);
                fuel.push_back(7 - fuel[0]);
        }
        return fuel;
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
                                fuel fuel = roll_dice(p.color, rand);
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


TEST_CASE("Do a thing with dice"){
        Board board;
        board.nest_count[Color::red] = 0;
        Game gmamae(board);

        SECTION("blerp"){
                std::vector<int> mlerp{3,3,4,4};
                //Add 1 to any input, to maintain compatibility with rand
                //E.g. 2 becomes 3
                REQUIRE(gmamae.roll_dice(Color::red, []()->int{return 2;}) == mlerp);
                fuel blerp{3,4};
                REQUIRE(gmamae.roll_dice(Color::red, []()->
                                         int{
                                                 static int count = 2;
                                                 return count++;
                                         }) ==
                        blerp);
        }
}
