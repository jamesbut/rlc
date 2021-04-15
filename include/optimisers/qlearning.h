#ifndef _Q_LEARNING_H_
#define _Q_LEARNING_H_

#include <optimisers/experience_tuple.h>

/* Q Table */
struct QTable
{
    unsigned num_states;
    unsigned num_actions;
    double* table;
};

struct QTable create_q_table(const unsigned num_states,
                             const unsigned num_actions);

void print_q_table(const struct QTable* q_table);

void set_q_value(struct QTable* q_table,
                 const unsigned state,
                 const unsigned action,
                 const double value);

double get_q_value(const struct QTable* q_table,
                   const unsigned state,
                   const unsigned action);

//Checks table bounds, exits program on failure
void check_table_index_bounds(const struct QTable* q_table,
                              const unsigned state,
                              const unsigned action);

/*
 * Q-Learning algorithm
 * ---------------------
 * Applies changes to a Q table
 */

struct QLearningParams
{
    const double alpha;
    const double gamma;
};

void q_learning_step(struct QTable* q_table, const struct QLearningParams* params,
                     const struct Experience experience);

//Calculates the action with the highest q value for a particular state
unsigned calculate_max_action(const struct QTable* q_table, const unsigned s);

//Samples action according to epsilon greedy policy
unsigned sample_action(const struct QTable* q_table, const unsigned state,
                       const double epsilon);

#endif
