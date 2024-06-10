#ifndef kobak_server
#define kobak_server

#include "ThreadPool.h"
#include "Socket.h"
#include <string>
using std::string;

class Server {
public:
    Server(string hostname, string port, int numThreads, int bufferSize);
    void run();
    static string calcPort();
private:
    ServerSocket socket;
    ThreadPool threadPool;
    int _bufferSize;
};

#endif