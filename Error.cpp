#include "Error.h"
#include <errno.h>
#include <iostream>
#include <netdb.h>
using std::cerr, std::endl;

void cleanExit(int){
    exit(0);
}

void debugMsg(string debugMsg) {
    cerr << debugMsg << endl;
}

void printErrno(string errorMsg) {
    cerr << errorMsg << "errno " << errno << endl;
}

void exitIfError(int status, string errorMsg) {
    if (status == -1) {
        printErrno(errorMsg);
        exit(-1);
    }
}

void exitIfThreadError(int status, string errorMsg) {
    if (status != 0) {
        cerr << errorMsg <<  status << endl;
        exit(-1);
    }
}

void exitIfGetAddrInfoError(int status) {
    if (status != 0) {
        cerr << "Error from getaddrinfo: " << gai_strerror(status) << endl;
        exit(-1);
    }
}