#ifndef _CLIFF_WALKING_H_
#define _CLIFF_WALKING_H_

#include <optimisers/experience_tuple.h>
#include <domains/cliff.h>

struct CliffWalking
{
    struct Cliff cliff;
    //Maps states to rewards
    double* reward_table;
};

struct CliffWalking create_cliff_walking(const unsigned cliff_width,
                                         const unsigned cliff_height);

double* build_reward_table(const struct Cliff* cliff);

//This is a simple domain so the reward is dependent only on the state that the agent
//has just moved into
double reward_function(const struct CliffWalking* cliff_walking);

struct Experience agent_step(struct CliffWalking* cliff_walking, const unsigned action);

#endif
