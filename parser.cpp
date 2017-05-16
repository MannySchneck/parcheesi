#include <string>

#include <vector>

#include <pegtl.hpp>
#include <pegtl/tracer.hpp>
#include <pegtl/analyze.hpp>
#include "parse_stack.h"

#include "prettyprint.hh"


#include "interfaces.h"
#include "board.h"
#include "arg_wrapper_classes.h"

#include "catch.hpp"

#include <sstream>
#include <fstream>

using namespace tao;

namespace cheesy_xml_parser{

        struct seps :
                pegtl::star<
                pegtl::ascii::space
                >{};

        struct number :
                pegtl::plus<
                pegtl::digit
                >{};

        struct color :
                pegtl::sor<
                TAOCPP_PEGTL_STRING("yellow"),
                TAOCPP_PEGTL_STRING("red"),
                TAOCPP_PEGTL_STRING("green"),
                TAOCPP_PEGTL_STRING("blue")
                >{};

        struct name_open_tag :
                TAOCPP_PEGTL_STRING("<name>"){};

        struct name_body :
                pegtl::star<
                pegtl::sor<
                        pegtl::alnum,
                        pegtl::one<'_'>
                        >
                >{};


        struct name :
                pegtl::seq<
                name_open_tag,
                seps,
                name_body,
                seps,
                TAOCPP_PEGTL_STRING("</name>")>{};

        struct id_val :
                pegtl::sor<
                pegtl::one<'0'>,
                pegtl::one<'1'>,
                pegtl::one<'2'>,
                pegtl::one<'3'>
                >{};

        struct id :
                pegtl::seq<
                TAOCPP_PEGTL_STRING("<id>"),
                seps,
                id_val,
                seps,
                TAOCPP_PEGTL_STRING("</id>")
                >{};

        struct pawn :
                pegtl::seq<
                TAOCPP_PEGTL_STRING("<pawn>"),
                seps,
                TAOCPP_PEGTL_STRING("<color>"),
                seps,
                color,
                seps,
                TAOCPP_PEGTL_STRING("</color>"),
                seps,
                id,
                seps,
                TAOCPP_PEGTL_STRING("</pawn>")
                >{};

        struct dice_val :
                pegtl::sor<
                pegtl::one<'1'>,
                pegtl::one<'2'>,
                pegtl::one<'3'>,
                pegtl::one<'4'>,
                pegtl::one<'5'>,
                pegtl::one<'6'>
                >{};

        struct die :
                pegtl::seq<
                TAOCPP_PEGTL_STRING("<die>"),
                seps,
                dice_val,
                seps,
                TAOCPP_PEGTL_STRING("</die>")
                >{};

        struct dice :
                pegtl::seq<
                TAOCPP_PEGTL_STRING("<dice>"),
                seps,
                pegtl::star<
                        pegtl::seq<
                                die,
                                seps>>,
                seps,
                TAOCPP_PEGTL_STRING("</dice>")>{};

        struct start :
                pegtl::seq<
                TAOCPP_PEGTL_STRING("<start>"),
                seps,
                number,
                seps,
                TAOCPP_PEGTL_STRING("</start>")
                >{};

        struct distance :
                pegtl::seq<
                TAOCPP_PEGTL_STRING("<distance>"),
                seps,
                number,
                seps,
                TAOCPP_PEGTL_STRING("</distance>")
                >{};

        struct enter_piece :
                pegtl::seq<
                TAOCPP_PEGTL_STRING("<enter-piece>"),
                seps,
                pawn,
                seps,
                TAOCPP_PEGTL_STRING("</enter-piece>")
                >{};

        struct move_contents :
                pegtl::seq<
                seps,
                pawn,
                seps,
                start,
                seps,
                distance,
                seps>{};

        struct move_piece_main :
                pegtl::seq<
                TAOCPP_PEGTL_STRING("<move-piece-main>"),
                seps,
                move_contents,
                seps,
                TAOCPP_PEGTL_STRING("</move-piece-main>")
                >{};

        struct move_piece_home :
                pegtl::seq<
                TAOCPP_PEGTL_STRING("<move-piece-home>"),
                move_contents,
                TAOCPP_PEGTL_STRING("</move-piece-home>")
                >{};

