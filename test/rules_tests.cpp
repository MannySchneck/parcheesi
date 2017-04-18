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
    REQUIRE(mv->inspect(rc, board)
            == Status::normal);
  }

  SECTION("Can't move onto blockade"){
    board.move_pawn(red_start, 5, p1);

    REQUIRE(board.positions[blockade_target][0] == p0);
    REQUIRE(board.positions[blockade_target][1] == p1);

    board.enter_pawn(p2);

    mv_ptr mv2(new MoveMain(red_start, 5, p2));

    REQUIRE(mv2->inspect(rc, board) == Status::cheated);

  }

  SECTION("Can't move past blockade"){
    board.move_pawn(red_start, 5, p1);

    REQUIRE(board.positions[blockade_target][0] == p0);
    REQUIRE(board.positions[blockade_target][1] == p1);

    board.enter_pawn(p2);

    mv_ptr mv2(new MoveMain(red_start, 6, p2));

    REQUIRE(mv2->inspect(rc, board) == Status::cheated);
  }

  // SECTION("can bop"){
  //   board.put_pawn(p0, board.get_color_start_space("blue") + 8);

  //   board.put_pawn(bp0, blue_start + 4);

  //   mv_ptr mv(new MoveMain(blue_start + 4, 4, bp0));

  //   Rules_Checker rc2{{4}};

  //   REQUIRE(mv->inspect(rc2, board) == Status::bop_bonus);

  // }

  SECTION("pawn exists at start space"){
    board.put_pawn(p0, board.get_color_start_space("blue") + 7);

    board.put_pawn(bp0, blue_start + 3);

    mv_ptr mv(new MoveMain(blue_start, 4, bp0));

    mv_ptr mv2(new MoveHome(0, 4, bp0));



    REQUIRE(mv->inspect(rc, board) == Status::cheated);

    REQUIRE(mv2->inspect(rc, board) == Status::cheated);
  }


  SECTION("can't bop a safety"){
    board.put_pawn(p0, board.get_color_start_space("blue") + 7);

    board.put_pawn(bp0, blue_start + 3);

    mv_ptr mv(new MoveMain(blue_start + 3, 4, bp0));

    REQUIRE(mv->inspect(rc, board) == Status::cheated);
  }

  SECTION("Moved past home"){
    int start = board.final_ring[p0.color];
    board.put_pawn(p0, start);

    board.move_pawn(start, 2, p0);

    mv_ptr mv(new MoveHome(2, 7, p0));
    REQUIRE(!rc.pawn_doesnt_exist(dynamic_cast<Move*>(mv.get()), board));

    REQUIRE(mv->inspect(rc, board) == Status::cheated);

    mv_ptr mv2(new MoveHome(2, 6, p0));
    REQUIRE(mv2->inspect(rc, board) == Status::normal);
  }

  SECTION("Can't move more than you have fuel"){
    mv_ptr mv (new MoveMain(2, 7, p0));

    board.put_pawn(p0, 2);

    Rules_Checker rc({3, 2 });
    REQUIRE(mv->inspect(rc, board) == Status::cheated);

    mv_ptr mv2(new MoveMain(2,7,p0));
    REQUIRE(mv2->inspect(rc, board) == Status::cheated);

    mv_ptr mv3(new MoveMain(2,3,p0));
    REQUIRE(mv3->inspect(rc, board) == Status::normal);
  }

  SECTION("Need 5 to enter"){
    mv_ptr mv{new EnterPiece(p0)};

    Rules_Checker rc({2,3});
    REQUIRE(mv->inspect(rc, board) == Status::normal);

    Rules_Checker rc2({1,4});
    REQUIRE(mv->inspect(rc2, board) == Status::normal);

    Rules_Checker rc3({4,1});
    REQUIRE(mv->inspect(rc3, board) == Status::normal);

    Rules_Checker rc4({4,4});
    REQUIRE(mv->inspect(rc4, board) == Status::cheated);

    Rules_Checker rc5({6,5});
    REQUIRE(mv->inspect(rc5, board) == Status::normal);

    Rules_Checker rc6({5,5});
    REQUIRE(mv->inspect(rc6, board) == Status::normal);

    Rules_Checker rc7({6,6});
    REQUIRE(mv->inspect(rc7, board) == Status::cheated);
  }

  SECTION("Can't enter blockaded"){
    int start = board.get_color_start_space(bp0.color);
    mv_ptr mv(new EnterPiece(bp0));
    board.put_pawn(p0, start);
    REQUIRE(mv->inspect(rc, board) == Status::normal);
    board.put_pawn(p1, start);
    REQUIRE(mv->inspect(rc, board) == Status::cheated);
  }
}
