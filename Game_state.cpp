#include "Game_state.h"

namespace Grid_game
{
    void Game_state::run_game_loop_step()
    {
        // print out the status:
        cout << " --- Game Status ---" << endl;;
        cout << "  hp: " << player_token.hit_points() << " / " << player_token.max_hit_points() << endl;
        cout << "  pp: " << player_token.power_points() << " / " << player_token.max_power_points() << endl;
        cout << "turn: " << turns_finished << " / " << max_turns() << endl;

        if (check_termination_condition() == game_mode::running)
        {
            driver.next_move();
            ++turns_finished;
        }
    }

    const game_mode Game_state::check_termination_condition() const
    {
        if (player_token.y_pos() == (game_board.height()-1))
        {
            cout << "GAME WON!" << endl;
            return game_mode::game_won;
        }
        else if(turns_finished >= max_turns())
        {
            cout << "GAME OVER - time up" << endl;
            return game_mode::game_lost;
        }
        else if(player_token.hit_points()<=0)
        {
            cout << "GAME OVER - hit points gone" << endl;
            return game_mode::game_lost;
        }
        else
        {
            return game_mode::running;
        }
    }
}