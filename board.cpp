#include "board.h"
#include <cmath>


bool Board::is_contains(int start, int end, int pos){
        if(end < start){
                return (pos < end) ||
                        (pos >= start);
        } else {
                return start <= pos && pos < end;
        }
}

bool Board::is_blockade(int start, int dist){
        for(int i = 0; i < dist; i++){
                if(positions[(start + i) % ring_spaces].size() > 1){
                        return true;
                }
        }
        return false;
}


bool Board:: hr_is_blockade(int start, int dist, Pawn p){
        for(int i = 0; i < dist; i++){
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
        if(!entering &&
           safety_spaces.count(pos) == 2)
                return Status::cheated;
        if(positions[pos].size() != 2)
                return Status::normal;
        if(positions[pos][0].color != p.color){
                positions[pos].erase(positions[pos].begin());
                return Status::bop_bonus;
        }
        return Status::normal;
}
