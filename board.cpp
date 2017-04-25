#include "board.h"
#include <cmath>
#include "catch.hpp"
#include <iostream>
#include <vector>
#include <tuple>


using Posn = std::tuple<Pawn, int, bool>;


int Board::get_nest_count(Color color){
        return nest_count.at(color);
}

int Board::get_hr_spaces() const{
        return home_row_spaces;
}



std::vector<Pawn> Board::get_pawns_at_pos(int pos) {
        return positions[pos];
}


std::vector<Posn> Board::get_pawns_of_color(Color color){
        std::vector<Posn> the_pawns;

        for(auto space : positions){
                for(auto p : space.second){
                        if(p.color == color) the_pawns.push_back(Posn{p, space.first, false});
                }
        }

        for(auto space : home_rows[color]){
                for(auto p : space.second){
                        if(p.color == color) the_pawns.push_back(Posn{p, space.first, true});
                }
        }

        return the_pawns;
}

std::vector<Pawn> Board::get_pawns_at_pos(int pos, Color color) {
        return home_rows[color][pos];
}

std::vector<std::vector<Pawn>>
Board::get_intermediate_spaces(int start,
                               int distance,
                               Section section){



        std::vector<std::vector<Pawn>> spaces;
        for(int i = 0; i <= distance; i++){
                if(section.count(start + i % ring_spaces)){
                        spaces.push_back(section[start + i % ring_spaces]);
                }
        }


        return spaces;
}


std::vector<Space> Board::get_intermediate_spaces_hr(int start, int distance, Pawn p) {
        return get_intermediate_spaces(start, distance, home_rows[p.color]);
}

std::vector<std::vector<Pawn>>
Board::get_intermediate_spaces_main(int start, int distance, Pawn p) {

        std::vector<std::vector<Pawn>> spaces;

        int final_pos = start + distance;
        if(is_contains(start, final_pos, final_ring[p.color])){
                int num_up_to_hr = modulo(final_ring[p.color] - start, ring_spaces);
                int num_into_hr = modulo(final_pos - final_ring[p.color], ring_spaces);

                auto intermediates_main = get_intermediate_spaces(start, num_up_to_hr,  positions);
                auto st = intermediates_main.begin();
                auto end = intermediates_main.end();

                spaces.insert(spaces.end(), st, end);

                auto intermediates_hr = get_intermediate_spaces(0, num_into_hr, home_rows[p.color]);
                st = intermediates_hr.begin();
                end = intermediates_hr.end();
                spaces.insert(spaces.end(), st, end);

                return spaces;
        }

        return get_intermediate_spaces(start, distance, positions);
}

bool Board::is_safety_space(int pos) const{
        return safety_spaces.count(pos);
}


bool Board::is_contains(int start, int end, int pos) const{
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
                return move_onto_hr(start, num_into_hr, p);
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
        Color color = p.color;
        int start_pos = starting_pos[color];
        if(is_blockade(start_pos, 0))
                return Status::cheated;
        if(nest_count[p.color] == 0){
                return Status::cheated;
        }
        positions[start_pos].push_back(p);
        nest_count[p.color] -= 1;
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
                return Status::bop_bonus;
        }
        return Status::normal;
}

std::unordered_set<int> Board::get_blockades(){
        std::unordered_set<int> original_blockades;

        for(auto space : positions){
                int pos = space.first;
                auto pawns = space.second;
                if(pawns.size() == 2) {
                        original_blockades.insert(pos);
                }
        }

        return original_blockades;
}

TEST_CASE("Enter Pawn", "[Enter Pawn]") {
        Board board;


        SECTION("Color::red"){
                Pawn p(0, Color::red);
                REQUIRE(board.enter_pawn(p) == Status::normal);
                REQUIRE(board.get_pawns_at_pos(board.get_color_start_space(Color::red))[0] == p);
        }

        SECTION("blue 1 pawn"){
                Pawn p(3,Color::blue);
                REQUIRE(board.enter_pawn(p) == Status::normal);
                REQUIRE(board.get_pawns_at_pos(board.get_color_start_space(Color::blue))[0] == p);

        }

        SECTION("blue 2 pawns"){
                Pawn p(3,Color::blue);
                Pawn p2(2, Color::blue);
                REQUIRE(board.enter_pawn(p) == Status::normal);
                REQUIRE(board.enter_pawn(p2) == Status::normal);
                REQUIRE(board.get_pawns_at_pos(board.get_color_start_space(Color::blue))[0] == p);
                REQUIRE(board.get_pawns_at_pos(board.get_color_start_space(Color::blue))[1] == p2);

        }

}

