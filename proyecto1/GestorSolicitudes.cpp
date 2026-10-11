#include "GestorSolicitudes.h"

GestorSolicitudes::GestorSolicitudes() : siguienteId(1) {}

int GestorSolicitudes::prioridadDeTipo(const std::string& tipo) {
    std::string t = tipo;
    for (auto& c : t) c = (char)tolower(c);
    if (t.find("error") != std::string::npos) return 1;
    if (t.find("revision") != std::string::npos || t.find("revisi") != std::string::npos) return 1;
    return 2;
}

std::string GestorSolicitudes::textoPrioridad(int p) {
    return (p == 1) ? "ALTA" : "NORMAL";
}

Solicitud GestorSolicitudes::insertar(int carne, const std::string& tipo,
                                      const std::string& descripcion,
                                      const std::string& fecha) {
    Solicitud s;
    s.id = siguienteId++;
    s.carne = carne;
    s.tipo = tipo;
    s.descripcion = descripcion;
    s.prioridad = prioridadDeTipo(tipo);
    s.fecha = fecha;
    if (s.prioridad == 1) colaAlta.push(s);
    else colaNormal.push(s);
    return s;
}

bool GestorSolicitudes::atenderSiguiente(Solicitud& atendida) {
    if (!colaAlta.empty()) { atendida = colaAlta.front(); colaAlta.pop(); return true; }
    if (!colaNormal.empty()) { atendida = colaNormal.front(); colaNormal.pop(); return true; }
    return false;
}

bool GestorSolicitudes::verFrente(Solicitud& out) const {
    if (!colaAlta.empty()) { out = colaAlta.front(); return true; }
    if (!colaNormal.empty()) { out = colaNormal.front(); return true; }
    return false;
}

void GestorSolicitudes::mostrarTodas(std::vector<Solicitud>& out) const {
    out.clear();
    std::queue<Solicitud> a = colaAlta, b = colaNormal;
    while (!a.empty()) { out.push_back(a.front()); a.pop(); }
    while (!b.empty()) { out.push_back(b.front()); b.pop(); }
}

int GestorSolicitudes::contar() const { return (int)(colaAlta.size() + colaNormal.size()); }
int GestorSolicitudes::contarAlta() const { return (int)colaAlta.size(); }
int GestorSolicitudes::contarNormal() const { return (int)colaNormal.size(); }
bool GestorSolicitudes::vacia() const { return colaAlta.empty() && colaNormal.empty(); }

void GestorSolicitudes::limpiar() {
    while (!colaAlta.empty()) colaAlta.pop();
    while (!colaNormal.empty()) colaNormal.pop();
}

std::vector<std::string> GestorSolicitudes::aLineasArchivo() const {
    std::vector<std::string> out;
    std::vector<Solicitud> todas;
    mostrarTodas(todas);
    for (auto& s : todas) {
        out.push_back(std::to_string(s.id) + "|" + std::to_string(s.carne) + "|" +
                      s.tipo + "|" + s.descripcion + "|" + std::to_string(s.prioridad) + "|" + s.fecha);
    }
    return out;
}

void GestorSolicitudes::cargarDesdeLineas(const std::vector<std::string>& lineas) {
    limpiar();
    int maxId = 0;
    for (auto& ln : lineas) {
        size_t p[5];
        p[0] = ln.find('|');
        if (p[0] == std::string::npos) continue;
        p[1] = ln.find('|', p[0] + 1); if (p[1] == std::string::npos) continue;
        p[2] = ln.find('|', p[1] + 1); if (p[2] == std::string::npos) continue;
        p[3] = ln.find('|', p[2] + 1); if (p[3] == std::string::npos) continue;
        p[4] = ln.find('|', p[3] + 1); if (p[4] == std::string::npos) continue;
        Solicitud s;
        try {
            s.id = std::stoi(ln.substr(0, p[0]));
            s.carne = std::stoi(ln.substr(p[0] + 1, p[1] - p[0] - 1));
        } catch (...) { continue; }
        s.tipo = ln.substr(p[1] + 1, p[2] - p[1] - 1);
        s.descripcion = ln.substr(p[2] + 1, p[3] - p[2] - 1);
        try { s.prioridad = std::stoi(ln.substr(p[3] + 1, p[4] - p[3] - 1)); }
        catch (...) { s.prioridad = 2; }
        s.fecha = ln.substr(p[4] + 1);
        if (s.id > maxId) maxId = s.id;
        if (s.prioridad == 1) colaAlta.push(s);
        else colaNormal.push(s);
    }
    siguienteId = maxId + 1;
    if (siguienteId < 1) siguienteId = 1;
}