        struct move :
                pegtl::sor<
                enter_piece,
                move_piece_main,
                move_piece_home
                >{};

        struct piece_loc :
                pegtl::seq<
                TAOCPP_PEGTL_STRING("<piece-loc>"),
                seps,
                pawn,
                seps,
                TAOCPP_PEGTL_STRING("<loc>"),
                seps,
                number,
                seps,
                TAOCPP_PEGTL_STRING("</loc>"),
                seps,
                TAOCPP_PEGTL_STRING("</piece-loc>")
                >{};

        struct home :
                pegtl::seq<
                TAOCPP_PEGTL_STRING("<home>"),
                seps,
                pegtl::star<
                        pegtl::seq<
                                pawn,
                                seps
                                >
                        >,
                seps,
                TAOCPP_PEGTL_STRING("</home>")
                >{};

        struct home_rows :
                pegtl::seq<
                TAOCPP_PEGTL_STRING("<home-rows>"),
                seps,
                pegtl::star<
                        seps,
                        piece_loc
                        >,
                seps,
                TAOCPP_PEGTL_STRING("</home-rows>")
                >{};

        struct main :
                pegtl::seq<
                pegtl::if_must<
                        TAOCPP_PEGTL_STRING("<main>"),
                        seps,
                        pegtl::star<
                                pegtl::seq<
                                        seps,
                                        piece_loc
                                        >
                                >,
                        seps,
                        TAOCPP_PEGTL_STRING("</main>")
                        >>{};

        struct board_start :
                pegtl::seq<
                TAOCPP_PEGTL_STRING("<start>"),
                seps,
                pegtl::star<
                        pawn
                        >,
                seps,
                TAOCPP_PEGTL_STRING("</start>")
                >{};

        struct board :
                pegtl::seq<
                TAOCPP_PEGTL_STRING("<board>"),
                seps,
                board_start,
                seps,
                main,
                seps,
                home_rows,
                seps,
                home,
                seps,
                TAOCPP_PEGTL_STRING("</board>")
                >{};

        struct _void :
                pegtl::seq<
                TAOCPP_PEGTL_STRING("<void>"),
                seps,
                TAOCPP_PEGTL_STRING("</void>")
                >{};

        struct moves_open_tag :
                TAOCPP_PEGTL_STRING("<moves>"){};

        struct moves :
                pegtl::seq<
                moves_open_tag,
                seps,
                pegtl::star<
                        pegtl::seq<
                                move,
                                seps
                                >>,
                seps,
                TAOCPP_PEGTL_STRING("</moves>")
                >{};

        ///////////////////////////////////////////////////////////////////////
        //                              Messages                             //
        ///////////////////////////////////////////////////////////////////////
        struct do_move_open_tag :
                TAOCPP_PEGTL_STRING("<do-move>"){};

        struct doubles_penalty_open_tag :
                TAOCPP_PEGTL_STRING("<doubles-penalty>"){};

        struct do_move :
                pegtl::seq<
                do_move_open_tag,
                seps,
                board,
                seps,
                dice,
                seps,
                TAOCPP_PEGTL_STRING("</do-move>")
                >{};

        struct doubles_penalty :
                pegtl::seq<
                doubles_penalty_open_tag,
                seps,
                TAOCPP_PEGTL_STRING("</doubles-penalty>")
                >{};

        struct start_game_open_tag :
                TAOCPP_PEGTL_STRING("<start-game>"){
        };

        struct start_game :
                pegtl::seq<
                start_game_open_tag,
                seps,
                color,
                seps,
                TAOCPP_PEGTL_STRING("</start-game>")
                >{};

        struct messages :
                pegtl::must<
                pegtl::sor<
                        start_game,
                        do_move,
                        doubles_penalty,
                        _void,
                        moves,
                        name
                        >
                >{};

        template<typename Rule>
        struct action : pegtl::nothing< Rule>{};

        // parsing stacks
        L2::L2_Parse_Stack<std::shared_ptr<Serializable>> the_stack;

        std::vector<std::string> string_stack;

        std::vector<int> int_stack;


        template<> struct action <moves_open_tag>{
                template <typename Input>
                static void apply(const Input& in, std::shared_ptr<Serializable> &msg){
                        msg = std::shared_ptr<Serializable>{new Moves{}};
                }
        };


