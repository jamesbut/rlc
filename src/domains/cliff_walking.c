#include <domains/cliff_walking.h>
#include <stdio.h>
#include <stdlib.h>

struct CliffWalking create_cliff_walking(const unsigned cliff_width,
                                         const unsigned cliff_height)
{
    //Create cliff
    struct Cliff cliff;
    cliff.width = cliff_width;
    cliff.height = cliff_height;
    reset_agent(&cliff);

    struct CliffWalking cliff_walking;
    cliff_walking.cliff = cliff;
    cliff_walking.reward_table = build_reward_table(&cliff);

    return cliff_walking;

}

double* build_reward_table(const struct Cliff* cliff)
{
    double* reward_table = calloc(cliff->width * cliff->height, sizeof(double));

    for(unsigned i = 0; i < cliff->height; i++)
        for(unsigned j = 0; j < cliff->width; j++)
    {
        const unsigned index = i * cliff->width + j;
        //-100 for falling off the cliff
        if(is_drop(j, i, cliff))
            reward_table[index] = -100.;
        //10 for getting to the goal
        else if(is_finish(j, i, cliff))
            reward_table[index] = 10;
        //-1 for every step taken
        else
            reward_table[index] = -1.;
    }

    return reward_table;
}

double reward_function(const struct CliffWalking* cliff_walking)
{
    return cliff_walking->reward_table[cliff_walking->cliff.agent.state];
}

struct Experience agent_step(struct CliffWalking* cliff_walking, const unsigned action)
{

    struct Experience experience;
    experience.s = cliff_walking->cliff.agent.state;
    experience.a = action;

    experience.episode_end = move_agent(action, &cliff_walking->cliff);

    experience.s_prime = cliff_walking->cliff.agent.state;
    experience.r = reward_function(cliff_walking);

    return experience;
}
