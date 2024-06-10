#ifndef kobak_error
#define kobak_error

#include <string>
using std::string;

void cleanExit(int);
void debugMsg(string debugMsg);
void printErrno(string errorMsg);
void exitIfError(int status, string errorMsg);
void exitIfThreadError(int status, string errorMsg);
void exitIfGetAddrInfoError(int status);

#endif