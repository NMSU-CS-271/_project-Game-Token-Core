#include "Game_state.h"
#include "Game_token.h"
#include "Window_game.h"
#include "Driver_scripted.h"
#include <string>
#include <chrono>
#include <thread>
#include <iostream>

int main(int argc, char *argv[])
{
    // first argument is game settings
    // second argument is map
    // third argument is scripted driver commands

    // our default game settings
    int hp = 3;
    int pp = 3;
    int max_rounds = 20;

    if (argc >= 2)
    { // update game settings if provided; file with 3 numbers: max hp, max pp, max_rounds
        ifstream settings_source{argv[1], std::ifstream::in};

        settings_source >> hp;
        settings_source >> pp;
        settings_source >> max_rounds;

        settings_source.close();
    }

    string map_filename = "simple.map";

    if (argc >= 3)
    { // get the map from the command line
        map_filename = argv[2];
    }

//    string simple_map = "simple.map";
//    string danger_map = "danger.map";
    
    string cmd_filename = "dodge_danger.cmd";

    if (argc >= 4)
    { // script commands, if provided (not always needed!)
        cmd_filename = argv[3];
    }

    // make the parts
    Grid_game::Board board{map_filename};
    Grid_game::Token token{hp, pp, board};
    Grid_game::Driver_scripted driver{board, token, cmd_filename};

    Grid_game::Game_state state{board, token, driver, max_rounds};
    
    while (state.active())
    {
        state.run_game_loop_step();
    }
}
