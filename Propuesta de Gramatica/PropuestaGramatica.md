## Propuesta de Gramática para el Trabajo

El trabajo propuesto que se va a realizar será un compilador de Pseudocódigo a un Lenguaje de Alto Nivel que es C++, para realizarlo se requiere el uso de una gramática:



```plaintext
<PseudoCode> ::= "algoritmo" <Nombre_Algoritmo> "{" "principal" "{" <Cuerpo> "}" "}"
```

- **Descripción:** Define la estructura básica de un algoritmo en pseudocódigo. Comienza con la palabra clave **algoritmo**, seguida del nombre del algoritmo, luego la sección principal que contiene el cuerpo principal del algoritmo.

```plaintext
<nombre Algoritmo> ::= <identificador>
```

- **Descripción:** Define el nombre del algoritmo utilizando un identificador válido.

```plaintext
<Cuerpo> ::= (<declaracion> | <asignacion> | <lectura> | <impresion> | <decision> | <repeticion>)*
```
- **Descripción:** El cuerpo principal del algoritmo puede contener múltiples declaraciones, asignaciones, lecturas, impresiones, decisiones o repeticiones.

```plaintext
<declaracion> ::= <Tipo> <declara_variable> ("," <declara_variable>)* ";"
```

- **Descripción:** Define una o más variables de un tipo específico en una sola línea, separadas por comas, terminando con un punto y coma.

```plaintext
<declara_variable> ::= <identificador> ("=" <expresion>)?
```

- **Descripción:** Declara una variable que puede opcionalmente ser inicializada con una expresión.

```plaintext
<Tipo> ::= "entero" | "real" | "cadena" | "booleano" | "caracter"
```

- **Descripción:** Define el tipo de variable que será.

```plaintext
<asignacion> ::= <variable> <operador asignación> <expresión> ";"
<operador asignación> ::= "=" | "+=" | "-=" | "*=" | "/="
```

- **Descripción:** Define una asignación de valor a una variable con diversos operadores de asignación.

```plaintext
<lectura> ::= "leer" "(" <variable> ")" ";"
```

- **Descripción:** Representa la lectura de un valor desde la entrada estándar y su almacenamiento en una variable.

```plaintext
<impresion> ::= "imprimir" "(" <expresiónCadena> ")" ";"
```

- **Descripción:** Representa la impresión de un valor o una cadena a la salida estándar.

```plaintext
<decision> ::= <decision simple> | <decision múltiple>
<decision simple> ::= "si" "(" <expresiónBooleana> ")" "{" <cuerpo> "}" 
                    ("sino" "{" <cuerpo> "}")?
<decision múltiple> ::= "segun" "(" <variable> ")" "{" 
                        ("caso" <valor> ":" <cuerpo> "termina" ";")* 
                        "otroCaso" ":" <cuerpo> 
                        "}"
```

- **Descripción:** Define las estructuras de decisión: **decisión simple** es una condición **if-else**, y **decisión múltiple** en una estructura **switch-case**.

```plaintext
<repeticion> ::= <ciclo Para> | <ciclo Mientras> | <ciclo Haga Mientras>
<ciclo Para> ::= "para" "(" <inicialización Para> ";" <expresiónBooleana> ";" <asignacion Para> ")" "{" <cuerpo> "}"
<inicialización Para> ::= <tipo>? <variable> "=" <expresión>
<asignacion Para> ::= <variable> <operador autoincremento> | <variable> <operador asignación> <expresión>
<operador autoincremento> ::= "++" | "--"
<ciclo Mientras> ::= "mientras" "(" <expresiónBooleana> ")" "{" <cuerpo> "}"
<ciclo Haga Mientras> ::= "haga" "{" <cuerpo> "}" "mientras" "(" <expresiónBooleana> ")" ";"
```

- **Descripción:** Define las estructuras de repetición como: **para, mientras y haga mientras**.

