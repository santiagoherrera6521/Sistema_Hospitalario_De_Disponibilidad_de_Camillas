#ifndef SERVIDOR_SOCKET_H
#define SERVIDOR_SOCKET_H

#include <atomic>
#include <string>
using namespace std;

// Clase que encapsula el servidor TCP.
// Escucha conexiones entrantes del generador (PC 1), lee el mensaje,
// lo delega a GestorCamas y devuelve la respuesta.
// Cada cliente se atiende en un hilo independiente para no bloquear
// nuevas conexiones mientras se procesa una admision o alta.
class ServidorSocket {
private:
    int           puerto;
    int           server_fd;
    atomic<bool>  activo;

    // Hilo de escucha principal (corre en background via thread::detach)
    void bucleEscucha();

    // Atiende a un cliente individual en su propio hilo
    void atenderCliente(int socket_cliente);

    // Sanitiza saltos de linea del mensaje recibido
    static void sanitizarMensaje(string& msg);

public:
    ServidorSocket(int puerto);
    ~ServidorSocket();

    // Inicia el servidor en un hilo separado
    void iniciar();

    // Detiene el servidor de forma segura
    void detener();

    // Retorna true si el servidor esta escuchando
    bool estaActivo() const;
};

#endif // SERVIDOR_SOCKET_H
