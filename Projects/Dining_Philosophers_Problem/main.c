#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>


#define THREAD_COUNT 5
#define MAX_MEALS 10
#define MAX_THINK_EAT_SECS 4


void pickup(int);
void putdown(int);
void test(int);

int meals_eaten [THREAD_COUNT] = {0};
enum {THINKING, HUNGRY, EATING} state [THREAD_COUNT] = {THINKING};
pthread_cond_t self[THREAD_COUNT];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void pickup(int philosopher_id){
    state[philosopher_id] = HUNGRY;
    test(philosopher_id);
    
    while (state[philosopher_id] != EATING) {
        pthread_cond_wait(&self[philosopher_id], &mutex);
    }
        
}

void putdown(int philosopher_id){
    state[philosopher_id] = THINKING;
    test((philosopher_id + 4) % THREAD_COUNT); // Test left neighbor
    test((philosopher_id + 1) % THREAD_COUNT); // Test right neighbor
}

void test(int philosopher_id){
    if (state[(philosopher_id + 4) % THREAD_COUNT] != EATING &&
        state[philosopher_id] == HUNGRY &&
        state[(philosopher_id + 1) % THREAD_COUNT] != EATING) {
        state[philosopher_id] = EATING;
        pthread_cond_signal(&self[philosopher_id]);
    }
}

//function the threads run when started up
void* philosopher_function(void* args){
    int * id = (int*) args;
    

    while(meals_eaten[*id] <= MAX_MEALS){
        printf("Philosopher %d is thinking\n", *id);
        sleep((rand() % MAX_THINK_EAT_SECS) + 1);

        pickup(*id);
        printf("Philosopher %d is eating\n", *id);
        meals_eaten[*id]++;

        sleep((rand() % MAX_THINK_EAT_SECS) + 1);
        putdown(*id);
        
    }
    return NULL;
}


//create THREAD_COUNT num of threads, start up threads and pass their ids to the running functions. wait for all threads to finish.
void main (int argc, char * argV[]){
    int sleep_time = 30;
    if(argc >= 2){
        sleep_time = atoi(argV[1]);
    }
    printf("%d\n", sleep_time);

    int * ids[THREAD_COUNT] = {NULL};
    pthread_t threads[THREAD_COUNT];
    srand(time(NULL));

    //init conditionals
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_cond_init(&self[i], NULL);
    }

    //init and start threads with thread ids
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        ids[i] = malloc(sizeof(int));
        *(ids[i]) = i;

        pthread_create(&threads[i], NULL, philosopher_function, (void *)ids[i]);
    }

    //main sleeps
    sleep(sleep_time);

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_cancel(threads[i]);
    }
    
    int total = 0;
    for (int i = 0; i < THREAD_COUNT; i++)
    {
       total += meals_eaten[i];
       printf("philosopher %d: %d\t", (i + 1), meals_eaten[i]);
    }
    printf("\n");

    double average = (double)total / THREAD_COUNT;
    printf("average times philosophers have eaten:\t%f\n", average);

    //clean up
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_cond_destroy(&self[i]);
        free(ids[i]);
    }

    pthread_mutex_destroy(&mutex);
}