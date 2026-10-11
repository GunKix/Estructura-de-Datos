#include "ArbolGenerico.h"
#include <iostream>

ArbolGenerico::ArbolGenerico() {}
ArbolGenerico::~ArbolGenerico() { limpiar(); }

NodoGenerico* ArbolGenerico::buscarRec(NodoGenerico* nodo, const std::string& nombre) const {
    if (!nodo) return nullptr;
    if (nodo->nombre == nombre) return nodo;
    for (auto h : nodo->hijos) {
        NodoGenerico* r = buscarRec(h, nombre);
        if (r) return r;
    }
    return nullptr;
}

NodoGenerico* ArbolGenerico::buscar(const std::string& nombre) const {
    for (auto r : raices) {
        NodoGenerico* n = buscarRec(r, nombre);
        if (n) return n;
    }
    return nullptr;
}

bool ArbolGenerico::existe(const std::string& nombre) const {
    return buscar(nombre) != nullptr;
}

bool ArbolGenerico::crearCurso(const std::string& nombre, const std::string& descripcion) {
    if (nombre.empty() || existe(nombre)) return false;
    NodoGenerico* n = new NodoGenerico(nombre, "Curso", descripcion);
    raices.push_back(n);
    return true;
}

bool ArbolGenerico::agregarHijo(const std::string& nombrePadre, const std::string& nombreHijo,
                                const std::string& tipoHijo, const std::string& descripcion) {
    if (nombreHijo.empty() || existe(nombreHijo)) return false;
    NodoGenerico* padre = buscar(nombrePadre);
    if (!padre) return false;
    NodoGenerico* hijo = new NodoGenerico(nombreHijo, tipoHijo, descripcion);
    hijo->padre = padre;
    padre->hijos.push_back(hijo);
    return true;
}

bool ArbolGenerico::eliminarHoja(const std::string& nombre) {
    NodoGenerico* n = buscar(nombre);
    if (!n) return false;
    if (!n->hijos.empty()) return false; // no es hoja: no se puede eliminar
    if (n->padre == nullptr) {
        // es raiz (curso sin hijos): quitar de raices
        for (size_t i = 0; i < raices.size(); i++) {
            if (raices[i] == n) { raices.erase(raices.begin() + i); break; }
        }
        delete n;
        return true;
    }
    // quitar de la lista de hijos del padre
    auto& h = n->padre->hijos;
    for (size_t i = 0; i < h.size(); i++) {
        if (h[i] == n) { h.erase(h.begin() + i); break; }
    }
    delete n;
    return true;
}

void ArbolGenerico::mostrarRec(NodoGenerico* nodo, int indent, std::ostream& os) const {
    for (int i = 0; i < indent; i++) os << "  ";
    os << "- [" << nodo->tipo << "] " << nodo->nombre;
    if (!nodo->descripcion.empty()) os << " (" << nodo->descripcion << ")";
    os << "\n";
    for (auto h : nodo->hijos) mostrarRec(h, indent + 1, os);
}

void ArbolGenerico::mostrar(std::ostream& os) const {
    if (raices.empty()) { os << "(sin cursos registrados)\n"; return; }
    for (auto r : raices) mostrarRec(r, 0, os);
}

int ArbolGenerico::nivelRec(NodoGenerico* nodo, const std::string& nombre, int nivelActual) const {
    if (!nodo) return -1;
    if (nodo->nombre == nombre) return nivelActual;
    for (auto h : nodo->hijos) {
        int r = nivelRec(h, nombre, nivelActual + 1);
        if (r != -1) return r;
    }
    return -1;
}

int ArbolGenerico::nivelDe(const std::string& nombre) const {
    for (auto r : raices) {
        int n = nivelRec(r, nombre, 0);
        if (n != -1) return n;
    }
    return -1;
}

int ArbolGenerico::contarDescendientesRec(NodoGenerico* nodo) const {
    int c = 0;
    for (auto h : nodo->hijos) c += 1 + contarDescendientesRec(h);
    return c;
}

int ArbolGenerico::contarDescendientes(const std::string& nombre) const {
    NodoGenerico* n = buscar(nombre);
    if (!n) return -1;
    return contarDescendientesRec(n);
}

int ArbolGenerico::alturaRec(NodoGenerico* nodo) const {
    if (nodo->hijos.empty()) return 0;
    int m = 0;
    for (auto h : nodo->hijos) {
        int a = alturaRec(h);
        if (a > m) m = a;
    }
    return 1 + m;
}

