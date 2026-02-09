# Autómata Finito del Analizador Léxico

## 🎯 Descripción General

El analizador léxico implementa un **Autómata Finito Determinista (AFD)** que reconoce patrones en código fuente. FLEX genera automáticamente este autómata a partir de las expresiones regulares definidas en `lexer.l`.

## 📊 Diagrama de Estados

```
                    [a-zA-Z_]
                        ↓
    ┌───────→ S1 [IDENTIFIER/KEYWORD] ←─────────────┐
    │          ↓                                      │
    │      [a-zA-Z0-9_]                              │
    │          →→→→→→→ (ciclo)                       │
    │                                                 │
S0  │         [0-9]                                  │
│   │          ↓                                      │
└──→┼─────→ S2 [NUMBER] ←─────┐                      │
    │          ↓               │                      │
    │     [0-9.eE+-]           │                      │
    │       (ciclo)            │                      │
    │                          │                      │
    │         ["]              │                      │
    │          ↓               │                      │
    ├─────→ S3 [STRING]        │                      │
    │          ↓               │                      │
    │     [^"\\]|\\.      (ciclo)  │                  │
    │                          │                      │
    │    [+\-*/%=<>&|!]       │                      │
    │          ↓               │                      │
    ├─────→ S4 [OPERATOR]      │                      │
    │     (Operadores           │                      │
    │      simples y           │                      │
    │     compuestos)          │                      │
    │                          │                      │
    │    [{};(),:.] │                      │
    │          ↓               │                      │
    └─────→ S5 [PUNCTUATION]   │                      │
             (Caracteres        │                      │
              únicos)           │                      │
              
    │         [/]              │                      │
    │          ↓               │                      │
    └─────→ S6 [COMMENT]       │                      │
             ├─→ // [...] \n   │                      │
             └─→ /* [...] */   │                      │
```

## 🔤 Estados del Autómata

### Estado S0 (Inicial)
- Estado de inicio donde se analiza el primer carácter
- Transiciones basadas en el tipo de carácter

### Estado S1 (Identificador/Palabra Clave)
- **Patrón**: `[a-zA-Z_][a-zA-Z0-9_]*`
- **Reconoce**: 
  - Palabras reservadas: `if`, `else`, `while`, `for`, `int`, `float`, etc.
  - Identificadores: `x`, `miVariable`, `_private`, `CamelCase`, etc.

### Estado S2 (Número)
- **Patrón**: `[0-9]+(\.[0-9]+)?([eE][+-]?[0-9]+)?`
- **Reconoce**:
  - Enteros: `42`, `0`, `999`
  - Decimales: `3.14`, `0.5`, `2.0`
  - Notación científica: `1.5e-10`, `2E+5`, `3.14e-2`

### Estado S3 (Cadena de Texto)
- **Patrón**: `"([^"\\]|\\.)*"`
- **Reconoce**:
  - Cadenas simples: `"hello"`
  - Cadenas con caracteres especiales: `"hola\nmund\"`
  - Caracteres escapados: `\"`, `\\`, `\t`, etc.

### Estado S4 (Operador)
- **Patrón**: `(\+\+|--|\+=|-=|\*=|\/=|==|!=|<=|>=|&&|\|\||[+\-*/%=<>&|!])`
- **Reconoce**:
  - Operadores simples: `+`, `-`, `*`, `/`, `%`, `=`, `<`, `>`, `&`, `|`, `!`
  - Operadores compuestos: `++`, `--`, `+=`, `-=`, `*=`, `/=`, `==`, `!=`, `<=`, `>=`, `&&`, `||`

### Estado S5 (Puntuación)
- **Patrón**: `[(){}\[\],;.:]`
- **Reconoce**:
  - Paréntesis: `(`, `)`
  - Llaves: `{`, `}`
  - Corchetes: `[`, `]`
  - Separadores: `,`, `;`, `.`, `:`

### Estado S6 (Comentarios)
- **Patrones**:
  - Comentario de línea: `//.*` (hasta fin de línea)
  - Comentario de bloque: `/*[\s\S]*?*/` (hasta `*/`)
- **Acción**: Se ignoran completamente

### Estado S7 (Espacios en Blanco)
- **Patrón**: `[ \t]+` y `\n`
- **Acción**: Se ignoran pero se rastrea el número de línea

## 📉 Tabla de Transiciones

