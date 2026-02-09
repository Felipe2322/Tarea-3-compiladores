# Analizador Léxico con FLEX y C++

## 📋 Descripción

Analizador léxico desarrollado usando **FLEX** (Fast Lexical Analyzer Generator) y **C++** con interfaz gráfica multiplataforma usando **wxWidgets**. El programa realiza el análisis léxico de código fuente, identificando y clasificando los tokens según su tipo.

## 🛠️ Requisitos de Instalación

### En Windows

1. **MinGW** (Compilador C++)
   - Descargar desde: https://www.mingw-w64.org/
   - Añadir a PATH

2. **FLEX**
   - Descargar desde: https://gnuwin32.sourceforge.net/packages/flex.htm
   - Instalar y añadir a PATH

3. **wxWidgets** (Opcional para GUI)
   ```bash
   # O descargar desde: https://www.wxwidgets.org/
   ```

4. **Git** (Ya tenemos)

## 📁 Estructura del Proyecto

```
analizador-lexico-js/
├── lexer.l              # Definición FLEX de tokens
├── lexer_gui.cpp        # Interfaz gráfica (C++ + wxWidgets)
├── Makefile             # Archivo de compilación
├── README.md            # Este archivo
└── AUTOMATA.md          # Documentación del autómata
```

## 🔧 Tokens Reconocidos

| Tipo | Patrón | Ejemplos |
|------|--------|----------|
| KEYWORD | Palabras reservadas | `if`, `else`, `while`, `for`, `return`, `int`, `float` |
| NUMBER | Dígitos con decimales | `42`, `3.14`, `1.5e-10` |
| IDENTIFIER | Nombres de variables | `x`, `miVariable`, `_private` |
| STRING | Cadenas entre comillas | `"hola"`, `"mundo"` |
| OPERATOR | Operadores matemáticos y lógicos | `+`, `-`, `==`, `&&`, `++` |
| PUNCTUATION | Signos de puntuación | `{`, `}`, `;`, `,`, `.` |
| COMMENT | Comentarios | `// comentario`, `/* bloque */` |

## 🚀 Compilación

```bash
# Compilar todo
make

# Limpiar archivos generados
make clean

# Ver ayuda
make help
```

### Pasos manuales de compilación:

```bash
# 1. Generar código C desde FLEX
flex -o lex.yy.c lexer.l

# 2. Compilar FLEX
g++ -std=c++11 -c lex.yy.c -o lex.yy.o

# 3. Compilar C++
g++ -std=c++11 -c lexer_gui.cpp -o lexer_gui.o `wx-config --cxxflags`

# 4. Enlazar
g++ lex.yy.o lexer_gui.o -o analizador_lexico.exe `wx-config --libs`
```

## 📊 Especificación del Autómata

El analizador léxico implementa un **Autómata Finito Determinista (AFD)** que reconoce los siguientes tokens:

### Estados del Autómata

```
S0 (Inicial)
├── [a-zA-Z_] → S1 (IDENTIFIER/KEYWORD)
├── [0-9] → S2 (NUMBER)
├── " → S3 (STRING)
├── [+\-*/%=<>&|!] → S4 (OPERATOR)
├── [{};(),.:] → S5 (PUNCTUATION)
├── / → S6 (COMMENT)
└── [ \t\n] → S7 (WHITESPACE - Ignore)
```

### Transiciones:

1. **IDENTIFIER/KEYWORD**: `[a-zA-Z_][a-zA-Z0-9_]*`
2. **NUMBER**: `[0-9]+(\.[0-9]+)?([eE][+-]?[0-9]+)?`
3. **STRING**: `"([^"\\]|\\.)*"`
4. **OPERATOR**: Expresiones como `++`, `--`, `==`, `!=`, `&&`, `||`
5. **PUNCTUATION**: Caracteres individuales
6. **COMMENTS**: 
   - Línea: `//.*`
   - Bloque: `/*[\s\S]*?*/`

## 💻 Uso

1. Ejecutar el programa:
   ```bash
   ./analizador_lexico.exe
   ```

2. Escribir o pegar código en el editor
3. Hacer clic en **"Analizar"** para procesar
4. Los tokens aparecen en la tabla con:
   - **ID**: Número secuencial
   - **Tipo**: Clasificación del token
   - **Lexema**: Valor exacto del token
   - **Línea**: Ubicación en el código
   - **Columna**: Posición en la línea

## 📝 Ejemplo de Entrada

```cpp
int main() {
    int x = 10;
    float y = 3.14;
    if (x > 5) {
        return x + y;
    }
}
```

### Salida Esperada

| ID | Tipo | Lexema | Línea | Columna |
|----|------|--------|-------|---------|
| 1 | KEYWORD | int | 1 | 1 |
| 2 | IDENTIFIER | main | 1 | 5 |
| 3 | PUNCTUATION | ( | 1 | 9 |
| 4 | PUNCTUATION | ) | 1 | 10 |
| 5 | PUNCTUATION | { | 1 | 11 |
| ... | ... | ... | ... | ... |

## 🔍 Características

- ✅ **Interfaz Gráfica**: Editor de código y tabla de tokens
- ✅ **Análisis en Tiempo Real**: Procesa código al hacer clic
- ✅ **Múltiples Tipos de Token**: Keywords, números, strings, operadores, etc.
- ✅ **Rastreo de Posición**: Registra línea y columna de cada token
- ✅ **Manejo de Comentarios**: Ignora comentarios de línea y bloque
- ✅ **Interfaz Multiplataforma**: wxWidgets funciona en Windows, Linux, macOS

## 📋 Teoría del Análisis Léxico

El análisis léxico es la **primera fase** de un compilador:

1. **Entrada**: Código fuente en texto
2. **Proceso**: Reconocimiento de patrones (expresiones regulares)
3. **Salida**: Stream de tokens clasificados

### Ventajas de usar FLEX

- Automatiza la generación del autómata
- Mejora mantenibilidad del código
- Reduce errores en patrones complejos
- Genera código C/C++ optimizado

## 📚 Referencias

- FLEX Manual: https://westes.github.io/flex/manual/
- wxWidgets Documentation: https://docs.wxwidgets.org/
- Teoría de Compiladores: Aho, Sethi, Ullman

## 👤 Autor

**Desarrollador**: Felipe
**Fecha**: Febrero 2026
**Lenguaje**: C++ con FLEX
**Herramientas**: FLEX, wxWidgets, MinGW

## 📄 Licencia

Proyecto educativo - Libre para uso académico

---

**¿Preguntas o problemas?** Revisa el archivo AUTOMATA.md para más detalles sobre el autómata finito.
