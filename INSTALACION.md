# Guía de Instalación y Compilación - Windows

## 📋 Requisitos Previos

1. **Windows 10 o superior**
2. Acceso a descargas e instalación de programas
3. Conexión a internet

## 🔧 Paso 1: Instalar MinGW (Compilador C++)

### Opción A: Usar MinGW-w64 (Recomendado)

1. Ir a: https://www.mingw-w64.org/
2. Descargar **Online Installer**
3. Ejecutar el instalador
4. Seleccionar:
   - **Version**: Latest (2024 o superior)
   - **Architecture**: x86_64 (para 64-bits)
   - **Threads**: posix
   - **Exception handling**: dwarf

5. Instalar en `C:\mingw64`

6. **Agregar a PATH**:
   - Presionar `Win + X` → Seleccionar "Sistema"
   - Click en "Configuración avanzada del sistema"
   - Click en "Variables de entorno"
   - Agregar a PATH de usuario: `C:\mingw64\bin`
   - Cerrar y reiniciar terminal

7. **Verificar instalación**:
```bash
g++ --version
gcc --version
```

## 🛠️ Paso 2: Instalar FLEX

### Opción A: GnuWin32

1. Ir a: https://gnuwin32.sourceforge.net/packages/flex.htm
2. Descargar:
   - **Binaries**: flex-2.5.4a-1.bin.zip
   - **Dependencies**: (si es necesario)

3. Extraer en `C:\Program Files\GnuWin32\`

4. **Agregar a PATH**:
   - Agregar `C:\Program Files\GnuWin32\bin` a PATH
   - Reiniciar terminal

5. **Verificar instalación**:
```bash
flex --version
```

### Opción B: Usar WinFlexBison (Más fácil)

1. Ir a: https://github.com/lexxmark/winflexbison/releases
2. Descargar `win_flex_bison-[version].zip`
3. Extraer en `C:\win_flex_bison`
4. Agregar a PATH: `C:\win_flex_bison`
5. Verificar:
```bash
flex --version
```

## 📦 Paso 3: Instalar wxWidgets (Opcional para GUI)

Si quieres compilar la versión con interfaz gráfica:

1. Descargar desde: https://www.wxwidgets.org/
2. Descargar **wxWidgets 3.2.x** (precompilado para Windows o Build tools)
3. Instalar en `C:\wxWidgets-3.2.0` o similar
4. Agregar variable de entorno:
   - `WXWIN = C:\wxWidgets-3.2.0`

## 🚀 Paso 4: Compilar el Analizador

### 1. Abrir PowerShell o Cmd

```bash
cd C:\Users\[TuUsuario]\Desktop\Compildores\analizador-lexico-js
```

### 2. Compilar con Make (Recomendado)

```bash
# Primero instalar Make (si no tienes)
choco install make          # Si tienes Chocolatey

# O descargar make.exe desde: https://gnuwin32.sourceforge.net/packages/make.htm
# Y agregar a PATH

# Compilar
make
```

### 3. Si No Tienes Make, Compilar Manualmente

```bash
# Paso 1: Generar código C desde FLEX
flex -o lex.yy.c lexer.l

# Paso 2: Compilar FLEX
g++ -std=c++11 -c lex.yy.c -o lex.yy.o

# Paso 3: Compilar C++ (versión sin GUI, más simple)
# Ver archivo: lexer_simple.cpp (próxima versión)

# O compilar con wxWidgets si está instalado:
g++ -std=c++11 -c lexer_gui.cpp -o lexer_gui.o

# Paso 4: Enlazar
g++ lex.yy.o lexer_gui.o -o analizador_lexico.exe
```

## 💾 Compilación Alternativa: Versión Simple (Sin wxWidgets)

Si prefieres una versión más simple que no requiera wxWidgets:

```bash
# Solo necesitas:
flex -o lex.yy.c lexer.l
g++ -std=c++11 lex.yy.c lexer_simple.cpp -o analizador_lexico_console.exe

# Luego ejecutar:
.\analizador_lexico_console.exe
```

## ✅ Verificar Compilación

```bash
# Debe existir el archivo ejecutable:
dir analizador_lexico.exe

# Debería mostrar algo como:
# -a---- 2.5 MB  15/02/2026 12:34 analizador_lexico.exe
```

## 🎯 Ejecutar el Analizador

```bash
# Ejecutar:
.\analizador_lexico.exe

# O simplemente:
analizador_lexico.exe
```

## 🔧 Solución de Problemas

### Problema: "flex: no se reconoce el comando"
**Solución**: 
```bash
# Verificar que FLEX esté en PATH:
where flex

# Si no aparece, agregar manualmente:
$env:PATH += ";C:\Program Files\GnuWin32\bin"

# O actualizar PATH permanentemente (requiere reiniciar)
```

### Problema: "g++: no se reconoce el comando"
**Solución**:
```bash
# Verificar PATH de MinGW:
where g++

# Si no aparece:
$env:PATH += ";C:\mingw64\bin"
```

### Problema: "Error compiling lexer_gui.cpp"
**Solución**: Es probable que wxWidgets no esté instalado. Intenta compilar sin GUI:
```bash
# Usar versión simple en consola en lugar de GUI
```

### Problema: "cl.exe not found" (al compilar)
**Solución**: Asegúrate de no usar Visual Studio. Usa MinGW con comando `g++` en lugar de `cl`

## 📊 Verificar Compilación Exitosa

Después de ejecutar `make` sin errores, debería aparecer:
```
Build completado: analizador_lexico.exe
```

Y el archivo debe ser ejecutable:
```bash
.\analizador_lexico.exe

# Debería abrirse una ventana con la interfaz gráfica
```

## 📝 Archivos Generados

Después de compilar, se generan:
```
lex.yy.c              # Código C generado por FLEX
lex.yy.o              # Objeto compilado de FLEX
lexer_gui.o           # Objeto compilado de C++
analizador_lexico.exe # Ejecutable final
```

Para limpiar:
```bash
make clean
```

## 🎓 Próximos Pasos

1. Ejecutar el programa
2. Escribir código en el editor
3. Presionar "Analizar"
4. Ver los tokens generados

## 📞 Ayuda Adicional

- FLEX Manual: https://westes.github.io/flex/manual/
- MinGW: https://www.mingw-w64.org/
- wxWidgets: https://docs.wxwidgets.org/

---

**¡Listo!** Ya puedes compilar y ejecutar el Analizador Léxico con FLEX y C++
