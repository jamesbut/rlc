#ifndef _CLIFF_WALKING_H_
#define _CLIFF_WALKING_H_

struct Agent 
{
    unsigned x_pos;
    unsigned y_pos;
};

struct Cliff
{
    unsigned width;
    unsigned height;
    struct Agent agent;
};

void display_agent(struct Agent agent);
void display_cliff(struct Cliff cliff);

void update_state(unsigned action);

#endif
