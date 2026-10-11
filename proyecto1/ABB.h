#ifndef ABB_H
#define ABB_H

#include <vector>
#include <string>
#include "Estudiante.h"

// Nodo del Arbol de Busqueda Binaria (manejo dinamico de memoria con new/delete).
struct NodoABB {
    Estudiante dato;
    NodoABB* izq;
    NodoABB* der;
    explicit NodoABB(const Estudiante& e) : dato(e), izq(nullptr), der(nullptr) {}
};

// ABB indexado por carne del estudiante.
// Respeta la propiedad de orden en todas las operaciones.
class ABB {
private:
    NodoABB* raiz;

    NodoABB* insertarRec(NodoABB* nodo, const Estudiante& e, bool& insertado);
    NodoABB* buscarRec(NodoABB* nodo, int carne) const;
    NodoABB* eliminarRec(NodoABB* nodo, int carne, bool& eliminado);
    NodoABB* minimoNodo(NodoABB* nodo) const;
    void inordenRec(NodoABB* nodo, std::vector<Estudiante>& out) const;
    void preordenRec(NodoABB* nodo, std::vector<Estudiante>& out) const;
    void postordenRec(NodoABB* nodo, std::vector<Estudiante>& out) const;
    int alturaRec(NodoABB* nodo) const;
    int contarRec(NodoABB* nodo) const;
    void destruirRec(NodoABB* nodo);

public:
    ABB();
    ~ABB();

    bool insertar(const Estudiante& e);      // false si carne duplicado
    bool buscar(int carne, Estudiante& out) const;
    bool existe(int carne) const;
    bool eliminar(int carne);                // maneja 3 casos: hoja, un hijo, dos hijos
    bool modificar(const Estudiante& e);     // actualiza datos (misma clave)

    void inorden(std::vector<Estudiante>& out) const;
    void preorden(std::vector<Estudiante>& out) const;
    void postorden(std::vector<Estudiante>& out) const;

    bool minimo(Estudiante& out) const;       // menor carne
    bool maximo(Estudiante& out) const;       // mayor carne
    int altura() const;
    int contarNodos() const;
    bool vacio() const;
    void limpiar();
};

#endif
