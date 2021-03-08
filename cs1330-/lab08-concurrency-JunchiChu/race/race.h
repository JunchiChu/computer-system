#ifndef RACE_H_
#define RACE_H_

/* struct for an individual racer */
typedef struct racer_t {
    int finished;
    pthread_mutex_t mutex;
    pthread_cond_t done;
    char *team_name;
} racer_t;

/* struct for a team of racers */
typedef struct race_team_t {
    racer_t *racer_a;
    racer_t *racer_b;
} race_team_t;

/* struct for the whole race */
typedef struct race_t {
    pthread_barrier_t barrier;
    pthread_mutex_t mutex;
    race_team_t *team_1;
    race_team_t *team_2;
} race_t;

/* struct wrapping arguments to run_racer functions */
typedef struct args_t {
    race_t *race;
    race_team_t *team;
} args_t;

/* intialize and destroy race structs */
race_t *race_init();
race_team_t *create_team(char *team_name);
racer_t *create_racer(char *team_name);
int create_racers(race_t *race);
void destroy_race(race_t *race);

/* run racers */
void *run_racer_a(void *args);
void *run_racer_b(void *args);

/* begin the race */
void start_race(race_t *race);

/* announcement functions */
void announce(char *team);
void handoff(char *team);

/* computation functions for the racers */
void calculate();
int fibonacci(unsigned int a);

#endif  // RACE_H_