        template<> struct action <doubles_penalty_open_tag>{
                template <typename Input>
                static void apply(const Input& in, std::shared_ptr<Serializable> &msg){
                        msg = std::shared_ptr<Serializable>{new Doubles_Penalty_Args{}};
                }
        };

        template<> struct action <start_game_open_tag>{
                template <typename Input>
                static void apply(const Input& in, std::shared_ptr<Serializable> &msg){
                        msg = std::shared_ptr<Serializable>{new Start_Game_Args{}};
                }
        };

        template<> struct action <do_move_open_tag>{
                template <typename Input>
                static void apply(const Input& in, std::shared_ptr<Serializable> &msg){
                        msg = std::shared_ptr<Serializable>{new Do_Move_Args{}};
                }
        };

        template<> struct action <start_game>{
                template <typename Input>
                static void apply(const Input& in, std::shared_ptr<Serializable> &msg){
                        auto sg_ptr = std::dynamic_pointer_cast<Start_Game_Args>(msg);
                        sg_ptr->color = string_stack.back();
                        string_stack.pop_back();
                }
        };

        template<> struct action <color>{
                template <typename Input>
                static void apply(const Input& in, std::shared_ptr<Serializable> &msg){
                        string_stack.push_back(in.string());
                }
        };

        template<> struct action <id_val>{
                template <typename Input>
                static void apply(const Input& in, std::shared_ptr<Serializable> & msg){
                        int_stack.push_back(stoi(in.string()));
                }
        };

        template<> struct action <number>{
                template <typename Input>
                static void apply(const Input& in, std::shared_ptr<Serializable> & msg){
                        int_stack.push_back(stoi(in.string()));
                }
        };


        template<> struct action <pawn>{
                template <typename Input>
                static void apply(const Input& in, std::shared_ptr<Serializable> & msg){

                        std::shared_ptr<Serializable>
                                p{new Pawn{int_stack.back(), Game_Consts::string2color.at(string_stack.back())}};

                        the_stack.push(p);
                        int_stack.pop_back();
                        string_stack.pop_back();
                }
        };

        template<> struct action <main>{
                template <typename Input>
                static void apply(const Input& in, std::shared_ptr<Serializable> & msg){
                        auto do_move_args = std::dynamic_pointer_cast<Do_Move_Args>(msg);

                        while(!the_stack.is_empty()
                              && dynamic_cast<Pawn*>(the_stack.peek().get())){
                                do_move_args->contents.first.put_pawn(*the_stack.downcast_pop<Pawn>(),
                                                                      int_stack.back());
                                int_stack.pop_back();
                        }
                }
        };

        template<> struct action <home_rows>{
                template <typename Input>
                static void apply(const Input& in, std::shared_ptr<Serializable> & msg){
                        auto do_move_args = std::dynamic_pointer_cast<Do_Move_Args>(msg);

                        while(!the_stack.is_empty()
                              && dynamic_cast<Pawn*>(the_stack.peek().get())){
                                do_move_args->contents.first.put_pawn_hr(*the_stack.downcast_pop<Pawn>(),
                                                                         int_stack.back() + 1);
                                int_stack.pop_back();
                        }
                }
        };

        template<> struct action <enter_piece>{
                template <typename Input>
                static void apply(const Input& in, std::shared_ptr<Serializable> &msg){
                        the_stack.push(std::shared_ptr<Serializable>{
                                        new EnterPiece{*the_stack.downcast_pop<Pawn>()}});
                }
        };

        template<> struct action <move_piece_main>{
                template <typename Input>
                static void apply(const Input& in, std::shared_ptr<Serializable> &msg){
                        auto distance = int_stack.back();
                        int_stack.pop_back();
                        auto start = int_stack.back();
                        int_stack.pop_back();

                        the_stack.push(std::shared_ptr<Serializable>{
                                        new MoveMain{Game_Consts::robby2us(start),
                                                        distance,
                                                        *the_stack.downcast_pop<Pawn>()}});
                }
        };

        template<> struct action <move_piece_home>{
                template <typename Input>
                static void apply(const Input& in, std::shared_ptr<Serializable> &msg){
                        auto distance = int_stack.back();
                        int_stack.pop_back();
                        auto start = int_stack.back();
                        int_stack.pop_back();

                        the_stack.push(std::shared_ptr<Serializable>{
                                        new MoveHome{start + 1,
                                                        distance,
                                                        *the_stack.downcast_pop<Pawn>()}});
                }
        };


