#include "splayer.h"
#include "catch.hpp"
#include <cstdlib>

Turn_Outcome S_Player::do_turn(Board board){

        fuel fuel = roll_dice(color, rand, board);
        Turn turn(board, color, fuel);

        return Turn_Outcome::normal;
}

S_Player::S_Player(Color color) :
        color(color) {}

fuel S_Player::roll_dice(Color color, std::function<int()> die, Board board){
        fuel fuel{(die() % 6) + 1, (die() % 6) + 1};
        if(board.get_nest_count(color) == 0 && fuel[0] == fuel[1]) {
                fuel.push_back(7 - fuel[0]);
                fuel.push_back(7 - fuel[0]);
        }
        return fuel;
}

TEST_CASE("Do a thing with dice"){
        Board board;
        board.nest_count[Color::red] = 0;

        SECTION("blerp"){
                S_Player p(Color::red);
                std::vector<int> mlerp{3,3,4,4};
                //Add 1 to any input, to maintain compatibility with rand
                //E.g. 2 becomes 3
                REQUIRE(p.roll_dice(Color::red, []()->int{return 2;}, board) == mlerp);
                fuel blerp{3,4};
                REQUIRE(p.roll_dice(Color::red, []()->
                                         int{
                                                 static int count = 2;
                                                 return count++;
                                    }, board) ==
                        blerp);
        }
}

