# Sistema Hospitalario De Disponibilidad de Camillas

Sistema distribuido en tiempo real para la gestión y visualización de disponibilidad de camillas hospitalarias. Desarrollado en C++ (backend y comunicación) y Python con Streamlit (visualizador).

---

##  Descripción

El sistema simula el flujo de admisión y alta de pacientes en un hospital. Cuando un paciente ingresa, se le asigna automáticamente una camilla disponible. Cuando es dado de alta, esa camilla queda libre nuevamente. Todo el estado se refleja en tiempo real en una interfaz gráfica.

### Componentes principales

| Componente | Lenguaje | Descripción |
|---|---|---|
| Generador de pacientes | C++ | Emite IDs de pacientes vía socket TCP |
| Servidor hospitalario | C++ | Gestiona camillas y expone el estado como librería dinámica |
| Visualizador | Python + Streamlit | Muestra disponibilidad en tiempo real mediante SWIG |

---

## Arquitectura

```
[PC 1]                        [PC 2]                        [PC 3]
Generador C++  ──socket──►  Servidor C++  ──lib .so──►   Visualizador
(emite IDs px)              (gestiona camas)   (SWIG)    Python/Streamlit
```

**Protocolo de comunicación (socket TCP):**
```
Mensaje enviado:    PX-00341|2026-05-14T10:32:00
Respuesta servidor: ADMITTED|CAMA-07
                    DISCHARGED|CAMA-07
```

---

##  Requisitos previos

### Generador y Servidor (C++)
- Windows 10 o superior
- MinGW-w64 o MSVC (Visual Studio 2019+)
- SWIG 4.x ([descargar aquí](https://www.swig.org/download.html))

### Visualizador (Python)
- Python 3.10 o superior
- pip actualizado

```bash
pip install streamlit
```

---

##  Estructura del repositorio

```
Sistema Hospitalario De Disponibilidad de Camillas/
│
├── generador/
│   ├── generator.cpp         # Generador de IDs de pacientes
│   ├── client socket.cpp     # Cliente socket TCP
│   └── README.md
│
├── servidor/
│   ├── hospital server.cpp   # Servidor TCP + lógica de camas
│   ├── hospital server.h     # Cabeceras
│   ├── hospital server.i     # Wrapper SWIG
│   └── README.md
│
├── visualizador/
│   ├── visualizer.py         # Interfaz Streamlit
│   └── README.md
│
├── docs/
│   ├── diagramas UML/
│   └── documentacion tecnica/
│
└── README.md
```

---

##  Instalación y ejecución

### 1. Clonar el repositorio

```bash
git clone https://github.com/santiagoherrera6521/Sistema_Hospitalario_De_Disponibilidad_de_Camillas.git
cd Sistema_Hospitalario_De_Disponibilidad_de_Camillas
```

### 2. Compilar el servidor (PC 2)

```bash
cd servidor
g++ -shared -o libhospital.so hospital_server.cpp -fPIC
swig -c++ -python hospital_server.i
g++ -shared -o _hospital_server.pyd hospital_server_wrap.cxx libhospital.so
```

### 3. Compilar el generador (PC 1)

```bash
cd generador
g++ -o generador generator.cpp client_socket.cpp -lws2_32
```

### 4. Ejecutar el servidor (PC 2)

```bash
./hospital_server
```

### 5. Ejecutar el visualizador (PC 3)

```bash
cd visualizador
streamlit run visualizer.py
```

### 6. Ejecutar el generador (PC 1)

```bash
./generador <IP_del_servidor> <puerto>
```

---

##  Ramas del repositorio

| Rama | Descripción |
|---|---|
| `main` | Versión estable e integrada |
| `feature/generador` | Desarrollo del generador de pacientes |
| `feature/servidor` | Desarrollo del servidor y librería dinámica |
| `feature/visualizador` | Desarrollo de la interfaz Streamlit |

---

##  Integrantes

| Nombre | Componente |
|---|---|
| Luis Felipe Orrego Rojas| Generador de pacientes (C++) |
| Julian Zapata Gallego | Servidor hospitalario + SWIG (C++) |
| Santiago Herrera Oquendo | Visualizador + Git/Docs (Python) |

---

##  Estado del proyecto

- [ ] Generador de IDs de pacientes
- [ ] Cliente socket TCP
- [ ] Servidor socket TCP
- [ ] Lógica de asignación y liberación de camillas
- [ ] Compilación como librería dinámica (.so)
- [ ] Wrapper SWIG
- [x] Interfaz Streamlit
- [ ] Integración completa entre los 3 componentes
- [x] Documentación y diagramas UML

