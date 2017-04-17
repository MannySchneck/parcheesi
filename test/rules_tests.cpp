#include "../catch.hpp"
#include "../board.h"
#include "../rule_checker.h"
#include "../moves.h"

TEST_CASE("blockades"){
        Pawn p0(0,"red");
        Pawn p1(1,"red");
        Pawn p2(2,"red");
        Pawn bp0(0, "blue");
        Board board;

        int red_start(board.get_color_start_space("red"));

        int blockade_target = red_start + 5;

        board.put_pawn(p0, blockade_target);

        board.enter_pawn(p1);

        Rules_Checker rc({5, 5});

        mv_ptr mv(new MoveMain(red_start, 5, p1));

        int blue_start = board.get_color_start_space("blue");

        SECTION("can move to make blockade"){
                REQUIRE(mv->validate(rc, board)
                        == Status::normal);
        }

        SECTION("Can't move onto blockade"){
                board.move_pawn(red_start, 5, p1);

                REQUIRE(board.positions[blockade_target][0] == p0);
                REQUIRE(board.positions[blockade_target][1] == p1);

                board.enter_pawn(p2);

                mv_ptr mv2(new MoveMain(red_start, 5, p2));

                REQUIRE(mv2->validate(rc, board) == Status::cheated);

        }

        SECTION("Can't move past blockade"){
                board.move_pawn(red_start, 5, p1);

                REQUIRE(board.positions[blockade_target][0] == p0);
                REQUIRE(board.positions[blockade_target][1] == p1);

                board.enter_pawn(p2);

                mv_ptr mv2(new MoveMain(red_start, 6, p2));

                REQUIRE(mv2->validate(rc, board) == Status::cheated);
        }

        SECTION("can bop"){
                board.put_pawn(p0, board.get_color_start_space("blue") + 8);

                board.put_pawn(bp0, blue_start + 4);

                mv_ptr mv(new MoveMain(blue_start + 4, 4, bp0));

                REQUIRE(mv->validate(rc, board) == Status::bop_bonus);

        }

        SECTION("pawn exists at start space"){
                board.put_pawn(p0, board.get_color_start_space("blue") + 7);

                board.put_pawn(bp0, blue_start + 3);

                mv_ptr mv(new MoveMain(blue_start, 4, bp0));

                mv_ptr mv2(new MoveHome(bp0, 0, 4));



                REQUIRE(mv->validate(rc, board) == Status::cheated);

                REQUIRE(mv2->validate(rc, board) == Status::cheated);
        }


        SECTION("can't bop a safety"){
                board.put_pawn(p0, board.get_color_start_space("blue") + 7);

                board.put_pawn(bp0, blue_start + 3);

                mv_ptr mv(new MoveMain(blue_start + 3, 4, bp0));

                REQUIRE(mv->validate(rc, board) == Status::cheated);
        }

        SECTION("Moved past home"){
                int start = board.final_ring[p0.color];
                board.put_pawn(p0, start);

                board.move_pawn(start, 2, p0);

                mv_ptr mv(new MoveHome(p0, 2, 7));

                REQUIRE(!rc.pawn_doesnt_exist(dynamic_cast<Move*>(mv.get()), board));

                REQUIRE(mv->validate(rc, board) == Status::cheated);

                mv_ptr mv2(new MoveHome(p0, 2, 6));
                REQUIRE(mv->validate(rc, board) == Status::normal);
        }
}
