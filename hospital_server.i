// hospital_server.i
// Wrapper SWIG: expone GestorCamas y ServidorSocket a Python (Persona 3 - Visualizador)

%module hospital_server

%{
#include "RegistroPaciente.h"
#include "GestorCamas.h"
#include "ServidorSocket.h"
%}

// Soporte para std::string en Python
%include "std_string.i"

// Exponer la clase RegistroPaciente (para acceso desde Python si se necesita)
%include "RegistroPaciente.h"

// Exponer la clase GestorCamas (consultas de estado para el dashboard)
%include "GestorCamas.h"

// Exponer la clase ServidorSocket (control del servidor desde Python)
%include "ServidorSocket.h"
