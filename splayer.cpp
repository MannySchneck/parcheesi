#include "splayer.h"

Turn_Outcome S_Player::do_turn(Turn turn){
        return Turn_Outcome::normal;
}

S_Player::S_Player(Color color, Board board) :
        color(color),
        turn(){}
