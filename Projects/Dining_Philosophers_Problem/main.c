#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <glib-2.0>
#include <semaphore.h>

#define THREAD_COUNT 5

//function the threads run when started up
void* worker_function(void* args){
    int * id = (int*) args;
    printf("thread id:%d\n", *id);
    free(id);

    return NULL;
}

void main (){
    //create THREAD_COUNT num of threads, start up threads and pass their ids to the running functions. wait for all threads to finish.
    pthread_t threads[THREAD_COUNT];
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        int *id = malloc(sizeof(int));
        *id = i;
        pthread_create(&threads[i], NULL, worker_function, (void *)id);
    }

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_join(threads[i], NULL);
    }
    
}