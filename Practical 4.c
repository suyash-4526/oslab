/* Practical 4: Deadlock-free Dining Philosophers using semaphores */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5
sem_t forks[N];
sem_t room;

void *philosopher(void *arg) {
    int id = *(int *)arg;
    for (int meal = 1; meal <= 2; meal++) {
        printf("Philosopher %d is thinking\n", id);
        sem_wait(&room);                 /* Allow at most N-1 philosophers */
        sem_wait(&forks[id]);
        sem_wait(&forks[(id + 1) % N]);

        printf("Philosopher %d is eating (meal %d)\n", id, meal);
        sleep(1);

        sem_post(&forks[(id + 1) % N]);
        sem_post(&forks[id]);
        sem_post(&room);
    }
    return NULL;
}

int main(void) {
    pthread_t t[N];
    int id[N];

    sem_init(&room, 0, N - 1);
    for (int i = 0; i < N; i++) sem_init(&forks[i], 0, 1);

    for (int i = 0; i < N; i++) {
        id[i] = i;
        pthread_create(&t[i], NULL, philosopher, &id[i]);
    }
    for (int i = 0; i < N; i++) pthread_join(t[i], NULL);

    for (int i = 0; i < N; i++) sem_destroy(&forks[i]);
    sem_destroy(&room);
    return 0;
}
