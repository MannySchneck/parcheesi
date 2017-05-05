#include <string>

#include <vector>

#include <pegtl.hpp>
#include <pegtl/tracer.hpp>
#include <pegtl/analyze.hpp>
#include "parse_stack.h"

#include "interfaces.h"
#include "board.h"
#include "arg_wrapper_classes.h"

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
                pegtl::plus<
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
                >
        {};

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
                >
        {};

        struct die :
                pegtl::seq<
                TAOCPP_PEGTL_STRING("<die>"),
                seps,
                dice_val,
                seps,
                TAOCPP_PEGTL_STRING("</die>")
                >{};

        struct dice :
                pegtl::star<die>{};

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
                pegtl::plus<
                        pawn
                        >,
                seps,
                TAOCPP_PEGTL_STRING("</home>")
                >{};

        struct home_rows :
                pegtl::seq<
                TAOCPP_PEGTL_STRING("<home-rows>"),
                seps,
                pegtl::plus<
                        piece_loc
                        >,
                seps,
                TAOCPP_PEGTL_STRING("</home-rows>")
                >{};

        struct main :
                pegtl::seq<
                TAOCPP_PEGTL_STRING("<main>"),
                seps,
                pegtl::plus<
                        piece_loc
                        >,
                seps,
                TAOCPP_PEGTL_STRING("</main>")
                >{};

        struct board_start :
                pegtl::seq<
                TAOCPP_PEGTL_STRING("<start>"),
                seps,
                pegtl::plus<
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
                pegtl::plus<
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
                pegtl::sor<
                start_game,
                do_move,
                doubles_penalty
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

                pegtl::string_input<> in(parse_buf, msg);

                std::shared_ptr<Serializable> parsed_message;

                pegtl::parse<messages, action>(in, parsed_message);

                return parsed_message;
        }
}
