#include <stdio.h>
#include <stdlib.h>

#define WIDTH 256
#define HEIGHT 256
#define MAX_PATH 4096

static char map[HEIGHT][WIDTH];
static unsigned width = 0, height = 0;
static unsigned tile_size;
struct coord {
    unsigned x : 8;
    unsigned y : 8;
    unsigned f : 2;
};

static unsigned gcd(unsigned x, unsigned y)
{
    if (x < y)
        return gcd(y, x);
    if (y == 0)
        return x;
    return gcd(y, x % y);
}

static union { unsigned n; char t; } path[MAX_PATH];
static unsigned path_size;

static void input(void)
{
    char new_line = 1, c;
    unsigned x = 0, y = 0;
    while ((c = getchar()) != EOF)
        switch (c)
        {
            case '.':
            case '#':
            case ' ':
                if (x >= WIDTH || y >= HEIGHT)
                    exit(1);
                map[y][x] = c;
                ++x;
                new_line = 0;
                break;
            case '\n':
                if (new_line)
                    goto read_path;
                if (width < x)
                    width = x;
                for (; x < WIDTH; ++x)
                    map[y][x] = ' ';
                x = 0; ++y;
                new_line = 1;
                break;
            default:
                exit(1);
        }
read_path:
    height = y;
    if (height == 0)
        exit(1);
    tile_size = gcd(width, height);
    for (path_size = 0;; ++path_size)
        if (path_size % 2 == 0) // number
        {
            if (scanf("%u", &path[path_size].n) != 1)
                return;
        }
        else // turn
            switch (getchar())
            {
                case 'L':
                    path[path_size].t = -1;
                    break;
                case 'R':
                    path[path_size].t = 1;
                    break;
                default:
                    return;
            }
}

static struct coord adj[HEIGHT][WIDTH][4];

static void part1_loop(unsigned width, unsigned height,
        char (*get_map)(unsigned, unsigned),
        void (*set_east)(unsigned, unsigned, const struct coord *),
        void (*set_west)(unsigned, unsigned, const struct coord *))
{
    struct coord pos;
    for (unsigned y = 0; y < height; ++y)
    {
        pos.y = y;
        unsigned first = 0; // in case the first column is valid
        for (unsigned x = 0; x < width; ++x)
        {
            const unsigned n = (x + 1) % width;
            switch ((get_map(x, y) != ' ') << 1 | (get_map(n, y) != ' '))
            {
                case 0: // 0b00
                    break;
                case 1: // 0b01
                    first = n;
                    break;
                case 2: // 0b10
                    pos.x = first; pos.f = 0;
                    set_east(x, y, &pos);
                    pos.x = x; pos.f = 2;
                    set_west(first, y, &pos);
                    break;
                case 3: // 0b11
                    pos.x = n; pos.f = 0;
                    set_east(x, y, &pos);
                    pos.x = x; pos.f = 2;
                    set_west(n, y, &pos);
                    break;
            }
        }
    }
}
static char get_map(unsigned x, unsigned y)
{ return map[y][x]; }
static char get_map_tr(unsigned x, unsigned y)
{ return get_map(y, x); }
static void set_east(unsigned x, unsigned y, const struct coord *dest)
{
    adj[y][x][0].x = dest->x;
    adj[y][x][0].y = dest->y;
    adj[y][x][0].f = dest->f;
}
static void set_west(unsigned x, unsigned y, const struct coord *dest)
{
    adj[y][x][2].x = dest->x;
    adj[y][x][2].y = dest->y;
    adj[y][x][2].f = dest->f;
}
static void set_south(unsigned x, unsigned y, const struct coord *dest)
{
    adj[x][y][1].x = dest->y;
    adj[x][y][1].y = dest->x;
    adj[x][y][1].f = dest->f ^ 1;
}
static void set_north(unsigned x, unsigned y, const struct coord *dest)
{
    adj[x][y][3].x = dest->y;
    adj[x][y][3].y = dest->x;
    adj[x][y][3].f = dest->f ^ 1;
}
static void part1(void)
{
    part1_loop(width, height, get_map, set_east, set_west);
    part1_loop(height, width, get_map_tr, set_south, set_north);
}