TEST_CASE("move_pawn", "test move to safety space"){

        SECTION("Test i = 12"){
                int i = 12;
                Pawn p(0,Color::blue);
                Board board;

                board.enter_pawn(p);

                REQUIRE(board.move_pawn(board.get_color_start_space(Color::blue), i, p) ==
                        Status::normal);

                REQUIRE(board.get_pawns_at_pos(board.get_color_start_space(Color::blue)
                                               + i % board.ring_spaces)[0]
                        == p);

                REQUIRE(board.positions[0].size() == 0);
        }


        SECTION("Test all moves from beginning"){
                for(int i = 1; i < 21; i++){
                        Pawn p(0,Color::blue);
                        Board board;

                        board.enter_pawn(p);

                        REQUIRE(board.move_pawn(board.get_color_start_space(Color::blue), i, p) ==
                                Status::normal);

                        REQUIRE(board.get_pawns_at_pos(board.get_color_start_space(Color::blue)
                                                       + i % board.ring_spaces)[0]
                                == p);

                        REQUIRE(board.positions[0].size() == 0);
                }
        }

        SECTION("Test Move into Blockade"){
                Pawn p0(0,Color::red);
                Pawn p1(1,Color::red);
                Pawn p2(2,Color::red);
                Board board;
                int red_start(board.get_color_start_space(Color::red));
                int blockade_target = red_start + 5;
                board.put_pawn(p0, blockade_target);
                board.enter_pawn(p1);

                REQUIRE(board.move_pawn(red_start, 5, p1)
                        == Status::normal);

                REQUIRE(board.positions[blockade_target][0] == p0);
                REQUIRE(board.positions[blockade_target][1] == p1);

                board.enter_pawn(p2);

                REQUIRE(board.move_pawn(board.get_color_start_space(Color::red), 5, p2)
                        == Status::cheated);
        }

        SECTION("Test bop"){
                Board board;
                Pawn p0(0, Color::red);

                board.put_pawn(p0, board.get_color_start_space(Color::blue) + 4);

                Pawn p1(0, Color::blue);
                board.enter_pawn(p1);

                REQUIRE(board.move_pawn(board.get_color_start_space(Color::blue), 4, p1)
                        == Status::bop_bonus);

                REQUIRE(board.positions[board.get_color_start_space(Color::blue)+4].size() == 1);
                REQUIRE(board.positions[board.get_color_start_space(Color::blue)+4][0] == p1);
        }

        SECTION("Test no bop on safety space"){
                Board board;
                Pawn p0(0, Color::red);

                board.put_pawn(p0, board.get_color_start_space(Color::blue) + 7);

                Pawn p1(0, Color::blue);
                board.enter_pawn(p1);

                REQUIRE(board.move_pawn(board.get_color_start_space(Color::blue), 7, p1)
                        == Status::cheated);

                REQUIRE(board.positions[board.get_color_start_space(Color::blue)][0] == p1);

                REQUIRE(board.positions[board.get_color_start_space(Color::blue) + 7][0] == p0);
        }

        SECTION("Test bop safety space on enter") {
                Board board;
                Pawn p0(0, Color::red);

                board.put_pawn(p0, board.get_color_start_space(Color::blue));

                Pawn p1(0, Color::blue);
                REQUIRE(board.enter_pawn(p1) == Status::bop_bonus);

                REQUIRE(board.positions[board.get_color_start_space(Color::blue)][0] == p1);
                REQUIRE(board.positions[board.get_color_start_space(Color::blue)].size() == 1);
        }
}


TEST_CASE("check_final_board", "test checking board after full turn complete"){
        Pawn p0(0, Color::blue);
        Pawn p1(1, Color::blue);




        SECTION("Get pawns of color"){
                Board board;
                Pawn p2(2, Color::blue);
                Pawn p3(0, Color::red);


                board.put_pawn(p0, 2);
                board.put_pawn(p1, 16);
                board.put_pawn(p2, board.final_ring[p2.color]);
                board.move_pawn(board.final_ring[p2.color], 5, p2);

                board.put_pawn(p3, 5);

                Posn pos0{p0, 2, false};
                Posn pos1{p1, 16, false};
                Posn pos2{p2, 5, true};
                Posn pos3{p3, 5, false};
                auto pawns = board.get_pawns_of_color(p0.color);

                REQUIRE(std::find(pawns.begin(), pawns.end(), pos0) != pawns.end());
                REQUIRE(std::find(pawns.begin(), pawns.end(), pos1) != pawns.end());
                REQUIRE(std::find(pawns.begin(), pawns.end(), pos2) != pawns.end());
                REQUIRE(std::find(pawns.begin(), pawns.end(), pos3) == pawns.end());

        }

}
