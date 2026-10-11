#include "Utilidades.h"
#include <iostream>
#include <limits>
#include <ctime>
#include <cctype>

namespace Utilidades {

static std::string leerLinea(const std::string& mensaje) {
    std::cout << mensaje;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

int leerEntero(const std::string& mensaje) {
    while (true) {
        std::string s = leerLinea(mensaje);
        s = trim(s);
        try {
            size_t pos = 0;
            int v = std::stoi(s, &pos);
            if (pos == s.size()) return v;
        } catch (...) {}
        std::cout << "  Entrada invalida. Ingrese un numero entero.\n";
    }
}

int leerEnteroRango(const std::string& mensaje, int min, int max) {
    while (true) {
        int v = leerEntero(mensaje);
        if (v >= min && v <= max) return v;
        std::cout << "  Debe estar entre " << min << " y " << max << ".\n";
    }
}

int leerCarne(const std::string& mensaje) {
    while (true) {
        int v = leerEntero(mensaje);
        if (v > 0) return v;
        std::cout << "  El carne debe ser un numero positivo.\n";
    }
}

std::string leerLineaNoVacia(const std::string& mensaje) {
    while (true) {
        std::string s = trim(leerLinea(mensaje));
        if (!s.empty()) return s;
        std::cout << "  El valor no puede estar vacio.\n";
    }
}

std::string leerCorreo(const std::string& mensaje) {
    while (true) {
        std::string s = trim(leerLinea(mensaje));
        size_t at = s.find('@');
        size_t dot = s.find('.', at == std::string::npos ? 0 : at);
        if (at != std::string::npos && dot != std::string::npos && at > 0 && dot > at + 1) return s;
        std::cout << "  Correo invalido. Ejemplo: nombre@institucion.ac.cr\n";
    }
}

std::string leerTipoNodo() {
    std::cout << "Tipos: 1=Unidad  2=Tema  3=Recurso  4=Actividad\n";
    int op = leerEnteroRango("Tipo (1-4): ", 1, 4);
    if (op == 1) return "Unidad";
    if (op == 2) return "Tema";
    if (op == 3) return "Recurso";
    return "Actividad";
}

std::string leerTipoSolicitud() {
    std::cout << "Tipos de solicitud:\n"
              << "  1. revision de tarea (ALTA)\n"
              << "  2. reporte de error en material (ALTA)\n"
              << "  3. habilitacion de recurso (NORMAL)\n"
              << "  4. consulta academica (NORMAL)\n";
    int op = leerEnteroRango("Tipo (1-4): ", 1, 4);
    if (op == 1) return "revision de tarea";
    if (op == 2) return "reporte de error en material";
    if (op == 3) return "habilitacion de recurso";
    return "consulta academica";
}

void pausar() {
    std::cout << "\n[Enter para continuar...]";
    std::string s;
    std::getline(std::cin, s);
}

void limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

std::string fechaActual() {
    std::time_t t = std::time(nullptr);
    char buf[32];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
    return std::string(buf);
}

std::string aMinusculas(std::string s) {
    for (auto& c : s) c = (char)tolower((unsigned char)c);
    return s;
}

std::string trim(const std::string& s) {
    size_t a = 0;
    while (a < s.size() && isspace((unsigned char)s[a])) a++;
    size_t b = s.size();
    while (b > a && isspace((unsigned char)s[b - 1])) b--;
    return s.substr(a, b - a);
}

std::vector<std::string> dividir(const std::string& s, char sep) {
    std::vector<std::string> out;
    std::string cur;
    for (char c : s) {
        if (c == sep) { out.push_back(cur); cur.clear(); }
        else cur += c;
    }
    out.push_back(cur);
    return out;
}

bool esSoloDigitos(const std::string& s) {
    if (s.empty()) return false;
    for (char c : s) if (!isdigit((unsigned char)c)) return false;
    return true;
}

}
