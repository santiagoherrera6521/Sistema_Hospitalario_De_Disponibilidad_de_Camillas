#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <set>
#include "client_socket.h"

int main() {
    std::string ip;
    std::cout << "Ingrese la IP del servidor: ";
    std::getline(std::cin, ip);

    SocketClient client(ip, 8080);

    if (!client.connectToServer()) {
        return 1;
    }

    std::set<std::string> admitidos;

    while (true) {
        std::string nombre;
        std::cout << "\nIngrese el nombre del paciente: ";
        std::getline(std::cin, nombre);

        std::time_t t = std::time(nullptr);
        std::tm* tm_info = std::localtime(&t);
        char buffer[20];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S", tm_info);
        std::string timestamp = buffer;

        std::string mensaje = nombre + "|" + timestamp;
        client.sendMessage(mensaje);
        std::string respuesta = client.receiveMessage();

        if (admitidos.count(nombre)) {
            std::cout << "[ALTA] " << nombre << "\n";
            admitidos.erase(nombre);
        } else {
            std::cout << "[ADMISION] " << nombre << "\n";
            admitidos.insert(nombre);
        }

        std::cout << "Respuesta servidor: " << respuesta << "\n";
    }

    client.disconnect();
    return 0;
}