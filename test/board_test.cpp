#define CATCH_CONFIG_MAIN
#include "../catch.hpp"
#include "../board.h"
#include <iostream>


TEST_CASE("Enter Pawn", "[Enter Pawn]") {
        Board board;


        SECTION("red"){
                Pawn p(0, "red");
                REQUIRE(board.enter_pawn(p) == Status::normal);
                REQUIRE(board.get_pawns_at_pos(board.get_color_start_space("red"))[0] == p);
        }

        SECTION("blue 1 pawn"){
                Pawn p(3,"blue");
                REQUIRE(board.enter_pawn(p) == Status::normal);
                REQUIRE(board.get_pawns_at_pos(board.get_color_start_space("blue"))[0] == p);

        }

        SECTION("blue 2 pawns"){
                Pawn p(3,"blue");
                Pawn p2(2, "blue");
                REQUIRE(board.enter_pawn(p) == Status::normal);
                REQUIRE(board.enter_pawn(p2) == Status::normal);
                REQUIRE(board.get_pawns_at_pos(board.get_color_start_space("blue"))[0] == p);
                REQUIRE(board.get_pawns_at_pos(board.get_color_start_space("blue"))[1] == p2);

        }
}

TEST_CASE("move_pawn", "test move to safety space"){

        SECTION("Test i = 12"){
                int i = 12;
                std::cout << "running test " << i << std::endl;
                Pawn p(0,"blue");
                Board board;

                board.enter_pawn(p);

                REQUIRE(board.move_pawn(board.get_color_start_space("blue"), i, p) ==
                        Status::normal);

                REQUIRE(board.get_pawns_at_pos(board.get_color_start_space("blue")
                                               + i % board.ring_spaces)[0]
                        == p);

                REQUIRE(board.positions[0].size() == 0);
        }


        SECTION("Test all moves from beginning"){
                for(int i = 1; i < 21; i++){
                        std::cout << "running test " << i << std::endl;
                        Pawn p(0,"blue");
                        Board board;

                        board.enter_pawn(p);

                        REQUIRE(board.move_pawn(board.get_color_start_space("blue"), i, p) ==
                                Status::normal);

                        REQUIRE(board.get_pawns_at_pos(board.get_color_start_space("blue")
                                                       + i % board.ring_spaces)[0]
                                == p);

                        REQUIRE(board.positions[0].size() == 0);
                }
        }
}
