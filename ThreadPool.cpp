#include "ThreadPool.h"
#include "Error.h"
#include "HTTPResponse.h"
#include "ReadAndWrite.h"
#include <unistd.h>

ThreadPool::ThreadPool(int numThreads) 
    : threads(numThreads) {
    int status  = sem_init(&sem, 0, 0);
    exitIfError(status, "Error when initializing semaphore: ");

    for (int i = 0; i < numThreads; i++) {
        status = pthread_create(&threads[i], nullptr, threadStart, this);
        exitIfThreadError(status, "Error when initializing threads: ");
    }
}

ThreadPool::~ThreadPool() {
}

void ThreadPool::addRequest(int reqFD) {
    int status = pthread_mutex_lock(&reqFDMutex);
    exitIfThreadError(status, "Error when locking mutex: ");

    reqFDQueue.push(reqFD);
    
    status = pthread_mutex_unlock(&reqFDMutex);
    exitIfThreadError(status, "Error when unlocking mutex: ");

    status = sem_post(&sem);
    exitIfError(status, "Error posting semaphore in main: ");
}

void* ThreadPool::threadStart(void* input) {
    ThreadPool *threadPool = (ThreadPool*) input;
    while(true) {
        int status = sem_wait(&threadPool->sem);
        exitIfError(status, "Error waiting for semaphore: ");

        int reqFD = threadPool->getNextReqFD();
        
        exitIfError(status, "Error posting semaphore in thread: ");

        string request = readFromFD(reqFD, "\r\n\r\n");
        handleHTTPRequest(reqFD, request);
        close(reqFD);
    }

    // no error checking needed :)
    pthread_exit(nullptr);
}

int ThreadPool::getNextReqFD() {
    int status = pthread_mutex_lock(&reqFDMutex);
    exitIfThreadError(status, "Error when locking mutex: ");

    int reqFD = reqFDQueue.front();
    reqFDQueue.pop();

    status = pthread_mutex_unlock(&reqFDMutex);
    exitIfThreadError(status, "Error when unlocking mutex: ");
    return reqFD;
}