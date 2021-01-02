#include <domains/cliff_walking.h>
#include <stdio.h>

bool move_agent(const unsigned action, struct Cliff* cliff) 
{
    struct Position new_pos = cliff->agent.position;

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

    //printf("New pos: X: %i Y: %i \n", new_pos.x, new_pos.y);

    //Check for invalid new state
    cliff->agent.position = calculate_new_position(new_pos,
                                                   cliff->agent.position,
                                                   cliff->width,
                                                   cliff->height);
    
    //Check for finish
    if(cliff->agent.position.x == cliff->width-1 && 
       cliff->agent.position.y == cliff->height-1)
        return true;
    
    return false;

}

//Check whether new state is valid, start, finish or a cliff drop
struct Position calculate_new_position(struct Position new_pos,
                                       const struct Position old_pos,
                                       const unsigned cliff_width,
                                       const unsigned cliff_height)
{
    //Check agent has not walked off side of cliff
    if(new_pos.x < 0 || new_pos.x >= cliff_width) 
        return old_pos;
    if(new_pos.y < 0 || new_pos.y >= cliff_height) 
        return old_pos;

    //Check agent has not fallen into cliff drop - if so, return to start
    if(new_pos.y == cliff_height-1 && new_pos.x != 0 && new_pos.x != cliff_width-1)
    {
        struct Position start_pos;
        start_pos.x = 0;
        start_pos.y = cliff_height-1;
        return start_pos;
    }

    return new_pos;
}

void display_cliff(struct Cliff cliff)
{
    for(unsigned i = 0; i < cliff.height; i++)
    {
        printf("|");
        for(unsigned j = 0; j < cliff.width; j++)
            printf("---|");
        printf("\n|");
        for(unsigned j = 0; j < cliff.width; j++)
        {
            //Determine grid square type
            char grid_square_char = ' ';
            //Draw start and finish
            if(i == cliff.height-1 && j == 0)
                grid_square_char = 'S';
            if(i == cliff.height-1 && j == cliff.width-1)
                grid_square_char = 'F';
            //Draw agent
            if(cliff.agent.position.x == j && cliff.agent.position.y == i)
                grid_square_char = 'A';
            //Draw drop
            if(i == cliff.height-1 && j != 0 && j != cliff.width-1)
                grid_square_char = 'X';

            printf(" %c |", grid_square_char);
        }
        printf("\n");
    }

    printf("|");
    for(unsigned i = 0; i < cliff.width; i++)
        printf("---|");

    printf("\n\n\n");
}
