#include <domains/cliff_walking.h>
#include <unistd.h>
#include <optimisers/qlearning.h>

int main() 
{

    /*
    struct Cliff cliff;
    cliff.width = 5;
    cliff.height = 3;
    cliff.agent.position.x = 0;
    cliff.agent.position.y = cliff.height-1;

    //enum Action moves[] = {Up, Up, Right, Right, Right, Right, Down, Down, Up, Up}; 
    enum Action moves[] = {Up, Up, Right, Right, Down, Down, Up, Up}; 
    const unsigned num_moves = sizeof(moves) / sizeof(moves[0]);

    display_cliff(cliff);
    for(size_t i = 0; i < num_moves; i++)
    {
        sleep(1);
        const bool finished = move_agent(moves[i], &cliff);
        display_cliff(cliff);
        if(finished)
            break;
    }
    */

    const unsigned num_states = 5;
    const unsigned num_actions = 3;
    double* q_table = create_q_table(num_states, num_actions);
    print_q_table(q_table, num_states, num_actions);

    return 0;

}
