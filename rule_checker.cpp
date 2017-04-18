#include "interfaces.h"
#include "rule_checker.h"
#include "moves.h"
#include <cassert>
#include <iostream>


Rules_Checker::Rules_Checker(::fuel fuel):
  fuel(fuel){}

bool Rules_Checker::validate_main_move(MoveMain* mv, Board &old_board){

  return !(encounters_blockade(mv, old_board) ||
           tried_safety_bop(mv, old_board) ||
           pawn_doesnt_exist(mv, old_board) ||
           illegal_distance(mv, old_board));
}


bool Rules_Checker::validate_home_move(MoveHome* mv, Board &old_board){
  return !(encounters_blockade(mv, old_board)
           || pawn_doesnt_exist(mv, old_board)
           || moved_past_home(mv, old_board)
           );
}

bool Rules_Checker::validate_enter_move(EnterPiece* mv, Board &old_board){
  return !(illegal_distance(mv, old_board)
           || start_blockaded(mv, old_board));
}

bool Rules_Checker::moved_past_home(MoveHome* mv, Board &old_board){
  return mv->get_start() + mv->get_distance() > old_board.get_hr_spaces() + 1;
}

bool Rules_Checker::encounters_blockade(Move* mv,
                                        Board &old_board){

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

bool Rules_Checker::tried_safety_bop(MoveMain* mv, Board &old_board){
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

bool Rules_Checker::pawn_doesnt_exist(Move* mv, Board &old_board){
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



bool Rules_Checker::illegal_distance(IMove* mv, Board &old_board){
  if(auto enter_move = dynamic_cast<EnterPiece*>(mv)){
    for(auto gallon1 : fuel){
      if (gallon1 == 5) return false;
      for(auto gallon2 : fuel){
        if(gallon1 + gallon2 == 5){
          return false;
        }
      }
    }
    return true;

  }else if(auto moving_move = dynamic_cast<Move*>(mv)){
    for(auto gallon : fuel){
      if(gallon == moving_move->get_distance()) return false;
    }
    return true;
  } else {
    throw std::logic_error("can't happen");
  }
}

bool Rules_Checker::start_blockaded(EnterPiece* mv, Board &old_board){
  auto pawns = old_board.
    get_pawns_at_pos(old_board. get_color_start_space(mv->get_pawn().color));

  return pawns.size() > 1;
}

// // @pre: check for blockades was run
// bool Rules_Checker::did_bop(IMove* mv, Board &old_board){

//   if(auto mm = dynamic_cast<MoveMain*>(mv)){
//     auto pawns = old_board.get_pawns_at_pos(mm->get_start() + mm->get_distance());

//     return(pawns.size() > 0 && pawns[0].color != mm->get_pawn().color);
//   }

//   return false;
// }
