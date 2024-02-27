#include "combinatorics.h"

uint64_t factorial(uint64_t val)
{   
    uint64_t result{1};
    for (uint64_t i = 1; i <= val; ++i)
    {
        result *= i;
    }
    return result;
}

uint64_t permutation(uint64_t val, uint64_t val2)
{   
    if (val < val2)
        return 0;
    uint64_t numerator{factorial(val)};
    uint64_t denominator{factorial(val - val2)};

    uint64_t result{numerator / denominator};
    return result;
}

uint64_t combination(uint64_t val, uint64_t val2)
{
    if (val < val2)
        return 0;
    uint64_t numerator{factorial(val)};
    uint64_t denominator{factorial(val2) * factorial(val - val2)};

    uint64_t result{numerator / denominator};
    return result;
}
