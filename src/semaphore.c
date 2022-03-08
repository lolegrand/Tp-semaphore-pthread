/********************************
 * Fichier semaphore.c
 ********************************/
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>

static int tabSemId;

static void erreurFin(const char *msg)
{
	perror(msg);
	exit(1);
}

static void appelSem(short unsigned int numSem, short int op)
{
	struct sembuf sempar;
	const size_t nbop = 1;

	sempar.sem_num = numSem;
	sempar.sem_op = op;
	sempar.sem_flg = 0;	// ne pas mettre a SEM_UNDO !!
	if (semop(tabSemId, &sempar, nbop) == -1)
		erreurFin("appelSem");
}

void P(short unsigned int numSem)
{
	appelSem(numSem, -1);
}

void V(short unsigned int numSem)
{
	appelSem(numSem, 1);
}

void initSem(int nbSem, char *argv0, int *pvinit)
{
	int i;
	key_t cle = ftok(argv0, '0');
	tabSemId = semget(cle, nbSem, IPC_CREAT | 0600);
	if (tabSemId == -1)
		erreurFin("initSem : semget");
	for (i = 0; i < nbSem; i++)
		if (pvinit == NULL) {
			if (semctl(tabSemId, i, SETVAL, 0) == -1)
				erreurFin("initSem : semctl 1");
		} else if (semctl(tabSemId, i, SETVAL, pvinit[i]) == -1)
			erreurFin("initSem : semctl 2");
}

void libereSem()
{
	if (semctl(tabSemId, 0, IPC_RMID, NULL))
		erreurFin("libereSem");
}
