# CMapReduce
## Project Description
* This project explores the implementation of parallel data processing in C using MapReduce as an inspiration. 
* There are two key phases in this implementation, the Map phase and the Reduce phase. 
    * The map phase consists of splitting a large dataset into smaller chunks to process independently.
    * The Reduce phase consists of combining those smaller chunks into one large array once they have been processed.
* We consider using multithreading and multiprocessing for this problem because threads allow for fast data transfer, we use multiprocessing because it is safer, and we use synchronization to ensure that we get the right result.
* For this particular project, threads don't share any writing to memory, so there is minimal synchronization needed and the merge step runs sequentially, so no race conditions would occur. 
### Problem 1
* The first MapReduce implementation focuses on using merge sort to sort a large array.
* There are two implementations, one using threads, the other using processes.
* Each implementation can be compiled and ran and it will test how effective each version is on your machine.
* If you would like to test each implementation with a different size of array, you can change the size of the array by editing the n variable within the file (in threads, n is on line 42. in processes, n is on line 34).
### Problem 2
* The second MapReduce implementation focuses on finding the largest number within an array. 
* There are two implementations, one using threads, the other using processes.
* Each implementation can be compiled and ran and it will test how effective each version is on your machine.
* If you would like to test the implementation with a different size of the array, you can change the size of the array by editing the n variable within the file. 
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
* You can run the process implementation of problem 2 by using the following commands
> gcc max_processes.c -o max_processes   
> ./max_processes
## Structure of Code
* I dont quite know how to include an image in markdown, so instead I will just describe the structure of the code to the best of my ability.  
First we start out with the main thread/process, which has the larger array that needs to be processed, then it breaks off into smaller processes/threads below it. Each thread communicates with each other using a global array. Each process uses a pipe to communicate with the parent process. Then we merge all of the threads/processes together to sort our array. 
* This supports MapReduce because the map phase is represented by the thread/process work, the reduce phase is represented by the merging, and the synchronization is achieved using mutexes and pthread_join(). 
* Because there is no shared writing in the thread version, there is no need for synchronization in the thread version. It does call pthread_join to get all of the threads to finish their processes first before merging. 
* Processes use wait to make sure that all of the processes are complete before merging. 
## Description of implementation
* pthread.h is used for threading.
* sys/time.h is used for timing the processes/threads.
* stdlib.h, stdio.h are used for memory and IO.
* pthread_create() and pthread_join() are used for thread manipulation (creation and completion)
* For the threaded version, it first sorts its array then gets merged and merges the array.
* For the process version, it spawns its child process using fork, then each child process gets assigned a portion of the array, then each process sorts its section, then it sends back its sorted chunk to the parent process using a pipe. 
## Performance Evaluation
* Problem 1 (Process):  
With 4 processes, this took over 30 minutes to sort the large array (131072) so I just stopped it for the large array.  
With the smaller arrays however, it performed very well, but the more workers included actually led to a longer time (1 process with a time of 0.44 ms, 4 processes with a time of 1.88 ms, 8 processes with a time of 2.03 ms).
* Problem 1 (Threads):  
Similarly to the process version, this took over 30 minutes to sort the large array, so I could not get performance data for the large array.  
With the smaller arrays however, it performed well, but the more workers included led to a longer time to sort (1 worker with a time of 0.13 ms, 4 workers with a time of 0.42 ms, 8 workers with a time of 1.29 ms). 
* Problem 2 (Threads):  
Unlike the other problems, I was able to use this on the large array and had the following results  
1 Worker: 0.58 ms  
2 Workers: 0.80 ms  
4 Workers: 0.43 ms  
8 Workers: 0.69 ms  
* Problem 2 (Processes): 
Unlike Problem 1, I was able to use this on the large array and had the following results
1 Worker: 0.75 ms
2 Workers: 1.45 ms  
4 Workers: 1.03 ms  
8 Workers: 2.44 ms
## Conclusion
* Some key findings were that thread-based MapReduce well for parallel tasks and that parallel merge sort shows good scalability when using multiple cores.
* Some challenges were balancing chunk sizes because we had to avoid load imbalance and implementing efficient merging for large datasets.
* Some limitations are that merge sort is sequential, so it could be made more parallel in the future and thread overhead could take up a lot of the resources for small data sets. 
* Future improvements are possibly making the merging phase parallel and using thread pools and dynamic load balancing. 