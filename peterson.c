#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <curses.h>

#define N 4
#define ARRAY_SIZE 10000
#define SECTION_SIZE (ARRAY_SIZE / N)

int var_com = 0;
int array[ARRAY_SIZE];
int flag[N];
int turn = 0;

void peterson_lock(int i) {
    flag[i] = 1;
    turn = (i + 1) % N;
    while (flag[turn] && turn != i) {
        // wait
    }
}

void peterson_unlock(int i) {
    flag[i] = 0;
}

void* thread_sum(void* arg) {
    int* id_ptr = (int*)arg;
    int id = *id_ptr;
    int start = id * SECTION_SIZE;
    int end = start + SECTION_SIZE;
    int sum_par = 0;

    for (int i = start; i < end; i++) {
        sum_par += array[i];
    }

    peterson_lock(id);
    var_com += sum_par;
    peterson_unlock(id);
    printf("suma parcial: %d\n",sum_par);
    return NULL;
}

int main() {
    // se llena el arreglo con numeros aleatorios
    srand(time(NULL));
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand() % 100;
    }

    // se crean los hilos
    pthread_t threads[N];
    int* thread_ids[N];
    for (int i = 0; i < N; i++) {
        thread_ids[i] = malloc(sizeof(int));
        *(thread_ids[i]) = i;
        pthread_create(&threads[i], NULL, thread_sum, thread_ids[i]);
    }

    // Ingresa a los hilos
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
        free(thread_ids[i]);
    }

    printf("Suma total: %d\n", var_com);

    return 0;
}