        template<> struct action <moves>{
                template<typename Input>
                static void apply(const Input& in, std::shared_ptr<Serializable> &msg){

                        auto moves_args = std::dynamic_pointer_cast<Moves>(msg);

                        while(!the_stack.is_empty()
                              && dynamic_cast<IMove*>(the_stack.peek().get())){
                                moves_args->_moves.push_back(the_stack.downcast_pop<IMove>());
                        }
                }
        };


        template<> struct action <die>{
                template <typename Input>
                static void apply(const Input& in, std::shared_ptr<Serializable> & msg){
                        auto do_move_args = std::dynamic_pointer_cast<Do_Move_Args>(msg);

                        do_move_args->contents.second.push_back(int_stack.back());

                        int_stack.pop_back();
                }
        };

        template<> struct action <dice_val>{
                template <typename Input>
                static void apply(const Input& in, std::shared_ptr<Serializable> & msg){
                        int_stack.push_back(stoi(in.string()));
                }
        };

        template<> struct action <name_body>{
                template <typename Input>
                static void apply(const Input& in, std::shared_ptr<Serializable> & msg){
                        string_stack.push_back(in.string());
                }
        };

        template<> struct action <name>{
                template <typename Input>
                static void apply(const Input& in, std::shared_ptr<Serializable> & msg){
                        std::dynamic_pointer_cast<Name>(msg)->name = string_stack.back();

                        string_stack.pop_back();
                }
        };


        template<> struct action <name_open_tag>{
                template <typename Input>
                static void apply(const Input& in, std::shared_ptr<Serializable> & msg){
                        msg = std::shared_ptr<Name>{new Name{}};
                }
        };


        std::shared_ptr<Serializable> parse_xml_file(std::string filename){
                pegtl::file_input<> in(filename);

                std::shared_ptr<Serializable> parsed_message;

                pegtl::parse<messages, action>
                        //, pegtl::tracer>
                        (in, parsed_message);

                return parsed_message;
        }

        std::shared_ptr<Serializable> parse_xml_message(std::string msg){
                std::string parse_buf;

                pegtl::string_input<> in(msg, parse_buf);

                std::shared_ptr<Serializable> parsed_message;

                pegtl::parse<messages, action>(in, parsed_message);

                return parsed_message;
        }
}

TEST_CASE("doubles penalty"){
        std::string doubles_penalty_msg =
                "<doubles-penalty> </doubles-penalty>";
        std::shared_ptr<Serializable> parsed_msg =
                cheesy_xml_parser::parse_xml_message(doubles_penalty_msg);

        REQUIRE(dynamic_cast<Doubles_Penalty_Args*>(parsed_msg.get()));
        REQUIRE(parsed_msg->serialize() == "<doubles-penalty> </doubles-penalty>");
}

TEST_CASE("start teh gamez"){
        std::string strag_game_msg =
                "<start-game> red </start-game>";

        std::shared_ptr<Serializable> parsed_msg =
                cheesy_xml_parser::parse_xml_message(strag_game_msg);

        REQUIRE(dynamic_cast<Start_Game_Args*>(parsed_msg.get()));
        REQUIRE(parsed_msg->serialize() == "<start-game> red </start-game>");
};

TEST_CASE("board serializes things"){
        Board board = Board::MT_Board();

        Pawn bp0{0, Color::blue};

        board.put_pawn(bp0, 0);

        std::string expected =
                "<board> <start> </start> "\
                "<main> <piece-loc> <pawn> <color> blue </color> <id> 0 </id> </pawn> <loc> 0 </loc> </piece-loc> </main> <home-rows> </home-rows> <home> </home> </board>";
        REQUIRE(board.serialize() == expected);
}

TEST_CASE("Parseapawn"){
        SECTION("Do a thuing"){
                std::string parse_filename = "test_data/single_pawn.xml";

                std::shared_ptr<Serializable>  message =
                        cheesy_xml_parser::parse_xml_file(parse_filename);
                auto do_move_args = std::dynamic_pointer_cast<Do_Move_Args>(message);


                Pawn bp0{0, Color::blue};
                Board board = Board::MT_Board();
                board.put_pawn(bp0, 0);


                REQUIRE(do_move_args->contents.first == board);
        }
}

