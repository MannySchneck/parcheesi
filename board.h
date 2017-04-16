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
                        for(int i = 0; i < 4; i++) {
                                safety_spaces.insert(i * 17);
                                safety_spaces.insert(i * 17 + 5);
                                safety_spaces.insert(i * 17 + 12);
                        }

                        starting_pos["blue"] = 5;
                        starting_pos["yellow"] = 22;
                        starting_pos["green"] = 39;
                        starting_pos["red"] = 56;

                        final_ring["blue"] = 0;
                        final_ring["yellow"] = 17;
                        final_ring["green"] = 34;
                        final_ring["red"] = 51;
                }
        /*********************************************************************/
        /*                             Interface                             */
        /*********************************************************************/
        std::vector<Pawn> get_pawns_at_pos(int pos) const{
                try{
                        Space posns = positions.at(pos);
                        return posns;
                } catch(const std::out_of_range& e){
                        return std::vector<Pawn>{};
                }
        }

        using Section = std::unordered_map<int, std::vector<Pawn>>;

        std::vector<Space> get_intermediate_spaces_main(int start, int distance, Pawn p) const;
        std::vector<Space> get_intermediate_spaces_hr(int start, int distance, Pawn p) const;

        bool is_safety_space(int pos) const;

        bool is_contains(int start, int end, int pos) const;


        bool is_blockade(int start, int dist);

        bool hr_is_blockade(int start, int dist, Pawn p);

        Status move_onto_hr(int start, int num_into_hr, Pawn p);

        Status move_pawn(int start, int dist, Pawn p);

        void remove_pawn(int pos, Pawn p);

        Status enter_pawn(Pawn p);

        Status try_bop(int pos, Pawn p, bool entering);

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
                                        Section section) const;

#ifdef TEST
 public:
#endif


        static const int ring_spaces = 68;
        static const int home_row_spaces = 7;
        std::unordered_set<int> safety_spaces;
        std::unordered_map<int, std::vector<Pawn>> positions;
        std::unordered_map<std::string, int> starting_pos;
        std::unordered_map<std::string, int> final_ring;
        using home_row_t = std::unordered_map<int, std::vector<Pawn>>;
        std::unordered_map<std::string, home_row_t> home_rows;
        std::unordered_map<std::string, int> pawn_reservoirs;
};
