#include <string>
#include <vector>

#include <pegtl.hpp>
#include <pegtl/tracer.hpp>
#include <pegtl/analyze.hpp>

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
        TAOCPP_PEGTL_STRING("blue"),
        >{};

struct start_game :
        pegtl::seq<
        TAOCPP_PEGTL_STRING("<start-game>"),
        color,
        TAOCPP_PEGTL_STRING("</start-game>"),
        >{};

struct name :
        pegtl::seq<
        TAOCPP_PEGTL_STRING("<name>"),
        seps,
        pegtl::plus<
                pegtl::sor<
                        pegtl::<pegtl::alnum>,
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
        TAOCPP_PEGTL_STRING("</pawn>")
        >{};

struct dice_val :
        pegtl::sor<
        pegtl::one<'1'>,
        pegtl::one<'2'>,
        pegtl::one<'3'>,
        pegtl::one<'4'>
        pegtl::one<'5'>,
        pegtl::one<'6'>,
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
        move_piece_home,
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

struct home_rows :
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

struct void :
        pegtl::seq<
        TAOCPP_PEGTL_STRING("<void>"),
                seps,
                TAOCPP_PEGTL_STRING("</void>")
        >{};

struct doubles_penalty :
        pegtl::seq<
        TAOCPP_PEGTL_STRING("<doubles-penalty>"),
        seps,
        TAOCPP_PEGTL_STRING("</doubles-penalty>")
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

struct do_move :
        pegtl::seq<
        TAOCPP_PEGTL_STRING("<do-move>"),
                seps,
                board,
                seps,
                dice,
                seps,
                TAOCPP_PEGTL_STRING("</do-move>")
        >{};
