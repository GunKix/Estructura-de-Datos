#include "ABB.h"

ABB::ABB() : raiz(nullptr) {}

ABB::~ABB() { limpiar(); }

NodoABB* ABB::insertarRec(NodoABB* nodo, const Estudiante& e, bool& insertado) {
    if (nodo == nullptr) { insertado = true; return new NodoABB(e); }
    if (e.carne == nodo->dato.carne) { insertado = false; return nodo; }
    if (e.carne < nodo->dato.carne) nodo->izq = insertarRec(nodo->izq, e, insertado);
    else nodo->der = insertarRec(nodo->der, e, insertado);
    return nodo;
}

bool ABB::insertar(const Estudiante& e) {
    bool ok = false;
    raiz = insertarRec(raiz, e, ok);
    return ok;
}

NodoABB* ABB::buscarRec(NodoABB* nodo, int carne) const {
    if (nodo == nullptr) return nullptr;
    if (carne == nodo->dato.carne) return nodo;
    if (carne < nodo->dato.carne) return buscarRec(nodo->izq, carne);
    return buscarRec(nodo->der, carne);
}

bool ABB::buscar(int carne, Estudiante& out) const {
    NodoABB* n = buscarRec(raiz, carne);
    if (n == nullptr) return false;
    out = n->dato;
    return true;
}

bool ABB::existe(int carne) const { return buscarRec(raiz, carne) != nullptr; }

NodoABB* ABB::minimoNodo(NodoABB* nodo) const {
    while (nodo && nodo->izq) nodo = nodo->izq;
    return nodo;
}

NodoABB* ABB::eliminarRec(NodoABB* nodo, int carne, bool& eliminado) {
    if (nodo == nullptr) { eliminado = false; return nullptr; }
    if (carne < nodo->dato.carne) {
        nodo->izq = eliminarRec(nodo->izq, carne, eliminado);
    } else if (carne > nodo->dato.carne) {
        nodo->der = eliminarRec(nodo->der, carne, eliminado);
    } else {
        // Encontrado: 3 casos clasicos.
        eliminado = true;
        if (nodo->izq == nullptr && nodo->der == nullptr) {  // Caso 1: hoja
            delete nodo;
            return nullptr;
        } else if (nodo->izq == nullptr) {                    // Caso 2a: solo hijo derecho
            NodoABB* tmp = nodo->der;
            delete nodo;
            return tmp;
        } else if (nodo->der == nullptr) {                    // Caso 2b: solo hijo izquierdo
            NodoABB* tmp = nodo->izq;
            delete nodo;
            return tmp;
        } else {                                              // Caso 3: dos hijos
            NodoABB* suc = minimoNodo(nodo->der);             // sucesor inorden
            nodo->dato = suc->dato;
            bool dummy = false;
            nodo->der = eliminarRec(nodo->der, suc->dato.carne, dummy);
        }
    }
    return nodo;
}

bool ABB::eliminar(int carne) {
    bool ok = false;
    raiz = eliminarRec(raiz, carne, ok);
    return ok;
}

bool ABB::modificar(const Estudiante& e) {
    NodoABB* n = buscarRec(raiz, e.carne);
    if (!n) return false;
    n->dato = e;
    return true;
}

void ABB::inordenRec(NodoABB* nodo, std::vector<Estudiante>& out) const {
    if (!nodo) return;
    inordenRec(nodo->izq, out);
    out.push_back(nodo->dato);
    inordenRec(nodo->der, out);
}
void ABB::preordenRec(NodoABB* nodo, std::vector<Estudiante>& out) const {
    if (!nodo) return;
    out.push_back(nodo->dato);
    preordenRec(nodo->izq, out);
    preordenRec(nodo->der, out);
}
void ABB::postordenRec(NodoABB* nodo, std::vector<Estudiante>& out) const {
    if (!nodo) return;
    postordenRec(nodo->izq, out);
    postordenRec(nodo->der, out);
    out.push_back(nodo->dato);
}

void ABB::inorden(std::vector<Estudiante>& out) const { inordenRec(raiz, out); }
void ABB::preorden(std::vector<Estudiante>& out) const { preordenRec(raiz, out); }
void ABB::postorden(std::vector<Estudiante>& out) const { postordenRec(raiz, out); }

bool ABB::minimo(Estudiante& out) const {
    if (!raiz) return false;
    out = minimoNodo(raiz)->dato;
    return true;
}
bool ABB::maximo(Estudiante& out) const {
    if (!raiz) return false;
    NodoABB* n = raiz;
    while (n->der) n = n->der;
    out = n->dato;
    return true;
}

int ABB::alturaRec(NodoABB* nodo) const {
    if (!nodo) return -1; // arbol vacio = -1, un nodo = 0
    int a = alturaRec(nodo->izq);
    int b = alturaRec(nodo->der);
    return 1 + (a > b ? a : b);
}
int ABB::altura() const { return alturaRec(raiz); }

int ABB::contarRec(NodoABB* nodo) const {
    if (!nodo) return 0;
    return 1 + contarRec(nodo->izq) + contarRec(nodo->der);
}
int ABB::contarNodos() const { return contarRec(raiz); }
bool ABB::vacio() const { return raiz == nullptr; }

void ABB::destruirRec(NodoABB* nodo) {
    if (!nodo) return;
    destruirRec(nodo->izq);
    destruirRec(nodo->der);
    delete nodo;
}
void ABB::limpiar() { destruirRec(raiz); raiz = nullptr; }
