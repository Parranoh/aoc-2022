#include <stdio.h>

#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define MAX_BLUEPRINTS 32
#define TIME 24

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

static int div_ceil(int x, int y)
{ return (x + y - 1) / y; }

struct state {
    int ore, clay, obsidian, geode;
    int ore_robots, clay_robots, obsidian_robots, geode_robots;
};
static const struct state init_state = {0, 0, 0, 0, 1, 0, 0, 0};

static int t_ore(const struct blueprint *bp, const struct state *s)
{ return div_ceil(bp->ore_ore - s->ore, s->ore_robots); }

static int t_clay(const struct blueprint *bp, const struct state *s)
{ return div_ceil(bp->clay_ore - s->ore, s->ore_robots); }

static int t_obsidian(const struct blueprint *bp, const struct state *s)
{
    if (s->clay_robots <= 0)
        return -1;
    const int t_obsidian_ore = div_ceil(bp->obsidian_ore - s->ore, s->ore_robots);
    const int t_obsidian_clay = div_ceil(bp->obsidian_clay - s->clay, s->clay_robots);
    return MAX(t_obsidian_ore, t_obsidian_clay);
}

static int t_geode(const struct blueprint *bp, const struct state *s)
{
    if (s->obsidian_robots <= 0)
        return -1;
    const int t_geode_ore = div_ceil(bp->geode_ore - s->ore, s->ore_robots);
    const int t_geode_obsidian = div_ceil(bp->geode_obsidian - s->obsidian, s->obsidian_robots);
    return MAX(t_geode_ore, t_geode_obsidian);
}

static void buy_ore_robot(const struct blueprint *bp, struct state *s)
{ s->ore -= bp->ore_ore; ++s->ore_robots; }
static void buy_clay_robot(const struct blueprint *bp, struct state *s)
{ s->ore -= bp->clay_ore; ++s->clay_robots; }
static void buy_obsidian_robot(const struct blueprint *bp, struct state *s)
{ s->ore -= bp->obsidian_ore; s->clay -= bp->obsidian_clay; ++s->obsidian_robots; }
static void buy_geode_robot(const struct blueprint *bp, struct state *s)
{ s->ore -= bp->geode_ore; s->obsidian -= bp->geode_obsidian; ++s->geode_robots; }

static void unbuy_ore_robot(const struct blueprint *bp, struct state *s)
{ s->ore += bp->ore_ore; --s->ore_robots; }
static void unbuy_clay_robot(const struct blueprint *bp, struct state *s)
{ s->ore += bp->clay_ore; --s->clay_robots; }
static void unbuy_obsidian_robot(const struct blueprint *bp, struct state *s)
{ s->ore += bp->obsidian_ore; s->clay += bp->obsidian_clay; --s->obsidian_robots; }
/* static void unbuy_geode_robot(const struct blueprint *bp, struct state *s) */
/* { s->ore += bp->geode_ore; s->obsidian += bp->geode_obsidian; --s->geode_robots; } */

int max_geodes_greedy(const struct blueprint *bp)
{
    struct state s = init_state;
    for (unsigned char t = 0; t < TIME; ++t)
    {
        struct state s_ = s;
        s_.ore      += s.ore_robots;
        s_.clay     += s.clay_robots;
        s_.obsidian += s.obsidian_robots;
        s_.geode    += s.geode_robots;

        const int t_g = t_geode(bp, &s);
        const int t_o = t_obsidian(bp, &s);
        const int t_c = t_clay(bp, &s);
        if (t_g == 0)
            buy_geode_robot(bp, &s_);
        else if (t_o == 0)
        {
            buy_obsidian_robot(bp, &s_);
            if (t_g >= 0 && t_g <= t_geode(bp, &s))
            {
                unbuy_obsidian_robot(bp, &s);
                goto try_clay;
            }
        }
        else try_clay: if (t_c == 0)
        {
            buy_clay_robot(bp, &s_);
            if (t_o >= 0 && t_o <= t_obsidian(bp, &s))
            {
                unbuy_clay_robot(bp, &s);
                goto try_ore;
            }
        }
        else try_ore: if (t_ore(bp, &s) == 0)
        {
            buy_ore_robot(bp, &s_);
            if (t_c >= 0 && t_c <= t_clay(bp, &s))
                unbuy_ore_robot(bp, &s);
        }

        s = s_;
    }
    return s.geode;
}

int part1(void)
{
    int res = 0;
    for (size_t i = 0; i < n_blueprints; ++i)
    {
        const struct blueprint *const bp = &blueprints[i];
        const int geodes = max_geodes_greedy(bp);
        printf("Blueprint %d yields %d geodes.\n", bp->id, geodes);
        res += bp->id * geodes;
    }
    return res;
}

int main(void)
{
    input();
    printf("%d\n", part1());
    return 0;
}
