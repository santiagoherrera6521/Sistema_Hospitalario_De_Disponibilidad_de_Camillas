# Servidor Hospitalario — Parte 2

Servidor TCP en C++ que recibe IDs de pacientes del **Generador (PC 1)**, gestiona la asignación y liberación de camas, y expone el estado del hospital a través de una librería dinámica (`.so`) consumida por el **Visualizador (PC 3)** mediante SWIG.

---

## Arquitectura del servidor

```
[PC 1 - Generador]                [PC 2 - Servidor]               [PC 3 - Visualizador]
                                  ┌─────────────────────────────┐
  generator.cpp  ──TCP socket──►  │  main.cpp                   │
                                  │    └─ ServidorSocket         │──── libhospital.so ──► Python/Streamlit
                                  │         └─ GestorCamas       │         (SWIG)
                                  │              └─ RegistroPac. │
                                  └─────────────────────────────┘
```

---

## Protocolo de comunicación

| Dirección | Formato | Ejemplo |
|-----------|---------|---------|
| Cliente → Servidor | `ID_PACIENTE\|TIMESTAMP` | `PX-00341\|2026-05-14T10:32:00` |
| Servidor → Cliente (admisión) | `ADMITTED\|CAMA-XX` | `ADMITTED\|CAMA-07` |
| Servidor → Cliente (alta) | `DISCHARGED\|CAMA-XX` | `DISCHARGED\|CAMA-07` |
| Servidor → Cliente (error) | `ERROR\|MOTIVO` | `ERROR\|HOSPITAL_FULL` |

**Lógica de alta:** si el servidor recibe el mismo `ID_PACIENTE` por segunda vez, interpreta que el paciente recibe el alta y libera su cama.

---

## Estructura de archivos

```
servidor/
├── RegistroPaciente.h      # Clase: datos de un paciente admitido
├── RegistroPaciente.cpp
├── GestorCamas.h           # Clase: lógica de negocio (admisión/alta/consultas)
├── GestorCamas.cpp
├── ServidorSocket.h        # Clase: servidor TCP con hilos por cliente
├── ServidorSocket.cpp
├── main.cpp                # Punto de entrada del ejecutable
├── hospital_server.i       # Wrapper SWIG para exponer clases a Python
├── Makefile                # Compilación en un comando
└── README.md               # Este archivo
```

---

## Diagrama de clases

```
┌──────────────────────┐        ┌──────────────────────────────────────┐
│   RegistroPaciente   │        │              GestorCamas             │
├──────────────────────┤        ├──────────────────────────────────────┤
│ + numero_cama: int   │◄───────│ - mapa_pacientes: map<str, Registro> │
│ + hora_ingreso: str  │        │ - camas_ocupadas: vector<bool>       │
├──────────────────────┤        ├──────────────────────────────────────┤
│ + RegistroPaciente() │        │ + instancia(): GestorCamas& [static] │
│ + RegistroPaciente(  │        │ + procesarMensaje(msg): string       │
│     cama, hora)      │        │ + getCamasDisponibles(): int         │
└──────────────────────┘        │ + getCamasOcupadas(): int            │
                                │ + getCamaPaciente(id): int           │
                                │ + getHoraIngreso(id): string         │
                                │ + getPacienteEnCama(n): string       │
                                └──────────────────────────────────────┘
                                               ▲
                                               │ usa
                                ┌──────────────────────────┐
                                │      ServidorSocket      │
                                ├──────────────────────────┤
                                │ - puerto: int            │
                                │ - server_fd: int         │
                                │ - activo: atomic<bool>   │
                                ├──────────────────────────┤
                                │ + iniciar()              │
                                │ + detener()              │
                                │ + estaActivo(): bool     │
                                │ - bucleEscucha()         │
                                │ - atenderCliente(fd)     │
                                └──────────────────────────┘
```

---

## Requisitos

- **Sistema operativo:** Linux (Ubuntu 20.04+) o WSL2 en Windows
- **Compilador:** `g++` con soporte C++17
- **SWIG:** versión 4.x
- **Python:** 3.10+ con `python3-dev`

Instalar dependencias en Ubuntu/Debian:
```bash
sudo apt update
sudo apt install g++ make swig python3-dev
```

---

## Compilación

```bash
# Compilar todo de una vez (ejecutable + .so + wrapper SWIG)
make

# O por partes:
make servidor   # solo el ejecutable
make lib        # solo libhospital.so
make swig       # solo el modulo Python
```

---

## Ejecución

```bash
# Puerto por defecto (8080)
./hospital_server

# Puerto personalizado
./hospital_server 9090
```

Salida esperada:
```
========================================
  Servidor Hospitalario activo
  Puerto : 8080
  Camas  : 20
========================================
[MAIN] Presiona Ctrl+C para detener el servidor.
[ADMISION] PX-00341 asignado a CAMA-01
[ADMISION] PX-00342 asignado a CAMA-02
[ALTA]     PX-00341 libero CAMA-01
```

---

## Integración con el Visualizador (PC 3)

El visualizador importa el módulo generado por SWIG:

```python
import hospital_server

gestor = hospital_server.GestorCamas.instancia()
disponibles = gestor.getCamasDisponibles()
ocupadas    = gestor.getCamasOcupadas()
paciente    = gestor.getPacienteEnCama(3)
```

> Los archivos `libhospital.so`, `_hospital_server.so` y `hospital_server.py` deben estar en el mismo directorio que el visualizador, o en el `PYTHONPATH`.

---

## Integrantes

| Nombre | Componente |
|--------|-----------|
| Luis Felipe Orrego | Generador de pacientes (C++) |
| Julian | **Servidor hospitalario + SWIG (C++)** ← esta parte |
| Santiago Herrera Oquendo | Visualizador + Git/Docs (Python) |
