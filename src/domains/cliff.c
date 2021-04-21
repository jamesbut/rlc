#include <domains/cliff.h>
#include <math.h>
#include <stdio.h>

bool move_agent(const unsigned action, struct Cliff* cliff)
{
    struct Position new_pos = cliff->agent.position;

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

    //Check for invalid new state
    if(!check_invalid_state(new_pos, cliff->width, cliff->height))
        //Move agent to new position
        cliff->agent.position = new_pos;

    //Recalculate agent's state value
    cliff->agent.state = get_agent_state(cliff);

    //Check for finish
    if(is_finish(cliff->agent.position.x, cliff->agent.position.y, cliff) ||
       is_drop(cliff->agent.position.x, cliff->agent.position.y, cliff))
        return true;

    return false;

}

void reset_agent(struct Cliff* cliff)
{
    cliff->agent.position.x = 0;
    cliff->agent.position.y = cliff->height-1;

    //Recalculate agent's state value
    cliff->agent.state = get_agent_state(cliff);
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

//Check whether state is on cliff
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

unsigned get_agent_state(const struct Cliff* cliff)
{
    return coords_to_state(cliff->agent.position.x, cliff->agent.position.y, cliff);
}

unsigned coords_to_state(const unsigned x, const unsigned y, const struct Cliff* cliff)
{
    return y * cliff->width + x;
}
struct Position state_to_coords(const unsigned state, const struct Cliff* cliff)
{
    const unsigned x = state % cliff->width;
    const unsigned y = floor((double)state / (double)cliff->width);
    struct Position pos = {x, y};
    return pos;
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


unsigned goal_state(const struct Cliff* cliff)
{
    return cliff->width * cliff->height - 1;
}

unsigned start_state(const struct Cliff* cliff)
{
    return cliff->width * (cliff->height-1);
}
