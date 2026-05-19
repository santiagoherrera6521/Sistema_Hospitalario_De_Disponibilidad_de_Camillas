#include "hospital_server.h"
#include <iostream>
#include <cstring>
#include <thread>
#include <atomic>
#include <sstream>

// Sockets POSIX
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

// Estado del hospital en memoria global
map<string, RegistroPaciente> mapa_pacientes;
vector<bool> camas_ocupadas(TOTAL_CAMAS + 1, false); // Indexado de 1 a 20
atomic<bool> servidor_activo(false);
int server_fd = -1;

int obtener_camas_disponibles() {
    return TOTAL_CAMAS - mapa_pacientes.size();
}

int obtener_camas_ocupadas() {
    return mapa_pacientes.size();
}

int obtener_cama_paciente(string id_paciente) {
    auto it = mapa_pacientes.find(id_paciente);
    if (it != mapa_pacientes.end()) {
        return it->second.numero_cama;
    }
    return -1; 
}

string obtener_hora_ingreso(string id_paciente) {
    auto it = mapa_pacientes.find(id_paciente);
    if (it != mapa_pacientes.end()) {
        return it->second.hora_ingreso;
    }
    return "";
}

string obtener_paciente_en_cama(int numero_cama) {
    if (numero_cama < 1 || numero_cama > TOTAL_CAMAS) return "";
    
    // Buscar el paciente que tiene asignada dicha celda/cama
    for (auto const& [id, registro] : mapa_pacientes) {
        if (registro.numero_cama == numero_cama) {
            return id;
        }
    }
    return ""; // Cama vacía
}

bool esta_servidor_activo() {
    return servidor_activo.load();
}

string procesar_mensaje_socket(string mensaje_recibido) {
    // Parser del protocolo: Separar ID_PACIENTE y TIMESTAMP usando el caracter '|'
    stringstream ss(mensaje_recibido);
    string id_paciente;
    string timestamp;
    
    if (!getline(ss, id_paciente, '|') || !getline(ss, timestamp)) {
        return "ERROR|INVALID_PROTOCOL_FORMAT";
    }

    // Lógica de repetición: Si el ID ya existe -> Proceso de ALTA
    if (mapa_pacientes.find(id_paciente) != mapa_pacientes.end()) {
        int cama_a_liberar = mapa_pacientes[id_paciente].numero_cama;
        camas_ocupadas[cama_a_liberar] = false;
        mapa_pacientes.erase(id_paciente);
        
        string cama_str = (cama_a_liberar < 10 ? "CAMA-0" : "CAMA-") + to_string(cama_a_liberar);
        cout << "[ALTA] Paciente " << id_paciente << " libero la celda " << cama_str << endl;
        return "DISCHARGED|" + cama_str;
    }

    // Lógica de ADMISIÓN: Buscar primera cama vacía
    int cama_asignada = -1;
    for (int i = 1; i <= TOTAL_CAMAS; ++i) {
        if (!camas_ocupadas[i]) {
            cama_asignada = i;
            break;
        }
    }

    if (cama_asignada != -1) {
        RegistroPaciente nuevo_registro = {cama_asignada, timestamp};
        mapa_pacientes[id_paciente] = nuevo_registro;
        camas_ocupadas[cama_asignada] = true;
        
        string cama_str = (cama_asignada < 10 ? "CAMA-0" : "CAMA-") + to_string(cama_asignada);
        cout << "[ADMISION] Paciente " << id_paciente << " asignado a " << cama_str << endl;
        return "ADMITTED|" + cama_str;
    } else {
        cout << "[RECHAZO] Hospital saturado. No hay camas para " << id_paciente << endl;
        return "ERROR|HOSPITAL_FULL";
    }
}

void bucle_escucha_socket(int puerto) {
    struct sockaddr_in direccion;
    int opt = 1;
    int addrlen = sizeof(direccion);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        cerr << "Error al abrir descriptor de socket nativo." << endl;
        return;
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        cerr << "Error en configuracion setsockopt." << endl;
        return;
    }

    direccion.sin_family = AF_INET;
    direccion.sin_addr.s_addr = INADDR_ANY;
    direccion.sin_port = htons(puerto);

    if (bind(server_fd, (struct sockaddr*)&direccion, sizeof(direccion)) < 0) {
        cerr << "Puerto " << puerto << " ocupado. Fallo en bind." << endl;
        return;
    }

    if (listen(server_fd, 10) < 0) {
        cerr << "Fallo en modo escucha del socket." << endl;
        return;
    }

    servidor_activo.store(true);
    cout << " >>> Servidor C++ Operativo Escuchando en Puerto " << puerto << " <<<" << endl;

    while (servidor_activo.load()) {
        int nuevo_socket = accept(server_fd, (struct sockaddr*)&direccion, (socklen_t*)&addrlen);
        if (nuevo_socket < 0) {
            if (!servidor_activo.load()) break;
            continue;
        }

        char buffer[1024] = {0};
        int bytes_leidos = read(nuevo_socket, buffer, 1024);
        if (bytes_leidos > 0) {
            string mensaje(buffer);
            
            // Sanitización básica de retornos de carro
            if (!mensaje.empty() && mensaje.back() == '\n') mensaje.pop_back();
            if (!mensaje.empty() && mensaje.back() == '\r') mensaje.pop_back();

            string respuesta = procesar_mensaje_socket(mensaje);
            send(nuevo_socket, respuesta.c_str(), respuesta.length(), 0);
        }
        close(nuevo_socket);
    }
}

void iniciar_servidor_hospital(int puerto) {
    if (!servidor_activo.load()) {
        thread(bucle_escucha_socket, puerto).detach();
    }
}

void detener_servidor_hospital() {
    if (servidor_activo.load()) {
        servidor_activo.store(false);
        if (server_fd != -1) {
            close(server_fd);
        }
        cout << "Servidor de sockets apagado de forma segura." << endl;
    }
}