#ifndef GESTOR_CAMAS_H
#define GESTOR_CAMAS_H

#include "RegistroPaciente.h"
#include <string>
#include <map>
#include <vector>
using namespace std;

const int TOTAL_CAMAS = 20;

class GestorCamas {
private:
    map<string, RegistroPaciente> mapa_pacientes;
    vector<bool>                  camas_ocupadas;

    GestorCamas();

public:
    static GestorCamas& instancia();

    GestorCamas(const GestorCamas&)            = delete;
    GestorCamas& operator=(const GestorCamas&) = delete;

    string procesarMensaje(const string& mensaje);
    string getEstadoCompleto() const;

    int    getCamasDisponibles()  const;
    int    getCamasOcupadas()     const;
    int    getCamaPaciente(const string& id_paciente) const;
    string getHoraIngreso(const string& id_paciente)  const;
    string getPacienteEnCama(int numero_cama)          const;
};

#endif
