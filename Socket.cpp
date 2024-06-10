#include "Socket.h"
#include "Error.h"
#include "ReadAndWrite.h"
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <signal.h>

addrinfo getHint() {
    addrinfo hint;
    memset(&hint, 0, sizeof(addrinfo));
    hint.ai_family = AF_UNSPEC;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_flags = AI_PASSIVE;
    return hint;
}

addrinfo * getResult(string hostname, string port) {
    addrinfo *result;
    addrinfo hint = getHint();

    int status = getaddrinfo(hostname.c_str(), port.c_str(), &hint, &result);
    exitIfGetAddrInfoError(status);
    return result;
} 

int getSocket(addrinfo *result) {
    int socketfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    exitIfError(socketfd, "Error creating socket: ");

    // Making sure socket gets handled on early termination:
    signal(SIGTERM, cleanExit);
    signal(SIGINT, cleanExit);
    return socketfd;
}

void bindSocket(int socketfd, addrinfo *result) {
    int status = bind(socketfd, result->ai_addr, result->ai_addrlen);
    exitIfError(status, "Error binding socket: ");
}

void connectSocket(int socketfd, addrinfo *result) {
    int status = connect(socketfd, result->ai_addr, result->ai_addrlen);
    exitIfError(status, "Error connecting socket: ");
}

Socket::Socket(string hostname, string port, bool isServer) {
    addrinfo *result = getResult(hostname, port);
    int socketfd = getSocket(result);
    if (isServer) {
        bindSocket(socketfd, result);
    } else {
        connectSocket(socketfd, result);
    }
    freeaddrinfo(result);
    _fd = socketfd;
}

Socket::~Socket() {
    close(_fd);
}

// Client --------------------------------------------------

ClientSocket::ClientSocket(string hostname, string port) 
    : Socket(hostname, port, false) {}

string ClientSocket::readString() {
    return readFromFD(_fd);
}

void ClientSocket::writeString(const string &toWrite) {
    writeToFD(_fd, toWrite);
}

// Server --------------------------------------------------

ServerSocket::ServerSocket(string hostname, string port) 
    : Socket(hostname, port, true) {}

void ServerSocket::setListen(int bufferSize) {
    int status = listen(_fd, bufferSize);
    exitIfError(status, "Error when setting socket to listen: ");
}

optional<int> ServerSocket::acceptRequest() {
    sockaddr_storage theirAddr;
    socklen_t theirSize = sizeof(theirAddr);
    int newfd = accept(_fd, (sockaddr*) &theirAddr, &theirSize);
    if (newfd == -1) {
        printErrno("Error accepting client: ");
        return optional<int>();
    }
    return newfd;
}
