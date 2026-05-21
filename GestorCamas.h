#ifndef GESTOR_CAMAS_H
#define GESTOR_CAMAS_H

#include "RegistroPaciente.h"
#include <string>
#include <map>
#include <vector>
using namespace std;

// Capacidad maxima del hospital para la demo
const int TOTAL_CAMAS = 20;

// Clase que encapsula toda la logica de negocio del hospital:
// admision de pacientes, alta medica, consulta de estado de camas.
// Es un Singleton para garantizar un unico estado global compartido
// entre el hilo del servidor y las consultas de SWIG.
class GestorCamas {
private:
    map<string, RegistroPaciente> mapa_pacientes;  // id_paciente -> registro
    vector<bool>                  camas_ocupadas;  // indice 1..TOTAL_CAMAS

    // Constructor privado (patron Singleton)
    GestorCamas();

public:
    // Obtener la instancia unica
    static GestorCamas& instancia();

    // Evitar copias del Singleton
    GestorCamas(const GestorCamas&)            = delete;
    GestorCamas& operator=(const GestorCamas&) = delete;

    // --- Logica de negocio ---

    // Procesa un mensaje del protocolo "ID_PACIENTE|TIMESTAMP"
    // Retorna "ADMITTED|CAMA-XX", "DISCHARGED|CAMA-XX" o "ERROR|..."
    string procesarMensaje(const string& mensaje);

    // --- Consultas de estado (expuestas via SWIG al visualizador) ---

    int    getCamasDisponibles()  const;
    int    getCamasOcupadas()     const;
    int    getCamaPaciente(const string& id_paciente) const;
    string getHoraIngreso(const string& id_paciente)  const;
    string getPacienteEnCama(int numero_cama)          const;
};

#endif // GESTOR_CAMAS_H
