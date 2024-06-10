#ifndef kobak_readandwrite
#define kobak_readandwrite

#include <string>
using std::string, std::pair;

// will return part after
string readFromFD(int fd, string stopStr = "");
void writeToFD(int fd, const string &toWrite);

#endif