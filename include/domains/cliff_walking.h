#ifndef _CLIFF_WALKING_H_
#define _CLIFF_WALKING_H_

#include <stdbool.h>
#include <optimisers/experience_tuple.h>

struct Position
{
    int x;
    int y;
};

struct Agent
{
    struct Position position;
    //State is a unique unsigned integer based on agent position and size of the
    //cliff that it is on
    unsigned state;
};

struct Cliff
{
    unsigned width;
    unsigned height;
    struct Agent agent;
};

enum Action
{
    Up,
    Down,
    Left,
    Right
};

void display_cliff(const struct Cliff* cliff);

//Returns true if agent gets to finish
bool move_agent(const unsigned action, struct Cliff* cliff);

//Resets agent to start
void reset_agent(struct Cliff* cliff);

bool check_invalid_state(struct Position new_pos,
                         const unsigned cliff_width,
                         const unsigned cliff_height);

//Determines type of state
bool is_drop(const unsigned x, const unsigned y, const struct Cliff* cliff);
bool is_start(const unsigned x, const unsigned y, const struct Cliff* cliff);
bool is_finish(const unsigned x, const unsigned y, const struct Cliff* cliff);

//Get agent state value from agent position on cliff
unsigned get_state_value(const struct Cliff* cliff);

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