| Estado | Entrada | Siguiente Estado | Acción |
|--------|---------|------------------|--------|
| S0 | [a-zA-Z_] | S1 | Leer identificador/keyword |
| S0 | [0-9] | S2 | Leer número |
| S0 | " | S3 | Leer string |
| S0 | [+\-*/=%<>&\|!] | S4 | Leer operador |
| S0 | [{};(),:.] | S5 | Leer puntuación |
| S0 | / | S6 | Leer comentario |
| S0 | [ \t] | S7 | Ignorar espacios |
| S0 | \n | S7 | Ignorar (incrementar línea) |
| S1 | [a-zA-Z0-9_] | S1 | Continuar leyendo |
| S1 | Otro | S0 | Finalizar token |
| S2 | [0-9] | S2 | Continuar leyendo |
| S2 | . | S2 | Leer decimales |
| S2 | [eE] | S2 | Notación científica |
| S2 | Otro | S0 | Finalizar token |
| S3 | [^"\\] | S3 | Leer carácter |
| S3 | \ | S3 | Leer carácter escapado |
| S3 | " | S0 | Finalizar string |

## 🏗️ Expresiones Regulares Utilizadas

```
KEYWORD:      if|else|while|for|return|int|float|string|bool|true|false|void|...
NUMBER:       [0-9]+(\.[0-9]+)?([eE][+-]?[0-9]+)?
IDENTIFIER:   [a-zA-Z_][a-zA-Z0-9_]*
STRING:       \"([^"\\]|\\.)*\"
OPERATOR:     (\+\+|--|\+=|-=|\*=|\/=|==|!=|<=|>=|&&|\|\||[+\-*/%=<>&|!])
PUNCTUATION:  [(){}\[\],;.:]
COMMENT_LINE: \/\/.*
COMMENT_BLOCK: \/\*[\s\S]*?\*\/
```

## 🔄 Flujo de Ejecución

```
Entrada: Código Fuente
    ↓
┌─────────────────────────────────────┐
│ Leer Carácter (S0)                  │
└─────────────────────────────────────┘
    ↓
┌─────────────────────────────────────┐
│ ¿Qué tipo de carácter es?           │
└─────────────────────────────────────┘
    ↓
    ├→ [a-zA-Z_] → S1 (Identificador)
    ├→ [0-9] → S2 (Número)
    ├→ " → S3 (String)
    ├→ Operador → S4
    ├→ Puntuación → S5
    ├→ / → S6 (Comentario)
    ├→ Espacio → Ignorar
    └→ Otro → Error
    ↓
┌─────────────────────────────────────┐
│ Procesar en Estado Específico        │
│ (leer más caracteres si es necesario)│
└─────────────────────────────────────┘
    ↓
┌─────────────────────────────────────┐
│ ¿Carácter siguiente pertenece?      │
│ al mismo token?                      │
└─────────────────────────────────────┘
    ↓
    Sí → Continuar en estado actual
    No → Crear Token + Volver a S0
    ↓
¿Fin de entrada?
    Sí → Finalizar
    No → Volver a leer siguiente carácter
```

## 📋 Ejemplo de Tokenización

### Entrada:
```cpp
int x = 10;
```

### Proceso:

1. **'i'** → Entra a S1 (IDENTIFIER)
   - Lee 'i', 'n', 't'
   - Siguiente es ' ' (espacio) → Finaliza
   - Token: `<KEYWORD, "int", 1, 1>`

2. **' '** → Ignorado, vuelve a S0

3. **'x'** → Entra a S1 (IDENTIFIER)
   - Lee 'x'
   - Siguiente es ' ' → Finaliza
   - Token: `<IDENTIFIER, "x", 1, 5>`

4. **' '** → Ignorado

5. **'='** → Entra a S4 (OPERATOR)
   - Lee '='
   - Siguiente es ' ' → Finaliza
   - Token: `<OPERATOR, "=", 1, 7>`

6. **' '** → Ignorado

7. **'1', '0'** → Entra a S2 (NUMBER)
   - Lee '1', '0'
   - Siguiente es ';' → Finaliza
   - Token: `<NUMBER, "10", 1, 9>`

8. **';'** → Entra a S5 (PUNCTUATION)
   - Token: `<PUNCTUATION, ";", 1, 11>`

### Salida:
```
Token 1: <KEYWORD,     "int", 1,  1>
Token 2: <IDENTIFIER,  "x",   1,  5>
Token 3: <OPERATOR,    "=",   1,  7>
Token 4: <NUMBER,      "10",  1,  9>
Token 5: <PUNCTUATION, ";",   1, 11>
```

## 🎓 Conceptos Clave

### Determinismo
Una vez en un estado, la entrada determina unívocamente el siguiente estado (no hay ambigüedad).

### Minimalidad
El autómata tiene el mínimo número de estados necesarios para reconocer los patrones.

### Completitud
La FLEX genera optimizaciones para hacer el autómata eficiente en tiempo de ejecución.

### Lookahead
En algunos casos, FLEX necesita ver un carácter adelante para decidir si un token termina.

## 🚀 Optimizaciones de FLEX

1. **Compresión de Tabla de Estados**: Reduce el tamaño del autómata
2. **Búsqueda Rápida**: Utiliza tablas hash para transiciones
3. **Buffering Eficiente**: Lee el archivo en bloques
4. **Expresiones Regulares Compiladas**: Pre-compila patrones

---

**Conclusión**: Este Autómata Finito Determinista implementado por FLEX es capaz de reconocer todos los tokens válidos del lenguaje definido de manera rápida y confiable.
