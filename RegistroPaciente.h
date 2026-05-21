#ifndef REGISTRO_PACIENTE_H
#define REGISTRO_PACIENTE_H

#include <string>
using namespace std;

// Clase que representa el estado de un paciente admitido en el hospital.
// Almacena el numero de cama asignada y la hora de ingreso.
class RegistroPaciente {
public:
    int    numero_cama;
    string hora_ingreso;

    // Constructor por defecto
    RegistroPaciente();

    // Constructor con parametros
    RegistroPaciente(int cama, const string& hora);
};

#endif // REGISTRO_PACIENTE_H
