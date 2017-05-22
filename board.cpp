#include "board.h"
#include <cmath>
#include "catch.hpp"
#include <iostream>
#include <vector>
#include <tuple>
#include "moves.h"
#include "prettyprint.hh"
#include <exception>
#include <algorithm>

#include <sstream>

Status Board::apply(std::shared_ptr<IMove> mv){

        Status status;

        if(auto mm = dynamic_cast<MoveMain*>(mv.get())){
                status = move_pawn(mm->get_start(),
                                   mm->get_distance(),
                                   mm->get_pawn());
        } else if(auto mh = dynamic_cast<MoveHome*>(mv.get())){
                status = move_pawn_hr(mh->get_start(),
                                      mh->get_distance(),
                                      mh->get_pawn());
        } else if(auto me = dynamic_cast<EnterPiece*>(mv.get())){
                status = enter_pawn(me->get_pawn());
        } else {
                throw std::logic_error("You added a new move type and forgot to change apply");
        }

        if(status == Status::cheated){
                throw std::logic_error("Tried to apply an invalid move!\n"
                                       "Dumbass.\n"
                                       "Did you call the rules checker before applying the move????????");
        }

        return Status::normal;
}


int Board::pos_to_dist(int pos, Color color){
        return modulo(pos - starting_pos[color], ring_spaces);
}


std::vector<Posn> Board::get_sorted_pawns(Color color, Direction dir){
        auto posns = get_pawns_of_color(color);
        std::sort(std::begin(posns), std::end(posns),
                  [this, dir, color](const Posn &fuck, const Posn &you){
                          if(std::get<loc>(you) == -1)
                                  return false;

                          bool same_ring = std::get<is_home>(fuck) == std::get<is_home>(you);
                          bool index_greater = std::get<is_home>(you) ? std::get<loc>(you) > std::get<loc>(fuck) :
                                  pos_to_dist(std::get<loc>(you), color) > pos_to_dist(std::get<loc>(fuck), color);
                          bool home_beats_ring = std::get<is_home>(you) && !std::get<is_home>(fuck);

                          bool result;


                          if(same_ring){
                                  result = index_greater;
                          }
                          else{
                                  result = home_beats_ring;
                          }

                          switch(dir){
                          case(Direction::increasing):
                                  return result;
                          case(Direction::decreasing):
                                  return !result;
                          default:
                                  throw std::logic_error("Fuck you. Shoulda used coq");
                          }
                  });
        return posns;
}

std::optional<Posn> Board::get_farthest_pawn(Color color){
        auto posns = get_pawns_of_color(color);

        return get_sorted_pawns(color, Direction::increasing).back();

}

void Board::reset_farthest_pawn(Color color){

        auto farthest_posn_maybe = get_farthest_pawn(color);

        if(!farthest_posn_maybe.has_value()){
                return;
        }

        Posn farthest_posn = farthest_posn_maybe.value();

        auto the_pawn = std::get<pawn>(farthest_posn);


        remove_pawn(std::get<loc>(farthest_posn),
                    std::get<pawn>(farthest_posn),
                    std::get<is_home>(farthest_posn) ? home_rows[the_pawn.color] : positions);

        nest[the_pawn.color].push_back(the_pawn);
}

int Board::get_nest_count(Color color) const{
        return nest.count(color) ? nest.at(color).size() : 0;
}

int Board::get_hr_spaces() const{
        return home_row_spaces;
}

std::vector<Pawn> Board::get_pawns_at_pos(int pos) {
        return positions[pos];
}


