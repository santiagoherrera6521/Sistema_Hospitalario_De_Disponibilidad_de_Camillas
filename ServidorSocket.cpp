#include "ServidorSocket.h"
#include "GestorCamas.h"

#include <iostream>
#include <thread>
#include <cstring>

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

ServidorSocket::ServidorSocket(int puerto)
    : puerto(puerto), server_fd(-1), activo(false) {}

ServidorSocket::~ServidorSocket() {
    detener();
}

void ServidorSocket::iniciar() {
    if (activo.load()) {
        cout << "[SERVIDOR] Ya esta corriendo en el puerto " << puerto << endl;
        return;
    }
    thread([this]() { bucleEscucha(); }).detach();
}

void ServidorSocket::detener() {
    if (activo.load()) {
        activo.store(false);
        if (server_fd != -1) {
            closesocket((SOCKET)server_fd);
            server_fd = -1;
        }
        WSACleanup();
        cout << "[SERVIDOR] Apagado de forma segura." << endl;
    }
}

bool ServidorSocket::estaActivo() const {
    return activo.load();
}

void ServidorSocket::bucleEscucha() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "[ERROR] No se pudo inicializar Winsock." << endl;
        return;
    }

    struct sockaddr_in direccion;
    int opt = 1;
    int addrlen = sizeof(direccion);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        cerr << "[ERROR] No se pudo crear el socket." << endl;
        WSACleanup();
        return;
    }
    server_fd = (int)sock;

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0) {
        cerr << "[ERROR] setsockopt fallo." << endl;
        WSACleanup();
        return;
    }

    direccion.sin_family      = AF_INET;
    direccion.sin_addr.s_addr = INADDR_ANY;
    direccion.sin_port        = htons(puerto);

    if (bind(sock, (struct sockaddr*)&direccion, sizeof(direccion)) == SOCKET_ERROR) {
        cerr << "[ERROR] Puerto " << puerto << " ocupado (bind fallo)." << endl;
        closesocket(sock);
        WSACleanup();
        return;
    }

    if (listen(sock, 10) == SOCKET_ERROR) {
        cerr << "[ERROR] listen fallo." << endl;
        closesocket(sock);
        WSACleanup();
        return;
    }

    activo.store(true);
    cout << "========================================" << endl;
    cout << "  Servidor Hospitalario activo"          << endl;
    cout << "  Puerto : " << puerto                   << endl;
    cout << "  Camas  : " << TOTAL_CAMAS              << endl;
    cout << "========================================" << endl;

    while (activo.load()) {
        SOCKET socket_cliente = accept(sock,
                                       (struct sockaddr*)&direccion,
                                       &addrlen);
        if (socket_cliente == INVALID_SOCKET) {
            if (!activo.load()) break;
            continue;
        }
        thread([this, socket_cliente]() {
            atenderCliente((int)socket_cliente);
        }).detach();
    }

    closesocket(sock);
    WSACleanup();
}

void ServidorSocket::atenderCliente(int socket_cliente) {
    char buffer[1024] = {0};
    int bytes = recv((SOCKET)socket_cliente, buffer, sizeof(buffer) - 1, 0);

    if (bytes > 0) {
        string mensaje(buffer, bytes);
        sanitizarMensaje(mensaje);

        string respuesta = GestorCamas::instancia().procesarMensaje(mensaje);
        send((SOCKET)socket_cliente, respuesta.c_str(), respuesta.length(), 0);
    }

    closesocket((SOCKET)socket_cliente);
}

void ServidorSocket::sanitizarMensaje(string& msg) {
    while (!msg.empty() && (msg.back() == '\n' || msg.back() == '\r')) {
        msg.pop_back();
    }
}
