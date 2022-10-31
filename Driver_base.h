#ifndef DRIVER_BASE_H
#define DRIVER_BASE_H

#include "Board.h"
#include "Game_token_base.h"
#include "Board_space.h"
#include <vector>

namespace Grid_game
{
    class Driver_base
    {
    public:
        Driver_base(const Board &in_board, Token_base &in_token) : board(in_board), token(in_token) {}

        /// @brief choses the next move and invokes a single step call on the token using the data provided
        virtual void next_move()=0;

    protected:
        const Board &board;
        Token_base &token;
    };
}

#endif