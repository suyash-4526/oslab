/* Practical 3: Producer-Consumer using semaphores and mutex */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define SIZE 5
int buffer[SIZE], in = 0, out = 0;
sem_t empty, full;
pthread_mutex_t mutex;

void *producer(void *arg) {
    for (int item = 1; item <= 5; item++) {
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buffer[in] = item;
        printf("Produced: %d\n", item);
        in = (in + 1) % SIZE;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
    return NULL;
}

void *consumer(void *arg) {
    for (int i = 0; i < 5; i++) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        int item = buffer[out];
        printf("Consumed: %d\n", item);
        out = (out + 1) % SIZE;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
    return NULL;
}

int main(void) {
    pthread_t p, c;
    sem_init(&empty, 0, SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);
    pthread_join(p, NULL);
    pthread_join(c, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
    return 0;
}
