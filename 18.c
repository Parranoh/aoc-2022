#include <stdio.h>
#include <stdlib.h>

#define MAX_COORD 22
#define MAX_COORD_2 (MAX_COORD * MAX_COORD)
#define N_CUBES (MAX_COORD * MAX_COORD * MAX_COORD)
#define ARRAY(a, x, y, z) (a[(z) * MAX_COORD_2 + (y) * MAX_COORD + (x)])

static char space[N_CUBES] = {0};

static void input(void)
{
    int x, y, z;
    while (scanf("%d,%d,%d\n", &x, &y, &z) == 3)
        ARRAY(space, x, y, z) = 1;
}

static size_t part1(void)
{
    size_t res = 0;
    for (int z = 0; z < MAX_COORD; ++z)
        for (int y = 0; y < MAX_COORD; ++y)
            for (int x = 0; x < MAX_COORD; ++x)
                if (ARRAY(space, x, y, z))
                {
                    res += 0 > x - 1 || !ARRAY(space, x - 1, y, z);
                    res += 0 > y - 1 || !ARRAY(space, x, y - 1, z);
                    res += 0 > z - 1 || !ARRAY(space, x, y, z - 1);
                    res += x + 1 >= MAX_COORD || !ARRAY(space, x + 1, y, z);
                    res += y + 1 >= MAX_COORD || !ARRAY(space, x, y + 1, z);
                    res += z + 1 >= MAX_COORD || !ARRAY(space, x, y, z + 1);
                }
    return res;
}

char on_exterior[N_CUBES] = {0};
struct coord { int x, y, z; } queue[N_CUBES];
size_t queue_size = 0;
void try(int x, int y, int z)
{
    if (       0 > x || x >= MAX_COORD
            || 0 > y || y >= MAX_COORD
            || 0 > z || z >= MAX_COORD)
        return;
    if (ARRAY(on_exterior, x, y, z) || ARRAY(space, x, y, z))
        return;
    ARRAY(on_exterior, x, y, z) = 1;
    struct coord *const c = &queue[queue_size++];
    c->x = x; c->y = y; c->z = z;
}
static size_t part2(void)
{
    for (int j = 0; j < MAX_COORD; ++j)
        for (int i = 0; i < MAX_COORD; ++i)
        {
            try(0, i, j);
            try(MAX_COORD - 1, i, j);
            try(i, 0, j);
            try(i, MAX_COORD - 1, j);
            try(i, j, 0);
            try(i, j, MAX_COORD - 1);
        }
    for (size_t i = 0; i < queue_size; ++i)
    {
        const struct coord *const c = &queue[i];
        try(c->x - 1, c->y, c->z);
        try(c->x, c->y - 1, c->z);
        try(c->x, c->y, c->z - 1);
        try(c->x + 1, c->y, c->z);
        try(c->x, c->y + 1, c->z);
        try(c->x, c->y, c->z + 1);
    }
    size_t res = 0;
    for (int z = 0; z < MAX_COORD; ++z)
        for (int y = 0; y < MAX_COORD; ++y)
            for (int x = 0; x < MAX_COORD; ++x)
                if (space[z * MAX_COORD_2 + y * MAX_COORD + x])
                {
                    res += 0 > x - 1 || ARRAY(on_exterior, x - 1, y, z);
                    res += 0 > y - 1 || ARRAY(on_exterior, x, y - 1, z);
                    res += 0 > z - 1 || ARRAY(on_exterior, x, y, z - 1);
                    res += x + 1 >= MAX_COORD || ARRAY(on_exterior, x + 1, y, z);
                    res += y + 1 >= MAX_COORD || ARRAY(on_exterior, x, y + 1, z);
                    res += z + 1 >= MAX_COORD || ARRAY(on_exterior, x, y, z + 1);
                }
    return res;
}

int main(void)
{
    input();
    printf("%ld\n", part1());
    printf("%ld\n", part2());
    return 0;
}
