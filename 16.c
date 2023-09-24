#include <string.h>
#include <stdio.h>

#define STRINGIFY(x) _STRINGIFY(x)
#define _STRINGIFY(x) #x
#define MAX(x, y) ((x) < (y) ? (y) : (x))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

#define MAX_VERTICES 64
#define MAX_OUTDEGREE 5
#define LABEL_LENGTH 2
#define STR_LABEL_LENGTH STRINGIFY(LABEL_LENGTH)
#define START_LABEL "AA"
#define TIME_LIMIT 30
#define TEACHING_TIME 4

typedef struct vertex {
    char label[LABEL_LENGTH];
    int flow_rate;
    size_t outdegree;
    struct vertex *edges[MAX_OUTDEGREE];
} vertex;

size_t n_vertices;
vertex graph[MAX_VERTICES];

void input(void)
{
    char edges[MAX_VERTICES][MAX_OUTDEGREE][LABEL_LENGTH] = {0};
    for (n_vertices = 0; n_vertices < MAX_VERTICES; ++n_vertices)
    {
        if (scanf("Valve %" STR_LABEL_LENGTH "c has flow rate=%d; tunnel%*c lead%*c to valve%*c %" STR_LABEL_LENGTH "c ",
                graph[n_vertices].label, &graph[n_vertices].flow_rate, edges[n_vertices][0]) < 3)
            break;
        for (graph[n_vertices].outdegree = 1; graph[n_vertices].outdegree < MAX_OUTDEGREE; ++graph[n_vertices].outdegree)
            if (scanf(", %" STR_LABEL_LENGTH "c ", edges[n_vertices][graph[n_vertices].outdegree]) < 1)
                break;
    }
    for (size_t v = 0; v < n_vertices; ++v)
        for (size_t e = 0; e < graph[v].outdegree; ++e)
            // find vertex with same label
            for (size_t w = 0; w < n_vertices; ++w)
                if (strncmp(edges[v][e], graph[w].label, LABEL_LENGTH) == 0)
                {
                    graph[v].edges[e] = &graph[w];
                    break;
                }
}

size_t n_valves;
size_t valves[MAX_VERTICES]; // indices into graph
size_t start_index;

void populate_valves(void)
{
    n_valves = 0;
    for (size_t v = 0; v < n_vertices; ++v)
        if (graph[v].flow_rate > 0)
        {
            valves[n_valves] = v;
            ++n_valves;
        }
        else if (strncmp(graph[v].label, START_LABEL, LABEL_LENGTH) == 0)
            start_index = v;
}

int distance[MAX_VERTICES][MAX_VERTICES];

void bfs(size_t r)
{
    for (size_t v = 0; v < n_vertices; ++v)
        distance[r][v] = -1;
    size_t q[MAX_VERTICES]; // indices into graph
    size_t deq = 0; // dequeue():  q[deq++]
    size_t enq = 0; // enqueue(x): q[enq++] = x
    q[enq++] = r;
    distance[r][r] = 0;
    while (deq < enq)
    {
        const size_t v = q[deq++];
        for (size_t e = 0; e < graph[v].outdegree; ++e)
        {
            const size_t w = graph[v].edges[e] - graph;
            if (distance[r][w] == -1)
            {
                distance[r][w] = distance[r][v] + 1;
                q[enq++] = w;
            }
        }
    }
}
void populate_distance(void)
{
    bfs(start_index);
    for (size_t i = 0; i < n_valves; ++i)
        bfs(valves[i]);
}

