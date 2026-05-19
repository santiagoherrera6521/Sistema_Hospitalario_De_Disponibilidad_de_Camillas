import streamlit as st
from datetime import datetime
from config import HOSPITAL_NOMBRE, REFRESH_INTERVAL


def render_header():
    ahora = datetime.now().strftime("%A %d %b %Y")
    st.markdown(f"""
    <div class="hospital-header">
        <div class="hospital-title">🏥 {HOSPITAL_NOMBRE}</div>
        <div class="hospital-subtitle">Sistema de Disponibilidad de Camillas · {ahora}</div>
    </div>
    """, unsafe_allow_html=True)


def render_stats(camas):
    total    = len(camas)
    ocupadas = sum(1 for c in camas.values() if c["ocupada"])
    libres   = total - ocupadas

    col1, col2, col3 = st.columns(3)
    with col1:
        st.markdown(f"""
        <div class="stat-card">
            <div class="stat-number stat-total">{total}</div>
            <div class="stat-label">Total camillas</div>
        </div>""", unsafe_allow_html=True)
    with col2:
        st.markdown(f"""
        <div class="stat-card">
            <div class="stat-number stat-libre">{libres}</div>
            <div class="stat-label">Disponibles</div>
        </div>""", unsafe_allow_html=True)
    with col3:
        st.markdown(f"""
        <div class="stat-card">
            <div class="stat-number stat-ocupada">{ocupadas}</div>
            <div class="stat-label">Ocupadas</div>
        </div>""", unsafe_allow_html=True)


def render_cama(cama_id, info):
    if info["ocupada"]:
        st.markdown(f"""
        <div class="cama-card cama-ocupada">
            <div class="cama-id">{cama_id}</div>
            <div class="cama-estado-ocupada"><span class="dot-ocupada"></span>Ocupada</div>
            <div class="cama-paciente">{info['paciente']}</div>
            <div class="cama-hora">⏱ Ingreso: {info['hora_ingreso']}</div>
        </div>
        """, unsafe_allow_html=True)
    else:
        st.markdown(f"""
        <div class="cama-card cama-libre">
            <div class="cama-id">{cama_id}</div>
            <div class="cama-estado-libre"><span class="dot-libre"></span>Disponible</div>
        </div>
        """, unsafe_allow_html=True)


def render_grid(camas):
    st.markdown("<br>", unsafe_allow_html=True)
    items         = list(camas.items())
    cols_por_fila = 5
    for i in range(0, len(items), cols_por_fila):
        fila = items[i:i + cols_por_fila]
        cols = st.columns(cols_por_fila)
        for j, (cama_id, info) in enumerate(fila):
            with cols[j]:
                render_cama(cama_id, info)


def render_footer():
    ahora = datetime.now().strftime("%H:%M:%S")
    st.markdown(f"""
    <div class="last-update">
        Última actualización: {ahora} · refresco cada {REFRESH_INTERVAL}s
    </div>
    """, unsafe_allow_html=True)