int ArbolGenerico::altura() const {
    if (raices.empty()) return -1;
    int m = 0;
    for (auto r : raices) {
        int a = alturaRec(r);
        if (a > m) m = a;
    }
    return m;
}

int ArbolGenerico::contarRec(NodoGenerico* nodo) const {
    int c = 1;
    for (auto h : nodo->hijos) c += contarRec(h);
    return c;
}

int ArbolGenerico::contarNodos() const {
    int c = 0;
    for (auto r : raices) c += contarRec(r);
    return c;
}

void ArbolGenerico::hojasRec(NodoGenerico* nodo, std::vector<NodoGenerico*>& out) const {
    if (nodo->hijos.empty()) { out.push_back(nodo); return; }
    for (auto h : nodo->hijos) hojasRec(h, out);
}

void ArbolGenerico::obtenerHojas(std::vector<NodoGenerico*>& out) const {
    out.clear();
    for (auto r : raices) hojasRec(r, out);
}

bool ArbolGenerico::rutaRec(NodoGenerico* nodo, const std::string& nombre,
                            std::vector<NodoGenerico*>& ruta) const {
    ruta.push_back(nodo);
    if (nodo->nombre == nombre) return true;
    for (auto h : nodo->hijos) {
        if (rutaRec(h, nombre, ruta)) return true;
    }
    ruta.pop_back();
    return false;
}

bool ArbolGenerico::rutaHasta(const std::string& nombre, std::vector<std::string>& rutaNombres) const {
    rutaNombres.clear();
    for (auto r : raices) {
        std::vector<NodoGenerico*> ruta;
        if (rutaRec(r, nombre, ruta)) {
            for (auto n : ruta) rutaNombres.push_back(n->nombre);
            return true;
        }
    }
    return false;
}

bool ArbolGenerico::vacio() const { return raices.empty(); }

void ArbolGenerico::destruirRec(NodoGenerico* nodo) {
    for (auto h : nodo->hijos) destruirRec(h);
    delete nodo;
}

void ArbolGenerico::limpiar() {
    for (auto r : raices) destruirRec(r);
    raices.clear();
}

void ArbolGenerico::preordenPlanoRec(NodoGenerico* nodo, int nivel,
                                     std::vector<std::string>& out) const {
    out.push_back(std::to_string(nivel) + "|" + nodo->tipo + "|" + nodo->nombre + "|" + nodo->descripcion);
    for (auto h : nodo->hijos) preordenPlanoRec(h, nivel + 1, out);
}

void ArbolGenerico::aPreordenPlano(std::vector<std::string>& out) const {
    out.clear();
    for (auto r : raices) preordenPlanoRec(r, 0, out);
}

bool ArbolGenerico::reconstruirDesdePlano(const std::vector<std::string>& lineas) {
    limpiar();
    // Pila de ultimos nodos por nivel.
    std::vector<NodoGenerico*> pila;
    for (auto& ln : lineas) {
        // formato: nivel|tipo|nombre|descripcion
        size_t p1 = ln.find('|');
        if (p1 == std::string::npos) continue;
        size_t p2 = ln.find('|', p1 + 1);
        if (p2 == std::string::npos) continue;
        size_t p3 = ln.find('|', p2 + 1);
        int nivel = 0;
        try { nivel = std::stoi(ln.substr(0, p1)); } catch (...) { continue; }
        std::string tipo = ln.substr(p1 + 1, p2 - p1 - 1);
        std::string nombre, desc;
        if (p3 == std::string::npos) { nombre = ln.substr(p2 + 1); desc = ""; }
        else { nombre = ln.substr(p2 + 1, p3 - p2 - 1); desc = ln.substr(p3 + 1); }
        if (nombre.empty()) continue;
        NodoGenerico* n = new NodoGenerico(nombre, tipo, desc);
        if (nivel == 0) {
            raices.push_back(n);
            pila.clear();
            pila.push_back(n);
        } else {
            if ((int)pila.size() < nivel) { delete n; continue; } // nivel invalido
            while ((int)pila.size() > nivel) pila.pop_back();
            NodoGenerico* padre = pila.back();
            n->padre = padre;
            padre->hijos.push_back(n);
            pila.push_back(n);
        }
    }
    return true;
}
