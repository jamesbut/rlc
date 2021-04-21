#include <optimisers/potential_function.h>
#include <stdlib.h>

double potential_function(const unsigned state, const struct Cliff* cliff,
                          const unsigned potential_function_type)
{
    switch(potential_function_type)
    {
        case 0:
            return 0.;

        case 1:
            //Add 2 to manhattan distance if start state because the cliff has to
            //be avoided
            if(state == start_state(cliff))
                return goal_state(cliff)-manhattan_distance(state, goal_state(cliff),
                                                            cliff)+2;
            else
                return goal_state(cliff)-manhattan_distance(state, goal_state(cliff),
                                                            cliff);

        default:
            return 0.;
    }
}

unsigned manhattan_distance(const unsigned state_1, const unsigned state_2,
                            const struct Cliff* cliff)
{
    const struct Position pos_1 = state_to_coords(state_1, cliff);
    const struct Position pos_2 = state_to_coords(state_2, cliff);

    const unsigned x_dist = abs(pos_1.x - pos_2.x);
    const unsigned y_dist = abs(pos_1.y - pos_2.y);

    return x_dist + y_dist;
}
