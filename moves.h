#pragma once

#include "interfaces.h"
#include "board.h"
#include "rule_checker.h"

class Rules_Checker;

class IMove {
public:
        //virtual Status update_board(Board &b) = 0;
        virtual Status validate(Rules_Checker &rc, Board board) = 0;
protected:
        IMove();
};

using mv_ptr = std::shared_ptr<IMove>;


// represents a move where a player enters a piece
class EnterPiece : public IMove{
public:
        EnterPiece(Pawn pawn);

        Status validate(Rules_Checker &rc, Board board) override;
private:
        Pawn pawn;
        //Status update_board(Board &b) override;
};

// represents a move that starts on the main ring
// (but does not have to end up there)
class MoveMain : public IMove{

 public:
        MoveMain(int start, int distance, Pawn pawn);

        Status validate(Rules_Checker &rc, Board board) override;
        //Status update_board(Board &b) override;
        Pawn pawn;
        int start;
        int distance;

};

// represents a move that starts on one of the home rows
class MoveHome : public IMove{
public:
        MoveHome(Pawn pawn, int start, int distance);

        Status validate(Rules_Checker &rc, Board board) override;

        //Status update_board(Board &b) override;
private:
        Pawn pawn;
        int start;
        int distance;

};
