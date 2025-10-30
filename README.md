# CMapReduce
## Project Description
* This project explores the implementation of parallel data processing in C using MapReduce as an inspiration. 
* There are two key phases in this implementation, the Map phase and the Reduce phase. 
    * The map phase consists of splitting a large dataset into smaller chunks to process independently.
    * The Reduce phase consists of combining those smaller chunks into one large array once they have been processed.
* 
### Problem 1
* The first MapReduce implementation focuses on using merge sort to sort a large array.
* There are two implementations, one using threads, the other using processes.
* Each implementation can be compiled and ran and it will test how effective each version is on your machine.
* If you would like to test each implementation with a different size of array, you can change the size of the array by editing the n variable within the file (in threads, n is on line 42. in processes, n is on line 34).
### Problem 2
* The second MapReduce implementation focuses on finding the largest number within an array. 
* There are two implementations, one using threads, the other using processes.

## Instructions
* You can run the thread version by using the following commands
> gcc parallel_sort_threads.c -o parallel_sort_threads -lpthread  
> ./parallel_sort_threads
* You can run the multiprocess version by using the following commands
> gcc -o parallel_sort_processes parallel_sort_processes.c  
> ./parallel_sort_processes  
* You can run the implementation of problem 2 by using the following commands
> gcc max_threads.c -o max_threads -lpthread  
> ./max_threads  
## Structure of Code

## Description of implementation
* pthread.h is used for threading.
* sys/time.h is used for timing the processes/threads.
* stdlib.h, stdio.h are used for memory and IO.
* pthread_create() and pthread_join() are used for thread manipulation (creation and completion)
* For the threaded version of Problem 1, there is no need for IPC. For the processes version of Problem 1, pipes are used for IPC.
* Threads were created manually, and each one handled a data chunk itself. Processes were forked manually and each handled a data chunk itself. 
* Threads shared memory, processes had their own isolated memory space. Threads used a single global array, processes used pipes. Threads used pthread_join to wait for all of the threads, processes waited. 
* Performance is measured by using the gettimeofday function to figure out when we start doing the problem, and then called again to figure out when we end the problem. 
## Performance Evaluation
* Problem 1 (Process):  
With 4 processes, this took over 30 minutes to sort the large array (131072) so I just stopped it for the large array.  
With the smaller arrays however, it performed very well, but the more workers included actually led to a longer time (1 process with a time of 0.44 ms, 4 processes with a time of 1.88 ms, 8 processes with a time of 2.03 ms).
* Problem 1 (Threads):  
Similarly to the process version, this took over 30 minutes to sort the large array, so I could not get performance data for the large array.  
With the smaller arrays however, it performed well, but the more workers included led to a longer time to sort (1 worker with a time of 0.13 ms, 4 workers with a time of 0.42 ms, 8 workers with a time of 1.29 ms). 
* Problem 2:  
Unlike the other problems, I was able to use this on the large array and had the following results  
1 Worker: 0.58 ms  
2 Workers: 0.80 ms  
4 Workers: 0.43 ms  
8 Workers: 0.69 ms  
## Conclusion
* Some key findings were that thread-based MapReduce well for parallel tasks and that parallel merge sort shows good scalability when using multiple cores.
* Some challenges were balancing chunk sizes because we had to avoid load imbalance and implementing efficient merging for large datasets.
* Some limitations are that merge sort is sequential, so it could be made more parallel in the future and thread overhead could take up a lot of the resources for small data sets. 
* Future improvements are possibly making the merging phase parallel and using thread pools and dynamic load balancing. 