```plaintext
<expresión> ::= <expresiónEntera> | <expresiónReal> | <expresiónCadena> | <expresiónBooleana>
<expresiónEntera> ::= <entero>
<expresiónReal> ::= <real>
<expresiónCadena> ::= <cadena>
<expresiónBooleana> ::= <booleana>
```

- **Descripción:** Define las expresiones que puedes ser enteras, reales, cadenas o booleanas.

```plaintext
<entero> ::= [0-9]+
<real> ::= [0-9]+"."[0-9]+
<cadena> ::= "\"" .*? "\""
<booleana> ::= "true" | "false"
```

- **Descripción:** Define los valores literales posibles de las expresiones.

```plaintext
<valor> ::= <entero> | <real> | <cadena> | <booleana>
```

- **Descripción:** Define los valores posibles que pueden ser utilizados en la estructura **case**.

```plaintext
<variable> ::= <identificador>
<identificador> ::= [a-zA-Z_][a-zA-Z0-9_]*
```

- **Descripcion:** Define las variables y los identificadores permitidos en el pseudocódigo.

## Gramática Completa

```plaintext

<PseudoCode> ::= <Comentario>? "algoritmo" <Nombre Algoritmo> "{" principal "{" <Cuerpo> "}" "}"

<Nombre Algoritmo> ::= <identificador>

<Cuerpo> ::= (<declaracion> | <asignacion> | <lectura> | <impresion> | <decision> | <repeticion>)*

<declaracion> ::= <Tipo> <declara variable> ("," <declara variable>)* ";"
<declara variable> ::= <variable> ("=" <expresión>)?
<Tipo> ::= "entero" | "real" | "cadena" | "booleano" | "caracter"

<asignacion> ::= <variable> <operador asignación> <expresión> ";"
<operador asignación> ::= "=" | "+=" | "-=" | "*=" | "/="

<lectura> ::= "leer" "(" <variable> ")" ";"
<impresion> ::= "imprimir" "(" <expresionCadena> ")" ";"

<decision> ::= <decision simple> | <decision multiple>
<decision simple> ::= "si" "(" <expresionBooleana> ")" "{" <Cuerpo> "}" ("sino" "{" <Cuerpo> "}")?
<decision multiple> ::= "segun" "(" <variable> ")" "{" ( "caso" <valor> ":" <Cuerpo> "termina" ";")* "otroCaso" ":" <Cuerpo> "}"

<repeticion> ::= <Ciclo Para> | <Ciclo Mientras> | <Ciclo Haga Mientras>
<Ciclo Para> ::= "para" "(" <Inicializacion Para> ";" <expresionBooleana> ";" <Asignacion Para> ")" "{" <Cuerpo> "}"
<Inicializacion Para> ::= <Tipo>? <variable> "=" <expresión>
<Asignacion Para> ::= <variable> <operador autoincremento> | <variable> <operador asignación> <expresión>
<operador autoincremento> ::= "++" | "--"

<Ciclo Mientras> ::= "mientras" "(" <expresionBooleana> ")" "{" <Cuerpo> "}"
<Ciclo Haga Mientras> ::= "haga" "{" <Cuerpo> "}" "mientras" "(" <expresionBooleana> ")" ";"

<expresión> ::= <expresionEntera> | <expresionReal> | <expresionCadena> | <expresionBooleana> | <expresionCaracter>
<expresionEntera> ::= <entero>
<expresionReal> ::= <real>
<expresionCadena> ::= <cadena>
<expresionBooleana> ::= <booleana>
<expresionCaracter> ::= <caracter>

<entero> ::= [0-9]+
<real> ::= [0-9]+"."[0-9]+
<cadena> ::= "\"" .* "\""
<booleana> ::= "true" | "false"
<caracter> ::= "'" . "'"
<valor> ::= <entero> | <real> | <cadena> | <booleana> | <caracter>

<variable> ::= <identificador>
<identificador> ::= [a-zA-Z_][a-zA-Z0-9_]*

```