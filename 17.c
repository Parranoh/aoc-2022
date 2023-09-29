#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 7
#define ROCK_SIZE 4
#define N_SHAPES 5
#define LEFT '<'
#define START_X 2
#define START_Y 3

#define N_ROCKS_1 2022
#define N_ROCKS_2 1000000000000

typedef char bool;
typedef int64_t i64;

static char *jets = NULL;
static size_t n_jets;

static void input(void)
{
    if (scanf("%m[<>]\n", &jets) < 1)
        exit(1);
    n_jets = strlen(jets);
}

static const char *rocks =
    "####"
    "...."
    "...."
    "...."

    ".#.."
    "###."
    ".#.."
    "...."

    "###."
    "..#."
    "..#."
    "...."

    "#..."
    "#..."
    "#..."
    "#..."

    "##.."
    "##.."
    "...."
    "....";
static const i64 shape_heights[] = {1, 3, 3, 4, 2};
static const i64 shape_widths[]  = {4, 3, 3, 1, 2};

static bool *room = NULL;
static i64 max_y;
static size_t shape = 0;
static size_t rock = 0;
static size_t next_jet = 0;
static i64 tower_height = 0;

static bool get_pixel(i64 u, i64 v)
{
    return rocks[shape * ROCK_SIZE * ROCK_SIZE + v * ROCK_SIZE + u] == '#';
}

static bool fits(i64 x, i64 y)
{
    if (x < 0 || y < 0 || x + shape_widths[shape] > WIDTH)
        // out of bounds
        return 0;
    for (i64 v = 0; v < ROCK_SIZE; ++v)
        for (i64 u = 0; u < ROCK_SIZE; ++u)
            if (get_pixel(u, v) && room[(y + v) * WIDTH + x + u])
                // collision
                return 0;
    return 1;
}

static bool drop_rock(void)
{
    bool jets_wrapped = 0;
    shape = (rock++) % N_SHAPES;
    i64 x = START_X;
    i64 y = tower_height + START_Y;

    if (y + ROCK_SIZE >= max_y)
    {
        // need more memory
        const i64 new_max_y = max_y * 2;
        bool *const new_room = realloc(room, new_max_y * WIDTH);
        if (!new_room)
            exit(1);
        room = new_room;
        memset(&room[max_y * WIDTH], 0, max_y * WIDTH);
        max_y = new_max_y;
    }

    while (1)
    {
        // move sideways
        const i64 dx = jets[next_jet] == LEFT ? -1 : 1;
        if (++next_jet == n_jets)
        {
            next_jet = 0;
            jets_wrapped = 1;
        }
        if (fits(x + dx, y))
            x += dx;
        // move down
        const i64 dy = -1;
        if (fits(x, y + dy))
            y += dy;
        else
            break;
    }
    const i64 top = y + shape_heights[shape];
    if (top > tower_height)
        tower_height = top;
    // paint into room
    for (i64 v = 0; v < ROCK_SIZE; ++v)
        for (i64 u = 0; u < ROCK_SIZE; ++u)
            room[(y + v) * WIDTH + x + u] |= get_pixel(u, v);
    return jets_wrapped;
}

static size_t offset;
static i64 height_after_offset;
static size_t period;
static i64 height_per_period;

static size_t gains_capacity = 4 * N_SHAPES;
static char *gains = NULL;
static size_t gains_size = 0;

static void gains_push_back(char gain)
{
    if (!gains)
        gains = malloc(gains_capacity);
    if (gains_size >= gains_capacity)
    {
        gains_capacity *= 2;
        char *const new_gains = realloc(gains, gains_capacity);
        if (!new_gains)
            exit(1);
        gains = new_gains;
    }
    gains[gains_size++] = gain;
}

static void find_period(void)
{
    max_y = 3 * n_jets;
    room = calloc(WIDTH * max_y, sizeof(char));
    if (!room)
        exit(1);
    for (size_t i = 0; i < N_ROCKS_1; ++i)
        drop_rock();
    offset = rock;
    height_after_offset = tower_height;

    for (period = N_SHAPES;; period += N_SHAPES)
    {
        const size_t gains_needed = 4 * period;
        while (gains_size < gains_needed)
        {
            const i64 height1 = tower_height;
            drop_rock();
            gains_push_back(tower_height - height1);
        }
        for (size_t i = 0; i < period; ++i)
            if (gains[i] != gains[period + i]
                    || gains[i] != gains[2 * period + i]
                    || gains[i] != gains[3 * period + i])
                goto continue_outer;
        // if it is the same for four periods, we should be fine
        break;
continue_outer:;
    }
    height_per_period = (tower_height - height_after_offset) / 4;
}

static i64 solve(size_t n_rocks)
{
    if (n_rocks < N_ROCKS_1)
        exit(1);
    const size_t full_periods = (n_rocks - offset) / period;
    i64 res = height_after_offset + full_periods * height_per_period;
    const size_t remaining = n_rocks - offset - full_periods * period;
    for (size_t i = 0; i < remaining; ++i)
        res += gains[i];
    return res;
}

static void free_all(void)
{
    free(jets);
    free(room);
    free(gains);
}

int main(void)
{
    atexit(free_all);
    input();
    find_period();
    printf("%ld\n", solve(N_ROCKS_1));
    printf("%ld\n", solve(N_ROCKS_2));
    return 0;
}
