#include "firstplayer.h"
#include <optional>
#include "catch.hpp"

First_Player::First_Player(Color color) : color(color)
{}

std::optional<Posn> First_Player::select_pawn(Board board){
        return board.get_farthest_pawn(color);
}
std::optional<mv_ptr> First_Player::construct_move(std::optional<Posn>, Board board, fuel fuel){
        return std::optional<mv_ptr>{std::nullopt};
}

TEST_CASE("Make a thing to do a thing") {
        First_Player pl0(Color::red);
        Pawn p0(0, Color::red);
        Board board;

        SECTION("Select only pawn on board") {
                int red_start = board.get_color_start_space(Color::red);
                board.put_pawn(p0, red_start);
                REQUIRE(pl0.select_pawn(board) == Posn(p0, red_start, false));
        }

        SECTION("Select returns nullopt when no pawns on board") {
                REQUIRE(pl0.select_pawn(board) == std::optional<Posn>(std::nullopt));
        }

        SECTION("Construct a move") {
                int red_start = board.get_color_start_space(Color::red);
                board.put_pawn(p0, red_start);
                auto posn = pl0.select_pawn(board);

                fuel fuel{1};
                mv_ptr mv{new MoveMain(red_start, 1, p0)};

                REQUIRE(*(std::dynamic_pointer_cast<MoveMain>(pl0.construct_move(posn, board, fuel).value()))  ==
                        *(std::dynamic_pointer_cast<MoveMain>(mv)));
        }
}