int part1(void)
{
    int result = 0;
    char is_open[MAX_VERTICES] = {0}; // indices into graph are indices into is_open
    struct frame {
        size_t pos; // indices into graph
        int time;
        int flow;
        int value;
        size_t next; // indices into valves
        char touched;
    } state[TIME_LIMIT];
    state[0].pos     = start_index;
    state[0].time    = 0;
    state[0].flow    = 0;
    state[0].value   = 0;
    state[0].next    = 0;
    state[0].touched = 0;
    size_t index = 0;
    while (1)
    {
        struct frame *s = &state[index];
        if (!s->touched)
        {
            s->touched = 1;
            const size_t new_pos = valves[s->next];
            if (is_open[new_pos])
                // don't walk in circles
                continue;
            const int time_diff = distance[s->pos][new_pos] + 1;
            const int new_time = s->time + time_diff;
            if (new_time < TIME_LIMIT)
            {
                // push
                is_open[new_pos] = 1;
                struct frame *s_ = &state[++index];
                s_->pos     = new_pos;
                s_->time    = new_time;
                s_->flow    = s->flow + graph[new_pos].flow_rate;
                s_->value   = s->value + s->flow * time_diff;
                s_->next    = 0;
                s_->touched = 0;
            }
        }
        else
            if (++s->next < n_valves)
                // continue in next iteration
                s->touched = 0;
            else
            {
                // try staying here until time expires
                const int time_left = TIME_LIMIT - s->time;
                const int value = s->value + s->flow * time_left;
                if (result < value)
                    result = value;
                // pop
                if (index == 0)
                    return result;
                is_open[s->pos] = 0;
                --index;
            }
    }
}

int part2(void)
{
    int result = 0;
    char is_open[MAX_VERTICES] = {0}; // indices into graph are indices into is_open
    struct frame {
        size_t my_pos; // indices into graph
        size_t el_pos; // indices into graph
        int my_time;
        int el_time;
        int my_flow;
        int el_flow;
        int value;
        size_t next; // indices into valves
        char touched;
    } state[2 * (TIME_LIMIT - TEACHING_TIME)];
    state[0].my_pos  = start_index;
    state[0].el_pos  = start_index;
    state[0].my_time = 0;
    state[0].el_time = 0;
    state[0].my_flow = 0;
    state[0].el_flow = 0;
    state[0].value   = 0;
    state[0].next    = 0;
    state[0].touched = 0;
    size_t index = 0;
    while (1)
    {
        struct frame *s = &state[index];
        if (!s->touched)
        {
            s->touched = 1;
            const size_t new_pos = valves[s->next];
            if (is_open[new_pos])
                // don't walk in circles
                continue;
            const char i_move = s->my_time <= s->el_time;
            const size_t pos = i_move ? s->my_pos : s->el_pos;
            const int time_diff = distance[pos][new_pos] + 1;
            const int new_time = (i_move ? s->my_time : s->el_time) + time_diff;
            if (new_time < TIME_LIMIT - TEACHING_TIME)
            {
                // push
                is_open[new_pos] = 1;
                struct frame *s_ = &state[++index];
                if (i_move)
                {
                    s_->my_pos  = new_pos;
                    s_->el_pos  = s->el_pos;
                    s_->my_time = new_time;
                    s_->el_time = s->el_time;
                    s_->my_flow = s->my_flow + graph[new_pos].flow_rate;
                    s_->el_flow = s->el_flow;
                    s_->value   = s->value + s->my_flow * time_diff;
                }
                else
                {
                    s_->my_pos  = s->my_pos;
                    s_->el_pos  = new_pos;
                    s_->my_time = s->my_time;
                    s_->el_time = new_time;
                    s_->my_flow = s->my_flow;
                    s_->el_flow = s->el_flow + graph[new_pos].flow_rate;
                    s_->value   = s->value + s->el_flow * time_diff;
                }
                s_->next    = 0;
                s_->touched = 0;
            }
        }
        else
            if (++s->next < n_valves)
                // continue in next iteration
                s->touched = 0;
            else
            {
                // try staying here until time expires
                const int my_time_left = TIME_LIMIT - TEACHING_TIME - s->my_time;
                const int el_time_left = TIME_LIMIT - TEACHING_TIME - s->el_time;
                const int value = s->value + s->my_flow * my_time_left + s->el_flow * el_time_left;
                if (result < value)
                    result = value;
                // pop
                if (index == 0)
                    return result;
                const struct frame *s_ = &state[--index];
                const char i_moved = s_->my_time <= s_->el_time;
                is_open[i_moved ? s->my_pos : s->el_pos] = 0;
            }
    }
}

int main(void)
{
    input();
    populate_valves();
    populate_distance();
    printf("%d\n", part1());
    printf("%d\n", part2());
    return 0;
}
