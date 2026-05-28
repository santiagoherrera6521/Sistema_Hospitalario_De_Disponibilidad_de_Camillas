#include <iostream>
#include <string>
#include <random>
#include <sstream>
#include <iomanip>
#include <ctime>
#include "client_socket.h"

int main() {
    std::string ip;
    std::cout << "Ingrese la IP del servidor: ";
    std::getline(std::cin, ip);

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> distID(1, 99999);

    while (true) {
        std::string nombre;
        std::cout << "Ingrese el nombre del paciente: ";
        std::getline(std::cin, nombre);

        int numero = distID(rng);

        std::ostringstream oss;
        oss << "PX-" << std::setw(5) << std::setfill('0') << numero;
        std::string id = oss.str();

        std::time_t t = std::time(nullptr);
        std::tm* tm_info = std::localtime(&t);
        char buffer[20];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S", tm_info);
        std::string timestamp = buffer;

        std::string mensaje = id + "|" + timestamp;

        // Reconectar por cada paciente
        SocketClient client(ip, 8080);
        if (!client.connectToServer()) {
            std::cout << "[Error] No se pudo conectar al servidor.\n";
            continue;
        }

        client.sendMessage(mensaje);
        std::string respuesta = client.receiveMessage();
        client.disconnect();

        std::cout << "ID del paciente " << nombre << ": " << mensaje << "\n";
        std::cout << "Respuesta servidor: " << respuesta << "\n\n";
    }

    return 0;
}
