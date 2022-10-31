#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <time.h>
#include <utility>
#include <iostream>
#include <fstream>
#include "Board_space.h"

using namespace std;

namespace Grid_game
{
    // constexpr unsigned int default_start_x = 0;
    // constexpr unsigned int default_start_y = 0;

    /**
     * Represents a grid board for a grid-based game.
     *
     * The board is addressed such that location (0, 0) is the upper left corner of the board.
     * Negative values are not allowed, as these are used for sentinel values when addressing
     * spaces on the board. Values increase going right and down such that the highest X value
     * is width-1 and the highest Y value is height-1.
     *
     * Tokens are meant to start somewhere on the top row of a board and need to make their way
     * to the bottom row to succeed.
     *
     * Movement left and right is meant to wrap around -- moving from right from X width-1 goes
     * back to an X value of 0. Similarly, moving left from an X value of 0 will go to width-1.
     *
     * The finish line is to move to Y == height.
     *
     * Tokens start at start_x, which is the column in the first row in which the token starts.
     */
    class Board
    {
    public:
        /**
         * Generates a board with randomly assigned terrain types and a start and end space.
         */
        Board(int w, int h, int seed, int start_x);
        /**
         * Generates a board that randomly assigns each space a terrain_type.
         */
        //        Board(unsigned int w, unsigned int h, unsigned int seed):Board(w, h, seed, default_start_x, default_start_y, w-1, h-1){}
        /**
         * Generates a randomized board using time as the random seed with a width of w spaces and a height of h spaces.
         */
        Board(int w, int h, int start_x) : Board(w, h, time(NULL), start_x) {}

        // use a file to specify the board
        Board(string filename);

        pair<int, int> get_start_position() const;
        void random_position(int seed);
        const Space &retrieve_space(int x, int y) const;

        const int width() const { return (contents.size() > 0 ? contents[0].size() : 0); }
        const int height() const { return contents.size(); }

        void print_one_line_per_row();

    protected:
        int w, h, start_x;
        vector<vector<Space>> contents;

        void set_start(int start_x);
    };
}
#endif