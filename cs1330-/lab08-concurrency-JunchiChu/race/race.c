#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./race.h"

#include "./race.h"

/*
 * Fibonacci number to calculate.
 */
#define NFIB 40

/*
 * Buffer size for passing messages between threads in handoff.
 */
#define BUF_SIZE 1024

/*
 * Records whether or not one of the teams has finished the race.
 */
int someone_finished = 0;

/*
 * Initializes and runs a two-team relay race.
 */
int main() {
    race_t *race = race_init();
    create_racers(race);
    start_race(race);
    return 0;
}

/*
 * Creates and returns a race_t struct, not including its two teams.
 */
race_t *race_init() {
    race_t *race = (race_t *)malloc(sizeof(race_t));

    /**
     * TODO:
     * 1. initialize barrier and race mutex
     */
    
    pthread_barrierattr_t attr;
    unsigned count=4;
     
    pthread_barrier_init(&race->barrier, &attr, count);
    pthread_mutex_init(&race->mutex,0);
    return race;
}

/*
 * Sets up the two teams participating in the race and stores them
 * in the given race_t struct.
 *
 * Returns 0 if successful and 1 if there are any errors.
 */
int create_racers(race_t *race) {
    race->team_1 = create_team("Oozma Kappa");
    race->team_2 = create_team("Roar Omega Roar");
    return 0;
}

/*
 * Creates and returns a single team with two racers that has the given
 * team name.
 */
race_team_t *create_team(char *team_name) {
    race_team_t *team = (race_team_t *)malloc(sizeof(race_team_t));
    team->racer_a = create_racer(team_name);
    team->racer_b = create_racer(team_name);
    return team;
}

/*
 * Creates and returns a single racer_t struct, initializing all
 * of its fields and concurrency structures.
 */
racer_t *create_racer(char *team_name) {
    racer_t *racer = (racer_t *)malloc(sizeof(racer_t));
    racer->finished = 0;
    racer->team_name = team_name;

    /**
     * TODO:
     * 1. initialize the racer's associated mutex and condition variable
     */
    pthread_mutex_init(&racer->mutex,0);
    pthread_cond_init(&racer->done,0);
    return racer;
}

/*
 * Cleans up a race_t struct. This includes destroying all concurrency
 * structures on the race, teams, and individual racers and freeing any
 * allocated memory.
 */
void destroy_race(race_t *race) {
    /**
     * TODO: destroy the race barrier, race mutex, and all racer condition
     * variables/mutexes
     */
  pthread_barrier_destroy(&race->barrier);
  pthread_mutex_destroy(&race->mutex);
  pthread_cond_destroy(&race->team_1->racer_a->done);
  pthread_cond_destroy(&race->team_1->racer_b->done);
  pthread_cond_destroy(&race->team_2->racer_a->done);
  pthread_cond_destroy(&race->team_2->racer_b->done);

    free(race->team_1->racer_a);
    free(race->team_1->racer_b);
    free(race->team_2->racer_a);
    free(race->team_2->racer_b);

    free(race->team_1);
    free(race->team_2);

    free(race);
}

/*
 * Runs the first racer for a particular team. The argument is a generic pointer
 * to an args_t struct that contains the race and the team this thread is for.
 *
 * Returns NULL
 */
void *run_racer_a(void *args) {
    /**
     * TODO:
     * 1. wait at barrier
     * 2. call calculate()
     * 3. set finished to 1
     * 4. signal condition variable
     */
  args_t* run = (args_t*) args;
  pthread_barrier_wait(&run->race->barrier);
  calculate();
  run->team->racer_a->finished=1;
  pthread_cond_signal(&run->team->racer_a->done);
    return NULL;
}

/*
 * Runs the second racer for a particular team. The argument is a generic
 * pointer to an args_t struct that contains the race and the team that this
 * thread is for.
 *
 * Returns NULL
 */
void *run_racer_b(void *args) {
    /**
     * TODO:
     * 1. wait at barrier
     * 2. wait on condition variable
     * 3. call handoff()
     * 4. call calculate()
     * 5. lock race mutex, call announce, unlock race mutex
     */
   args_t* run = (args_t*) args;

  pthread_barrier_wait(&run->race->barrier);
  
  
  pthread_cond_wait(&run->team->racer_a->done,&run->team->racer_a->mutex);
  handoff(run->team->racer_a->team_name);
   calculate();
   pthread_mutex_lock(&run->race->mutex);
   announce(run->team->racer_b->team_name);
   pthread_mutex_unlock(&run->race->mutex);
    return NULL;
}

/*
 * Runs a race using 4 threads, one for each racer. The race is cleaned up after
 * it finishes.
 */
void start_race(race_t *race) {
    /**
     * TODO:
     * 1. initialize the argument wrapper
     * 2. create racer threads
     * 3. start race!
     * 4. join threads
     */
  //I helped a student to debug, student name: yuming fu  
  struct args_t teama = {race,race->team_1};
   struct args_t teamb = {race,race->team_1};
 struct args_t teamc = {race,race->team_2};
 struct args_t teamd = {race,race->team_2};
 
  int i=0;
    pthread_t threads[4];
        
    if(pthread_create(&threads[0],0,run_racer_a,&teama)!=0){
      fprintf(stderr,"fail");
    }
    
    if(pthread_create(&threads[1],0,run_racer_a,&teamc)!=0){
        fprintf(stderr,"fail");

    }
    if(pthread_create(&threads[2],0,run_racer_b,&teamb)!=0){
            fprintf(stderr,"fail");

    }
    if( pthread_create(&threads[3],0,run_racer_b,&teamd)!=0){
            fprintf(stderr,"fail");

    }

    i=0;
       for (i = 0; i<4; i++) {
	 if( pthread_join(threads[i], 0)!=0){
           fprintf(stderr,"fail");

	 }
    }


    // all racers are done
    printf("All racers have finished\n");
    destroy_race(race);
}

/*
 * Perform a calculation (the 40th Fibonacci number) to simulate racing.
 */
void calculate() { fibonacci(NFIB); }

/*
 * Computes the ath Fibonacci number recursively.
 */
int fibonacci(unsigned int a) {
    if (a < 2) {
        return 1;
    } else {
        return fibonacci(a - 1) + fibonacci(a - 2);
    }
}

/*
 * Announces that the given team has completed the race, meaning that both racer
 * A and racer B have finished. Depending on whether or not the other team has
 * already finished, this will print out that the team has come in first or
 * second place.
 */
void announce(char *team) {
    if (!someone_finished) {
        printf("Team %s has locked the Golden Mutex and won the race!\n", team);
    } else {
        printf("Team %s has come in second place, securing the Silver Mutex!\n",
               team);
    }

    someone_finished = 1;
}

/*
 * Helper function that can be run in a thread to print a message. printf is
 * thread-safe, so using it blocks other threads also trying to print. This
 * function is used by handoff so that racing threads can announce when their
 * racer has handed off without impeding the progress of the other team.
 */
void *handoff_print(void *msg) {
    printf("%s", (char *)msg);
    free(msg);
    return 0;
}

/*
 * Prints a message that a team's first racer has finished and will hand off to
 * the second racer.
 */
void handoff(char *team) {
    pthread_t print_thread;
    char *buf = malloc(BUF_SIZE);
    snprintf(buf, BUF_SIZE, "Racer A of team %s has finished!\n", team);
    pthread_create(&print_thread, 0, handoff_print, buf);
}
