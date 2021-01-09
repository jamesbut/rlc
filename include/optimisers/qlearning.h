#ifndef _Q_LEARNING_H_
#define _Q_LEARNING_H_

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

double get_q_value(struct QTable* q_table,
                   const unsigned state,
                   const unsigned action);

//Checks table bounds, exits program on failure
void check_table_index_bounds(const struct QTable* q_table,
                              const unsigned state,
                              const unsigned action);

#endif
