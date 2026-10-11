#include "HistorialNavegacion.h"

void HistorialNavegacion::registrarNavegacion(int carne, const std::string& contenido,
                                             const std::string& accion,
                                             const std::string& fechaHora) {
    Visita v{contenido, accion, fechaHora};
    historiales[carne].push(v);
    if (!accion.empty()) acciones[carne].push(accion + " @ " + contenido);
}

void HistorialNavegacion::registrarAccion(int carne, const std::string& accion) {
    acciones[carne].push(accion);
}

bool HistorialNavegacion::tieneHistorial(int carne) const {
    auto it = historiales.find(carne);
    return it != historiales.end() && !it->second.empty();
}

bool HistorialNavegacion::mostrarHistorial(int carne, std::vector<Visita>& out) const {
    out.clear();
    auto it = historiales.find(carne);
    if (it == historiales.end() || it->second.empty()) return false;
    std::stack<Visita> copia = it->second; // copiar para no destruir
    while (!copia.empty()) { out.push_back(copia.top()); copia.pop(); }
    return true;
}

bool HistorialNavegacion::retroceder(int carne, Visita& saliente) {
    auto it = historiales.find(carne);
    if (it == historiales.end() || it->second.empty()) return false;
    saliente = it->second.top();
    it->second.pop();
    return true;
}

void HistorialNavegacion::limpiarHistorial(int carne) {
    historiales.erase(carne);
    acciones.erase(carne);
}

void HistorialNavegacion::limpiarTodo() {
    historiales.clear();
    acciones.clear();
}

bool HistorialNavegacion::deshacerAccion(int carne, std::string& accionDeshecha) {
    auto it = acciones.find(carne);
    if (it == acciones.end() || it->second.empty()) return false;
    accionDeshecha = it->second.top();
    it->second.pop();
    // Tambien se quita la visita asociada si existe (deshacer visita por error).
    auto h = historiales.find(carne);
    if (h != historiales.end() && !h->second.empty()) h->second.pop();
    return true;
}

int HistorialNavegacion::tamanoHistorial(int carne) const {
    auto it = historiales.find(carne);
    return (it == historiales.end()) ? 0 : (int)it->second.size();
}

int HistorialNavegacion::tamanoAcciones(int carne) const {
    auto it = acciones.find(carne);
    return (it == acciones.end()) ? 0 : (int)it->second.size();
}

std::vector<int> HistorialNavegacion::carnesConHistorial() const {
    std::vector<int> out;
    for (auto& kv : historiales) if (!kv.second.empty()) out.push_back(kv.first);
    return out;
}

std::vector<std::string> HistorialNavegacion::aLineasArchivo() const {
    std::vector<std::string> lineas;
    for (auto& kv : historiales) {
        std::stack<Visita> copia = kv.second;
        std::vector<Visita> tmp;
        while (!copia.empty()) { tmp.push_back(copia.top()); copia.pop(); }
        // tmp esta tope->fondo; guardar fondo->tope para reconstruir igual
        for (int i = (int)tmp.size() - 1; i >= 0; i--) {
            lineas.push_back(std::to_string(kv.first) + "|" + tmp[i].contenido +
                             "|" + tmp[i].accion + "|" + tmp[i].fechaHora);
        }
    }
    return lineas;
}

void HistorialNavegacion::cargarDesdeLineas(const std::vector<std::string>& lineas) {
    limpiarTodo();
    for (auto& ln : lineas) {
        size_t p1 = ln.find('|');
        if (p1 == std::string::npos) continue;
        size_t p2 = ln.find('|', p1 + 1);
        if (p2 == std::string::npos) continue;
        size_t p3 = ln.find('|', p2 + 1);
        if (p3 == std::string::npos) continue;
        int carne = 0;
        try { carne = std::stoi(ln.substr(0, p1)); } catch (...) { continue; }
        Visita v{ln.substr(p1 + 1, p2 - p1 - 1), ln.substr(p2 + 1, p3 - p2 - 1), ln.substr(p3 + 1)};
        historiales[carne].push(v);
    }
}
