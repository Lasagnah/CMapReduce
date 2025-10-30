#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include <sys/time.h>

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
    int workers = 1;
    int chunk = N / workers;
    pthread_t threads[workers];
    ThreadArg args[workers];
    pthread_mutex_init(&lock, NULL);

    struct timeval start, end;
    gettimeofday(&start, NULL);

    // Create worker threads
    for (int i = 0; i < workers; i++) {
        args[i].start = i * chunk;
        args[i].end = (i == workers - 1) ? N : (i + 1) * chunk;
        pthread_create(&threads[i], NULL, find_local_max, &args[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < workers; i++)
        pthread_join(threads[i], NULL);

    gettimeofday(&end, NULL);
    double elapsed = (end.tv_sec - start.tv_sec) * 1000.0;
    elapsed += (end.tv_usec - start.tv_usec) / 1000.0;

    printf("Workers: %d | Global max: %d | Time: %.2f ms\n", workers, global_max, elapsed);

    pthread_mutex_destroy(&lock);
    return 0;
}