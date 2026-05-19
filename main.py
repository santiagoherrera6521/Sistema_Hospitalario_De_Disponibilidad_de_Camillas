import time
import streamlit as st
from config     import HOSPITAL_NOMBRE, REFRESH_INTERVAL
from data       import get_estado_camas
from styles     import inject_styles
from components import render_header, render_stats, render_grid, render_footer


def main():
    st.set_page_config(
        page_title=HOSPITAL_NOMBRE,
        page_icon="🏥",
        layout="wide",
    )

    inject_styles()
    render_header()

    camas = get_estado_camas()  # TODO: reemplazar con hospital_bridge.getCamas()

    render_stats(camas)
    render_grid(camas)
    render_footer()

    time.sleep(REFRESH_INTERVAL)
    st.rerun()


if __name__ == "__main__":
    main()
