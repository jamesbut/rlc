#include <domains/cliff_walking.h>
#include <unistd.h>
#include <optimisers/qlearning.h>
#include <stdio.h>

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
    struct QTable q_table = create_q_table(num_states, num_actions);
    print_q_table(&q_table);
    set_q_value(&q_table, 4, 1, 5.0);
    print_q_table(&q_table);

    const double val1 = get_q_value(&q_table, 4, 1);
    printf("Val1: %f\n", val1);
    const double val2 = get_q_value(&q_table, 3, 1);
    printf("Val2: %f\n", val2);

    return 0;

}
