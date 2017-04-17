#include "interfaces.h"
#include "rule_checker.h"
#include "moves.h"
#include <cassert>


Rules_Checker::Rules_Checker(::fuel fuel) :
        fuel(fuel){}

Status Rules_Checker::validate_main_move(MoveMain* mv, const Board &old_board){
        bool bop_bonus;
        bool home_bonus;

        if(encounters_blockade(mv, old_board) ||
           tried_safety_bop(mv, old_board) ||
           pawn_doesnt_exist(mv, old_board)){
                return Status::cheated;
        }

        return Status::normal;
}


Status Rules_Checker::validate_home_move(MoveHome* mv, const Board &old_board){
        if(encounters_blockade(mv, old_board)
           || pawn_doesnt_exist(mv, old_board)
           || moved_past_home(mv, old_board)
           ){
                return Status::cheated;
        }
        return Status::normal;
}

bool Rules_Checker::moved_past_home(MoveHome* mv, const Board &old_board){
        return mv->get_start() + mv->get_distance() > old_board.get_hr_spaces() + 1;
}

bool Rules_Checker::encounters_blockade(Move* mv,
                                        const Board &old_board){

        std::vector<Space> ispaces;
        if(dynamic_cast<MoveMain*>(mv)){
                ispaces = old_board.get_intermediate_spaces_main(mv->get_start(),
                                                                 mv->get_distance(),
                                                                 mv->get_pawn());
        }
        else{
                ispaces = old_board.get_intermediate_spaces_hr(mv->get_start(),
                                                               mv->get_distance(),
                                                               mv->get_pawn());
        }

        for(auto space : ispaces){
                if(space.size() > 1){
                        return true;
                }
        }

        return false;
}

bool Rules_Checker::tried_safety_bop(MoveMain* mv, const Board &old_board){
        int pos = mv->get_start() + mv->get_distance();
        Space target = old_board.get_pawns_at_pos(pos);
        if(target.size() == 0)
                return false;

        if(target[0].color != mv->get_pawn().color &&
           old_board.is_safety_space(pos)){
                return true;
        }

        return false;
}

bool Rules_Checker::pawn_doesnt_exist(Move* mv, const Board &old_board){
        std::vector<Pawn> pawns;

        if(dynamic_cast<MoveMain*>(mv)){
                pawns = old_board.get_pawns_at_pos(mv->get_start());
        } else {
                pawns = old_board.get_pawns_at_pos(mv->get_start(),
                                                   mv->get_pawn().color);
        }

        for(auto p : pawns){
                if(p == mv->get_pawn()) return false;
        }

        return true;
}
