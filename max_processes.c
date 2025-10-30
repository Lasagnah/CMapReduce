#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>
#include <sys/time.h>

#define N 131072
int arr[N];

int main(int argc, char *argv[]) {
    int workers = 4; // Default number of processes
    if (argc > 1)
        workers = atoi(argv[1]);

    // Initialize random data
    for (int i = 0; i < N; i++)
        arr[i] = rand() % 1000000;

    int chunk = N / workers;
    int pipes[workers][2]; // One pipe per child

    struct timeval start, end;
    gettimeofday(&start, NULL);

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
        else if (pid == 0) {
            // Child process
            close(pipes[i][0]); // Close read end

            int start_idx = i * chunk;
            int end_idx = (i == workers - 1) ? N : (i + 1) * chunk;
            int local_max = INT_MIN;

            for (int j = start_idx; j < end_idx; j++) {
                if (arr[j] > local_max)
                    local_max = arr[j];
            }

            // Write local max to parent
            write(pipes[i][1], &local_max, sizeof(int));
            close(pipes[i][1]);
            exit(0);
        }
        else {
            // Parent process - continue to next child
            close(pipes[i][1]); // Close write end
        }
    }

    // Parent collects results
    int global_max = INT_MIN;
    for (int i = 0; i < workers; i++) {
        int local_max;
        read(pipes[i][0], &local_max, sizeof(int));
        close(pipes[i][0]);

        if (local_max > global_max)
            global_max = local_max;
    }

    // Wait for all children to finish
    for (int i = 0; i < workers; i++)
        wait(NULL);

    gettimeofday(&end, NULL);
    double elapsed = (end.tv_sec - start.tv_sec) * 1000.0;
    elapsed += (end.tv_usec - start.tv_usec) / 1000.0;

    printf("Workers: %d | Global max: %d | Time: %.2f ms\n",
           workers, global_max, elapsed);

    return 0;
}


