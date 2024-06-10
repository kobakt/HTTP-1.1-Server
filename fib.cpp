#include "HTTPResponse.h"
#include <iostream>
#include <string>
using std::cout, std::endl, std::string;

const int BUFFER_SIZE = 512;
const int MODULO_NUM = 1000000007;
const char *FORMAT = "%s, welcome to the CGI Program!\nThe %sth Fibonacci number is %d.";

unsigned int fib(int n) {
    unsigned int n1 = 0;
    unsigned int n2 = 1;
    for (int i = 0; i < n; i++) {
        unsigned int t = n2;
        n2 = (n1 + n2) % MODULO_NUM;
        n1 = t;
    }
    return n1;
}

int main() {
    char *user = getenv("user");
    char *n = getenv("n");
    if (user == nullptr || n == nullptr) {
        sendHTTPError(1, HTTPError::InternalServerError);
        return -1;
    }
    unsigned int val = fib(atoi(n));
    
    char bodyBuf[BUFFER_SIZE];
    snprintf(bodyBuf, BUFFER_SIZE, FORMAT, user, n, val);
    string body = putIntoHTML(bodyBuf);
    sendHTTPResponse(1, body);
    
    return 0;
}