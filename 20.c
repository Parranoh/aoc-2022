#include <stdio.h>
#include <stdlib.h>

#define ABS(x) ((x) < 0 ? -(x) : (x))

#define MAX_NUMBERS 5000
#define COORD_STEPS 1000
#define N_COORDS 3
#define DECRYPTION_KEY 811589153L
#define N_ROUNDS_2 10

typedef int64_t i64;

static struct list_node {
    struct list_node *prev;
    struct list_node *next;
    i64 val;
} list[MAX_NUMBERS];
static size_t n_numbers;
static size_t idx0;

static void input(void)
{
    for (n_numbers = 0; scanf("%ld\n", &list[n_numbers].val) == 1; ++n_numbers);
}

static void init_list(void)
{
    for (size_t i = 0; i < n_numbers; ++i)
    {
        list[i].prev = &list[(n_numbers + i - 1) % n_numbers];
        list[i].next = &list[(i + 1) % n_numbers];
        if (list[i].val == 0)
            idx0 = i;
    }
}

static void swap_forward(struct list_node *ln)
{
    struct list_node *const bw = ln->prev;
    struct list_node *const fw = ln->next;
    struct list_node *const fw2 = fw->next;
    bw->next = fw;
    fw->prev = bw;
    fw->next = ln;
    ln->prev = fw;
    ln->next = fw2;
    fw2->prev = ln;
}

static void swap_backward(struct list_node *ln)
{ swap_forward(ln->prev); }

void mix(void)
{
    const i64 n = n_numbers - 1;
    for (size_t i = 0; i < n_numbers; ++i)
    {
        const i64 steps = list[i].val % n;
        const i64 fw_steps = (steps + n) % n;
        const i64 bw_steps = fw_steps - n;
        if (ABS(bw_steps) < fw_steps)
            for (i64 j = 0; j < -bw_steps; ++j)
                swap_backward(&list[i]);
        else
            for (i64 j = 0; j < fw_steps; ++j)
                swap_forward(&list[i]);
    }
}

static i64 get_coords(void)
{
    i64 res = 0;
    const struct list_node *ln = &list[idx0];
    for (size_t i = 0; i < N_COORDS; ++i)
    {
        for (size_t j = 0; j < COORD_STEPS % n_numbers; ++j)
            ln = ln->next;
        res += ln->val;
    }
    return res;
}

static i64 part1(void)
{
    init_list();
    mix();
    return get_coords();
}

static i64 part2(void)
{
    for (size_t i = 0; i < n_numbers; ++i)
        list[i].val *= DECRYPTION_KEY;
    init_list();
    for (size_t i = 0; i < N_ROUNDS_2; ++i)
        mix();
    return get_coords();
}

int main(void)
{
    input();
    printf("%ld\n", part1());
    printf("%ld\n", part2());
    return 0;
}
