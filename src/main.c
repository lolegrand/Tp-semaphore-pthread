#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "dataArray.h"
#include "semaphore.h"

static size_t da_length;
static DataArray da;
static int lect = 1;
static int s = 2;
static int ecr = 0;
static int nb_lect = 0;

_Noreturn void* writer(void *arg) {
    int id =*((int*)arg);

    while (DA_length(&da) <= da_length) {
        P(ecr);
        P(s);
        int val = rand() % 50;
        printf("I'm the writer %d and i'm writing %d\n", id, val);
        DA_insert_back(&da, (size_t)val);
        V(ecr);
        V(s);
        sleep(5);
    }
    pthread_exit(NULL);
}

_Noreturn void* reader(void *arg) {
    int id = *((int*)arg);

    while (1) {
        P(ecr);
        V(ecr);
        P(lect);
        nb_lect++;
        if (nb_lect == 1) {
            P(s);
        }
        V(lect);
        for (size_t i = 0; i < DA_length(&da); i++){
            printf("I'm the reader %d and i'm reading %zu\n", id, DA_value(&da, (size_t)i));
        }
        P(lect);
        nb_lect--;
        if (nb_lect == 0) {
            V(s);
        }
        V(lect);
        sleep(1);
    }

    pthread_exit(NULL);
}

void generate_actors(pthread_t* pthread, int number, void*(*func)(void*)) {
    for (int i = 0; i < number; i++) {

        if (pthread_create(&pthread[i], NULL, func, &i) != 0) {
            perror("pthread_create() error");
            exit(1);
        }
    }
}

int main() {
    int init_sem[3] = {1,1,1};

    initSem(3, "MySemaphores", init_sem);

    da_length = 30;
    DA_init(&da, da_length);

    int nbr_reader = 10;
    int nbr_writer = 2;
    pthread_t readers[nbr_reader];
    pthread_t writers[nbr_writer];

    generate_actors(writers,nbr_writer, writer);
    generate_actors(readers,nbr_reader, reader);

    for (int i = 0; i < nbr_reader; i++) {
        pthread_join(readers[i], NULL);
    }

    for (int i = 0; i < nbr_writer; i++) {
        pthread_join(readers[i], NULL);
    }

    DA_free(&da);

    return 0;
}