std::vector<Posn> Board::get_pawns_of_color(Color color) const{
        std::vector<Posn> the_pawns;

        if(nest.count(color)){
                for(auto pawn : nest.at(color)){
                        the_pawns.push_back(Posn{pawn, -1, false});
                }
        }

        for(auto space : positions){
                for(auto p : space.second){
                        if(p.color == color) the_pawns.push_back(Posn{p, space.first, false});
                }
        }

        for(auto space : home_rows.at(color)){
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

Status Board::move_pawn_hr(int start, int distance, Pawn p){
        home_row_t &row = home_rows.at(p.color);

        int target_pos = start + distance;

        if(hr_is_blockade(start, target_pos, p)){
                return Status::cheated;
        }

        auto& space = row[start];
        // lol
        space.erase((space.begin()->id == p.id) ?
                    space.begin() :
                    std::next(space.begin()));


        if(start + distance == home_row_spaces){
                home[p.color].push_back(p);
                return Status::home_bonus;
        }


        row[target_pos].push_back(p);
        return Status::normal;
}


Status Board::move_onto_hr(int start, int num_into_hr, Pawn p){
        remove_pawn(start, p, positions);

        if(hr_is_blockade(start, num_into_hr, p)){
                return Status::cheated;
        }
        if(num_into_hr == home_row_spaces){
                home[p.color].push_back(p);
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
        remove_pawn(start, p, positions);
        positions[final_pos].push_back(p);
        return try_bop(final_pos, p, false);
}

// pre: pawn must exist at pos. rules_checker will ensure this.
void Board::remove_pawn(int pos, Pawn p, Section &section){
        auto& space = section[pos];
        space.erase((space.begin()->id == p.id) ? space.begin() : std::next(space.begin()));
}


Status Board::enter_pawn(Pawn p){
        Color color = p.color;
        int start_pos = starting_pos[color];
        if(is_blockade(start_pos, 0))
                return Status::cheated;
        if(nest[p.color].size() == 0){
                return Status::cheated;
        }
        positions[start_pos].push_back(p);

        auto pos = std::find(std::begin(nest[p.color]), std::end(nest[p.color]), p);
        if (pos != std::end(nest[p.color])) {
                nest[p.color].erase(pos);
        }

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
                auto the_pawn = positions[pos].begin();
                nest[the_pawn->color].push_back(*the_pawn);
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

std::string Board::serialize() const{
        std::stringstream ss;

        ss << "<board> ";

        ss << serialize_start();

        ss << serialize_main();

        ss << serialize_home_row();

        ss << serialize_home();

        ss << "</board>";

        return ss.str();
}



std::string Board::serialize_start() const{
        std::stringstream ss;

        ss << "<start> ";

        for(auto color : Game_Consts::colors){
                if(nest.count(color)){
                        for(auto pawn : nest.at(color)){
                                ss << pawn.serialize();
                        }
                }
        }

        ss << "</start> ";
        return ss.str();
}

std::string serialize(Posn p){
        std::stringstream ss;
        ss << "<piece-loc> ";

        ss << std::get<pawn>(p).serialize();

        ss << "<loc> ";
        ss << std::get<loc>(p);
        ss << " </loc> ";

        ss << "</piece-loc> ";

        return ss.str();
}

std::string Board::serialize_main() const{
        std::stringstream ss;

        ss << "<main> ";

        for(auto color : Game_Consts::colors){
                for(auto the_posn : get_pawns_of_color(color)){
                        if(!std::get<is_home>(the_posn) && std::get<loc>(the_posn) != -1){
                                ss << ::serialize(the_posn);
                        }
                }
        }

        ss << "</main> ";

        return ss.str();
}

std::string Board::serialize_home_row() const{
        std::stringstream ss;

        ss << "<home-rows> ";

        for(auto color : Game_Consts::colors){
                for(auto the_posn : get_pawns_of_color(color)){
                        if(std::get<is_home>(the_posn)){
                                ss << ::serialize(the_posn);
                        }
                }
        }
        ss << "</home-rows> ";

        return ss.str();
}


std::string Board::serialize_home() const{
        std::stringstream ss;

        ss << "<home> ";

        for(auto color : Game_Consts::colors){
                if(home.count(color)){
                        for(auto pawn : home.at(color)){
                                ss << pawn.serialize();
                        }
                }
        }

        ss << "</home> ";

        return ss.str();
}

bool Board::operator==(const Board &rhs){
        auto cmp_things = [](const Board &board1, const Board &board2){
                //Main rings should be same
                for(auto pos : board1.positions){
                        auto loc = pos.first;
                        auto pawns = pos.second;

                        for(auto pawn : pawns){
                                if(board1.positions.count(loc) != board2.positions.count(loc)){
                                        return false;
                                }else {
                                        auto pos = std::find(std::begin(board2.positions.at(loc)),
                                                             std::end(board2.positions.at(loc)),
                                                             pawn);
                                        if (pos == std::end(board2.positions.at(loc))) {
                                                return  false;
                                        }
                                }
                        }
                }

                //Nests should be same
                for(auto color : Game_Consts::colors){
                        if(board1.nest.count(color)){
                                for(auto pawn : board1.nest.at(color)){

                                        auto pos = std::find(std::begin(board2.nest.at(color)),
                                                             std::end(board2.nest.at(color)),
                                                             pawn);
                                        if (pos == std::end(board2.nest.at(color))) {
                                                return  false;
                                        }
                                }
                                if(board1.get_nest_count(color) != board2.get_nest_count(color)) return false;
                        }
                }

                //Home rows should be same
                for(auto color : Game_Consts::colors){

                        auto section = board1.home_rows.at(color);
                        auto rhs_section = board2.home_rows.at(color);

                        for(auto pos : section){
                                for(auto pawn : pos.second){
                                        auto loc = pos.first;
                                        if(rhs_section.count(loc)){
                                                auto pos = std::find(std::begin(rhs_section.at(loc)),
                                                                     std::end(rhs_section.at(loc)),
                                                                     pawn);
                                                if (pos == std::end(rhs_section.at(loc))) {
                                                        return  false;
                                                }
                                        }
                                }
                        }

                }
                return true; //=)
        };

        return cmp_things(*this, rhs) && cmp_things(rhs, *this);bool found = true;
}

std::ostream& operator<<(std::ostream& os, const Board& c)
{
        os << c.serialize();
        return os;
}


TEST_CASE("operator== isn't completely broken"){
        Board board1;
        Board board2;

        REQUIRE(board1 == board2);

        board2.put_pawn(Pawn{0, Color::blue}, 0);

        REQUIRE(!(board1 == board2));
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

                int thing = board.nest[p0.color].size();

                int thingo = board.nest[p1.color].size();

                REQUIRE(board.enter_pawn(p1) == Status::bop_bonus);
                REQUIRE(board.nest[p1.color].size() == thingo - 1);
                REQUIRE(board.nest[p0.color].size() == thing + 1);

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


TEST_CASE("moving on home row"){
        Board board;
        Pawn p0(0, Color::blue);

        board.home_rows[Color::blue][2].push_back(p0);

        REQUIRE(board.move_pawn_hr(2, 3, p0) == Status::normal);
        REQUIRE(board.home_rows[p0.color][5].size() == 1);
        REQUIRE(board.home_rows[p0.color][5][0] == p0);
        REQUIRE(board.home_rows[p0.color][2].size() == 0);
}

TEST_CASE("moving home all the way baby"){
        Board board;
        Pawn p0(0, Color::blue);

        board.home_rows[Color::blue][2].push_back(p0);

        REQUIRE(board.move_pawn_hr(2, 5, p0) == Status::home_bonus);
        REQUIRE(board.home_rows[p0.color][2].size() == 0);
        REQUIRE(board.home[Color::blue][0] == p0);
}

TEST_CASE("reset_farthest pawn"){
        Board board;

        Pawn p0(0, Color::blue);
        Pawn p1(1, Color::blue);
        Pawn p2(2, Color::blue);

        SECTION("home main"){
                board.put_pawn(p0, 19);
                board.put_pawn(p1, 60);


                board.put_pawn(p2, board.final_ring[p0.color]);

                board.move_pawn(board.final_ring[p0.color], 5, p2);

                REQUIRE(board.get_pawns_at_pos(19).size() == 1);
                REQUIRE(board.get_pawns_at_pos(60).size() == 1);
                REQUIRE(board.home_rows[p0.color][5].size() == 1);

                board.reset_farthest_pawn(p0.color);

                REQUIRE(board.get_pawns_at_pos(19).size() == 1);
                REQUIRE(board.get_pawns_at_pos(60).size() == 1);
                REQUIRE(board.home_rows[p0.color][5].size() == 0);
        }

        SECTION("main main"){
                board.put_pawn(p0, 19);
                board.put_pawn(p1, 60);

                REQUIRE(board.get_pawns_at_pos(19).size() == 1);
                REQUIRE(board.get_pawns_at_pos(60).size() == 1);

                board.reset_farthest_pawn(p0.color);

                REQUIRE(board.get_pawns_at_pos(19).size() == 1);
                REQUIRE(board.get_pawns_at_pos(60).size() == 0);

        }

        SECTION("home home"){

                board.put_pawn(p1, board.final_ring[p0.color]);
                board.move_pawn(board.final_ring[p0.color], 5, p1);

                board.put_pawn(p2, board.final_ring[p0.color]);
                board.move_pawn(board.final_ring[p0.color], 4, p2);

                REQUIRE(board.home_rows[p0.color][4].size() == 1);
                REQUIRE(board.home_rows[p0.color][5].size() == 1);

                board.reset_farthest_pawn(p0.color);

                REQUIRE(board.home_rows[p0.color][4].size() == 1);
                REQUIRE(board.home_rows[p0.color][5].size() == 0);
        }
}
