#ifndef GAME_VISUALIZER_H
#define GAME_VISUALIZER_H

#include "GUI/Window.h"
#include "Game_state.h"
#include "Board.h"
#include "Board_space.h"
#include "Game_token_base.h"
#include "GUI/Graph.h"
#include "Complex_circle.h"
#include <string>

namespace Grid_game
{
    using namespace Graph_lib;

    class Window_game : public Graph_lib::Window
    {
        public: 
            Window_game(const Game_state& in_state, const Board& in_board, Token_base& in_token, int w, int h, string title) : 
            Window{w, h, title}, 
            state(in_state),
            board(in_board), 
            token(in_token), 
            token_viz(shape())
            {
                play_state = new Graph_lib::Text{Graph_lib::Point{0+status_band_padding,status_band_baseline},"INIT"};
                
                hp_state = new Graph_lib::Text{Graph_lib::Point{(w/4)+status_band_padding,status_band_baseline},"HP: "+to_string(token.hit_points())+"/"+to_string(token.max_hit_points())};
                
                pp_state = new Graph_lib::Text{Graph_lib::Point{2*(w/4)+status_band_padding,status_band_baseline},"PP: "+to_string(token.power_points())+"/"+to_string(token.max_power_points())}; 

                turn_state = new Graph_lib::Text{Graph_lib::Point{3*(w/4)+status_band_padding, status_band_baseline}, "Turn: "+to_string(state.turn())+"/"+to_string(state.max_turns())};
            }

            void init();
            void draw_frame();

            const int status_band_height = 20;
            const int status_band_baseline = 16;
            const int status_band_padding = 4;
            const Graph_lib::Color gg_color_empty{143};
            const Graph_lib::Color gg_color_empty_fill{140};
            const Graph_lib::Color gg_color_barrier{16};
            const Graph_lib::Color gg_color_barrier_fill{8};
            const Graph_lib::Color gg_color_water{222};
            const Graph_lib::Color gg_color_water_fill{218};
            const Graph_lib::Color gg_color_lava{88};
            const Graph_lib::Color gg_color_lava_fill{72};

            ~Window_game()
            {
                delete play_state;
                delete hp_state;
                delete pp_state;
                delete turn_state;
                delete token_viz;
            }

        protected:
            void draw() override;
            Shape* shape();

            const Game_state& state;
            const Board& board;
            Token_base& token;
            Graph_lib::Vector_ref<Graph_lib::Rectangle> board_rects;
            Shape* token_viz;
            Graph_lib::Text* play_state;
            Graph_lib::Text* hp_state;
            Graph_lib::Text* pp_state;
            Graph_lib::Text* turn_state;

        private:
            // set up in init() based on window size
            int x_scale, y_scale;
            int x_offset, y_offset;
    };
}

#endif