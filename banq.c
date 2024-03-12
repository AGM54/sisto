#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUM_PROCESOS 5
#define NUM_RECURSOS 3

// Declaración de matrices y vectores
int disponibles[NUM_RECURSOS] = {10, 5, 7}; // Recursos disponibles
int maximos[NUM_PROCESOS][NUM_RECURSOS] = {{7, 5, 3}, {3, 2, 2}, {9, 0, 2}, {2, 2, 2}, {4, 3, 3}}; // Máximo de recursos por proceso
int asignados[NUM_PROCESOS][NUM_RECURSOS] = {{0, 1, 0}, {2, 0, 0}, {3, 0, 2}, {2, 1, 1}, {0, 0, 2}}; // Recursos asignados a cada proceso
int necesarios[NUM_PROCESOS][NUM_RECURSOS]; // Recursos necesarios para cada proceso

// Función para calcular los recursos necesarios
void calcularNecesarios() {
    for (int i = 0; i < NUM_PROCESOS; i++) {
        for (int j = 0; j < NUM_RECURSOS; j++) {
            necesarios[i][j] = maximos[i][j] - asignados[i][j];
        }
    }
}

// Función para verificar si un proceso puede ejecutarse de manera segura
bool esSeguro() {
    int trabajo[NUM_RECURSOS];
    for (int i = 0; i < NUM_RECURSOS; i++) {
        trabajo[i] = disponibles[i];
    }

    bool terminado[NUM_PROCESOS] = {0};

    while (true) {
        bool encontrado = false;
        for (int i = 0; i < NUM_PROCESOS; i++) {
            if (!terminado[i]) {
                bool posible = true;
                for (int j = 0; j < NUM_RECURSOS; j++) {
                    if (necesarios[i][j] > trabajo[j]) {
                        posible = false;
                        break;
                    }
                }

                if (posible) {
                    for (int j = 0; j < NUM_RECURSOS; j++) {
                        trabajo[j] += asignados[i][j];
                    }
                    terminado[i] = true;
                    encontrado = true;
                }
            }
        }

        if (!encontrado) {
            break;
        }
    }

    for (int i = 0; i < NUM_PROCESOS; i++) {
        if (!terminado[i]) {
            return false;
        }
    }
    return true;
}

// Función para solicitar recursos
bool solicitarRecursos(int pid, int solicitud[]) {
    for (int i = 0; i < NUM_RECURSOS; i++) {
        if (solicitud[i] > necesarios[pid][i]) {
            printf("Error: El proceso %d ha solicitado más recursos de los necesarios.\n", pid);
            return false;
        }

        if (solicitud[i] > disponibles[i]) {
            printf("Error: El proceso %d ha solicitado más recursos de los disponibles.\n", pid);
            return false;
        }
    }

    for (int i = 0; i < NUM_RECURSOS; i++) {
        disponibles[i] -= solicitud[i];
        asignados[pid][i] += solicitud[i];
        necesarios[pid][i] -= solicitud[i];
    }

    if (esSeguro()) {
        return true;
    } else {
        for (int i = 0; i < NUM_RECURSOS; i++) {
            disponibles[i] += solicitud[i];
            asignados[pid][i] -= solicitud[i];
            necesarios[pid][i] += solicitud[i];
        }
        return false;
    }
}

// Función principal
int main() {
    calcularNecesarios();

    // Solicitudes de ejemplo
    int solicitud1[NUM_RECURSOS] = {1, 0, 2};
    int solicitud2[NUM_RECURSOS] = {2, 1, 1};

    if (solicitarRecursos(1, solicitud1)) {
        printf("La solicitud del proceso 1 ha sido aprobada.\n");
    } else {
        printf("La solicitud del proceso 1 ha sido denegada.\n");
    }

    if (solicitarRecursos(3, solicitud2)) {
        printf("La solicitud del proceso 3 ha sido aprobada.\n");
    } else {
        printf("La solicitud del proceso 3 ha sido denegada.\n");
    }

    return 0;
}