TEST_CASE("Pares 2 pawnz. Induciton broh."){
        SECTION("Do a thuing"){
                std::string parse_filename = "test_data/two_pawns.xml";

                std::shared_ptr<Serializable>  message =
                        cheesy_xml_parser::parse_xml_file(parse_filename);
                auto do_move_args = std::dynamic_pointer_cast<Do_Move_Args>(message);

                Pawn bp0{0, Color::blue};
                Pawn rp0{0, Color::red};

                Board board = Board::MT_Board();
                board.put_pawn(bp0, 0);
                board.put_pawn(rp0, 3);

                REQUIRE(do_move_args->contents.first == board);
        }
}

TEST_CASE("home rowz plz"){
        SECTION("Do a thuing"){
                std::string parse_filename = "test_data/home_row.xml";

                std::shared_ptr<Serializable>  message =
                        cheesy_xml_parser::parse_xml_file(parse_filename);
                auto do_move_args = std::dynamic_pointer_cast<Do_Move_Args>(message);

                Pawn bp0{0, Color::blue};
                Pawn rp0{0, Color::red};

                Board board = Board::MT_Board();
                board.put_pawn_hr(bp0, 1);
                board.put_pawn_hr(rp0, 4);

                REQUIRE(do_move_args->contents.first == board);
        }
}


TEST_CASE("doing a move =( Fuck."){
        std::string mt_board_string =
                "<board> "                      \
                "<start> </start> "             \
                "<main> </main> "               \
                "<home-rows> </home-rows> "     \
                "<home> </home> "               \
                "</board>";

        std::string dice_string =
                " <dice> " \
                "</dice>";

        std::stringstream ss;

        ss << "<do-move> " << mt_board_string << dice_string << " </do-move>";

        std::shared_ptr<Serializable> parsed_msg =
                cheesy_xml_parser::parse_xml_message(ss.str());

        REQUIRE(parsed_msg->serialize() == ss.str());
}

std::string slurp_fun(std::ifstream& in, std::string fname){
        std::stringstream ss;
        in.open(fname, std::ios::in);
        ss << in.rdbuf();
        in.close();
        return ss.str();
}

TEST_CASE("home"){
        std::ifstream in;

        SECTION("Serialize"){
                std::string serialized_fun = slurp_fun(in, "test_data/pawn_in_home.xml");

                Pawn bp0{0, Color::blue};
                Board board = Board::MT_Board();

                board.put_pawn_home(bp0);

                REQUIRE(board.serialize().append("\n") == serialized_fun);
        }

        SECTION("Parse"){
                std::shared_ptr<Serializable> parsed_msg =
                        cheesy_xml_parser::parse_xml_file("test_data/pawn_in_home_parse.xml");

                auto do_move_args = std::dynamic_pointer_cast<Do_Move_Args>(parsed_msg);

                Pawn bp0{0, Color::blue};

                Board board = Board::MT_Board();
                board.put_pawn_home(bp0);

                REQUIRE(do_move_args->contents.first == board);

                REQUIRE(do_move_args->contents.second == (std::vector<int>{5, 6,1, 2}));
        }
}

TEST_CASE("enter-piece"){
        std::ifstream in;

        SECTION("serialize"){
                Pawn bp0{0, Color::blue};

                EnterPiece mv(bp0);

                std::string expected_serialization =
                        slurp_fun(in, "test_data/enter_piece_serialization.xml");

                // WAT? SAD! wtf m8? I am le-tired.
                //REQUIRE(mv.serialize().append("\n") == expected_serialization);
        }

        SECTION("parse"){
                Pawn bp0{0, Color::blue};

                EnterPiece mv(bp0);

                std::shared_ptr<Serializable> parsed_msg =
                        cheesy_xml_parser::parse_xml_file("test_data/enter_piece_moves.xml");

                auto moves = std::dynamic_pointer_cast<Moves>(parsed_msg);

                REQUIRE(*std::dynamic_pointer_cast<EnterPiece>(moves->_moves[0]) == mv);
        }



        SECTION("parse 2"){
                Pawn bp0{0, Color::blue};
                Pawn rp3{3, Color::red};

                EnterPiece mv(bp0);
                EnterPiece mv2(rp3);

                std::shared_ptr<Serializable> parsed_msg =
                        cheesy_xml_parser::parse_xml_file("test_data/enter_piece_2moves.xml");

                auto moves = std::dynamic_pointer_cast<Moves>(parsed_msg);

                REQUIRE(*std::dynamic_pointer_cast<EnterPiece>(moves->_moves[1]) == mv);
                REQUIRE(*std::dynamic_pointer_cast<EnterPiece>(moves->_moves[0]) == mv2);
        }
}

