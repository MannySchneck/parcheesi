#include "board.h"
#include <cmath>
#include "catch.hpp"
#include <iostream>
#include <vector>

bool Board::is_contains(int start, int end, int pos){
        if(end < start){
                return (pos < end) ||
                        (pos >= start);
        } else {
                return start <= pos && pos < end;
        }
}


// Todo:
// refactor to include
bool Board::is_blockade(int start, int dist){
        for(int i = 0; i <= dist; i++){
                if(positions[(start + i) % ring_spaces].size() > 1){
                        return true;
                }
        }
        return false;
}


bool Board:: hr_is_blockade(int start, int dist, Pawn p){
        for(int i = 0; i <= dist; i++){
                if(home_rows[p.color][(start + i) % ring_spaces].size() > 1){
                        return true;
                }
        }
        return false;
}

Status Board::move_onto_hr(int start, int num_into_hr, Pawn p){
        remove_pawn(start, p);

        if(hr_is_blockade(start, num_into_hr, p)){
                return Status::cheated;
        }
        if(num_into_hr == home_row_spaces){
                return Status::home_bonus;
        }
        home_rows[p.color][num_into_hr].push_back(p);
        return Status::normal;
}

Status Board::move_pawn(int start, int dist, Pawn p){ // XXX
        int final_pos = (start + dist) % ring_spaces;
        if(is_contains(start, final_pos, final_ring[p.color])){
                if(is_blockade(start, modulo(final_ring[p.color] - start, ring_spaces)))
                        return Status::cheated;

                int num_into_hr = modulo(final_pos - final_ring[p.color], ring_spaces);
                move_onto_hr(start, num_into_hr, p);
        }

        if(is_blockade(start, dist))
                return Status::cheated;
        remove_pawn(start, p);
        positions[final_pos].push_back(p);
        return try_bop(final_pos, p, false);
}

void Board::remove_pawn(int pos, Pawn p){
        auto& space = positions[pos];
        space.erase((space.begin()->id == p.id) ? space.begin() : space.begin()++);
}


Status Board::enter_pawn(Pawn p){
        std::string color = p.color;
        int start_pos = starting_pos[color];
        if(is_blockade(start_pos, 0))
                return Status::cheated;
        positions[start_pos].push_back(p);
        return try_bop(start_pos, p, true);
}

Status Board::try_bop(int pos, Pawn p, bool entering){
        if(!entering
           && safety_spaces.count(pos)
           && positions[pos].size() > 1)
                return Status::cheated;
        if(positions[pos].size() != 2)
                return Status::normal;
        if(positions[pos][0].color != p.color){
                positions[pos].erase(positions[pos].begin());
                std::cout << safety_spaces.count(pos) << std::endl;
                return Status::bop_bonus;
        }
        return Status::normal;
}

bool Board::valid_board_transition(Board b, std::pair<int, int> dice){
        //std::vector<int> changed_positions = board_diff(b);
        std::unordered_set<int> original_blockades;

        for(auto space : positions){
                int pos = space.first;
                auto pawns = space.second;
                if(pawns.size() == 2) {
                        original_blockades.insert(pos);
                }
        }

        for(auto space : b.positions){
                int pos = space.first;
                auto pawns = space.second;

                if(pawns.size() == 2)
                        if(!original_blockades.count(pos) && original_blockades.count(pos - dice.first)){
                                return false;
                        }
        }

        return true;
}

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

        SECTION("Test Move into Blockade"){
                Pawn p0(0,"red");
                Pawn p1(1,"red");
                Pawn p2(2,"red");
                Board board;
                int red_start(board.get_color_start_space("red"));
                int blockade_target = red_start + 5;
                board.put_pawn(p0, blockade_target);
                board.enter_pawn(p1);

                REQUIRE(board.move_pawn(red_start, 5, p1)
                        == Status::normal);

                REQUIRE(board.positions[blockade_target][0] == p0);
                REQUIRE(board.positions[blockade_target][1] == p1);

                board.enter_pawn(p2);

                REQUIRE(board.move_pawn(board.get_color_start_space("red"), 5, p2)
                        == Status::cheated);
        }

        SECTION("Test bop"){
                Board board;
                Pawn p0(0, "red");

                board.put_pawn(p0, board.get_color_start_space("blue") + 4);

                Pawn p1(0, "blue");
                board.enter_pawn(p1);

                REQUIRE(board.move_pawn(board.get_color_start_space("blue"), 4, p1)
                        == Status::bop_bonus);

                REQUIRE(board.positions[board.get_color_start_space("blue")+4].size() == 1);
                REQUIRE(board.positions[board.get_color_start_space("blue")+4][0] == p1);
        }

        SECTION("Test no bop on safety space"){
                Board board;
                Pawn p0(0, "red");

                board.put_pawn(p0, board.get_color_start_space("blue") + 7);

                Pawn p1(0, "blue");
                board.enter_pawn(p1);

                REQUIRE(board.move_pawn(board.get_color_start_space("blue"), 7, p1)
                        == Status::cheated);

                REQUIRE(board.positions[board.get_color_start_space("blue")][0] == p1);

                REQUIRE(board.positions[board.get_color_start_space("blue") + 7][0] == p0);
        }

        SECTION("Test bop safety space on enter") {
                Board board;
                Pawn p0(0, "red");

                board.put_pawn(p0, board.get_color_start_space("blue"));

                Pawn p1(0, "blue");
                REQUIRE(board.enter_pawn(p1) == Status::bop_bonus);

                REQUIRE(board.positions[board.get_color_start_space("blue")][0] == p1);
                REQUIRE(board.positions[board.get_color_start_space("blue")].size() == 1);
        }
}


TEST_CASE("check_final_board", "test checking board after full turn complete"){
        SECTION("Test move blockade together"){
                Board board1;
                Pawn p0(0, "blue");
                Pawn p1(1, "blue");

                std::pair<int, int> dice(3,3);

                board1.enter_pawn(p0);
                board1.enter_pawn(p1);


                Board board2;
                board2.put_pawn(p0, board2.get_color_start_space("blue") + 3);
                board2.put_pawn(p1, board2.get_color_start_space("blue") + 3);

                REQUIRE(board1.valid_board_transition(board2, dice) == false);
        }


        SECTION("Test move blockade separate (should be ok)"){
                Board board1;
                Pawn p0(0, "blue");
                Pawn p1(1, "blue");

                std::pair<int, int> dice(3,4);

                board1.enter_pawn(p0);
                board1.enter_pawn(p1);

                Board board2;
                board2.put_pawn(p0, board2.get_color_start_space("blue") + 3);
                board2.put_pawn(p1, board2.get_color_start_space("blue") + 4);

                REQUIRE(board1.valid_board_transition(board2, dice) == true);
        }

}
