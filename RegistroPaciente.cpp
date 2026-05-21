#include "RegistroPaciente.h"

// Constructor por defecto: cama invalida, hora vacia
RegistroPaciente::RegistroPaciente() : numero_cama(-1), hora_ingreso("") {}

// Constructor con parametros
RegistroPaciente::RegistroPaciente(int cama, const string& hora)
    : numero_cama(cama), hora_ingreso(hora) {}
