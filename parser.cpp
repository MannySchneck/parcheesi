#include <string>

#include <vector>

#include <pegtl.hpp>
#include <pegtl/contrib/tracer.hpp>
#include <pegtl/analyze.hpp>
#include "parse_stack.h"

#include "interfaces.h"
#include "board.h"
#include "arg_wrapper_classes.h"

#include "catch.hpp"

#include <sstream>

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

        struct name :
                pegtl::seq<
                TAOCPP_PEGTL_STRING("<name>"),
                seps,
                pegtl::star<
                        pegtl::sor<
                                pegtl::alnum,
                                pegtl::one<'_'>
                                >
                        >,
                seps,
                TAOCPP_PEGTL_STRING("</name>")>{};

        struct id_val :
                pegtl::sor<
                pegtl::one<'1'>,
                pegtl::one<'2'>,
                pegtl::one<'3'>,
                pegtl::one<'4'>
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
                        pegtl::star<die>,
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
                        move_contents,
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
                        pawn
                        >,
                        seps,
                        TAOCPP_PEGTL_STRING("</home>")
                        >{};

        struct home_rows :
                pegtl::seq<
                TAOCPP_PEGTL_STRING("<home-rows>"),
                        seps,
                        pegtl::star<
                        piece_loc
                        >,
                        seps,
                        TAOCPP_PEGTL_STRING("</home-rows>")
                        >{};

        struct main :
                pegtl::seq<
                TAOCPP_PEGTL_STRING("<main>"),
                        seps,
                        pegtl::star<
                        piece_loc
                        >,
                        seps,
                        TAOCPP_PEGTL_STRING("</main>")
                        >{};

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

        struct moves :
                pegtl::seq<
                TAOCPP_PEGTL_STRING("<moves>"),
                        seps,
                        pegtl::star<
                        move
                        >,
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
                        doubles_penalty
                        >
                >{};

        template<typename Rule>
        struct action : pegtl::nothing< Rule>{};

        // parsing stacks
        L2::L2_Parse_Stack<std::shared_ptr<Serializable>> the_stack;

        std::vector<std::string> string_stack;

        std::vector<int> int_stack;

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

        template<> struct action <pawn>{
                template <typename Input>
                static void apply(const Input& in, std::shared_ptr<Serializable> & msg){
                        ; //TODO
                }
        };

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
        Board board;

        Pawn bp0{0, Color::blue};

        board.put_pawn(bp0, 0);

        std::string expected =
                "<board> <start> <pawn> "\
                "<color> red </color> <id> 3 </id> </pawn> "\
                "<pawn> "\
                "<color> red </color> <id> 2 </id> </pawn> <pawn> <color> red </color> "\
                "<id> 1 </id> </pawn> <pawn> "\
                "<color> red </color> <id> 0 </id> </pawn> <pawn> "\
                "<color> yellow </color> <id> 3 </id> </pawn> <pawn> "\
                "<color> yellow </color> "\
                "<id> 2 </id> </pawn> <pawn> "\
                "<color> yellow </color> <id> 1 </id> </pawn> <pawn> "\
                "<color> yellow </color> <id> 0 </id> </pawn> <pawn> "\
                "<color> green </color> "\
                "<id> 3 </id> </pawn> <pawn> "\
                "<color> green </color> <id> 2 </id> </pawn> <pawn> "\
                "<color> green </color> <id> 1 </id> </pawn> <pawn> "\
                "<color> green </color> "\
                "<id> 0 </id> </pawn> <pawn> "\
                "<color> blue </color> <id> 3 </id> </pawn> <pawn> "\
                "<color> blue </color> <id> 2 </id> </pawn> <pawn> "\
                "<color> blue </color> <id> "\
                "1 </id> </pawn> <pawn> "\
                "<color> blue </color> <id> 0 </id> </pawn> </start> "\
                "<main> <piece-loc> <pawn> <color> blue </color> <id> 0 </id> </pawn> <loc> 0 </loc> </piece-loc> </main> <home-rows> </home-rows> <home> </home> </board>";

        REQUIRE(board.serialize() == expected);
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
