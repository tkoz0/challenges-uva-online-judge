#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

// this is pretty much a reuse of p100 code with some changes

typedef uint64_t u32;
#define CACHE_SIZE 1000000
#define U32_MAX_SAFE_3N_1 1431655765

int main(int argc, char **argv)
{
    u32 a, b;

    // cache to speed up computation, index i stores 0 or its sequence length
    u32 *cache = (u32*) calloc(CACHE_SIZE, sizeof(u32));
    // place 1 in the cache as the initial "base case"
    cache[1] = 1;

    // main loop, process each input line
    while (scanf("%lu %lu", &a, &b) == 2)
    {
        if (a == 0 and b == 0) break;
        // specification says inputs between 0 and 10000
        // submitting with the assert uncommented causes a runtime error
//        assert(a > 0 and a < 10000 and b > 0 and b < 10000);
        // choose proper order
        u32 start, end;
        if (a < b) start = a, end = b;
        else start = b, end = a;

        // main loop, determine length of all sequences
        u32 original_start = start;
        u32 longest = 1, bestnum = start;
        for (; start <= end; ++start)
        {
            u32 i = start, len = 0;
            for (;;)
            {
                // use value existing in cache if present
                if (i < CACHE_SIZE and cache[i])
                {
                    len += cache[i];
                    break;
                }
                // otherwise generate next in sequence
                if (i & 1) // odd
                {
                    // limit to prevent integer overflow
//                    assert(i < U32_MAX_SAFE_3N_1);
                    i = 3 * i + 1;
                    ++len;
                }
                else i >>= 1, ++len;
            }
            // include in cache
            if (start < CACHE_SIZE and !cache[start]) cache[start] = len;
            if (start == 1) len = 4; // special case for p371 (see examples)
            // update longest
            if (len > longest) longest = len, bestnum = start;
        }
        start = original_start;
        // output the 2 inputs in original order along with max length
//        printf("%u %u %u\n", a, b, longest);
        printf("Between %lu and %lu, %lu generates",start,end,bestnum);
        printf(" the longest sequence of %lu values.\n",longest-1);
    }
    free(cache);
    return 0;
}
