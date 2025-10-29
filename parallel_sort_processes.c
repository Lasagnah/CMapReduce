#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>

// Merge Sort Implementation
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
    free(L);
    free(R);
}

void merge_sort(int *arr, int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        merge_sort(arr, l, m);
        merge_sort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// Main Program 
int main(int argc, char *argv[]) {
    int n = 131072; // test sizes: 32 or 131072
    int workers = 4; // try 1, 2, 4, 8

    int *arr = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) arr[i] = rand() % 100000;

    int chunk_size = n / workers;
    int pipes[workers][2];  // one pipe per process

    struct timeval start, end;
    gettimeofday(&start, NULL);

    // Map Phase: Spawn child processes 
    for (int i = 0; i < workers; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            // Child process
            close(pipes[i][0]); // Close read end

            int start_idx = i * chunk_size;
            int end_idx = (i == workers - 1) ? n : (i + 1) * chunk_size;

            int size = end_idx - start_idx;
            int *chunk = malloc(size * sizeof(int));
            for (int j = 0; j < size; j++)
                chunk[j] = arr[start_idx + j];

            merge_sort(chunk, 0, size - 1);

            // Send sorted chunk to parent
            write(pipes[i][1], chunk, size * sizeof(int));
            close(pipes[i][1]);
            free(chunk);
            exit(0);
        }
        else {
            // Parent continues
            close(pipes[i][1]); // Close write end
        }
    }

    // Parent: Wait for all children 
    for (int i = 0; i < workers; i++)
        wait(NULL);

    // Reduce Phase: Collect and merge sorted chunks
    int *sorted_arr = malloc(n * sizeof(int));
    int offset = 0;
    for (int i = 0; i < workers; i++) {
        int start_idx = i * chunk_size;
        int end_idx = (i == workers - 1) ? n : (i + 1) * chunk_size;
        int size = end_idx - start_idx;
        read(pipes[i][0], sorted_arr + start_idx, size * sizeof(int));
        close(pipes[i][0]);
    }

    // Sequential merge of sorted chunks
    for (int size = chunk_size; size < n; size *= 2) {
        for (int left = 0; left < n - size; left += 2 * size) {
            int mid = left + size - 1;
            int right = (left + 2 * size - 1 < n - 1) ? left + 2 * size - 1 : n - 1;
            merge(sorted_arr, left, mid, right);
        }
    }

    gettimeofday(&end, NULL);
    double elapsed = (end.tv_sec - start.tv_sec) * 1000.0;
    elapsed += (end.tv_usec - start.tv_usec) / 1000.0;

    printf("Workers (processes): %d | Time: %.2f ms\n", workers, elapsed);

    free(sorted_arr);
    free(arr);
    return 0;
}
