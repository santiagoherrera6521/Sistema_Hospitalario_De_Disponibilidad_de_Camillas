#include "ServidorSocket.h"
#include "GestorCamas.h"

#include <iostream>
#include <csignal>
#include <thread>
#include <chrono>

using namespace std;

// Puerto por defecto (puede sobreescribirse con argumento de linea de comandos)
const int PUERTO_DEFAULT = 8080;

// Referencia global al servidor para poder apagarlo desde la senal SIGINT
ServidorSocket* servidor_global = nullptr;

// Manejador de SIGINT (Ctrl+C): apaga el servidor limpiamente
void manejadorSenal(int senal) {
    cout << "\n[MAIN] Senal " << senal << " recibida. Apagando servidor..." << endl;
    if (servidor_global != nullptr) {
        servidor_global->detener();
    }
}

int main(int argc, char* argv[]) {
    // Leer puerto desde argumentos o usar el default
    int puerto = PUERTO_DEFAULT;
    if (argc >= 2) {
        try {
            puerto = stoi(argv[1]);
        } catch (...) {
            cerr << "[MAIN] Puerto invalido. Usando puerto por defecto: "
                 << PUERTO_DEFAULT << endl;
        }
    }

    // Registrar manejador de SIGINT para apagado limpio con Ctrl+C
    signal(SIGINT, manejadorSenal);

    // Crear e iniciar el servidor
    ServidorSocket servidor(puerto);
    servidor_global = &servidor;
    servidor.iniciar();

    // Mantener el proceso vivo mientras el servidor este activo
    // El hilo principal espera con sleep para no consumir CPU
    cout << "[MAIN] Presiona Ctrl+C para detener el servidor." << endl;
    while (servidor.estaActivo()) {
        this_thread::sleep_for(chrono::milliseconds(500));
    }

    cout << "[MAIN] Servidor terminado." << endl;
    return 0;
}
