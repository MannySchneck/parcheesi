#pragma once

#include <vector>
#include <memory>
#include <string>
#include <utility>


using fuel = std::vector<int>;

class Board;
class IMove;

enum class Status {
        normal,
        bop_bonus,
        home_bonus,
        cheated
};

enum class Color {
        red,
                green,
                blue,
                yellow
                };

struct enum_hash
{
        template <typename T>
        inline
        typename std::enable_if<std::is_enum<T>::value, std::size_t>::type
                operator ()(T const value) const
        {
                return static_cast<std::size_t>(value);
        }
};


class IPlayer {
public:
        // inform the player that a game has started
        // and what color the player is.
        virtual void startGame(Color color) = 0;

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
        Pawn (int id, Color color) {
                this->id=id;
                this->color=color;
        }

        Pawn (const Pawn &rhs) = default;

        bool operator==(const Pawn &rhs) const{
                return id == rhs.id &&
                        color == rhs.color;
        }

        bool operator!=(const Pawn &rhs) const{
                return !(id == rhs.id &&
                        color == rhs.color);
        }

        int /* 0-3 */ id;
        Color color;

};

inline int modulo(int a, int b){
        const int result = a % b;
        return result >= 0 ? result : result + b;
}