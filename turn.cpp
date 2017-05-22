#include "rule_checker.h"
#include "interfaces.h"
#include "turn.h"
#include "catch.hpp"
#include <iostream>
#include <exception>
#include "game_params.h"


Turn::Turn(Board old_board,  Color color, ::fuel fuel):
        old_board(old_board),
        cur_board(old_board),
        color(color),
        fuel(fuel){}

Turn::Turn(Board old_board, Board new_board,  Color color, ::fuel fuel):
        old_board(old_board),
        cur_board(new_board),
        color(color),
        fuel(fuel){}

Board Turn::get_old_board(){
        return old_board;
}
Board Turn::get_new_board(){
        return cur_board;
}


Status Turn::update_cur_board(mv_ptr mv){
        Rules_Checker rc{fuel};
        auto mv_result = mv->inspect(rc, old_board);

        switch(mv_result){
        case(Status::normal):
                guzzle_gas({mv->get_cost()});
                return mv->do_move(cur_board);
                break;
        case(Status::bop_bonus):
                guzzle_gas({mv->get_cost()});
                fuel.push_back(game_params::bop_bonus);
                return mv->do_move(cur_board);
                break;
        case(Status::home_bonus):
                guzzle_gas({mv->get_cost()});
                fuel.push_back(game_params::home_bonus);
                return mv->do_move(cur_board);
                break;
        case(Status::cheated):
                return Status::cheated;
                break;
        default:
                std::logic_error("Added a move result and forgot to extend this");
        }
}

bool Turn::validate(){
        return Rules_Checker{fuel}.validate_turn(old_board, cur_board, color);
}

void Turn::add_gas(int gallons){
        fuel.push_back(gallons);
}

// \pre: gallons exists in fuel
void Turn::guzzle_gas(std::vector<int> gallons){
        for(auto gallon : gallons){
                auto it = std::find(fuel.begin(), fuel.end(), gallon);
                if(it == fuel.end()){
                        std::cerr << "Tried to use non-existent fuel" << std::endl;
                        throw std::logic_error("ya done goofed.");
                }
                fuel.erase(it);
        }
}

TEST_CASE("Turn tests"){
        Board old_board;
        Board good_board;
        Board bad_board;

        SECTION("Validate is false when blockade moved together"){
                Pawn rp0(0, Color::red);
                Pawn rp1(1, Color::red);

                old_board.put_pawn(rp0, 1);
                old_board.put_pawn(rp1, 1);

                bad_board.put_pawn(rp0, 5);
                bad_board.put_pawn(rp1, 5);
                Turn turn1(old_board, bad_board, Color::red, {});

                REQUIRE(turn1.validate() == false);

                good_board.put_pawn(rp0, 5);
                good_board.put_pawn(rp1, 7);
                Turn turn2(old_board, good_board, Color::red, {});

                REQUIRE(turn2.validate() == true);
        }

        SECTION("out of gas"){
                Pawn rp0(0, Color::red);

                old_board.put_pawn(rp0, 1);

                Turn turn1(old_board, bad_board, Color::red, {});

                REQUIRE(turn1.validate() == true);
        }

        SECTION("still need to go"){
                Pawn rp0(0, Color::red);

                old_board.put_pawn(rp0, 1);
                bad_board.put_pawn(rp0, 1);

                Turn turn1(old_board, bad_board, Color::red, {5});

                REQUIRE(turn1.validate() == false);
        }

        SECTION("Would move blockade"){
                Pawn p0(0, Color::blue);
                Pawn p2(1, Color::blue);

                Pawn rp0(0, Color::red);
                Pawn rp1(1, Color::red);

                old_board.put_pawn(p0, 5);
                old_board.put_pawn(p2, 5);

                bad_board.put_pawn(p0, 7);
                bad_board.put_pawn(rp0, 8);
                bad_board.put_pawn(rp1, 8);

                Turn turn(old_board, bad_board, Color::blue, {2});

                REQUIRE(turn.validate() == true);
        }

        SECTION("removing fuel"){
                Turn turn(old_board, bad_board, Color::red, {5});


                turn.guzzle_gas({Game_Consts::entry_roll});

                REQUIRE(turn.fuel == fuel{});

        }

        SECTION("Remove two gallons from fuel"){
                Turn turn(old_board, bad_board, Color::red, {3, 2});

                turn.guzzle_gas({3, 2});

                REQUIRE(turn.fuel == fuel{});
        }


        SECTION("Entering with {3,2} correctly consumes fuel"){
                //TODO: WRITE this
        }

        SECTION("updating the board updates the board"){
                Board board;
                Pawn bp0(0, Color::blue);

                int blue_start = board.get_color_start_space(Color::blue);

                board.put_pawn(bp0, blue_start);

                Turn t{board, Color::blue, {1}};

                mv_ptr mv{new MoveMain(blue_start, 1, bp0)};

                Board new_board;

                new_board.put_pawn(bp0, blue_start + 1);

                REQUIRE(t.update_cur_board(mv) == Status::normal);

                REQUIRE(new_board == t.get_new_board());
        }
}
