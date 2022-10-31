#ifndef SPACE_GUARD
#define SPACE_GUARD 1

using namespace std;

namespace Grid_game
{
    /**
     * @brief Represents a single space on a game board.
     *
     */
    class Space
    {
    public:
        enum terrain_type
        {
            empty,
            barrier,
            water,
            lava,
            FINAL_terrain_type
        };
        enum unique_terrain_type
        {
            start,
            end
        };

        Space(terrain_type type) : t_type(type) {}
        void print_one_line();
        const terrain_type &type() const { return t_type; }

    protected:
        terrain_type t_type;
    };
}
#endif