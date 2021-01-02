#include <optimisers/qlearning.h>
#include <stdlib.h>
#include <stdio.h>

double* create_q_table(const unsigned num_states,
                       const unsigned num_actions)
{
    
    double* q_table = malloc(num_states * num_actions * sizeof(double));

    for(unsigned i = 0; i < num_states; i++)
        for(unsigned j = 0; j < num_actions; j++)
            q_table[i * num_states + j]  = i * num_states + j;

    return q_table;
}

void print_q_table(const double* q_table,
                   const unsigned num_states,
                   const unsigned num_actions)
{
    for(unsigned i = 0; i < num_states; i++)
        for(unsigned j = 0; j < num_actions; j++)
        {
            printf("%f ", q_table[i * num_states + j]);
            if(j == num_actions - 1)
                printf("\n");
        }
}
