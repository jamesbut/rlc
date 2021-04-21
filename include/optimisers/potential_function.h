#ifndef _POTENTIAL_FUNCTION_H_
#define _POTENTIAL_FUNCTION_H_

#include <domains/cliff.h>

double potential_function(const unsigned state, const struct Cliff* cliff,
                          const unsigned potential_function_type);

//Manhattan distance between two states on a cliff
unsigned manhattan_distance(const unsigned state_1, const unsigned state_2,
                            const struct Cliff* cliff);

#endif
