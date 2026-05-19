#include "client_socket.h"
#include <iostream>
#include <ws2tcpip.h>

SocketClient::SocketClient(std::string host, int port) {
    this->host = host;
    this->port = port;
}

bool SocketClient::connectToServer() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);

    sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &server.sin_addr);

    if (connect(sock, (sockaddr*)&server, sizeof(server)) < 0) {
        std::cout << "[Error] No se pudo conectar al servidor.\n";
        return false;
    }

    std::cout << "[OK] Conectado al servidor.\n";
    return true;
}

bool SocketClient::sendMessage(std::string mensaje) {
    int result = send(sock, mensaje.c_str(), mensaje.size(), 0);
    return result != SOCKET_ERROR;
}

void SocketClient::disconnect() {
    closesocket(sock);
    WSACleanup();
}