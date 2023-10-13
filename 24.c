#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 4096
#define EMPTY 0
#define WALL 1
#define UP 2
#define DOWN 4
#define LEFT 8
#define RIGHT 16

char map[MAX_SIZE];
size_t width, height, size;
#define START 1
#define TARGET (size - 2)

void input(void)
{
    size = height = 0;
    while (1)
        switch (getchar())
        {
            case EOF:  width = size / height; return;
            case '\n': ++height; break;
            case '.':  map[size++] = EMPTY; break;
            case '#':  map[size++] = WALL;  break;
            case '^':  map[size++] = UP;    break;
            case 'v':  map[size++] = DOWN;  break;
            case '<':  map[size++] = LEFT;  break;
            case '>':  map[size++] = RIGHT; break;
            default:   exit(1);
        }
}

char *reachable;
char *reachable2;
char *blizzards;
char *blizzards2;
unsigned time;

void destroy(void)
{
    free(reachable);
    free(reachable2);
    free(blizzards);
    free(blizzards2);
}

void init(void)
{
    atexit(destroy);
    destroy();
    reachable  = malloc(size * sizeof(char));
    reachable2 = malloc(size * sizeof(char));
    blizzards  = malloc(size * sizeof(char));
    blizzards2 = malloc(size * sizeof(char));
    if (!reachable || !reachable2 || !blizzards || !blizzards2)
        exit(1);
    memcpy(blizzards, map, size * sizeof(char));
    time = 0;
}

void solve(size_t start, size_t target)
{
    memset(reachable, 0, size * sizeof(char));
    reachable[start] = 1;
    for (; !reachable[target]; ++time)
    {
        memset(reachable2, 0, size * sizeof(char));
        memset(blizzards2, 0, size * sizeof(char));
        for (size_t y = 0; y < height; ++y)
            for (size_t x = 0; x < width; ++x)
            {
                const size_t i = y * width + x;
                const char r = reachable[i];
                reachable2[i] |= r;
                if (1 <= x)
                    reachable2[i - 1] |= r;
                if (x + 1 < width)
                    reachable2[i + 1] |= r;
                if (1 <= y)
                    reachable2[i - width] |= r;
                if (y + 1 < height)
                    reachable2[i + width] |= r;

                char const b = blizzards[i];
                if (b == 1)
                    blizzards2[i] = b;
                else if (b)
                {
                    if (b & UP)
                    {
                        if (1 < y)
                            blizzards2[i - width] |= UP;
                        else
                            blizzards2[(height - 2) * width + x] |= UP;
                    }
                    if (b & DOWN)
                    {
                        if (y < height - 2)
                            blizzards2[i + width] |= DOWN;
                        else
                            blizzards2[width + x] |= DOWN;
                    }
                    if (b & LEFT)
                    {
                        if (1 < x)
                            blizzards2[i - 1] |= LEFT;
                        else
                            blizzards2[y * width + width - 2] |= LEFT;
                    }
                    if (b & RIGHT)
                    {
                        if (x < width - 2)
                            blizzards2[i + 1] |= RIGHT;
                        else
                            blizzards2[y * width + 1] |= RIGHT;
                    }
                }
            }

        for (size_t i = 0; i < size; ++i)
            if (blizzards2[i])
                reachable2[i] = 0;

        char *tmp = reachable;
        reachable = reachable2;
        reachable2 = tmp;
        tmp = blizzards;
        blizzards = blizzards2;
        blizzards2 = tmp;
    }
}

unsigned part1(void)
{
    solve(START, TARGET);
    return time;
}

unsigned part2(void)
{
    solve(TARGET, START);
    solve(START, TARGET);
    return time;
}

int main(void)
{
    input();
    init();
    printf("%u\n", part1());
    printf("%u\n", part2());
    return 0;
}
