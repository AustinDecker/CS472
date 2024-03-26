#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define THREAD_COUNT 5


void* worker_function(void* args){
    int * id = (int*) args;
    printf("thread id:%d\n", *id);
    free(id);

    return NULL;
}

void main (){
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