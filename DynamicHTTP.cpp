#include "DynamicHTTP.h"
#include "Error.h"
#include "HTTPResponse.h"
#include <vector>
#include <unistd.h>
using std::vector;

extern char **environ;

vector<string> getValues(string &filePath) {
    vector<string> vals;
    size_t endOfCGI = filePath.find(".cgi") + 4;
    if (filePath.length() == endOfCGI) {
        return vals;
    }
    if (filePath[endOfCGI] != '?') {
        // garbage data
        return vals;
    }
    string curSection = filePath.substr(endOfCGI + 1);

    // TODO make this better maybe
    for (size_t i = curSection.find('&'); i < string::npos; i = curSection.find('&')) {
        // this is safe if i = npos
        vals.push_back(curSection.substr(0, i));
        curSection = curSection.substr(i + 1);
    }
    vals.push_back(curSection);
    return vals;
}

void setEnv(string &filePath) {
    vector<string> valuesList = getValues(filePath);
    for (string val : valuesList) {
        size_t equalsPos = val.find("=");
        if (equalsPos == string::npos) {
            // this should catch empty strings too
            continue;
        }
        int status = setenv(
            val.substr(0, equalsPos).c_str(), 
            val.substr(equalsPos + 1).c_str(), 
            0
        );
        exitIfError(status, "Error setting environment variables for dynamic request: ");
    }
}

void setFD(int reqFD) {
    // I'm only setting stdout because if there is an error
    // we will just send an HTTP error msg to stdout too
    int status = dup2(reqFD, 1);
    if (status == -1) {
        sendHTTPError(reqFD, HTTPError::InternalServerError);   
    }
    exitIfError(status, "Error setting dup2 for dynamic: ");
}

void execReq(string filePath) {
    string justFile = filePath.substr(0, filePath.find(".cgi") + 4);
    execle(justFile.c_str(), justFile.c_str(), nullptr, environ);
    
    printErrno("Error executing dynamic request: ");
    sendHTTPError(STDOUT_FILENO, HTTPError::InternalServerError);   
    // exit should close fd for us
    exit(-1);
}

void handleDynamic(int reqFD, string &filePath) {
  int status = fork();
  if (status == 0) {
    setEnv(filePath);
    setFD(reqFD);
    execReq(filePath);
    exit(-1); // just in case
  }
  if (status == -1) {
    printErrno("Error forking dynamic request: ");
    sendHTTPError(reqFD, HTTPError::InternalServerError);   
  }
  close(reqFD);
}

