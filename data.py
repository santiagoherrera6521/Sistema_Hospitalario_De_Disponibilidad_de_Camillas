import socket
import streamlit as st
from datetime import datetime
from config import TOTAL_CAMAS

# IP y puerto del servidor C++ (PC de Julian)
SERVIDOR_IP   = "192.168.1.9"
SERVIDOR_PORT = 8080

def _consultar_servidor():
    """Envía STATUS al servidor y retorna el texto de respuesta."""
    try:
        with socket.create_connection((SERVIDOR_IP, SERVIDOR_PORT), timeout=2) as s:
            s.sendall(b"STATUS")
            respuesta = s.recv(4096).decode("utf-8")
            return respuesta
    except Exception:
        return None

def _parsear_respuesta(respuesta):
    """
    Parsea la respuesta del servidor con formato:
    STATUS|CAMA-01:LIBRE,CAMA-02:PX-12345:10:32:00,...
    """
    camas = {}
    try:
        # Quitar prefijo STATUS|
        if respuesta.startswith("STATUS|"):
            respuesta = respuesta[7:]

        entradas = respuesta.split(",")
        for entrada in entradas:
            partes = entrada.split(":")
            cama_id = partes[0]  # CAMA-01
            if partes[1] == "LIBRE":
                camas[cama_id] = {
                    "ocupada": False,
                    "paciente": None,
                    "hora_ingreso": None,
                }
            else:
                camas[cama_id] = {
                    "ocupada": True,
                    "paciente": partes[1],
                    "hora_ingreso": partes[2] if len(partes) > 2 else "",
                }
    except Exception:
        return None
    return camas

def get_estado_camas():
    respuesta = _consultar_servidor()
    if respuesta:
        camas = _parsear_respuesta(respuesta)
        if camas:
            return camas
    # Fallback: datos simulados si no hay servidor
    st.warning("⚠️ No se pudo conectar al servidor. Mostrando datos simulados.")
    return _get_camas_vacias()

def _get_camas_vacias():
    return {
        f"CAMA-{str(i+1).zfill(2)}": {
            "ocupada": False,
            "paciente": None,
            "hora_ingreso": None,
        }
        for i in range(TOTAL_CAMAS)
    }

def get_modo():
    respuesta = _consultar_servidor()
    if respuesta:
        return "🟢 Servidor real (socket)"
    return "🟡 Modo simulación"
