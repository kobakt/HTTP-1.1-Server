#include "HTTPResponse.h"
#include "ReadAndWrite.h"
#include "Error.h"
#include <optional>
#include <fcntl.h>
#include "DynamicHTTP.h"
#include <sstream>
#include <chrono>
#include <iomanip>
using std::string_view, std::to_string, std::optional;

const string getFormat = "GET ";
const string httpFormat = " HTTP/";
const string htmlStart = R"(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8">
    <title>Kobak HTTP Server</title>
  </head>
  <body>
    <p><h1>)";
const string htmlEnd = R"(</h1></p>
  </body>
</html>
)"; 

string putIntoHTML(string text) {
    return htmlStart + text + htmlEnd;
}

string getErrorCode(HTTPError errorType) {
    switch (errorType) {
        case HTTPError::NotImplemented:
            return "Not Implemented";
        case HTTPError::NotSupported:
            return "Not Supported";
        case HTTPError::NotFound:
            return "Not Found";
        case HTTPError::Forbidden:
            return "Forbidden";
        default: // just in case
        case HTTPError::InternalServerError:
            return "Internal Server Error";
    }
}

string getErrorText(HTTPError errorType) {
    switch (errorType) {
        case HTTPError::NotImplemented:
            return "Server does not implement this HTTP method.";
        case HTTPError::NotSupported:
            return "Server does not support this version of HTTP.";
        case HTTPError::NotFound:
            return "Server could not find this item.";
        case HTTPError::Forbidden:
            return "Server could not read this item.";
        default: // just in case
        case HTTPError::InternalServerError:
            return "Server could not complete this request.";
    }
}

string getTime() {
    auto time = std::time(nullptr);
    std::stringstream buf;
    buf << std::put_time(std::gmtime(&time), "%a, %d %b %Y %H:%M:%S GMT");
    return buf.str();
}

void sendHTTP(int reqFD, string &firstLine, string &body) {
    string header = firstLine;
    header += "Content-Length: " + to_string(body.length()) + "\r\n";
    header += "Date: " + getTime() + "\r\n";
    header += "Content-Type: text/html\r\n";
    header += "Server: Kobak Server\r\n";
    header += "Connection: close\r\n";
    header += "\r\n";
    writeToFD(reqFD, header);
    writeToFD(reqFD, body);
}

void sendHTTPError(int reqFD, HTTPError errorType) {
    string error = to_string(errorType) + " " + getErrorCode(errorType);
    string firstLine = "HTTP/1.1 " + error + "\r\n";

    string body = putIntoHTML(error + "\r\n" + getErrorText(errorType));
    sendHTTP(reqFD, firstLine, body);
}

void sendHTTPResponse(int reqFD, string &body) {
    string firstLine = "HTTP/1.1 200 OK\r\n";
    sendHTTP(reqFD, firstLine, body);
}

// only call if request/line has valid GET and HTTP
inline string getFilePath(string &request) {
    string url = request.substr(
        getFormat.length(), 
        request.find(httpFormat) - getFormat.length()
    );
    int firstDot = url.find('.');
    int itemStart = url.find('/', firstDot);
    string filePath = url.substr(itemStart + 1);
    if (filePath.empty() || filePath == "/") {
        return "index.html";
    }
    return filePath;
}

optional<HTTPError> checkErrors(string &request) {
    string line = request.substr(0, request.find("\r\n"));
    size_t getStart = line.find(getFormat);
    size_t httpStart = line.rfind(httpFormat);

    if (httpStart == string::npos) {
        // garbage request
        return HTTPError::InternalServerError;
    }
    if (line.substr(httpStart + httpFormat.length()) != "1.1") {
        return HTTPError::NotSupported;
    }
    if (getStart != 0) {
        return HTTPError::NotImplemented;
    }
    // this is safe because getStart == 0 and httpStart exists
    // if (line.substr(getStart + getFormat.length(), httpStart).find("..") != string::npos) {
    if (getFilePath(line).find("..") != string::npos) {
        return HTTPError::Forbidden;
    }
    return optional<HTTPError>();
}

void handleStatic(int reqFD, string &filePath) {
    int fileFD = open(filePath.c_str(), O_RDONLY);
    if (fileFD == -1) {
        if (errno == ENOENT) {
            sendHTTPError(reqFD, HTTPError::NotFound);
        } else {
            sendHTTPError(reqFD, HTTPError::InternalServerError);
            printErrno("Error opnening file: ");
        }
        return;
    }
    string file = readFromFD(fileFD);
    sendHTTPResponse(reqFD, file);
}

bool isDynamic(string &filePath) {
    size_t lastDot = filePath.rfind(".");
    return lastDot != string::npos 
        && filePath.find("cgi", lastDot) == lastDot + 1;
}

void attemptResponse(int reqFD, string &request) {
    string filePath = getFilePath(request);
    // debugMsg(filePath);
    if (isDynamic(filePath)) {
        handleDynamic(reqFD, filePath);
    } else {
        handleStatic(reqFD, filePath);
    }
}

void handleHTTPRequest(int reqFD, string &request) {
    // just a nice message so you know server is working.
    string firstLine = request.substr(0, request.find("\r\n"));
    debugMsg("Handling request: " + firstLine);
    
    optional<HTTPError> formatErrors = checkErrors(request);
    if (formatErrors.has_value()) {
        sendHTTPError(reqFD, formatErrors.value());
    } else {
        attemptResponse(reqFD, request);
    }
}