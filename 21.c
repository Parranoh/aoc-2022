#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)

#define NAME_LENGTH 4
#define MAX_MONKEYS 2000
#define ROOT_NAME "root"
#define HUMAN_NAME "humn"

typedef int64_t i64;

static struct monkey {
    char operator;
    struct monkey *parent;
    union {
        i64 number; // valid iff operator == '\0'
        struct { struct monkey *lhs, *rhs; };
    };
} monkeys[MAX_MONKEYS] = {0};
static size_t n_monkeys;
static struct monkey *root = NULL;
static struct monkey *human = NULL;

static void input(void)
{
    char names[MAX_MONKEYS][3][NAME_LENGTH] = {0};
    for (n_monkeys = 0; scanf("%" STRINGIFY(NAME_LENGTH) "s: ", names[n_monkeys][0]) == 1; ++n_monkeys)
    {
        if (scanf("%ld\n", &monkeys[n_monkeys].number) == 1)
            continue;
        if (scanf("%" STRINGIFY(NAME_LENGTH) "s %c %" STRINGIFY(NAME_LENGTH) "s\n",
                    names[n_monkeys][1],
                    &monkeys[n_monkeys].operator,
                    names[n_monkeys][2]) == 3)
            continue;
        exit(1);
    }
    for (size_t i = 0; i < n_monkeys; ++i)
    {
        if (bcmp(names[i][0], ROOT_NAME, NAME_LENGTH) == 0)
            root = &monkeys[i];
        else if (bcmp(names[i][0], HUMAN_NAME, NAME_LENGTH) == 0)
            human = &monkeys[i];
        for (size_t j = 0; j < n_monkeys; ++j)
            if (bcmp(names[i][0], names[j][1], NAME_LENGTH) == 0)
            {
                monkeys[j].lhs = &monkeys[i];
                monkeys[i].parent = &monkeys[j];
            }
            else if (bcmp(names[i][0], names[j][2], NAME_LENGTH) == 0)
            {
                monkeys[j].rhs = &monkeys[i];
                monkeys[i].parent = &monkeys[j];
            }
    }
    if (!root || !human)
        exit(1);
}

static i64 part1(const struct monkey *m)
{
    switch (m->operator)
    {
        case '\0': return m->number;
        case '+':  return part1(m->lhs) + part1(m->rhs);
        case '-':  return part1(m->lhs) - part1(m->rhs);
        case '*':  return part1(m->lhs) * part1(m->rhs);
        case '/':  return part1(m->lhs) / part1(m->rhs);
        default:   exit(1);
    }
}

static i64 part2(struct monkey *p, struct monkey *m)
{
    const char c_is_lhs = m == p->lhs;
    const i64 ohs = part1(c_is_lhs ? p->rhs : p->lhs);
    if (p == root)
        return ohs;
    const i64 res = part2(p->parent, p);
    switch (p->operator)
    {
        case '+': return res - ohs;
        case '-': return c_is_lhs ? ohs + res : ohs - res;
        case '*': return res / ohs;
        case '/': return c_is_lhs ? ohs * res : ohs / res;
        default:  exit(1);
    }
}

int main(void)
{
    input();
    printf("%ld\n", part1(root));
    printf("%ld\n", part2(human->parent, human));
    return 0;
}
