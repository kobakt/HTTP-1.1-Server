#include "Socket.h"
#include <iostream>
#include <string.h>
#include <utility>
using std::cin, std::cout, std::cerr, std::endl, std::pair;

const char *PORT_ARG = "-p";
const char *SERVER_ARG = "-s"; 
const string BLANK_LINE = "\r\n\r\n";
const string CONTENT_LENGTH = "Content-Length: ";

string getBody(string msg)
{
    int blankLinePosEnd = msg.find(BLANK_LINE) + BLANK_LINE.length();
    return msg.substr(blankLinePosEnd);
}

bool checkContentLength(string msg) {
    string body = getBody(msg);

    unsigned int contLenValStart = msg.find(CONTENT_LENGTH) + CONTENT_LENGTH.length();
    if (contLenValStart == string::npos) {
        return body.length() == 0;
    }

    int contLenValEnd = msg.find(BLANK_LINE, contLenValStart);
    string contLenValStr = msg.substr(contLenValStart, contLenValEnd);
    unsigned int contLen = atoi(contLenValStr.c_str());

    return body.length() == contLen;
}

struct ClientArgs {
    string serverName = "cs2.seattleu.edu";
    string port = "10474";
};

void handleArgPair(ClientArgs &clientArgs, char *arg1, char *arg2) {
    if (strcmp(arg1, PORT_ARG) == 0) {
        clientArgs.port = string(arg2);
    } else if (strcmp(arg1, SERVER_ARG) == 0) {
        clientArgs.serverName = string(arg2);
    } else {
        cerr << "Incorrect format for args " << arg1 << " and " << arg2 << "." << endl;
    }
}

ClientArgs handleArgs(int argc, char **argv) {
    ClientArgs clientArgs;
    for (int i = 1; i + 1 < argc; i+=2) {
        handleArgPair(clientArgs, argv[i], argv[i+1]);
    }
    return clientArgs;
}

pair<string, string> splitURL(string url) {
    // to support https:// at beginning, first looking for a dot
    // and then the slash
    unsigned long firstDot = url.find(".");
    unsigned long firstSlash = url.find("/", firstDot);
    if (firstSlash == string::npos) {
        return pair<string, string>(url, "/");
    }
    return pair<string, string>(
        url.substr(0, firstSlash), 
        url.substr(firstSlash + 1)
    );
}

string createReq(string url) {
    pair<string, string> urlParts = splitURL(url);
    string request = "GET " + urlParts.second + " HTTP/1.1\r\n";
    request += "Host: " + urlParts.first + "\r\n";;
    request += "\r\n";
    return request;
}

int main(int argc, char **argv) {
    if (argc % 2 != 1 || argc > 5) {
        cerr << "Improper argument format." << endl;
        exit(-1);
    }

    ClientArgs clientArgs = handleArgs(argc, argv);
    ClientSocket socket(clientArgs.serverName, clientArgs.port);

    string url;
    cin >> url;
    string request = createReq(url);

    socket.writeString(request);
    string response = socket.readString();

    if (!checkContentLength(response)) {
        cerr << "Error receiving full message. ";
        cerr << "Printing partial message below:" << endl;
    }

    cout << response << endl;

    return 0;
}