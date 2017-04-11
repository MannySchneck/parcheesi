#pragma once

#include <unordered_map>
#include <vector>
#include "interfaces.h"
#include <unordered_set>

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

        bool is_contains(int start, int end, int pos);

        bool is_blockade(int start, int dist);

        bool hr_is_blockade(int start, int dist, Pawn p);

        Status move_onto_hr(int start, int num_into_hr, Pawn p);

        Status move_pawn(int start, int dist, Pawn p);

        void remove_pawn(int pos, Pawn p);

        Status enter_pawn(Pawn p);

        Status try_bop(int pos, Pawn p, bool entering);

        #ifdef TEST
        std::vector<Pawn> get_pawns_at_pos(int pos){
                return positions[pos];
        }

        int get_color_start_space(std::string color){
                return starting_pos[color];
        }
        #endif
private:
        // The board's 0 position is the blue color's
        // home row entry square

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
};
