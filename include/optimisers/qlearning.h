#ifndef _Q_LEARNING_H_
#define _Q_LEARNING_H_

double* create_q_table(const unsigned num_states,
                       const unsigned num_actions);

void print_q_table(const double* q_table,
                   const unsigned num_states,
                   const unsigned num_actions);

#endif
