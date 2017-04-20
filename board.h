#pragma once

#include <unordered_map>
#include <vector>
#include "interfaces.h"
#include <unordered_set>


using Space = std::vector<Pawn>;

class Board {
 public:
        Board()
                {
                        // TODO player nums
                        for(int i = 0; i < 4; i++) {
                                safety_spaces.insert(i * section_spacing);
                                safety_spaces.insert(i * section_spacing + safety_1_offset);
                                safety_spaces.insert(i * section_spacing + safety_2_offset);
                        }

                        // TODO put these in an array
                        starting_pos["blue"] = starting_pos_offset;
                        starting_pos["yellow"] = starting_pos["blue"] + section_spacing;
                        starting_pos["green"] = starting_pos["yellow"] + section_spacing;
                        starting_pos["red"] = starting_pos["green"] + section_spacing;

                        final_ring["blue"] = 0; // board anchor
                        final_ring["yellow"] = final_ring["blue"] + section_spacing;
                        final_ring["green"] = final_ring["yellow"] + section_spacing;
                        final_ring["red"] = final_ring["green"] + section_spacing;
                }
        /*********************************************************************/
        /*                             Interface                             */
        /*********************************************************************/
        using Posn = std::tuple<Pawn, int, bool>;

        std::vector<Pawn> get_pawns_at_pos(int pos);
        std::vector<Pawn> get_pawns_at_pos(int pos, std::string);
        std::vector<Posn> get_pawns_of_color(std::string);

        using Section = std::unordered_map<int, std::vector<Pawn>>;

        std::vector<Space> get_intermediate_spaces_main(int start, int distance, Pawn p) ;
        std::vector<Space> get_intermediate_spaces_hr(int start, int distance, Pawn p) ;
        std::unordered_set<int> get_blockades();

        bool is_safety_space(int pos) const;
        bool is_contains(int start, int end, int pos) const;
        bool is_blockade(int start, int dist);
        bool hr_is_blockade(int start, int dist, Pawn p);
        Status move_onto_hr(int start, int num_into_hr, Pawn p);
        Status move_pawn(int start, int dist, Pawn p);
        void remove_pawn(int pos, Pawn p);
        Status enter_pawn(Pawn p);
        Status try_bop(int pos, Pawn p, bool entering);

        int get_hr_spaces() const;

        std::vector<int> board_diff(Board b);

        bool valid_board_transition(Board b, std::pair<int, int> dice);


        int get_color_start_space(std::string color){
                return starting_pos[color];
        }

        void put_pawn(Pawn p, int pos){
                positions[pos].push_back(p);
        }
 private:
        // The board's 0 position is the blue color's
        // home row entry square

        std::vector<std::vector<Pawn>>
                get_intermediate_spaces(int start,
                                        int distance,
                                        Pawn p,
                                        Section section);

#ifdef TEST
 public:
#endif


        static const int safety_1_offset = 5;
        static const int safety_2_offset = 12;
        static const int starting_pos_offset = 5;
        static const int section_spacing = 17;
        static const int ring_spaces = 68;
        static const int home_row_spaces = 7;
        std::unordered_set<int> safety_spaces;
        std::unordered_map<int, std::vector<Pawn>> positions;
        std::unordered_map<Color, int, enum_hash> starting_pos;
        std::unordered_map<Color, int, enum_hash> final_ring;
        using home_row_t = std::unordered_map<int, std::vector<Pawn>>;
        std::unordered_map<Color, home_row_t> home_rows;
        std::unordered_map<Color, int, enum_hash> pawn_reservoirs;
};
