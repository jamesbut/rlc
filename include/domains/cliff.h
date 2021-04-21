#ifndef _CLIFF_H_
#define _CLIFF_H_

#include <stdbool.h>

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

//Returns true if agent gets to finish
bool move_agent(const unsigned action, struct Cliff* cliff);

//Resets agent to start
void reset_agent(struct Cliff* cliff);

void display_cliff(const struct Cliff* cliff);

bool check_invalid_state(struct Position new_pos,
                         const unsigned cliff_width,
                         const unsigned cliff_height);

//Get agent state value from agent position on cliff
unsigned get_agent_state(const struct Cliff* cliff);

//Switching between coordinates and state integer
unsigned coords_to_state(const unsigned x, const unsigned y, const struct Cliff* cliff);
struct Position state_to_coords(const unsigned state, const struct Cliff* cliff);

//Determines type of state
bool is_drop(const unsigned x, const unsigned y, const struct Cliff* cliff);
bool is_start(const unsigned x, const unsigned y, const struct Cliff* cliff);
bool is_finish(const unsigned x, const unsigned y, const struct Cliff* cliff);

//Gives particular state values
unsigned goal_state(const struct Cliff* cliff);
unsigned start_state(const struct Cliff* cliff);

#endif
