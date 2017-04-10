#include <vector>

class Player {
        // inform the player that a game has started
        // and what color the player is.

        virtual void startGame(std::string color) = 0;

        // ask the player what move they want to make
        virtual Move doMove(Board brd, std::vector<int> dice) = 0;

        // inform the player that ehy have suffered a doubles penalty
        virtual void DoublesPenalty() = 0;
}

class Game {

        // add a player to the game
        virtual void register(Player p) = 0;

        // start a game
        virtual void start() = 0;
}

class Move {
 protected:
        Move();
}

// represents a move where a player enters a piece
class EnterPiece implements Move {
        Pawn pawn;
        EnterPiece(Pawn pawn) {
                this->pawn=pawn;
        }
}

// represents a move that starts on the main ring
// (but does not have to end up there)
class MoveMain implements Move {
        Pawn pawn;
        int start;
        int distance;
        MoveMain(Pawn pawn, int start, int distance) {
                this->pawn=pawn;
                this->start=start;
                this->distance=distance;
        }
}

// represents a move that starts on one of the home rows
class MoveHome implements Move {
        Pawn pawn;
        int start;
        int distance;
        MoveHome(Pawn pawn, int start, int distance) {
                this->pawn=pawn;
                this->start=start;
                this->distance=distance;
        }
}

class Pawn {
        int /* 0-3 */ id;
        String color;
        Pawn (int id, String color) {
                this->id=id;
                this->color=color;
        }
}
