#include <domains/cliff_walking.h>
#include <stdio.h>
#include <stdlib.h>

bool move_agent(const unsigned action, struct Cliff* cliff)
{
    struct Position new_pos = cliff->agent.position;

    //Reset agent to start if agent has fallen
    if(is_drop(cliff->agent.position.x, cliff->agent.position.y, cliff))
    {
        new_pos.x = 0;
        new_pos.y = cliff->height-1;
    }
    //Otherwise apply action
    else
    {
        printf("Action: %u\n", action);

        switch(action)
        {
            //Up
            case 0:
                new_pos.y -= 1;
                break;
            //Down
            case 1:
                new_pos.y += 1;
                break;
            //Left
            case 2:
                new_pos.x -= 1;
                break;
            //Right
            case 3:
                new_pos.x += 1;
                break;
        }

    }

    //Check for invalid new state
    if(!check_invalid_state(new_pos, cliff->width, cliff->height))
        //Move agent to new position
        cliff->agent.position = new_pos;

    //Recalculate agent's state value
    cliff->agent.state = get_state_value(cliff);

    //Check for finish
    if(is_finish(cliff->agent.position.x, cliff->agent.position.y, cliff))
        return true;

    return false;

}

//Check whether new state is valid, start, finish or a cliff drop
bool check_invalid_state(struct Position new_pos,
                         const unsigned cliff_width,
                         const unsigned cliff_height)
{
    //Check agent has not walked off side of cliff
    if(new_pos.x < 0 || new_pos.x >= cliff_width)
        return true;
    if(new_pos.y < 0 || new_pos.y >= cliff_height)
        return true;

    return false;
}

bool is_drop(const unsigned x, const unsigned y, const struct Cliff* cliff)
{
    if(y == cliff->height-1 && x != 0 && x != cliff->width-1)
        return true;
    else
        return false;
}

bool is_start(const unsigned x, const unsigned y, const struct Cliff* cliff)
{
    if(y == cliff->height-1 && x == 0)
        return true;
    else
        return false;
}

bool is_finish(const unsigned x, const unsigned y, const struct Cliff* cliff)
{
    if(y == cliff->height-1 && x == cliff->width-1)
        return true;
    else
        return false;
}

void display_cliff(const struct Cliff* cliff)
{
    for(unsigned i = 0; i < cliff->height; i++)
    {
        printf("|");
        for(unsigned j = 0; j < cliff->width; j++)
            printf("---|");
        printf("\n|");
        for(unsigned j = 0; j < cliff->width; j++)
        {
            //Determine grid square type
            char grid_square_char = ' ';
            //Draw start and finish
            if(is_start(j, i, cliff))
                grid_square_char = 'S';
            if(is_finish(j, i, cliff))
                grid_square_char = 'F';
            //Draw agent
            if(cliff->agent.position.x == j && cliff->agent.position.y == i)
                grid_square_char = 'A';
            //Draw drop
            if(is_drop(j, i, cliff))
                grid_square_char = 'X';

            printf(" %c |", grid_square_char);
        }
        printf("\n");
    }

    printf("|");
    for(unsigned i = 0; i < cliff->width; i++)
        printf("---|");

    printf("\n\n\n");
}

unsigned get_state_value(const struct Cliff* cliff)
{
    return cliff->agent.position.y * cliff->width + cliff->agent.position.x;
}

struct CliffWalking create_cliff_walking(const unsigned cliff_width,
                                         const unsigned cliff_height)
{
    //Create cliff
    struct Cliff cliff;
    cliff.width = cliff_width;
    cliff.height = cliff_height;
    cliff.agent.position.x = 0;
    cliff.agent.position.y = cliff.height-1;
    cliff.agent.state = get_state_value(&cliff);

    struct CliffWalking cliff_walking;
    cliff_walking.cliff = cliff;
    cliff_walking.reward_table = build_reward_table(&cliff);

    return cliff_walking;

}

double* build_reward_table(const struct Cliff* cliff)
{
    double* reward_table = calloc(cliff->width * cliff->height, sizeof(double));

    for(unsigned i = 0; i < cliff->height; i++)
        for(unsigned j = 0; j < cliff->width; j++)
    {
        //-100 for falling off the cliff
        if(is_drop(j, i, cliff))
            reward_table[i * cliff->width + j] = -100.;
        //-1 for every step taken
        else
            reward_table[i * cliff->width + j] = -1.;
    }

    return reward_table;
}

double reward_function(const struct CliffWalking* cliff_walking)
{
    return cliff_walking->reward_table[cliff_walking->cliff.agent.state];
}

struct Experience agent_step(struct CliffWalking* cliff_walking, const unsigned action)
{

    struct Experience experience;
    experience.s = cliff_walking->cliff.agent.state;
    experience.a = action;

    move_agent(action, &cliff_walking->cliff);

    experience.s_prime = cliff_walking->cliff.agent.state;
    experience.r = reward_function(cliff_walking);

    return experience;
}
