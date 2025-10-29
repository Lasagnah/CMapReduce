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
* You can run the thread version by using the following commands
> gcc parallel_sort_threads.c -o parallel_sort_threads -lpthread  
> ./parallel_sort_threads
* You can run the multiprocess version by using the following commands
> gcc -o parallel_sort_processes parallel_sort_processes.c  
> ./parallel_sort_processes
* If you would like to test each implementation with a different size of array, you can change the size of the array by editing the n variable within the file (in threads, n is on line 42. in processes, n is on line 34).
### Problem 2
* The second MapReduce implementation focuses on finding the largest number within an array. 
* There are two implementations, one using threads, the other using processes.

## Instructions

## Structure of Code

## Description of implementation
* pthread.h is used for threading.
* sys/time.h is used for timing the processes/threads.
* stdlib.h, stdio.h are used for memory and IO.
* pthread_create() and pthread_join() are used for thread manipulation (creation and completion)
* For the threaded version of Problem 1, there is no need for IPC. For the processes version of Problem 1, pipes are used for IPC.

## Performance Evaluation
* Problem 1 (Process):  
| Workers | Size | Time Taken |
|---------|------|------------|
| 1       | 131072   |            |
| 4       | 131072   |            |
| 8       | 131072   |            |
* Problem 1 (Threads):  
|Workers|Size|Time Taken|  
|---|---|---|  
|1|131072| |  
|4|131072| |  
|8|131072| |  
* Problem 2:  
|Workers|Size|Time Taken|  
|---|---|---|  
|1|131072| |  
|4|131072| |  
|8|131072| |  
## Conclusion
