#include "arg_wrapper_classes.h"
#include <sstream>
#include "interfaces.h"

Do_Move_Args::Do_Move_Args(){
        contents.first = Board::MT_Board();
}

std::string Do_Move_Args::serialize() const{
        std::stringstream ss;

        ss << "<do-move> ";
        ss << contents.first.serialize() << " ";
        ss << ::serialize(contents.second);
        ss << " </do-move>";
        return ss.str();
}

std::string Start_Game_Args::serialize() const{
        std::stringstream ss;

        ss << "<start-game> ";
        ss << color;
        ss << " </start-game>";

        return ss.str();
}

std::string Doubles_Penalty_Args::serialize() const{
        return "<doubles-penalty> </doubles-penalty>";
}
