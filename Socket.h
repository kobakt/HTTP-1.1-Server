#ifndef kobak_socket
#define kobak_socket

#include <string>
#include <optional>
using std::string, std::optional;

class Socket {
public:
    Socket(string hostname, string port, bool isServer);
    virtual ~Socket();
protected:
    int _fd = 0; // 0 should never be used, but just in case
};

class ClientSocket : public Socket {
public:
    ClientSocket(string hostname, string port);
    string readString();
    void writeString(const string &);
};

class ServerSocket : public Socket {
public:
    ServerSocket(string hostname, string port);
    void setListen(int bufferSize);
    // optional just so you know error checking has been done
    optional<int> acceptRequest();    
};

#endif