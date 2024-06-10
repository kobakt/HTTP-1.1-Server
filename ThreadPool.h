#ifndef kobak_threadpool
#define kobak_threadpool

#include <string>
#include <queue>
#include <vector>
#include <pthread.h>
#include <semaphore.h>

using std::string, std::queue, std::vector;

class ThreadPool {
public:
    ThreadPool(int numThreads);
    ~ThreadPool();
    void addRequest(int reqFD);
private:
    sem_t sem;
    pthread_mutex_t reqFDMutex = PTHREAD_MUTEX_INITIALIZER;
    queue<int> reqFDQueue; 
    vector<pthread_t> threads;
    static void* threadStart(void *);
    int getNextReqFD();
};

#endif