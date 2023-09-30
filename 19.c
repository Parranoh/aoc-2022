#include <stdio.h>

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define MAX_BLUEPRINTS 32
#define TIME_1 24
#define TIME_2 32
#define N_BLUEPRINTS_2 3

static struct blueprint {
    int id;
    int ore_ore;
    int clay_ore;
    int obsidian_ore, obsidian_clay;
    int geode_ore, geode_obsidian;
} blueprints[MAX_BLUEPRINTS];
static size_t n_blueprints;

void input(void)
{
    for (n_blueprints = 0;
            scanf("Blueprint %d: "
                "Each ore robot costs %d ore. "
                "Each clay robot costs %d ore. "
                "Each obsidian robot costs %d ore and %d clay. "
                "Each geode robot costs %d ore and %d obsidian.\n",
                &blueprints[n_blueprints].id,
                &blueprints[n_blueprints].ore_ore,
                &blueprints[n_blueprints].clay_ore,
                &blueprints[n_blueprints].obsidian_ore, &blueprints[n_blueprints].obsidian_clay,
                &blueprints[n_blueprints].geode_ore, &blueprints[n_blueprints].geode_obsidian) == 7;
            ++n_blueprints);
}

struct state {
    int ore, clay, obsidian, geode;
    int ore_robots, clay_robots, obsidian_robots, geode_robots;
};
static const struct state init_state = {0, 0, 0, 0, 1, 0, 0, 0};

static char can_buy_ore_robot(const struct blueprint *bp, const struct state *s)
{ return s->ore >= bp->ore_ore; }
static char can_buy_clay_robot(const struct blueprint *bp, const struct state *s)
{ return s->ore >= bp->clay_ore; }
static char can_buy_obsidian_robot(const struct blueprint *bp, const struct state *s)
{ return s->ore >= bp->obsidian_ore && s->clay >= bp->obsidian_clay; }
static char can_buy_geode_robot(const struct blueprint *bp, const struct state *s)
{ return s->ore >= bp->geode_ore && s->obsidian >= bp->geode_obsidian; }
static int saving_for_obsidian_robot(const struct blueprint *bp, const struct state *s)
{ return (s->ore < bp->obsidian_ore || (s->clay < bp->obsidian_clay && s->clay_robots > 0)); }
static int saving_for_geode_robot(const struct blueprint *bp, const struct state *s)
{ return (s->ore < bp->geode_ore || (s->obsidian < bp->geode_obsidian && s->obsidian_robots > 0)); }

static void buy_ore_robot(const struct blueprint *bp, struct state *s)
{ s->ore -= bp->ore_ore; ++s->ore_robots; }
static void buy_clay_robot(const struct blueprint *bp, struct state *s)
{ s->ore -= bp->clay_ore; ++s->clay_robots; }
static void buy_obsidian_robot(const struct blueprint *bp, struct state *s)
{ s->ore -= bp->obsidian_ore; s->clay -= bp->obsidian_clay; ++s->obsidian_robots; }
static void buy_geode_robot(const struct blueprint *bp, struct state *s)
{ s->ore -= bp->geode_ore; s->obsidian -= bp->geode_obsidian; ++s->geode_robots; }

static void tick(struct state *s)
{
    s->ore += s->ore_robots;
    s->clay += s->clay_robots;
    s->obsidian += s->obsidian_robots;
    s->geode += s->geode_robots;
}

static int max_geodes_brute_force(const struct blueprint *bp, size_t time)
{
    int res = 0;
    int max_ore_cost = MAX(bp->clay_ore, bp->obsidian_ore);
    max_ore_cost = MAX(max_ore_cost, bp->geode_ore);
    struct frame {
        struct state s;
        enum {
            nop = 0,
            ore = 1,
            clay = 2,
            obsidian = 3,
            geode = 4,
            done = 5
        } n;
    } stack[TIME_2];
    stack[0].s = init_state;
    stack[0].n = 0;
    size_t index = 0;
    while (1)
    {
        struct frame *const f = &stack[index];
        if (index == time - 1)
        {
            tick(&f->s);
            res = MAX(res, f->s.geode);
            --index;
            continue;
        }

        switch (f->n)
        {
            case nop:
                // do not nop unless we are saving for something or time is almost up
                if (index >= time - 5
                    || !can_buy_ore_robot(bp, &f->s)
                    || !can_buy_clay_robot(bp, &f->s)
                    || saving_for_obsidian_robot(bp, &f->s)
                    || saving_for_geode_robot(bp, &f->s))
                {
                    struct frame *const f_ = &stack[++index];
                    f_->s = f->s;
                    tick(&f_->s);
                    f_->n = 0;
                }
                break;
            case ore:
                if (f->s.ore_robots < max_ore_cost && can_buy_ore_robot(bp, &f->s) && index < time - 3)
                {
                    struct frame *const f_ = &stack[++index];
                    f_->s = f->s;
                    tick(&f_->s);
                    buy_ore_robot(bp, &f_->s);
                    f_->n = 0;
                }
                break;
            case clay:
                if (f->s.clay_robots < bp->obsidian_clay && can_buy_clay_robot(bp, &f->s) && f->s.obsidian_robots < bp->geode_obsidian && index < time - 5)
                {
                    struct frame *const f_ = &stack[++index];
                    f_->s = f->s;
                    tick(&f_->s);
                    buy_clay_robot(bp, &f_->s);
                    f_->n = 0;
                }
                break;
            case obsidian:
                if (f->s.obsidian_robots < bp->geode_obsidian && can_buy_obsidian_robot(bp, &f->s) && index < time - 3)
                {
                    struct frame *const f_ = &stack[++index];
                    f_->s = f->s;
                    tick(&f_->s);
                    buy_obsidian_robot(bp, &f_->s);
                    f_->n = 0;
                }
                break;
            case geode:
                if (can_buy_geode_robot(bp, &f->s))
                {
                    struct frame *const f_ = &stack[++index];
                    f_->s = f->s;
                    tick(&f_->s);
                    buy_geode_robot(bp, &f_->s);
                    f_->n = 0;
                }
                break;
            case done:
                if (index == 0)
                    return res;
                else
                    --index;
        }
        ++f->n;
    }
}

static int part1(void)
{
    int res = 0;
    for (size_t i = 0; i < n_blueprints; ++i)
    {
        const struct blueprint *const bp = &blueprints[i];
        res += bp->id * max_geodes_brute_force(bp, TIME_1);
    }
    return res;
}

static int part2(void)
{
    int res = 1;
    for (size_t i = 0; i < MIN(N_BLUEPRINTS_2, n_blueprints); ++i)
    {
        const int m = max_geodes_brute_force(&blueprints[i], TIME_2);
        res *= m;
        printf(" %d\n", m);
    }
    return res;
}

int main(void)
{
    input();
    printf("%d\n", part1());
    printf("%d\n", part2());
    return 0;
}
