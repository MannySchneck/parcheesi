#include <utility>
#include <memory>
#include "interfaces.h"
#include "board.h"
#include "moves.h"

class Do_Move_Args :
        public Serializable{
public:
        Do_Move_Args();
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

class Network_Void :
        public Serializable {
public:
        std::string serialize() const override;
};

class Name :
        public Serializable {
public:
        Name();
        Name(std::string name);

        std::string serialize() const override;

        std::string name;
};

class Moves :
        public Serializable{
public:
        std::string serialize() const override;
        std::vector<mv_ptr> _moves;
};
