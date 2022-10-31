#include <vector>
#include <random>
#include <stdexcept>
#include "Board.h"

using namespace std;

namespace Grid_game
{
    Board::Board(int w, int h, int seed, int start_x) : w(w), h(h), start_x(start_x)
    {
        if (start_x < 0)
            throw std::range_error("Locations cannot be less than 0.");

        if (start_x >= w)
            throw std::range_error("Locations cannot be placed beyond set width " + std::to_string(w) + " or height " + std::to_string(h) + ".");

        // check that start and end positions are safe
        std::srand(seed);

        for (int i = 0; i < h; i++)
        {
            vector<Space> spaces_row;
            for (int j = 0; j < w; j++)
            {
                Space::terrain_type type = (Space::terrain_type)(rand() % ((int)Space::FINAL_terrain_type));
                spaces_row.push_back(Space(type));
            }
            contents.push_back(spaces_row);
        }
    }

    // use a file to specify the board
    Board::Board(string filename)
    {
        // all variables come from the file
        // two integers, width, start_x
        // then stream of characters representing spaces
        ifstream map_source{filename, std::ifstream::in};

        int map_width, map_start_x;

        map_source >> map_width;
        map_source >> map_start_x;

        w = map_width;
        start_x = map_start_x;

        char space_char;
        int col_tracker = 0;

        vector<Space> spaces_row{};
        while (map_source >> space_char)
        {
            if (col_tracker == map_width)
            {
                col_tracker = 0;
                contents.push_back(spaces_row);
                spaces_row = vector<Space>{};
            }

            Space::terrain_type type = Space::terrain_type::empty;
            switch (space_char)
            {
            case 'b':
                type = Space::terrain_type::barrier;
                break;
            case 'w':
                type = Space::terrain_type::water;
                break;
            case 'l':
                type = Space::terrain_type::lava;
                break;
            case 'e':
                type = Space::terrain_type::empty;
                break;
            }

            spaces_row.push_back(Space(type));

            ++col_tracker;
        }

        for (; col_tracker < map_width; col_tracker++)
        { // fill in remainder with empty
            spaces_row.push_back(Space(Space::terrain_type::empty));
        }
        contents.push_back(spaces_row);

        map_source.close();
    }

    void Board::print_one_line_per_row()
    {
        for (vector<Space> row : contents)
        {
            for (Space tile : row)
            {
                tile.print_one_line();
            }
            cout << "\n";
        }
    }

    pair<int, int> Board::get_start_position() const
    {
        return pair<int, int>{start_x, 0};
    }

    const Space &Board::retrieve_space(int x, int y) const
    {
        if (x < 0 || y < 0 || x >= width() || y >= height()) // check bounds
            throw std::range_error("Attempt to retrieve space out of bounds. Requested: (" + std::to_string(x) + ", " + std::to_string(y) + "). " + "Valid spaces: x: 0--" + std::to_string(w - 1) + ", y: 0--" + std::to_string(h - 1) + ".");

        return contents[y][x];
    }
}