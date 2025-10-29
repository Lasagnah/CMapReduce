#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>

#define N 131072
int arr[N];
int global_max = INT_MIN;
pthread_mutex_t lock;

typedef struct {
    int start;
    int end;
} ThreadArg;

void *find_local_max(void *arg) {
    ThreadArg *t = (ThreadArg *)arg;
    int local_max = INT_MIN;
    for (int i = t->start; i < t->end; i++)
        if (arr[i] > local_max) local_max = arr[i];

    pthread_mutex_lock(&lock);
    if (local_max > global_max)
        global_max = local_max;
    pthread_mutex_unlock(&lock);

    return NULL;
}

int main() {
    for (int i = 0; i < N; i++) arr[i] = rand() % 1000000;
    int workers = 4;
    int chunk = N / workers;
    pthread_t threads[workers];
    ThreadArg args[workers];
    pthread_mutex_init(&lock, NULL);

    for (int i = 0; i < workers; i++) {
        args[i].start = i * chunk;
        args[i].end = (i == workers - 1) ? N : (i + 1) * chunk;
        pthread_create(&threads[i], NULL, find_local_max, &args[i]);
    }

    for (int i = 0; i < workers; i++)
        pthread_join(threads[i], NULL);

    printf("Global max: %d\n", global_max);
    pthread_mutex_destroy(&lock);
    return 0;
}