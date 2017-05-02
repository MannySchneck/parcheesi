#include "interfaces.h"

std::ostream& operator<<(std::ostream& os, const Pawn& p)
{
        os << "(pawn";
        os << p.id;
        os << ")";
        return os;
}
