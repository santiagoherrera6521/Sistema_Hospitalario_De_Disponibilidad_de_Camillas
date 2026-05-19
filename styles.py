import streamlit as st

def inject_styles():
    st.markdown("""
    <style>
        @import url('https://fonts.googleapis.com/css2?family=Inter:wght@300;400;500;600&family=JetBrains+Mono:wght@400;500&display=swap');

        html, body, [class*="css"] { font-family: 'Inter', sans-serif; }

        .stApp {
            background-color: #020e1f;
            background-image:
                radial-gradient(ellipse at 20% 10%, rgba(0, 82, 165, 0.18) 0%, transparent 55%),
                radial-gradient(ellipse at 80% 90%, rgba(0, 48, 115, 0.22) 0%, transparent 55%),
                repeating-linear-gradient(
                    0deg, transparent, transparent 38px,
                    rgba(255,255,255,0.012) 38px, rgba(255,255,255,0.012) 40px
                ),
                repeating-linear-gradient(
                    90deg, transparent, transparent 38px,
                    rgba(255,255,255,0.012) 38px, rgba(255,255,255,0.012) 40px
                );
        }

        .hospital-header {
            background: linear-gradient(135deg, #031a3a 0%, #052d5e 100%);
            border: 1px solid #0a4a8a;
            border-radius: 16px;
            padding: 24px 32px;
            margin-bottom: 28px;
        }

        .hospital-title {
            font-size: 22px;
            font-weight: 600;
            color: #e8f4ff;
        }

        .hospital-subtitle {
            font-size: 13px;
            color: #4a90d9;
            margin-top: 4px;
            font-family: 'JetBrains Mono', monospace;
        }

        .stat-card {
            background: linear-gradient(135deg, #031a3a 0%, #041e42 100%);
            border: 1px solid #0a4a8a;
            border-radius: 12px;
            padding: 20px;
            text-align: center;
        }

        .stat-number {
            font-size: 36px;
            font-weight: 600;
            font-family: 'JetBrains Mono', monospace;
        }

        .stat-label {
            font-size: 12px;
            color: #4a6fa5;
            text-transform: uppercase;
            letter-spacing: 1px;
            margin-top: 4px;
        }

        .stat-libre   { color: #3ecfcf; }
        .stat-ocupada { color: #e06c75; }
        .stat-total   { color: #5aabff; }

        .cama-card {
            border-radius: 14px;
            padding: 18px;
            margin-bottom: 12px;
            border: 1px solid transparent;
        }

        .cama-libre   { background: #021428; border-color: #0a4a8a; }
        .cama-ocupada { background: #1a0a0e; border-color: #5a1a24; }

        .cama-id {
            font-family: 'JetBrains Mono', monospace;
            font-size: 12px;
            color: #4a6fa5;
            margin-bottom: 8px;
        }

        .cama-estado-libre   { font-size: 14px; font-weight: 500; color: #3ecfcf; }
        .cama-estado-ocupada { font-size: 14px; font-weight: 500; color: #e06c75; }

        .cama-paciente {
            font-family: 'JetBrains Mono', monospace;
            font-size: 17px;
            font-weight: 600;
            color: #e8f4ff;
            margin-top: 4px;
        }

        .cama-hora {
            font-family: 'JetBrains Mono', monospace;
            font-size: 11px;
            color: #2a4a6a;
            margin-top: 6px;
        }

        .dot-libre {
            display: inline-block;
            width: 8px; height: 8px;
            background: #3ecfcf;
            border-radius: 50%;
            margin-right: 6px;
            animation: pulse 2s infinite;
        }

        .dot-ocupada {
            display: inline-block;
            width: 8px; height: 8px;
            background: #e06c75;
            border-radius: 50%;
            margin-right: 6px;
        }

        @keyframes pulse {
            0%, 100% { opacity: 1; }
            50%       { opacity: 0.3; }
        }

        .last-update {
            font-family: 'JetBrains Mono', monospace;
            font-size: 11px;
            color: #1a3a5a;
            text-align: right;
            margin-top: 24px;
        }

        #MainMenu, footer, header { visibility: hidden; }
        .block-container { padding-top: 2rem; }
    </style>
    """, unsafe_allow_html=True)