static void part2_switch(unsigned i, const struct coord *et, struct coord *e)
{
    switch (et->f)
    {
        case 0:
            e->x = et->x * tile_size;
            e->y = et->y * tile_size + i;
            break;
        case 1:
            e->x = (et->x + 1) * tile_size - 1 - i;
            e->y = et->y * tile_size;
            break;
        case 2:
            e->x = (et->x + 1) * tile_size - 1;
            e->y = (et->y + 1) * tile_size - 1 - i;
            break;
        case 3:
            e->x = et->x * tile_size + i;
            e->y = (et->y + 1) * tile_size - 1;
    }
    e->f = et->f;
}
static void part2(void)
{
    struct coord net[5][5][4] = {0};
    for (unsigned y = 0; y < height / tile_size; ++y)
        for (unsigned x = 0; x < width / tile_size; ++x)
            for (unsigned f = 0; f < 4; ++f)
            {
                net[y][x][f].x = 255;
                if (       (f == 0 && x + 1 == width / tile_size)
                        || (f == 1 && y + 1 == height / tile_size)
                        || (f == 2 && x == 0)
                        || (f == 3 && y == 0))
                    continue;
                if (map[y * tile_size][x * tile_size] == ' ')
                    continue;
                unsigned dx = f == 0 ? x + 1 : f == 2 ? x - 1 : x;
                unsigned dy = f == 1 ? y + 1 : f == 3 ? y - 1 : y;
                if (map[dy * tile_size][dx * tile_size] == ' ')
                    continue;
                net[y][x][f].x = dx;
                net[y][x][f].y = dy;
                net[y][x][f].f = f;
            }
    for (unsigned i = 0; i < 7; ++i)
        for (unsigned y = 0; y < height / tile_size; ++y)
            for (unsigned x = 0; x < width / tile_size; ++x)
            {
                if (map[y * tile_size][x * tile_size] == ' ')
                    continue;
                for (unsigned f = 0; f < 4; ++f)
                {
                    const struct coord *const l = &net[y][x][f];
                    const struct coord *const r = &net[y][x][(f + 1) % 4];
                    if (l->x == 255 || r->x == 255)
                        continue;
                    struct coord *const lr = &net[l->y][l->x][(l->f + 1) % 4];
                    struct coord *const rl = &net[r->y][r->x][(r->f + 3) % 4];
                    lr->x = r->x;
                    lr->y = r->y;
                    lr->f = (r->f + 1) % 4;
                    rl->x = l->x;
                    rl->y = l->y;
                    rl->f = (l->f + 3) % 4;
                }
            }
    for (unsigned y = 0; y < height / tile_size; ++y)
        for (unsigned x = 0; x < width / tile_size; ++x)
        {
            if (map[y * tile_size][x * tile_size] == ' ')
                continue;
            for (unsigned v = 0; v < tile_size; ++v)
                for (unsigned u = 0; u < tile_size; ++u)
                {
                    struct coord *const c = adj[y * tile_size + v][x * tile_size + u];
                    for (unsigned f = 0; f < 4; ++f)
                    {
                        c[f].x = x * tile_size + u;
                        c[f].y = y * tile_size + v;
                        c[f].f = f;
                    }
                    ++c[0].x;
                    ++c[1].y;
                    --c[2].x;
                    --c[3].y;
                }
            const struct coord *const et = &net[y][x][0];
            const struct coord *const st = &net[y][x][1];
            const struct coord *const wt = &net[y][x][2];
            const struct coord *const nt = &net[y][x][3];
            for (unsigned i = 0; i < tile_size; ++i)
            {
                part2_switch(i, et, &adj[y * tile_size + i][(x + 1) * tile_size - 1][0]);
                part2_switch(i, st, &adj[(y + 1) * tile_size - 1][(x + 1) * tile_size - 1 - i][1]);
                part2_switch(i, wt, &adj[(y + 1) * tile_size - 1 - i][x * tile_size][2]);
                part2_switch(i, nt, &adj[y * tile_size][x * tile_size + i][3]);
            }
        }
}

static unsigned solve(void (*compute_adj)(void))
{
    compute_adj();
    struct coord pos = {0};
    while (map[pos.y][pos.x] != '.')
        ++pos.x;
    for (unsigned i = 0; i < path_size; ++i)
        if (i % 2 == 0) // number
            for (int n = path[i].n; n > 0; --n)
            {
                const struct coord *dest = &adj[pos.y][pos.x][pos.f];
                if (map[dest->y][dest->x] == '#')
                    break;
                pos.x = dest->x;
                pos.y = dest->y;
                pos.f = dest->f;
            }
        else // turn
            pos.f = (pos.f + 4 + path[i].t) % 4;
    return 1000 * (pos.y + 1) + 4 * (pos.x + 1) + pos.f;
}

int main(void)
{
    input();
    printf("%d\n", solve(part1));
    printf("%d\n", solve(part2));
    return 0;
}
