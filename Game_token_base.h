#ifndef GAME_TOKEN_BASE_H
#define GAME_TOKEN_BASE_H

#include <vector>
#include "GUI/Graph.h"
#include "Complex_circle.h"
#include "Board_space.h"
#include "Board.h"

using namespace Graph_lib;

namespace Grid_game
{
    enum direction
    {
        up,
        right,
        down,
        left
    };

    class Token_base
    {
    public:
        Token_base(int start_x, int start_y, int start_hp, int start_pp, const Board &the_board) : x(start_x), y(start_y), hp(start_hp), pp(start_pp), max_hp(start_hp), max_pp(start_pp), board(the_board), token_shape(nullptr)
        {
        }

        Token_base(int start_hp, int start_pp, const Board &the_board) : Token_base(the_board.get_start_position().first, the_board.get_start_position().second, start_hp, start_pp, the_board)
        {
        }

        void step_normal(direction dir)
        {
//            cout << "Game_token_base::step_normal" << endl;
            if (pp < max_pp)
                ++pp;
            move(dir);
//            cout << "Game_token_base::step_normal -- apply terrain" << endl;
            apply_terrain_effect(board.retrieve_space(x, y).type());
//            cout << "Game_token_base::step_normal finished" << endl;
        }

        void step_special(vector<direction> dirs)
        {
            if (pp > 0)
            {
                --pp;
                move_special(dirs);
            }
            else
            {
                move(dirs[0]);
            }
            apply_terrain_effect(board.retrieve_space(x, y).type());
        }

        const int hit_points() const { return hp; }
        const int max_hit_points() const { return max_hp; }
        const int power_points() const { return pp; }
        const int max_power_points() const { return max_pp; }
        const int x_pos() const { return x; }
        const int y_pos() const { return y; }
        Shape* shape()
        {
            if (!token_shape)
            { // token shape doesn't exist yet, so construct it
                token_shape = make_shape();
            }

            return token_shape;
        }

        ~Token_base()
        {
            delete token_shape;
        }

    protected:
        int x, y;
        int hp, pp, max_hp, max_pp;
        const Board &board;
        Shape *token_shape;

        virtual void move(direction) = 0;
        virtual void move_special(vector<direction>) = 0;
        virtual void apply_terrain_effect(const Space::terrain_type) = 0;

        virtual Shape* make_shape()
        {
//            cout << "Token_base::make_shape()" << endl;
            // doesn't exist, so construct it
            Shape *the_shape = new Graph_lib::Complex_circle{Point(0, 0), 5};
            the_shape->set_color(Graph_lib::Color::cyan);
            the_shape->set_fill_color(Graph_lib::Color::cyan);

            return the_shape;
        };
    };
}

#endif