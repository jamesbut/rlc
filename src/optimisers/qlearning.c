#include <optimisers/qlearning.h>
#include <stdlib.h>
#include <stdio.h>
#include <optimisers/potential_function.h>

struct QTable create_q_table(const unsigned num_states,
                             const unsigned num_actions)
{

    struct QTable q_table;
    q_table.num_states = num_states;
    q_table.num_actions = num_actions;

    //double* q_table = malloc(num_states * num_actions * sizeof(double));
    q_table.table = calloc(num_states * num_actions, sizeof(double));

    /*
    for(unsigned i = 0; i < num_states; i++)
        for(unsigned j = 0; j < num_actions; j++)
            q_table[i * num_actions + j] = i * num_actions + j;
    */

    return q_table;
}

void print_q_table(const struct QTable* q_table)
{
    for(unsigned i = 0; i < q_table->num_states; i++)
        for(unsigned j = 0; j < q_table->num_actions; j++)
        {
            printf("%f ", q_table->table[i * q_table->num_actions + j]);
            if(j == q_table->num_actions - 1)
                printf("\n");
        }
    printf("\n");
}

void set_q_value(struct QTable* q_table,
                 const unsigned state,
                 const unsigned action,
                 const double value)
{

    //Bounds checking
    check_table_index_bounds(q_table, state, action);

    //Insertion
    q_table->table[state * q_table->num_actions + action] = value;

}

double get_q_value(const struct QTable* q_table,
                   const unsigned state,
                   const unsigned action)
{
    //Bounds checking
    check_table_index_bounds(q_table, state, action);

    return q_table->table[state * q_table->num_actions + action];

}

void check_table_index_bounds(const struct QTable* q_table,
                              const unsigned state,
                              const unsigned action)
{
    //Out of bounds checking
    if(state >= q_table->num_states)
    {
        printf("There are not this many states in QTable!\n");
        printf("Number of states in q table: %u\n", q_table->num_states);
        printf("Given state: %u\n", state);
        printf("Exiting program...\n");
        exit(0);
    }
    if(action >= q_table->num_actions)
    {
        printf("There are not this many actions in QTable!\n");
        printf("Number of actions in q table %u\n", q_table->num_actions);
        printf("Given action: %u\n", action);
        printf("Exiting program...\n");
        exit(0);
    }

}

void q_learning_step(struct QTable* q_table, const struct QLearningParams* params,
                     const struct Experience experience,
                     const struct Cliff* cliff)
{
    //Unpack experience to make code look nicer
    const unsigned s = experience.s;
    const unsigned a = experience.a;
    const unsigned s_prime = experience.s_prime;
    const double r = experience.r;

    //Q learning step
    const double max_action_q_val = get_q_value(q_table, experience.s_prime,
                                                calculate_max_action(q_table, s_prime));
    //Q-Learning with potential function
    const double new_q_val = get_q_value(q_table, s, a) + params->alpha *
            (r + params->gamma * (max_action_q_val +
                                  potential_function(s_prime, cliff,
                                                     params->potential_reward_type)) -
             get_q_value(q_table, s, a) -
             potential_function(s, cliff, params->potential_reward_type));

    //Update QTable
    set_q_value(q_table, s, a, new_q_val);
}

unsigned calculate_max_action(const struct QTable* q_table, const unsigned s)
{
    double max_q_val = get_q_value(q_table, s, 0);
    unsigned max_action = 0;
    for(unsigned i = 0; i < q_table->num_actions; i++)
        if(get_q_value(q_table, s, i) > max_q_val)
            max_action = i;
    return max_action;
}

unsigned sample_action(const struct QTable* q_table, const unsigned state,
                       const double epsilon)
{
    //Sample random number between 0 and 1
    const double rand_num = (double)rand() / (double)RAND_MAX;
    if(rand_num < epsilon)
        //Return random action
        return rand() % q_table->num_actions;
    else
        //Return action with max q value
        return calculate_max_action(q_table, state);
}
