#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "dataArray.h"
#include "semaphore.h"

static size_t da_length;
//static DataArray da;

/* Semaphore relative to the reader writer */
static int lect = 1;
static int s = 2;
static int ecr = 0;
static int nb_lect = 0;

/* Semaphore to create reader and writer */
static int generate_actor = 3;

typedef struct {
    DataArray *da;
    int i;
} ActorParameters;

_Noreturn void* writer(void *arg) {
    ActorParameters* actorParameters = (ActorParameters*)arg;
    V(generate_actor);

    while (DA_length(actorParameters->da) <= da_length) {
        P(ecr);
        P(s);
        int val = rand() % 50;
        printf("I'm the writer %d and i'm writing %d\n", actorParameters->i, val);
        DA_insert_back(actorParameters->da, (size_t)val);
        V(ecr);
        V(s);
        sleep(5);
    }
    free(actorParameters);
    pthread_exit(NULL);
}

_Noreturn void* reader(void *arg) {
    ActorParameters* actorParameters = (ActorParameters*)arg;
    int id = actorParameters->i;
    DataArray *da = actorParameters->da;
    V(generate_actor);

    while (1) {
        P(ecr);
        V(ecr);
        P(lect);
        nb_lect++;
        if (nb_lect == 1) {
            P(s);
        }
        V(lect);
        for (size_t i = 0; i < DA_length(da); i++){
            printf("I'm the reader %d and i'm reading %zu\n", id, DA_value(da, (size_t)i));
        }
        P(lect);
        nb_lect--;
        if (nb_lect == 0) {
            V(s);
        }
        V(lect);
        sleep(1);
    }
    free(actorParameters);
    pthread_exit(NULL);
}

void generate_actors(DataArray *da, pthread_t* pthread, int number, void*(*func)(void*)) {
    for (int i = 0; i < number; i++) {
        P(generate_actor);
        ActorParameters* actorParameters = (ActorParameters*)malloc(sizeof (ActorParameters));
        if (actorParameters == 0x00) {
            perror("malloc() error");
            exit(1);
        }
        actorParameters->da = da;
        actorParameters->i = i;
        if (pthread_create(&pthread[i], NULL, func, actorParameters) != 0) {
            perror("pthread_create() error");
            exit(1);
        }
    }
}

int main() {
    /*Semaphore init*/
    int init_sem[4] = {1,1,1, 1};
    initSem(4, "MySemaphores", init_sem);

    /*Data array init*/
    DataArray da;
    da_length = 30;
    DA_init(&da, da_length);

    /*Program start*/
    int nbr_reader = 10;
    int nbr_writer = 2;
    pthread_t readers[nbr_reader];
    pthread_t writers[nbr_writer];

    generate_actors(&da,writers,nbr_writer, writer);
    generate_actors(&da,readers,nbr_reader, reader);

    for (int i = 0; i < nbr_reader; i++) {
        pthread_join(readers[i], NULL);
    }

    for (int i = 0; i < nbr_writer; i++) {
        pthread_join(readers[i], NULL);
    }

    DA_free(&da);

    return 0;
}
