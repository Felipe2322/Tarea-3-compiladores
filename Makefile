# Makefile para Analizador Léxico (FLEX + C++)
# Compilador y herramientas
CXX = g++
FLEX = flex
RM = rm -f
LS = dir

# Flags de compilación
CXXFLAGS = -std=c++11 -Wall -g `wx-config --cxxflags`
LDFLAGS = `wx-config --libs`

# Archivos fuente
FLEX_SOURCE = lexer.l
CPP_SOURCE = lexer_gui.cpp
FLEX_OUTPUT = lex.yy.c
FLEX_HEADER = lex.yy.h
OBJECT_FILES = lex.yy.o lexer_gui.o
TARGET = analizador_lexico.exe

# Reglas de compilación
all: $(TARGET)

# Generar código C desde FLEX
$(FLEX_OUTPUT): $(FLEX_SOURCE)
	$(FLEX) -o $(FLEX_OUTPUT) $(FLEX_SOURCE)

# Compilar FLEX
lex.yy.o: $(FLEX_OUTPUT)
	$(CXX) $(CXXFLAGS) -c $(FLEX_OUTPUT) -o lex.yy.o

# Compilar C++
lexer_gui.o: $(CPP_SOURCE)
	$(CXX) $(CXXFLAGS) -c $(CPP_SOURCE) -o lexer_gui.o

# Enlazar
$(TARGET): $(OBJECT_FILES)
	$(CXX) $(OBJECT_FILES) -o $(TARGET) $(LDFLAGS)
	@echo "Build completado: $(TARGET)"

# Limpiar archivos generados
clean:
	$(RM) $(FLEX_OUTPUT) lex.yy.o lexer_gui.o $(TARGET)
	@echo "Archivos limpios"

# Ayuda
help:
	@echo "Comandos disponibles:"
	@echo "  make       - Compilar el proyecto"
	@echo "  make clean - Limpiar archivos generados"
	@echo "  make help  - Mostrar este mensaje"

.PHONY: all clean help
