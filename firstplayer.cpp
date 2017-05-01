#include "firstplayer.h"
#include <optional>
#include "catch.hpp"
#include <algorithm>
#include <iostream>
#include "prettyprint.hh"

First_Player::First_Player(Color color) : color(color)
{}

std::ostream& operator<<(std::ostream& out, const Pawn &p){
        out << "(Pawn " << p.id << " )";
        return out;
}

std::optional<mv_ptr> First_Player::construct_move(Board board, fuel fuel){

        auto posns = board.get_sorted_pawns(color, Direction::increasing);

        if(std::find(fuel.begin(), fuel.end(), 5) != fuel.end()
           && board.get_nest_count(color)){
                return mv_ptr{new EnterPiece(Pawn(board.get_nest_count(color) - 1, color))};
        }

        if(posns.size() == 0){
                return std::optional<mv_ptr>{std::nullopt};
        }


        for(auto posn : posns){
                auto the_is_home = std::get<is_home>(posn);
                auto the_loc = std::get<loc>(posn);
                auto the_pawn = std::get<pawn>(posn);


                if(the_is_home){
                        for(auto gallon : fuel){
                                auto mv = mv_ptr{new MoveHome(the_loc, gallon, the_pawn)};
                                Rules_Checker rc{fuel};
                                if(mv->inspect(rc, board) == Status::cheated){
                                        return std::optional<mv_ptr>{mv};
                                }
                        }
                } else{
                        for(auto gallon : fuel){
                                auto mv = mv_ptr{new MoveMain(the_loc, gallon, the_pawn)};
                                Rules_Checker rc{fuel};
                                if(mv->inspect(rc, board) != Status::cheated){
                                        return std::optional<mv_ptr>{mv};
                                }
                        }
                }

        }

        return std::optional<mv_ptr>{std::nullopt};
}

TEST_CASE("Make a thing to do a thing") {
        First_Player pl0(Color::red);
        Pawn p0(0, Color::red);
        Board board;

        SECTION("Construct a move") {
                int red_start = board.get_color_start_space(Color::red);
                board.put_pawn(p0, red_start);

                fuel fuel{1};
                mv_ptr mv{new MoveMain(red_start, 1, p0)};


                REQUIRE(*(std::dynamic_pointer_cast<MoveMain>(pl0.construct_move(board, fuel).value()))  ==
                        *(std::dynamic_pointer_cast<MoveMain>(mv)));
        }
}
