#include "ServidorSocket.h"
#include "GestorCamas.h"

#include <iostream>
#include <thread>
#include <cstring>

// Sockets POSIX (Linux)
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

// --- Constructor / Destructor ---

ServidorSocket::ServidorSocket(int puerto)
    : puerto(puerto), server_fd(-1), activo(false) {}

ServidorSocket::~ServidorSocket() {
    detener();
}

// --- API publica ---

void ServidorSocket::iniciar() {
    if (activo.load()) {
        cout << "[SERVIDOR] Ya esta corriendo en el puerto " << puerto << endl;
        return;
    }
    // Lanzar el bucle de escucha en un hilo separado (no bloquea main)
    thread([this]() { bucleEscucha(); }).detach();
}

void ServidorSocket::detener() {
    if (activo.load()) {
        activo.store(false);
        if (server_fd != -1) {
            close(server_fd);
            server_fd = -1;
        }
        cout << "[SERVIDOR] Apagado de forma segura." << endl;
    }
}

bool ServidorSocket::estaActivo() const {
    return activo.load();
}

// --- Logica interna ---

void ServidorSocket::bucleEscucha() {
    struct sockaddr_in direccion;
    int opt = 1;
    socklen_t addrlen = sizeof(direccion);

    // 1. Crear descriptor de socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        cerr << "[ERROR] No se pudo crear el socket." << endl;
        return;
    }

    // 2. Permitir reusar el puerto tras reinicios rapidos
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        cerr << "[ERROR] setsockopt fallo." << endl;
        return;
    }

    // 3. Configurar direccion y puerto
    direccion.sin_family      = AF_INET;
    direccion.sin_addr.s_addr = INADDR_ANY;
    direccion.sin_port        = htons(puerto);

    // 4. Bind
    if (bind(server_fd, (struct sockaddr*)&direccion, sizeof(direccion)) < 0) {
        cerr << "[ERROR] Puerto " << puerto << " ocupado (bind fallo)." << endl;
        return;
    }

    // 5. Listen con backlog de 10 conexiones en cola
    if (listen(server_fd, 10) < 0) {
        cerr << "[ERROR] listen fallo." << endl;
        return;
    }

    activo.store(true);
    cout << "========================================" << endl;
    cout << "  Servidor Hospitalario activo"          << endl;
    cout << "  Puerto : " << puerto                   << endl;
    cout << "  Camas  : " << TOTAL_CAMAS              << endl;
    cout << "========================================" << endl;

    // 6. Bucle de aceptacion: cada cliente en su propio hilo
    while (activo.load()) {
        int socket_cliente = accept(server_fd,
                                    (struct sockaddr*)&direccion,
                                    &addrlen);
        if (socket_cliente < 0) {
            if (!activo.load()) break;  // cierre limpio
            continue;
        }
        // Lanzar hilo por cliente para no bloquear el accept
        thread([this, socket_cliente]() {
            atenderCliente(socket_cliente);
        }).detach();
    }
}

void ServidorSocket::atenderCliente(int socket_cliente) {
    char buffer[1024] = {0};
    int bytes = read(socket_cliente, buffer, sizeof(buffer) - 1);

    if (bytes > 0) {
        string mensaje(buffer, bytes);
        sanitizarMensaje(mensaje);

        // Delegar al gestor de camas (estado global Singleton)
        string respuesta = GestorCamas::instancia().procesarMensaje(mensaje);

        send(socket_cliente, respuesta.c_str(), respuesta.length(), 0);
    }

    close(socket_cliente);
}

void ServidorSocket::sanitizarMensaje(string& msg) {
    // Eliminar \r y \n del final del mensaje
    while (!msg.empty() && (msg.back() == '\n' || msg.back() == '\r')) {
        msg.pop_back();
    }
}
