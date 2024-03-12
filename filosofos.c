#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_FILOSOFOS 5

sem_t tenedores[NUM_FILOSOFOS];

void *filosofo(void *num) {
    int id = *(int *)num;

    while (1) {
        printf("Filosofo %d está pensando\n", id);
        sleep(1);

        // Intentar agarrar tenedores
        sem_wait(&tenedores[id]);
        sem_wait(&tenedores[(id + 1) % NUM_FILOSOFOS]);

        // Comiendo
        printf("Filosofo %d está comiendo\n", id);
        sleep(1);

        // Dejar tenedores
        sem_post(&tenedores[id]);
        sem_post(&tenedores[(id + 1) % NUM_FILOSOFOS]);
    }

    return NULL;
}

int main() {
    pthread_t hilos[NUM_FILOSOFOS];
    int ids[NUM_FILOSOFOS];


    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        sem_init(&tenedores[i], 0, 1);
    }

    // Crear hilos de filósofos
    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        ids[i] = i;
        pthread_create(&hilos[i], NULL, filosofo, (void *)&ids[i]);
    }


    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    // Limpiar
    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        sem_destroy(&tenedores[i]);
    }

    return 0;
}
