#include "interfaces.h"
#include "moves.h"
#include <typeinfo>
#include <sstream>

Status Move::do_move(Board &board){
        return board.move_pawn(start, distance, pawn);
}

std::vector<int> Move::get_cost() const{
        return {distance};
}

std::vector<int> EnterPiece::get_cost() const{
        return cost;
}

std::string EnterPiece::serialize() const{
        std::stringstream ss;

        ss << "<enter-piece> ";
        ss << pawn.serialize();
        ss << "</enter-piece>";

        return ss.str();
}

std::string Move::serialize() const{
        std::stringstream ss;

        ss << pawn.serialize();
        ss << "<start> " << Game_Consts::us2robby(get_start()) << " </start> ";
        ss << "<distance> " << get_distance() << " </distance> ";

        return ss.str();
}

std::string MoveMain::serialize() const{
        std::stringstream ss;

        ss << "<move-piece-main> ";
        ss << Move::serialize();
        ss << "</move-piece-main>";

        return ss.str();
}

std::string MoveHome::serialize() const{
        std::stringstream ss;

        ss << "<move-piece-home> ";
        ss << pawn.serialize();
        ss << "<start> " << (get_start() - 1) << " </start> ";
        ss << "<distance> " << get_distance() << " </distance> ";
        ss << "</move-piece-home>";

        return ss.str();
}

Status MoveMain::inspect(Rules_Checker &rc, Board board){
        if(!rc.validate_main_move(this, board)){
                return Status::cheated;
        }

        if(rc.did_bop(this, board)){
                return Status::bop_bonus;
        }

        if(rc.moved_home(this, board)){
                return Status::home_bonus;
        }

        return Status::normal;
}

bool Move::operator==(const Move& rhs) {
        return start == rhs.start && distance == rhs.distance && pawn == rhs.pawn;
}

std::ostream& operator<<(std::ostream &out, const Move &mv){
        out << "(move ";
        out << mv.start << " ";
        out << mv.distance << " ";
        out << mv.pawn;
        out << ")";
        return out;
}

bool MoveMain::operator==(const MoveMain& rhs) {
        return Move::operator==(rhs);
}

bool MoveMain::operator==(const  std::shared_ptr<IMove> rhs) {
        const MoveMain* m;
        if((m = dynamic_cast<const MoveMain*>(rhs.get()))){
                return *this == *m;
        }
        return false;
}
bool MoveMain::operator!=(const  std::shared_ptr<IMove> rhs) {
        return !(*this == rhs);
}

bool MoveHome::operator==(const MoveHome& rhs) {
        return Move::operator==(rhs);
}

bool MoveHome::operator==(const  std::shared_ptr<IMove> rhs) {
        const MoveHome* m;
        if((m = dynamic_cast<const MoveHome*>(rhs.get()))){
                return *this == *m;
        }
        return false;
}
bool MoveHome::operator!=(const  std::shared_ptr<IMove> rhs) {
        return !(*this == rhs);
}



Status EnterPiece::do_move(Board &board){
       return board.enter_pawn(pawn);
}

bool EnterPiece::operator==(const  std::shared_ptr<IMove> rhs) {
        const EnterPiece* m;
        if((m = dynamic_cast<const EnterPiece*>(rhs.get()))){
                return *this == *m;
        }
        return false;
}
bool EnterPiece::operator!=(const  std::shared_ptr<IMove> rhs) {
        return !(*this == rhs);
}

bool EnterPiece::operator==(const EnterPiece& rhs) {
        return pawn == rhs.pawn;
}

Status EnterPiece::inspect(Rules_Checker &rc, Board board){

        if(!rc.validate_enter_move(this, board)){
                return Status::cheated;
        }

        if(rc.did_bop(this, board)){
                return Status::bop_bonus;
        }

        return Status::normal;
}

Pawn EnterPiece::get_pawn(){
        return pawn;
}

Status MoveHome::inspect(Rules_Checker &rc, Board board){
        if(!rc.validate_home_move(this, board)){
                return Status::cheated;
        }

        if(rc.moved_home(this, board)) return Status::home_bonus;

        return Status::normal;

}


IMove::IMove() = default;

EnterPiece::EnterPiece(Pawn pawn):
        pawn(pawn),
        cost({5 })
{}

EnterPiece::EnterPiece(Pawn pawn, std::vector<int> cost) :
        pawn(pawn),
        cost(cost){}

Move::Move(int start, int distance, Pawn pawn)
        : pawn(pawn),
          start(start),
          distance(distance){}

Pawn Move::get_pawn(){
        return pawn;
}

int Move::get_start() const{
        return start;
}

int Move::get_distance() const{
        return distance;
}


MoveMain::MoveMain(int start, int distance, Pawn pawn)
        : Move(start,distance,pawn){}

MoveHome::MoveHome(int start, int distance, Pawn pawn)
        : Move(start,distance,pawn){}
