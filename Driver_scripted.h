#ifndef DRIVER_SCRIPTED_H
#define DRIVER_SCRIPTED_H

#include "Driver_base.h"
#include "Board.h"
#include "Game_token_base.h"
#include "Board_space.h"
#include <vector>
#include <iostream>

namespace Grid_game
{
    const char CMD_UP = 'u';
    const char CMD_RIGHT = 'r';
    const char CMD_DOWN = 'd';
    const char CMD_LEFT = 'l';

    const char CMD_SPC_UP = 'U';
    const char CMD_SPC_RIGHT = 'R';
    const char CMD_SPC_DOWN = 'D';
    const char CMD_SPC_LEFT = 'L';

    class Driver_scripted : public Driver_base
    {
    public:
        Driver_scripted(const Board &in_board, Token_base &in_token, string in_filename) : Driver_base(in_board, in_token), filename(in_filename)
        {
            script_source.open(filename, ios_base::in);
        }

        void next_move() override;

        ~Driver_scripted()
        {
            script_source.close();
        }

    private:
        using Driver_base::Driver_base; // hide the constructor; need a file!

        string filename;
        ifstream script_source;
    };
}

#endif