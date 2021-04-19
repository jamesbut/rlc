#ifndef _EXPERIENCE_TUPLE_H_
#define _EXPERIENCE_TUPLE_H_

#include <stdbool.h>

struct Experience
{
    unsigned s;
    unsigned a;
    unsigned s_prime;
    double r;
    bool episode_end;
};

#endif
