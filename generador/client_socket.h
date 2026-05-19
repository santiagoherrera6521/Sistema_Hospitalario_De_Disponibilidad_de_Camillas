#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H

#include <string>
#include <winsock2.h>

class SocketClient {
public:
    SocketClient(std::string host, int port);
    bool connectToServer();
    bool sendMessage(std::string mensaje);
    void disconnect();

private:
    std::string host;
    int port;
    SOCKET sock;
};

#endif