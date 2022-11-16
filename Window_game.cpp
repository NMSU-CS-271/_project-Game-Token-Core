#include "Window_game.h"

namespace Grid_game
{
    using namespace Graph_lib;

    Shape* Window_game::shape()
    {
        Shape *the_shape = new Graph_lib::Complex_circle{Point(0, 0), 5};
        the_shape->set_color(Graph_lib::Color::magenta);
        the_shape->set_fill_color(Graph_lib::Color::magenta);

        return the_shape;
    }

    void Window_game::init()
    {
        cout << "starting init()" << endl;
        int rect_w = x_max() / board.width();
        int rect_h = (y_max() - status_band_height) / board.height();
        x_scale = rect_w;
        x_offset = rect_w / 2;
        y_scale = rect_h;
        y_offset = (rect_h / 2) + status_band_height;

        // iterate over the board to draw the spaces
        for (int j = 0; j < board.height(); j++)
        {
            for (int i = 0; i < board.width(); i++)
            {
                const Space &space = board.retrieve_space(i, j);
                Rectangle *space_rect = new Rectangle(Point{i * rect_w, (j * rect_h) + status_band_height}, rect_w, rect_h);
                switch (space.type())
                {
                case Space::barrier:
                    space_rect->set_color(gg_color_barrier);
                    space_rect->set_fill_color(gg_color_barrier_fill);
                    break;
                case Space::empty:
                    space_rect->set_color(gg_color_empty);
                    space_rect->set_fill_color(gg_color_empty_fill);
                    break;
                case Space::water:
                    space_rect->set_color(gg_color_water);
                    space_rect->set_fill_color(gg_color_water_fill);
                    break;
                case Space::lava:
                    space_rect->set_color(gg_color_lava);
                    space_rect->set_fill_color(gg_color_lava_fill);
                    break;
                default:
                    space_rect->set_color(Color::dark_magenta);
                    space_rect->set_fill_color(Color::magenta);
                    break;
                }
                board_rects.push_back(space_rect);
                attach(*space_rect);
            }
        }

        attach(*play_state);
        attach(*hp_state);
        attach(*pp_state);
        attach(*turn_state);

        cout << "show the window" << endl;
        show();
    }

    void Window_game::draw()
    {
        //        cout << "call Window::draw()" << endl;
        Window::draw();

        //        cout << "fl_push_matrix()" << endl;
        fl_push_matrix();

        //        cout << "trans: x: " << (token.x_pos() * x_scale) + x_offset << ", y: " << (token.y_pos() * y_scale) + y_offset << endl;
        //        cout << "trans details: x_pos: " << token.x_pos() << ", x_scale: " << x_scale << ", x_offset: " << x_offset << ", y_pos: " << token.y_pos() << ", y_scale: " << y_scale << ", y_offset: " << y_offset << endl;

        fl_translate((token.x_pos() * x_scale) + x_offset, (token.y_pos() * y_scale) + y_offset);

        // draw token
        token_viz->draw();

        fl_translate((token.x_pos() * x_scale) + x_offset, (token.y_pos() * y_scale) + y_offset);

        fl_pop_matrix();
    }

    void Window_game::draw_frame()
    {
        switch (state.check_termination_condition())
        {
        case game_mode::game_lost:
            play_state->set_label("GAME OVER!");
            break;
        case game_mode::game_won:
            play_state->set_label("GAME WON!");
            break;
        case game_mode::running:
            play_state->set_label("Playing...");
            break;
        }
        hp_state->set_label("HP: " + to_string(token.hit_points()) + "/" + to_string(token.max_hit_points()));

        pp_state->set_label("PP: " + to_string(token.power_points()) + "/" + to_string(token.max_power_points()));

        turn_state->set_label("Turn: " + to_string(state.turn()) + "/" + to_string(state.max_turns()));

        Fl::flush();
        redraw();
    }
}