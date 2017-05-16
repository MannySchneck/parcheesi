#include "dumb_player.h"
#include <optional>
#include "catch.hpp"
#include <algorithm>
#include <iostream>
#include "prettyprint.hh"
#include <vector>

Dumb_Player::Dumb_Player(Color color, Direction direction) :
        color(color),
        direction(direction) {}


fuel find_entry_rolls(fuel fuel){
        for(auto gallon : fuel){
                if(gallon == Game_Consts::entry_roll) return {Game_Consts::entry_roll};
        }

        for(auto gallon : fuel){
                if(auto gallon2 = std::find(fuel.begin(), fuel.end(), Game_Consts::entry_roll - gallon) != fuel.end())
                        return {gallon, gallon2};
        }
        return {};
}

std::optional<mv_ptr> Dumb_Player::construct_move(Board board, fuel fuel, std::vector<mv_ptr> bad_moves){

        auto posns = board.get_sorted_pawns(color, direction);

        if(direction == Direction::decreasing){
                if(find_entry_rolls(fuel).size())
                        return mv_ptr{new EnterPiece(Pawn(board.get_nest_count(color) - 1, color))};
        }

        if(posns.size() == 0){
                return std::optional<mv_ptr>{std::nullopt};
        }

        for(auto posn : posns){
                auto the_is_home = std::get<is_home>(posn);
                auto the_loc = std::get<loc>(posn);
                auto the_pawn = std::get<pawn>(posn);

                for (auto gallon : fuel){
                        auto mv = the_is_home ? mv_ptr{new MoveHome(the_loc, gallon, the_pawn)} :
                        mv_ptr{new MoveMain(the_loc, gallon, the_pawn)};

                        if (std::find(std::begin(bad_moves), std::end(bad_moves), mv) !=
                            std::end(bad_moves)){
                                continue;
                        }

                        Rules_Checker rc{fuel};
                        if(mv->inspect(rc, board) != Status::cheated){
                                return std::optional<mv_ptr>{mv};
                        }
                }
        }
        return std::optional<mv_ptr>{std::nullopt};

}

TEST_CASE("Make a thing to do a thing") {
        Dumb_Player pl0(Color::red, Direction::decreasing);
        Pawn p0(0, Color::red);
        Pawn p1(1, Color::red);
        Pawn bp0(0, Color::blue);
        Pawn bp1(1, Color::blue);
        Board board;
        int red_start = board.get_color_start_space(Color::red);

        SECTION("Construct a move, 1 from start") {
                board.put_pawn(p0, red_start);

                fuel fuel{1};
                mv_ptr mv{new MoveMain(red_start, 1, p0)};


                REQUIRE(*(std::dynamic_pointer_cast<MoveMain>(pl0.construct_move(board, fuel, {}).value()))  ==
                        *(std::dynamic_pointer_cast<MoveMain>(mv)));
        }

        SECTION("can't pass blockade, but can move up to it"){
                board.put_pawn(p0, red_start + 1);
                board.put_pawn(bp0, red_start + 2);
                board.put_pawn(bp1, red_start + 2);

                fuel fuel{2};


                REQUIRE(!pl0.construct_move(board, fuel, {}).has_value());

                board.put_pawn(p1, red_start - 1);


                mv_ptr mv{new MoveMain(red_start - 1, 2, p1)};

                REQUIRE(*(std::dynamic_pointer_cast<MoveMain>(pl0.construct_move(board, fuel, {}).value()))  ==
                        *(std::dynamic_pointer_cast<MoveMain>(mv)));
        }

        SECTION("can't move beyond home"){
                board.put_pawn(p0, board.final_ring[Color::red]);

                board.move_pawn(board.final_ring[Color::red], 5, p0);

                fuel fuelzo{6};

                REQUIRE(!pl0.construct_move(board, fuelzo, {}).has_value());


                fuel fuelz{3};

                mv_ptr mv{new MoveHome{5, 3, p0}};

                REQUIRE(*(std::dynamic_pointer_cast<MoveHome>(pl0.construct_move(board, fuelz, {}).value()))  ==
                        *(std::dynamic_pointer_cast<MoveHome>(mv)));
        }

        SECTION("two legal movez"){

                board.put_pawn(p0, red_start + 1);
                board.put_pawn(p1, red_start - 1);

                fuel fuel{2, 6};

                mv_ptr mv{new MoveMain{red_start - 1, 2, p1}};

                REQUIRE(*(std::dynamic_pointer_cast<MoveMain>(pl0.construct_move(board, fuel, {}).value()))  ==
                        *(std::dynamic_pointer_cast<MoveMain>(mv)));
        }

        SECTION("flippy floppy"){
                Dumb_Player pl1(Color::red, Direction::increasing);

                board.put_pawn(p0, red_start + 1);
                board.put_pawn(p1, red_start - 1);

                fuel fuel{2, 6};

                mv_ptr mv{new MoveMain{red_start + 1, 2, p0}};

                REQUIRE(*(std::dynamic_pointer_cast<MoveMain>(pl1.construct_move(board, fuel, {}).value()))  ==
                        mv);
        }

        SECTION("making sure that bad move set prevents moves being called"){
                Dumb_Player pl1(Color::red, Direction::increasing);

                board.put_pawn(p0, red_start + 1);
                board.put_pawn(p1, red_start - 1);

                board.put_pawn(bp0, red_start + 3);
                board.put_pawn(bp1, red_start + 3);

                fuel fuel{2, 6};

                mv_ptr mv{new MoveMain{red_start + 1, 2, p0}};


                REQUIRE(*(std::dynamic_pointer_cast<MoveMain>(pl1.construct_move(board, fuel, {mv}).value())) !=
                        mv);
        }

        SECTION("can't move a blockade"){
                Dumb_Player pl1(Color::red, Direction::increasing);
                //TODO: WOOT
        }

}
