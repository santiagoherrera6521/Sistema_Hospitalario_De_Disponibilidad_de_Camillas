# ============================================================
# Makefile - Servidor Hospitalario
# ============================================================
# Uso:
#   make            -> compila todo (ejecutable + .so + SWIG)
#   make servidor   -> solo el ejecutable hospital_server
#   make lib        -> solo la libreria dinamica libhospital.so
#   make swig       -> genera el wrapper Python via SWIG
#   make clean      -> elimina todos los binarios generados
# ============================================================

CXX      = g++
CXXFLAGS = -std=c++17 -Wall -fPIC
LDFLAGS  = -lpthread

# Detectar Python automaticamente
PYTHON_INC := $(shell python3-config --includes 2>/dev/null || echo "-I/usr/include/python3.10")
PYTHON_EXT := $(shell python3-config --extension-suffix 2>/dev/null || echo ".so")

# Archivos fuente de la logica (sin main)
SRCS_LIB = RegistroPaciente.cpp GestorCamas.cpp ServidorSocket.cpp

# Archivos objeto de la logica
OBJS_LIB = $(SRCS_LIB:.cpp=.o)

# ============================================================
# Objetivo por defecto: compila todo
# ============================================================
all: servidor lib swig

# ============================================================
# Ejecutable standalone del servidor (para PC 2)
# ============================================================
servidor: $(OBJS_LIB) main.o
	$(CXX) $(CXXFLAGS) -o hospital_server $^ $(LDFLAGS)
	@echo ">>> Ejecutable listo: ./hospital_server [puerto]"

# ============================================================
# Libreria dinamica compartida (usada por SWIG / visualizador)
# ============================================================
lib: $(OBJS_LIB)
	$(CXX) $(CXXFLAGS) -shared -o libhospital.so $^ $(LDFLAGS)
	@echo ">>> Libreria lista: libhospital.so"

# ============================================================
# Wrapper SWIG para Python
# ============================================================
swig: lib hospital_server_wrap.o
	$(CXX) $(CXXFLAGS) -shared -o _hospital_server$(PYTHON_EXT) \
		hospital_server_wrap.o -L. -lhospital $(LDFLAGS)
	@echo ">>> Modulo Python listo: _hospital_server$(PYTHON_EXT)"

hospital_server_wrap.cxx: hospital_server.i RegistroPaciente.h GestorCamas.h ServidorSocket.h
	swig -c++ -python hospital_server.i

hospital_server_wrap.o: hospital_server_wrap.cxx
	$(CXX) $(CXXFLAGS) $(PYTHON_INC) -c hospital_server_wrap.cxx -o hospital_server_wrap.o

# ============================================================
# Regla generica para objetos
# ============================================================
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ============================================================
# Limpieza
# ============================================================
clean:
	rm -f *.o *.so hospital_server hospital_server_wrap.cxx \
	      hospital_server.py hospital_server_wrap.h *$(PYTHON_EXT)
	@echo ">>> Limpieza completa."

.PHONY: all servidor lib swig clean
