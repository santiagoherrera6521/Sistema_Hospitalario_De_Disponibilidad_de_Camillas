#include "GestorCamas.h"
#include <sstream>
#include <iostream>
using namespace std;

// --- Singleton ---

GestorCamas::GestorCamas()
    : camas_ocupadas(TOTAL_CAMAS + 1, false) {}  // indice 0 ignorado, 1..20 usados

GestorCamas& GestorCamas::instancia() {
    static GestorCamas instancia_unica;
    return instancia_unica;
}

// --- Logica de negocio ---

string GestorCamas::procesarMensaje(const string& mensaje) {
    // Protocolo esperado: "PX-XXXXX|2026-05-14T10:32:00"
    stringstream ss(mensaje);
    string id_paciente, timestamp;

    if (!getline(ss, id_paciente, '|') || !getline(ss, timestamp)) {
        return "ERROR|INVALID_PROTOCOL_FORMAT";
    }

    // Si el paciente ya existe -> ALTA medica
    auto it = mapa_pacientes.find(id_paciente);
    if (it != mapa_pacientes.end()) {
        int cama = it->second.numero_cama;
        camas_ocupadas[cama] = false;
        mapa_pacientes.erase(it);

        string cama_str = (cama < 10 ? "CAMA-0" : "CAMA-") + to_string(cama);
        cout << "[ALTA]     " << id_paciente << " libero " << cama_str << endl;
        return "DISCHARGED|" + cama_str;
    }

    // Si el paciente es nuevo -> ADMISION
    int cama_asignada = -1;
    for (int i = 1; i <= TOTAL_CAMAS; ++i) {
        if (!camas_ocupadas[i]) {
            cama_asignada = i;
            break;
        }
    }

    if (cama_asignada == -1) {
        cout << "[RECHAZO]  Hospital saturado. No hay camas para " << id_paciente << endl;
        return "ERROR|HOSPITAL_FULL";
    }

    mapa_pacientes[id_paciente] = RegistroPaciente(cama_asignada, timestamp);
    camas_ocupadas[cama_asignada] = true;

    string cama_str = (cama_asignada < 10 ? "CAMA-0" : "CAMA-") + to_string(cama_asignada);
    cout << "[ADMISION] " << id_paciente << " asignado a " << cama_str << endl;
    return "ADMITTED|" + cama_str;
}

// --- Consultas de estado ---

int GestorCamas::getCamasDisponibles() const {
    return TOTAL_CAMAS - (int)mapa_pacientes.size();
}

int GestorCamas::getCamasOcupadas() const {
    return (int)mapa_pacientes.size();
}

int GestorCamas::getCamaPaciente(const string& id_paciente) const {
    auto it = mapa_pacientes.find(id_paciente);
    return (it != mapa_pacientes.end()) ? it->second.numero_cama : -1;
}

string GestorCamas::getHoraIngreso(const string& id_paciente) const {
    auto it = mapa_pacientes.find(id_paciente);
    return (it != mapa_pacientes.end()) ? it->second.hora_ingreso : "";
}

string GestorCamas::getPacienteEnCama(int numero_cama) const {
    if (numero_cama < 1 || numero_cama > TOTAL_CAMAS) return "";
    for (auto const& [id, registro] : mapa_pacientes) {
        if (registro.numero_cama == numero_cama) return id;
    }
    return "";  // cama libre
}
