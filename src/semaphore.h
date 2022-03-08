/********************************
 * Fichier semaphore.h
 ********************************/
#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
extern void erreurFin(const char *msg);
extern void initSem(int nbSem, char *nomFich, int *pvinit);
extern void libereSem();
extern void P(int numSem);
extern void V(int numSem);
#endif
