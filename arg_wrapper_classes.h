#include <utility>
#include <memory>
#include "interfaces.h"
#include "board.h"

class Do_Move_Args :
        public Serializable,
        public std::pair<Board, fuel>{
public:
        std::string serialize() const override;
};

class Start_Game_Args :
        public Serializable,
        public std::string{
        std::string serialize() const override;
};

class Doubles_Penalty_Args :
        public Serializable{
        std::string serialize() const override;
};
