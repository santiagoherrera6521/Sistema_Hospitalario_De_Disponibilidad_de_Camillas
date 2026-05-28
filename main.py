import time
import streamlit as st
import hospital_server as hs
from config     import HOSPITAL_NOMBRE, REFRESH_INTERVAL
from styles     import inject_styles
from components import render_header, render_stats, render_grid, render_footer

def get_estado_camas():
    gestor = hs.GestorCamas.instancia()
    camas = {}
    for i in range(1, 21):
        cama_id = f"CAMA-{str(i).zfill(2)}"
        paciente = gestor.getPacienteEnCama(i)
        if paciente and paciente != "":
            hora = gestor.getHoraIngreso(paciente)
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

def main():
    st.set_page_config(
        page_title=HOSPITAL_NOMBRE,
        page_icon="🏥",
        layout="wide",
    )

    # Iniciar servidor una sola vez
    if "servidor_iniciado" not in st.session_state:
        servidor = hs.ServidorSocket(8080)
        servidor.iniciar()
        st.session_state.servidor_iniciado = True
        st.session_state.servidor = servidor

    inject_styles()
    render_header()

    camas = get_estado_camas()

    render_stats(camas)
    render_grid(camas)
    render_footer()

    time.sleep(REFRESH_INTERVAL)
    st.rerun()

if __name__ == "__main__":
    main()