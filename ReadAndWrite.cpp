#include "ReadAndWrite.h"
#include "Error.h"
#include <unistd.h>
#include <sstream>
using std::stringbuf, std::string, std::pair;

constexpr static int BUFFER_SIZE = 1024;

string readFromFD(int fd, string stopStr) {
    stringbuf sbuf;
    char cbuf[BUFFER_SIZE];
    int status = 1;
    do {
        status = read(fd, &cbuf, BUFFER_SIZE);
        exitIfError(status, "Error while reading message: ");
        sbuf.sputn(cbuf, status);
        // If stopStr is "", then just waiting till read status is 0 or -1
        if (!stopStr.empty() && sbuf.str().rfind(stopStr) != string::npos) {
            break;
        }
    } while (status > 0);
    return sbuf.str();
}


void writeToFD(int fd, const string &toWrite) {
    const char *cStr = toWrite.c_str();
    const int length = toWrite.length();
    int curSent = 0;
    int status = 0;
    do {
        status = write(fd, cStr + curSent, length - curSent);
        exitIfError(status, "Error while writing message: ");
        curSent += status;
    } while (curSent < length);
}

