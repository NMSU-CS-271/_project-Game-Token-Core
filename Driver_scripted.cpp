#include "Driver_scripted.h"

namespace Grid_game
{
    void Driver_scripted::next_move()
    {
        char next_command;

        if (script_source >> next_command)
        {
            cout << "next command: " << next_command << endl;
            switch (next_command)
            {
            case CMD_UP:
                cout << "going: " << next_command << endl;
                token.step_normal(direction::up);
                break;
            case CMD_RIGHT:
                cout << "going: " << next_command << endl;
                token.step_normal(direction::right);
                break;
            case CMD_DOWN:
                cout << "going: " << next_command << endl;
                token.step_normal(direction::down);
                break;
            case CMD_LEFT:
                cout << "going: " << next_command << endl;
                token.step_normal(direction::left);
                break;
            case CMD_SPC_UP:
                token.step_special(vector<direction>(direction::up));
                break;
            case CMD_SPC_RIGHT:
                token.step_special(vector<direction>(direction::right));
                break;
            case CMD_SPC_DOWN:
                token.step_special(vector<direction>(direction::down));
                break;
            case CMD_SPC_LEFT:
                token.step_special(vector<direction>(direction::left));
                break;
            }
        }
    }
}