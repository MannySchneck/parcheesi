#include "interfaces.h"
#include <sstream>

std::string serialize(fuel fuel){
        std::stringstream ss;
        ss << "<dice> ";
        for(auto gallon : fuel){
                ss << "<die>";
                ss << gallon;
                ss << "</die>";
        }
        ss << "</dice>";
        return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Pawn& p)
{
        os << "(pawn";
        os << p.id;
        os << ")";
        return os;
}

std::ostream& operator<<(std::ostream& os, const Color& color){
        switch(color) {

        case(Color::red) :
                os << "red";
                break;
        case(Color::green) :
                os << "green";
                break;
        case(Color::yellow) :
                os << "yellow";
                break;
        case(Color::blue) :
                os << "blue";
                break;
        default:
                std::logic_error("Added a color and forgot to extend this operator. SAD!");
        }
        return os;
}

std::string Pawn::serialize() const{
        std::stringstream ss;

        ss << "<pawn> ";
        ss << "<color> ";
        ss << color;
        ss << " </color> ";

        ss << "<id> ";
        ss << id;
        ss << " </id> ";
        ss << "</pawn> ";

        return ss.str();
}


const std::vector<Color> Game_Consts::colors  = {Color::red, Color::yellow, Color::green, Color::blue};

const std::unordered_map<std::string, Color>  Game_Consts::string2color =
        {
                {"red", Color::red},
                {"yellow", Color::yellow},
                {"green", Color::green},
                {"blue", Color::blue}
        };
