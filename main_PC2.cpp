#include "ServidorSocket.h"
#include "GestorCamas.h"

#include <iostream>
#include <csignal>
#include <thread>
#include <chrono>

using namespace std;

const int PUERTO_DEFAULT = 8080;

ServidorSocket* servidor_global = nullptr;

void manejadorSenal(int senal) {
    cout << "\n[MAIN] Senal " << senal << " recibida. Apagando servidor..." << endl;
    if (servidor_global != nullptr) {
        servidor_global->detener();
    }
}

int main(int argc, char* argv[]) {
    int puerto = PUERTO_DEFAULT;
    if (argc >= 2) {
        try {
            puerto = stoi(argv[1]);
        } catch (...) {
            cerr << "[MAIN] Puerto invalido. Usando puerto por defecto: "
                 << PUERTO_DEFAULT << endl;
        }
    }

    signal(SIGINT, manejadorSenal);

    ServidorSocket servidor(puerto);
    servidor_global = &servidor;
    servidor.iniciar();

    cout << "[MAIN] Presiona Ctrl+C para detener el servidor." << endl;

    // Esperar indefinidamente hasta que el servidor sea detenido con Ctrl+C
    while (true) {
        this_thread::sleep_for(chrono::milliseconds(500));
    }

    cout << "[MAIN] Servidor terminado." << endl;
    return 0;
}
