#include <memory>
#include <iostream>
#include <string.h>
#include "Server.h"
using std::make_shared, std::shared_ptr;
using std::cerr, std::endl;

string HOSTNAME = "cs2.seattleu.edu";
const char PORT_ARG[] = "-p";
const char THREAD_ARG[] = "-t";
const char BUFFER_ARG[] = "-b";

struct ServerVals {
    string port = "100401";
    int numThreads = 1;
    int bufferSize = 1;
};

void handleArgs(const char arg1[], const char arg2[], ServerVals &vals) {
    if (strcmp(arg1, PORT_ARG) == 0) {
        vals.port = arg2;
        if (strcmp(arg2, "kobak") == 0) {
            // easier to test with
            vals.port = Server::calcPort();
        }
    } else if (strcmp(arg1, THREAD_ARG) == 0) {
        vals.numThreads = strtol(arg2, nullptr, 10);
        if (vals.numThreads == 0) {
            cerr << "Error: Args \"" << arg1 << " " << arg2 << "\" are improperly formatted." << endl;
            exit(-1);
        }
    } else if (strcmp(arg1, BUFFER_ARG) == 0) {
        vals.bufferSize = strtol(arg2, nullptr, 10);
        if (vals.bufferSize == 0) {
            cerr << "Error: Args \"" << arg1 << " " << arg2 << "\" are improperly formatted." << endl;
            exit(-1);
        }
    } else {
        cerr << "Error: Args \"" << arg1 << " " << arg2 << "\" are improperly formatted." << endl;
        exit(-1);
    }
}

int main(int argc, char *argv[]) {
    if (argc > 7 || argc % 2 == 0) {
        cerr << "Error: Incorrect number of arguments." << endl;
        exit(-1);
    }

    ServerVals vals;
    for (int i = 1; i + 1 < argc; i+=2) {
        handleArgs(argv[i], argv[i+1], vals);
    }

    Server server(HOSTNAME, vals.port, vals.numThreads, vals.bufferSize);
    server.run();

    return 0;
}