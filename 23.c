#include <stdio.h>
#include <stdlib.h>

#define START_DIMEN 128
#define N_ROUNDS 10

static unsigned char *map;
static size_t max_dimen;

static void input(void)
{
    max_dimen = START_DIMEN;
    map = calloc(max_dimen * max_dimen, sizeof(unsigned char));
    if (!map)
        exit(1);
    size_t x = N_ROUNDS, y = N_ROUNDS;
    char c;
    while ((c = getchar()) != EOF)
        switch (c)
        {
            case '\n':
                x = N_ROUNDS;
                ++y;
                break;
            case '#':
                map[y * max_dimen + x] = 1;
                __attribute__ ((fallthrough));
            case '.':
                ++x;
                break;
            default:
                exit(1);
        }
}

static char first_dir = 0;

static void propose(size_t x, size_t y)
{
    if (!(map[y * max_dimen + x] & 1))
        return;
    unsigned char *const nw = &map[(y - 1) * max_dimen + x - 1];
    unsigned char *const n  = &map[(y - 1) * max_dimen + x    ];
    unsigned char *const ne = &map[(y - 1) * max_dimen + x + 1];
    unsigned char *const w  = &map[ y      * max_dimen + x - 1];
    unsigned char *const e  = &map[ y      * max_dimen + x + 1];
    unsigned char *const sw = &map[(y + 1) * max_dimen + x - 1];
    unsigned char *const s  = &map[(y + 1) * max_dimen + x    ];
    unsigned char *const se = &map[(y + 1) * max_dimen + x + 1];
    if (!(         *nw & 1 || *n & 1 || *ne & 1
                || *w  & 1           || *e  & 1
                || *sw & 1 || *s & 1 || *se & 1))
        return;
    unsigned char *dest = NULL;
    unsigned dir;
    for (char i = 0; i < 4; ++i)
    {
        switch (dir = (first_dir + i) % 4)
        {
            case 0:
                if (*n & 1 || *ne & 1 || *nw & 1)
                    continue;
                dest = n;
                break;
            case 1:
                if (*s & 1 || *se & 1 || *sw & 1)
                    continue;
                dest = s;
                break;
            case 2:
                if (*w & 1 || *nw & 1 || *sw & 1)
                    continue;
                dest = w;
                break;
            case 3:
                if (*e & 1 || *ne & 1 || *se & 1)
                    continue;
                dest = e;
                break;
            default:
                exit(1);
        }
        if (*dest & 2)
            *dest |= 4;
        else
            *dest |= 2;
        map[y * max_dimen + x] |= 1 << (dir + 3);
        break;
    }
}

static char step(void)
{
    for (size_t i = 0; i < max_dimen; ++i)
        if (       map[                              i]
                || map[(max_dimen - 1) * max_dimen + i]
                || map[ i              * max_dimen]
                || map[ i              * max_dimen + max_dimen - 1])
        {
            unsigned char *const new = calloc(9 * max_dimen * max_dimen, sizeof(char));
            if (!new)
                exit(1);
            for (size_t y = 0; y < max_dimen; ++y)
                for (size_t x = 0; x < max_dimen; ++x)
                    new[(max_dimen + y) * (3 * max_dimen) + max_dimen + x] = map[y * max_dimen + x];
            free(map);
            map = new;
            max_dimen *= 3;
            break;
        }
    char changed = 0;
    for (size_t y = 0; y < max_dimen; ++y)
        for (size_t x = 0; x < max_dimen; ++x)
            if (map[y * max_dimen + x] & 1)
                propose(x, y);
    for (size_t y = 0; y < max_dimen; ++y)
        for (size_t x = 0; x < max_dimen; ++x)
        {
            unsigned char *dest;
            switch (map[y * max_dimen + x] >> 3)
            {
                case 0: continue;
                case 1: dest = &map[(y - 1) * max_dimen + x    ]; break;
                case 2: dest = &map[(y + 1) * max_dimen + x    ]; break;
                case 4: dest = &map[ y      * max_dimen + x - 1]; break;
                case 8: dest = &map[ y      * max_dimen + x + 1]; break;
                default: exit(1);
            }
            if (!(*dest & 4))
            {
                map[y * max_dimen + x] = 0;
                *dest |= 1;
                changed = 1;
            }
        }
    for (size_t y = 0; y < max_dimen; ++y)
        for (size_t x = 0; x < max_dimen; ++x)
            map[y * max_dimen + x] &= 1;
    first_dir = (first_dir + 1) % 4;
    return changed;
}

static size_t part1(void)
{
    for (char i = 0; i < N_ROUNDS; ++i)
        step();
    size_t minx = max_dimen, maxx = 0;
    size_t miny = max_dimen, maxy = 0;
    for (size_t y = 0; y < max_dimen; ++y)
        for (size_t x = 0; x < max_dimen; ++x)
            if (map[y * max_dimen + x])
            {
                if (x < minx)
                    minx = x;
                if (maxx < x)
                    maxx = x;
                if (y < miny)
                    miny = y;
                if (maxy < y)
                    maxy = y;
            }
    size_t res = 0;
    for (size_t y = miny; y <= maxy; ++y)
        for (size_t x = minx; x <= maxx; ++x)
            if (!map[y * max_dimen + x])
                ++res;
    return res;
}

static size_t part2(void)
{
    size_t res = N_ROUNDS;
    while (step())
        ++res;
    return res + 1;
}

int main(void)
{
    input();
    printf("%ld\n", part1());
    printf("%ld\n", part2());
    free(map);
    return 0;
}
