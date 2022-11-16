#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "Board.h"
#include "Game_token_base.h"
#include "Driver_base.h"

namespace Grid_game
{
    enum game_mode {not_running, running, game_won, game_lost};

    class Game_state
    {
        public:
            Game_state(Board &in_board, Token_base &in_token, Driver_base &in_driver, int in_max_turns) : game_board(in_board), player_token(in_token), driver(in_driver), maximum_turns(in_max_turns)
            { } // construct the game

            void run_game_loop_step();

            const game_mode check_termination_condition() const;

            const Board& board() { return game_board; }

            const Token_base& token() { return player_token; }

            bool active()
            {
                game_mode gm = check_termination_condition();

//                cout << "active? " << !(gm == game_won || gm == game_lost) << endl;

                return !(gm == game_won || gm == game_lost);
            }

            const int max_turns() const { return maximum_turns; }
            const int turn() const { return turns_finished; }

        private:
            Board &game_board;
            Token_base &player_token;
            Driver_base &driver;
            int turns_finished = 0;
            int maximum_turns;
    };
}

#endif