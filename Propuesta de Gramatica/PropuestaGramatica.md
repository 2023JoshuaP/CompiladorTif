## Propuesta de Gramática para el Trabajo

El trabajo propuesto que se va a realizar será un compilador de Pseudocódigo a un Lenguaje de Alto Nivel que es C++, para realizarlo se requiere el uso de una gramática:

```plaintext
<PseudoCode> ::= <comentario>? "algoritmo" <nombre Algoritmo> "{" 
                "principal" "{" <cuerpo> "}" 
                "{" <declaraciones> "}"
            "}"

<nombre Algoritmo> ::= <identificador> // Nombre del algoritmo

// Declaraciones y definiciones de variables
<declaraciones> ::= <declaracion>*

<declaracion> ::= <tipo> <declara variable> ("," <declara variable>)* ";"

<declara variable> ::= <variable> ("=" <expresión>)?

<tipo> ::= "int" | "float" | "string" | "bool" | "char"

// Cuerpo del programa que incluye diferentes sentencias
<cuerpo> ::= (<asignacion> | <lectura> | <impresion> | <decision> | <repeticion>)*

// Asignaciones de valores a variables
<asignacion> ::= <variable> <operador asignación> <expresión> ";"
<operador asignación> ::= "=" | "+=" | "-=" | "*=" | "/="

// Lectura de valores desde la entrada estándar
<lectura> ::= "leer" "(" <variable> ")" ";"

// Impresión de valores a la salida estándar
<impresion> ::= "imprimir" "(" <expresiónCadena> ")" ";"

// Estructuras de decisión: condicionales y decisiones múltiples
<decision> ::= <decision simple> | <decision múltiple>

<decision simple> ::= "si" "(" <expresiónBooleana> ")" "{" <cuerpo> "}" 
                    ("sino" "{" <cuerpo> "}")?

<decision múltiple> ::= "segun" "(" <variable> ")" "{" 
                        ("caso" <valor> ":" <cuerpo> "termina" ";")* 
                        "otroCaso" ":" <cuerpo> 
                        "}"

// Estructuras de repetición: para, mientras y haga mientras
<repeticion> ::= <ciclo Para> | <ciclo Mientras> | <ciclo Haga Mientras>

// Ciclo para
<ciclo Para> ::= "para" "(" <inicialización Para> ";" <expresiónBooleana> ";" <asignacion Para> ")" "{" <cuerpo> "}"

<inicialización Para> ::= "int" <variable> "=" <expresiónEntera> | "float" <variable> "=" <expresiónReal>

<asignacion Para> ::= <variable> <operador autoincremento> | <variable> <operador asignación> <expresión>
<operador autoincremento> ::= "++" | "--"

// Ciclo mientras
<ciclo Mientras> ::= "mientras" "(" <expresiónBooleana> ")" "{" <cuerpo> "}"

// Ciclo haga mientras
<ciclo Haga Mientras> ::= "haga" "{" <cuerpo> "}" "mientras" "(" <expresiónBooleana> ")" ";"

// Definiciones de expresiones, valores y variables
<expresión> ::= <expresiónEntera> | <expresiónReal> | <expresiónCadena> | <expresiónBooleana>

<expresiónEntera> ::= <entero>
<expresiónReal> ::= <real>
<expresiónCadena> ::= <cadena>
<expresiónBooleana> ::= <booleana>

<entero> ::= [0-9]+
<real> ::= [0-9]+"."[0-9]+
<cadena> ::= "\"" .*? "\""
<booleana> ::= "true" | "false"

<valor> ::= <entero> | <real> | <cadena> | <booleana>

<variable> ::= <identificador>
<identificador> ::= [a-zA-Z_][a-zA-Z0-9_]*
