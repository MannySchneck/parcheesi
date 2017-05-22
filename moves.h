#pragma once

#include "interfaces.h"
#include "board.h"
#include "rule_checker.h"
#include <sstream>

class Rules_Checker;

class IMove :
public Serializable{
 public:
        virtual Status inspect(Rules_Checker &rc, Board board) = 0;
        virtual Status do_move(Board &board) = 0;

        virtual std::vector<int> get_cost() const = 0;

        virtual bool operator==(const  std::shared_ptr<IMove> rhs)= 0;
        virtual bool operator!=(const  std::shared_ptr<IMove> rhs)= 0;
 protected:
        IMove();
};

using mv_ptr = std::shared_ptr<IMove>;

// represents a move where a player enters a piece
class EnterPiece : public IMove{
 public:
        EnterPiece(Pawn pawn);

        EnterPiece(Pawn pawn, std::vector<int> cost);

        Status inspect(Rules_Checker &rc, Board board) override;

        Status do_move(Board &board) override;

        Pawn get_pawn();

        std::vector<int> get_cost() const override;

        virtual bool operator==(const  std::shared_ptr<IMove> rhs)override;
        virtual bool operator!=(const  std::shared_ptr<IMove> rhs)override;

        bool operator==(const EnterPiece& rhs);

        std::string serialize() const override;
 private:
        Pawn pawn;
        std::vector<int> cost;
        //Status update_board(Board &b) override;
};

class Move : public IMove{
 public:
        Move(int start, int distance, Pawn pawn);
        Status do_move(Board &board) override;
        Pawn get_pawn();
        int get_start() const;
        int get_distance() const;

        std::vector<int> get_cost() const override;

        bool operator==(const Move& rhs) ;

        friend std::ostream& operator<<(std::ostream &out, const Move &mv);


        std::string serialize() const override;
 protected:
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

        virtual bool operator==(const  std::shared_ptr<IMove> rhs) override;
        virtual bool operator!=(const  std::shared_ptr<IMove> rhs) override;
        bool operator==(const MoveMain& rhs);


        std::string serialize() const override;
};

// represents a move that starts on one of the home rows
class MoveHome : public Move{
 public:
        MoveHome(int start, int distance, Pawn p);

        Status inspect(Rules_Checker &rc, Board board) override;

        virtual bool operator==(const  std::shared_ptr<IMove> rhs) override;
        virtual bool operator!=(const  std::shared_ptr<IMove> rhs) override;

        bool operator==(const MoveHome& rhs) ;

        std::string serialize() const override;
};

std::string serialize_mv_ptr(const mv_ptr &mv);

