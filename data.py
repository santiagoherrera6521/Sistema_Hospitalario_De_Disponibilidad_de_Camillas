import random
import streamlit as st
from datetime import datetime
from config import TOTAL_CAMAS

# TODO: cuando tu compañero entregue la .so, eliminar get_estado_camas()
# y en main.py reemplazar su llamada por:
#
#   import hospital_bridge
#   camas = hospital_bridge.getCamas()
#
# El dict debe tener esta estructura:
# {
#   "CAMA-01": { "ocupada": bool, "paciente": str | None, "hora_ingreso": str | None },
#   ...
# }

def get_estado_camas():
    if "camas" not in st.session_state:
        st.session_state.camas = {
            f"CAMA-{str(i+1).zfill(2)}": {
                "ocupada": False,
                "paciente": None,
                "hora_ingreso": None,
            }
            for i in range(TOTAL_CAMAS)
        }

    camas   = st.session_state.camas
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
