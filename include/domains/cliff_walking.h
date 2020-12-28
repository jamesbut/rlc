#ifndef _CLIFF_WALKING_H_
#define _CLIFF_WALKING_H_

#include <stdbool.h>

struct Position
{
    int x;
    int y;
};

struct Agent 
{
    struct Position position;
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

void display_cliff(struct Cliff cliff);

//Returns true if agent gets to finish
bool move_agent(const unsigned action, struct Cliff* cliff);
struct Position calculate_new_position(struct Position new_pos, 
                                       const struct Position old_pos,
                                       const unsigned cliff_width,
                                       const unsigned cliff_height);

#endif
