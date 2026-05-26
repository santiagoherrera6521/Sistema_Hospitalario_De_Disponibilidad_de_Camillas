import random
import streamlit as st
from datetime import datetime
from config import TOTAL_CAMAS

# Intentar importar el módulo SWIG generado por PC2
# Para que funcione, hospital_server.py y _hospital_server.pyd
# deben estar en la misma carpeta que main.py
try:
    import hospital_server as hs
    MODO_REAL = True
except ImportError:
    MODO_REAL = False

def get_estado_camas():
    if MODO_REAL:
        return _get_camas_desde_servidor()
    else:
        return _get_camas_simuladas()

def _get_camas_desde_servidor():
    """Consulta el estado real de las camas via SWIG desde el servidor C++ de PC2."""
    try:
        camas = {}
        for i in range(1, TOTAL_CAMAS + 1):
            cama_id = f"CAMA-{str(i).zfill(2)}"
            paciente = hs.obtener_paciente_en_cama(i)

            if paciente and paciente != "":
                hora = hs.obtener_hora_ingreso(paciente)
                camas[cama_id] = {
                    "ocupada": True,
                    "paciente": paciente,
                    "hora_ingreso": hora,
                }
            else:
                camas[cama_id] = {
                    "ocupada": False,
                    "paciente": None,
                    "hora_ingreso": None,
                }
        return camas
    except Exception as e:
        st.warning(f"⚠️ Error al consultar el servidor: {e}. Usando datos simulados.")
        return _get_camas_simuladas()

def _get_camas_simuladas():
    """Datos simulados para pruebas cuando el servidor C++ no está disponible."""
    if "camas" not in st.session_state:
        st.session_state.camas = {
            f"CAMA-{str(i+1).zfill(2)}": {
                "ocupada": False,
                "paciente": None,
                "hora_ingreso": None,
            }
            for i in range(TOTAL_CAMAS)
        }

    camas = st.session_state.camas
    cama_id = random.choice(list(camas.keys()))

    if camas[cama_id]["ocupada"]:
        camas[cama_id] = {"ocupada": False, "paciente": None, "hora_ingreso": None}
    else:
        camas[cama_id] = {
            "ocupada": True,
            "paciente": f"PX-{random.randint(10000, 99999)}",
            "hora_ingreso": datetime.now().strftime("%H:%M:%S"),
        }

    return camas

def get_modo():
    """Retorna si estamos en modo real o simulado, útil para mostrarlo en la interfaz."""
    return "🟢 Servidor real (SWIG)" if MODO_REAL else "🟡 Modo simulación"
