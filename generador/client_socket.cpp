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
    // Reconectar antes de cada envío
    closesocket(sock);
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &server.sin_addr);
    
    if (connect(sock, (sockaddr*)&server, sizeof(server)) < 0) {
        std::cout << "[Error] No se pudo reconectar al servidor.\n";
        return false;
    }
    
    int result = send(sock, mensaje.c_str(), mensaje.size(), 0);
    return result != SOCKET_ERROR;
}

std::string SocketClient::receiveMessage() {
    char buffer[1024] = {0};
    int bytes = recv(sock, buffer, sizeof(buffer), 0);
    if (bytes > 0) {
        return std::string(buffer, bytes);
    }
    return "";
}

void SocketClient::disconnect() {
    closesocket(sock);
    WSACleanup();
}