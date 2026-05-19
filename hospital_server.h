#ifndef HOSPITAL_SERVER_H
#define HOSPITAL_SERVER_H

#include <string>
#include <map>
#include <vector>

using namespace std;

// Estructura para almacenar el estado del paciente admitido
struct RegistroPaciente {
    int numero_cama;
    string hora_ingreso;
};

// Capacidad instalada optimizada para la demostración en el Dashboard
const int TOTAL_CAMAS = 20;

// Funciones del ciclo de vida del servidor de red
void iniciar_servidor_hospital(int puerto);
void detener_servidor_hospital();

// API de consulta expuesta mediante SWIG para el Visualizador (Persona 3)
int obtener_cama_paciente(string id_paciente);
string obtener_hora_ingreso(string id_paciente);
string obtener_paciente_en_cama(int numero_cama);
int obtener_camas_disponibles();
int obtener_camas_ocupadas();
bool esta_servidor_activo();

// Lógica interna de procesamiento de protocolos
string procesar_mensaje_socket(string mensaje_recibido);

#endif // HOSPITAL_SERVER_H