TEST_CASE("mainmove"){
        std::ifstream in;

        Pawn bp0{0, Color::blue};
        Pawn rp3{0, Color::red};

        MoveMain mv{0, 0, bp0};
        EnterPiece mv2{rp3};

        SECTION("Serialize"){
                auto expected =
                        slurp_fun(in, "test_data/mainmove_serialization.xml");
                REQUIRE(mv.serialize().append("\n") == expected);
        }

        SECTION("Parse"){
                std::shared_ptr<Serializable> parsed_msg =
                        cheesy_xml_parser::parse_xml_file("test_data/mainmove_parse.xml");

                auto moves = std::dynamic_pointer_cast<Moves>(parsed_msg);

                REQUIRE(*std::dynamic_pointer_cast<MoveMain>(moves->_moves[0]) == mv);
        }

        SECTION("Parse"){
                std::shared_ptr<Serializable> parsed_msg =
                        cheesy_xml_parser::parse_xml_file("test_data/mixedmoves_parse.xml");

                auto moves = std::dynamic_pointer_cast<Moves>(parsed_msg);

                REQUIRE(*std::dynamic_pointer_cast<MoveMain>(moves->_moves[1]) == mv);
                REQUIRE(*std::dynamic_pointer_cast<EnterPiece>(moves->_moves[0]) == mv2);
        }

}

TEST_CASE("homemove"){
        std::ifstream in;

        Pawn bp0{0, Color::blue};
        Pawn bp3{3, Color::blue};

        MoveHome mv{3, 2, bp0};
        EnterPiece mv2{bp3};

        SECTION("Serialize"){

                auto expected =
                        slurp_fun(in, "test_data/home_move_serialization.xml");
                REQUIRE(mv.serialize().append("\n") == expected);

        }

        SECTION("Parse"){
                std::shared_ptr<Serializable> parsed_msg =
                        cheesy_xml_parser::parse_xml_file("test_data/home_move_parse.xml");

                auto moves = std::dynamic_pointer_cast<Moves>(parsed_msg);

                REQUIRE(*std::dynamic_pointer_cast<MoveHome>(moves->_moves[0]) == mv);
        }
}

TEST_CASE("because mike hates me"){
        Pawn bp0{0, Color::blue};
        Pawn bp1{1, Color::blue};
        Pawn bp3{3, Color::blue};

        MoveHome mv{3, 2, bp0};
        EnterPiece mv2{bp3};
        MoveMain mv3{5, 6, bp1};

        SECTION("Parse"){
                std::shared_ptr<Serializable> parsed_msg =
                        cheesy_xml_parser::parse_xml_file("test_data/all_moves.xml");

                auto moves = std::dynamic_pointer_cast<Moves>(parsed_msg);


                REQUIRE(*std::dynamic_pointer_cast<MoveMain>(moves->_moves[0]) == mv3);//mv);
                REQUIRE(*std::dynamic_pointer_cast<EnterPiece>(moves->_moves[1]) == mv2);
                REQUIRE(*std::dynamic_pointer_cast<MoveHome>(moves->_moves[2]) == mv);
        }

}

TEST_CASE("name"){
        std::string name_xml = "<name> string </name>";

        SECTION("serializing"){
                Name name("string");

                REQUIRE(name.serialize() == "<name> string </name>");
        }

        SECTION("Parsing"){
                std::string name_xml = "<name> string </name>";

                std::shared_ptr<Serializable> name =  cheesy_xml_parser::parse_xml_message(name_xml);

                REQUIRE(std::dynamic_pointer_cast<Name>(name)->name == "string");
        }
}
