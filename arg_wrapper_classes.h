#include <utility>
#include <memory>
#include "interfaces.h"
#include "board.h"

class Do_Move_Args :
        public Serializable{
public:
        std::string serialize() const override;
        std::pair<Board, fuel> contents;
};

class Start_Game_Args :
        public Serializable {
public:
        std::string serialize() const override;
        std::string color;
};

class Doubles_Penalty_Args :
        public Serializable{
public:
        std::string serialize() const override;
};
