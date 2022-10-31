#include <iostream>
#include "Board_space.h"

using namespace std;
namespace Grid_game
{
    void Space::print_one_line()
    {
        switch (t_type)
        {
        case Space::empty:
            cout << "e";
            break;
        case Space::barrier:
            cout << "b";
            break;
        case Space::water:
            cout << "w";
            break;
        case Space::lava:
            cout << "l";
            break;
        }
    }
}
