#include "Estudiante.h"
#include <sstream>

Estudiante::Estudiante() : carne(0) {}
Estudiante::Estudiante(int c, const std::string& n, const std::string& co, const std::string& ca)
    : carne(c), nombre(n), correo(co), carrera(ca) {}

std::string Estudiante::toString() const {
    return "Carne: " + std::to_string(carne) + " | Nombre: " + nombre +
           " | Correo: " + correo + " | Carrera: " + carrera;
}

std::string Estudiante::aLineaArchivo() const {
    return std::to_string(carne) + "|" + nombre + "|" + correo + "|" + carrera;
}

bool Estudiante::desdeLineaArchivo(const std::string& linea, Estudiante& out) {
    size_t p1 = linea.find('|');
    if (p1 == std::string::npos) return false;
    size_t p2 = linea.find('|', p1 + 1);
    if (p2 == std::string::npos) return false;
    size_t p3 = linea.find('|', p2 + 1);
    if (p3 == std::string::npos) return false;
    try { out.carne = std::stoi(linea.substr(0, p1)); }
    catch (...) { return false; }
    out.nombre = linea.substr(p1 + 1, p2 - p1 - 1);
    out.correo = linea.substr(p2 + 1, p3 - p2 - 1);
    out.carrera = linea.substr(p3 + 1);
    return out.carne > 0 && !out.nombre.empty();
}
