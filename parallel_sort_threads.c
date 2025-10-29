#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct {
    int *arr;
    int start;
    int end;
} ThreadArg;

void merge(int *arr, int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    int *L = malloc(n1 * sizeof(int));
    int *R = malloc(n2 * sizeof(int));
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2)
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    free(L); free(R);
}

void merge_sort(int *arr, int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        merge_sort(arr, l, m);
        merge_sort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void *sort_chunk(void *arg) {
    ThreadArg *t = (ThreadArg *)arg;
    merge_sort(t->arr, t->start, t->end - 1);
    return NULL;
}

int main(int argc, char *argv[]) {
    int n = 131072; // test sizes: 32 or 131072
    int workers = 4; // try 1, 2, 4, 8
    int *arr = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) arr[i] = rand() % 100000;

    pthread_t threads[workers];
    ThreadArg args[workers];
    int chunk_size = n / workers;

    struct timeval start, end;
    gettimeofday(&start, NULL);

    for (int i = 0; i < workers; i++) {
        args[i].arr = arr;
        args[i].start = i * chunk_size;
        args[i].end = (i == workers - 1) ? n : (i + 1) * chunk_size;
        pthread_create(&threads[i], NULL, sort_chunk, &args[i]);
    }
    for (int i = 0; i < workers; i++)
        pthread_join(threads[i], NULL);

    // Reduce: merge sorted chunks
    for (int size = chunk_size; size < n; size *= 2) {
        for (int left = 0; left < n - size; left += 2 * size) {
            int mid = left + size - 1;
            int right = (left + 2 * size - 1 < n - 1) ? left + 2 * size - 1 : n - 1;
            merge(arr, left, mid, right);
        }
    }

    gettimeofday(&end, NULL);
    double elapsed = (end.tv_sec - start.tv_sec) * 1000.0;
    elapsed += (end.tv_usec - start.tv_usec) / 1000.0;
    printf("Workers: %d | Time: %.2f ms\n", workers, elapsed);

    free(arr);
    return 0;
}
