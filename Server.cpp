#include "Server.h"
#include "Error.h"
#include "HTTPResponse.h"
#include <iostream>
using std::cout, std::endl, std::hash, std::to_string, std::optional;

Server::Server(string hostname, string port, int numThreads, int bufferSize)
    : socket(hostname, port), threadPool(numThreads), 
    _bufferSize(bufferSize) 
{
    cout << "Running server at host: " << hostname << " port: " << port;
    cout << " with " << numThreads;
    cout << " threads and a buffer size of " << bufferSize << "." << endl;
}

void Server::run() {
    socket.setListen(_bufferSize);

    while (true) {
        optional<int> newfd = socket.acceptRequest();
        if (newfd.has_value()) {
            threadPool.addRequest(newfd.value());
        }
    }
}

string Server::calcPort() {
    const string& student_id = "4186535";
    // Use std::hash function to hash the student ID
    size_t hash_value = hash<string>{}(student_id);
    // Map the hash value to the range of 0-99
    int port_number = static_cast<int>(hash_value % 100);
    // Map the port number to the range of 10401-10500
    port_number += 10401;
    return to_string(port_number);
}
