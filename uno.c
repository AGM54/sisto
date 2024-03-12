#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t recurso1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t recurso2 = PTHREAD_MUTEX_INITIALIZER;

void *proceso1(void *arg) {
    while (1) {
        printf("Proceso 1 intenta adquirir el recurso 1\n");
        pthread_mutex_lock(&recurso1);
        printf("Proceso 1 ha adquirido el recurso 1\n");

        sleep(1); // Simula trabajo y aumenta la probabilidad de deadlock

        printf("Proceso 1 intenta adquirir el recurso 2\n");
        pthread_mutex_lock(&recurso2);
        printf("Proceso 1 ha adquirido el recurso 2\n");

        sleep(1); // Simula trabajo con los recursos

        pthread_mutex_unlock(&recurso2);
        pthread_mutex_unlock(&recurso1);
        printf("Proceso 1 ha liberado ambos recursos\n");
    }
    return NULL;
}

void *proceso2(void *arg) {
    while (1) {
        printf("Proceso 2 intenta adquirir el recurso 2\n");
        pthread_mutex_lock(&recurso2);
        printf("Proceso 2 ha adquirido el recurso 2\n");

        sleep(1); // Simula trabajo y aumenta la probabilidad de deadlock

        printf("Proceso 2 intenta adquirir el recurso 1\n");
        pthread_mutex_lock(&recurso1);
        printf("Proceso 2 ha adquirido el recurso 1\n");

        sleep(1); // Simula trabajo con los recursos

        pthread_mutex_unlock(&recurso1);
        pthread_mutex_unlock(&recurso2);
        printf("Proceso 2 ha liberado ambos recursos\n");
    }
    return NULL;
}

int main() {
    pthread_t hilo1, hilo2;

    pthread_create(&hilo1, NULL, proceso1, NULL);
    pthread_create(&hilo2, NULL, proceso2, NULL);

    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);

    return 0;
}
