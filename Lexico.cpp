#include "Lexico.h"

using namespace std;

Lexico::Lexico(int argc, char* argv[]) {
    file = stdin;  // Entrada estándar del teclado
    if (argc == 2) {  // Si se especificó un archivo de lectura
        file = fopen(argv[1], "rt");  // Lectura en modo texto
        if (file == nullptr)
            file = stdin;
    }
    if (file == stdin)
        cout << "Ingrese texto ........ termine con Ctrl+Z" << endl;

    // Inicializar el mapa de palabras clave de pseudocódigo a C++
    pseudocodeToCpp["Proceso"] = "int main() {";
    pseudocodeToCpp["FinProceso"] = "return 0; }";
    pseudocodeToCpp["Escribir"] = "cout <<";
    pseudocodeToCpp["Leer"] = "cin >>";
}

Lexico::~Lexico() {
    if (file != stdin)  // Si la entrada fue de un archivo
        fclose(file);  // Entonces cerrar el archivo
}

void Lexico::run() {
    int token;
    while (true) {
        token = scan();
        if (token == EOF) break;
        show(token);
    }
    cout << endl;
}

int Lexico::scan() {
    int c;
    int i;
    do {
        c = fgetc(file);
    } while (isspace(c) && c != '\n');  // Ignorar espacios en blanco excepto nuevas líneas

    if (c == EOF) return EOF;
    if (c == '\n') return c;

    if (isalpha(c)) {  // Regla para ID y palabras clave
        i = 0;
        do {
            lexeme[i++] = c;
            c = fgetc(file);
        } while (isalnum(c) || c == '_');

        lexeme[i] = 0;
        ungetc(c, file);  // Devolver c al flujo de entrada
        i = isReserved();  // Comprobar si es una palabra reservada
        if (i >= 0)
            return i;
        return ID;  // Es un ID
    }
    if (isdigit(c)) {  // Regla para NUM
        i = 0;
        do {
            lexeme[i++] = c;
            c = fgetc(file);
        } while (isdigit(c));

        lexeme[i] = 0;
        ungetc(c, file);
        return NUM;
    }
    if (c == '<') {
        int next = fgetc(file);
        if (next == '-') {
            lexeme[0] = '<';
            lexeme[1] = '-';
            lexeme[2] = 0;
            return ASIGNACION;
        }
        ungetc(next, file);
    }
    // Reglas para tokens de un solo carácter
    if (c == ';') return PUNTOYCOMA;
    if (c == ',') return COMA;
    if (c == '(') return PARI;
    if (c == ')') return PARD;
    if (c == '[') return CORI;
    if (c == ']') return CORD;
    if (c == '{') return LLAVEI;
    if (c == '}') return LLAVED;

    return c;  // Para otros caracteres no definidos, devolver el carácter en sí
}

int Lexico::isReserved() {
    string lexemeStr = string(lexeme);
    auto it = pseudocodeToCpp.find(lexemeStr);
    if (it != pseudocodeToCpp.end()) {
        strcpy(lexeme, it->second.c_str());
        return 0;
    }
    return -1;
}

void Lexico::show(int token) {
    switch (token) {
        case ID:
            cout << lexeme << " ";
            break;
        case NUM:
            cout << lexeme << " ";
            break;
        case ASIGNACION:
            cout << "= ";
            break;
        case '\n':
            cout << endl;
            break;
        case PUNTOYCOMA:
        case COMA:
        case PARI:
        case PARD:
        case CORI:
        case CORD:
        case LLAVEI:
        case LLAVED:
            cout << static_cast<char>(token) << " ";
            break;
        default:
            cout << lexeme << " ";
            break;
    }
}
