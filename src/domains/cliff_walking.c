#include <domains/cliff_walking.h>
#include <stdio.h>

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
            if(cliff.agent.x_pos == j && cliff.agent.y_pos == i)
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
    printf("\n");
}
