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

std::string Pawn::serialize() const{
        return "ACK!";
}
