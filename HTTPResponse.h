#ifndef kobak_response
#define kobak_response

#include <string>
using std::string;

enum HTTPError {
    NotImplemented = 501,
    NotSupported = 502,
    NotFound = 404,
    Forbidden = 403,
    InternalServerError = 500,
};

void handleHTTPRequest(int reqFD, string &request);
void sendHTTPError(int reqFD, HTTPError errorType);
void sendHTTPResponse(int reqFD, string &body);
string putIntoHTML(string text);

#endif
