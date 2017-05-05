#include "arg_wrapper_classes.h"

std::string Do_Move_Args::serialize() const{
        return std::string{"ACK!"};
}

std::string Start_Game_Args::serialize() const{
        return std::string{"ACK!"};
}

std::string Doubles_Penalty_Args::serialize() const{
        return std::string{"ACK!"};
}
