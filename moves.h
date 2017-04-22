#pragma once

#include "interfaces.h"
#include "board.h"
#include "rule_checker.h"

class Rules_Checker;

class IMove {
public:
        //virtual Status update_board(Board &b) = 0;
        virtual Status inspect(Rules_Checker &rc, Board board) = 0;
        virtual Status do_move(Board &board) = 0;
protected:
        IMove();
};

using mv_ptr = std::shared_ptr<IMove>;


// represents a move where a player enters a piece
class EnterPiece : public IMove{
public:
        EnterPiece(Pawn pawn);

        Status inspect(Rules_Checker &rc, Board board) override;

        Status do_move(Board &board) override;

        Pawn get_pawn();
private:
        Pawn pawn;
        //Status update_board(Board &b) override;
};

class Move : public IMove{
public:
        Move(int start, int distance, Pawn pawn);
        Status do_move(Board &board) override;
        Pawn get_pawn();
        int get_start();
        int get_distance();
private:
        Pawn pawn;
        int start;
        int distance;
};

// represents a move that starts on the main ring
// (but does not have to end up there)
class MoveMain : public Move{

public:
        MoveMain(int start, int distance, Pawn pawn);

        Status inspect(Rules_Checker &rc, Board board) override;
        //Status update_board(Board &b) override;
};

// represents a move that starts on one of the home rows
class MoveHome : public Move{
public:
  MoveHome(int start, int distance, Pawn p);

        Status inspect(Rules_Checker &rc, Board board) override;

        //Status update_board(Board &b) override;
};
