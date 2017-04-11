#pragma once

#include <vector>
#include <memory>
#include <string>
#include <utility>

class Board;

enum class Status {
        normal,
        bop_bonus,
        home_bonus,
        cheated
};


class IMove {
public:
        virtual Status update_board(Board &b) = 0;
protected:
        IMove();
};

class IPlayer {
public:
        // inform the player that a game has started
        // and what color the player is.
        virtual void startGame(std::string color) = 0;

        // ask the player what move they want to make
        virtual std::vector<std::shared_ptr<IMove>> doMove(Board brd, std::pair<int, int> dice) = 0;

        // inform the player that they have suffered a doubles penalty
        virtual void DoublesPenalty() = 0;
};


class IGame {
        // add a player to the game
        virtual void register_player(std::shared_ptr<IPlayer> p) = 0;

        // start a game

        virtual void start() = 0;
};

class Pawn {
public:
        Pawn (int id, std::string color) {
                this->id=id;
                this->color=color;
        }

        Pawn (const Pawn &rhs) = default;

        bool operator==(const Pawn &rhs){
                return id == rhs.id &&
                        color == rhs.color;
        }

        int /* 0-3 */ id;
        std::string color;

};

inline int modulo(int a, int b){
        const int result = a % b;
        return result >= 0 ? result : result + b;
}
