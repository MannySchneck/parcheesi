#include "splayer.h"
#include "catch.hpp"
#include "turn.h"
#include <cstdlib>

// Need to call do move somewhere.
std::pair<Turn_Outcome, Board> S_Player::do_turn(Board board, int doubles_count){

        fuel fuel = roll_dice(color, rand, board);

        if(fuel[0] == fuel[1] && doubles_count == 2){
                board.reset_farthest_pawn(color);
                return std::pair<Turn_Outcome, Board>{Turn_Outcome::normal, board};
        }
        Turn turn(board, color, fuel);

        auto moves = player->doMove(board, fuel); // actually does turn

        for(auto move : moves){
                if(turn.update_cur_board(move) == Status::cheated){
                        // TODO: kick out of game
                        return std::pair<Turn_Outcome, Board>
                                {Turn_Outcome::cheated, turn.get_old_board()};
                }
        }

        return turn.validate() ?
                std::pair<Turn_Outcome, Board>{Turn_Outcome::normal, turn.get_new_board()} :
        std::pair<Turn_Outcome, Board>{Turn_Outcome::cheated, turn.get_old_board()};
}


S_Player::S_Player(Color color) :
        color(color){}

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
        board.nest[Color::red] = std::vector<Pawn>{};

        SECTION("blerp"){
                S_Player p(Color::red);
                std::vector<int> mlerp{3,3,4,4};
                //Add 1 to any input, to maintain compatibility with rand
                //E.g. 2 becomes 3
                REQUIRE(p.roll_dice(Color::red, []()->int{
                                        return 2;
                                },
                                board) ==
                        mlerp);
                fuel blerp{3,4};
                REQUIRE(p.roll_dice(Color::red, []()->
                                    int{
                                            static int count = 2;
                                            return count++;
                                    }, board) ==
                        blerp);
        }
}
