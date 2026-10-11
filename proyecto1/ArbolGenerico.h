#ifndef ARBOL_GENERICO_H
#define ARBOL_GENERICO_H

#include <string>
#include <vector>

// Nodo del arbol generico (puede tener MAS de dos hijos).
// Manejo dinamico de memoria: cada nodo se crea con new y se libera con delete.
struct NodoGenerico {
    std::string nombre;                    // nombre unico del nodo (ej: "Unidad 1: Fundamentos")
    std::string tipo;                      // Curso | Unidad | Tema | Recurso | Actividad
    std::string descripcion;               // detalle opcional
    NodoGenerico* padre;                   // nullptr si es raiz (curso)
    std::vector<NodoGenerico*> hijos;

    NodoGenerico(const std::string& n, const std::string& t, const std::string& d = "")
        : nombre(n), tipo(t), descripcion(d), padre(nullptr) {}
};

// Bosque de cursos: cada raiz es un Curso.
// Operaciones minimas exigidas por el enunciado.
class ArbolGenerico {
private:
    std::vector<NodoGenerico*> raices;      // cada raiz = un curso

    NodoGenerico* buscarRec(NodoGenerico* nodo, const std::string& nombre) const;
    void mostrarRec(NodoGenerico* nodo, int indent, std::ostream& os) const;
    int alturaRec(NodoGenerico* nodo) const;
    int contarRec(NodoGenerico* nodo) const;          // incluye al propio nodo
    int contarDescendientesRec(NodoGenerico* nodo) const; // excluye al propio nodo
    void hojasRec(NodoGenerico* nodo, std::vector<NodoGenerico*>& out) const;
    bool rutaRec(NodoGenerico* nodo, const std::string& nombre,
                 std::vector<NodoGenerico*>& ruta) const;
    int nivelRec(NodoGenerico* nodo, const std::string& nombre, int nivelActual) const;
    void destruirRec(NodoGenerico* nodo);
    void preordenPlanoRec(NodoGenerico* nodo, int nivel,
                          std::vector<std::string>& out) const;

public:
    ArbolGenerico();
    ~ArbolGenerico();

    // --- Creacion / modificacion ---
    bool crearCurso(const std::string& nombre, const std::string& descripcion = "");
    bool agregarHijo(const std::string& nombrePadre, const std::string& nombreHijo,
                     const std::string& tipoHijo, const std::string& descripcion = "");
    // Solo se pueden eliminar nodos hoja (regla del enunciado).
    bool eliminarHoja(const std::string& nombre);

    // --- Consultas ---
    NodoGenerico* buscar(const std::string& nombre) const;
    bool existe(const std::string& nombre) const;
    int nivelDe(const std::string& nombre) const;          // -1 si no existe (raiz = nivel 0)
    int contarDescendientes(const std::string& nombre) const; // -1 si no existe
    int altura() const;                                    // altura del bosque (max de raices)
    int contarNodos() const;
    void obtenerHojas(std::vector<NodoGenerico*>& out) const;
    // Ruta desde la raiz hasta el nodo (lista de nombres de raiz->nodo). false si no existe.
    bool rutaHasta(const std::string& nombre, std::vector<std::string>& rutaNombres) const;

    void mostrar(std::ostream& os) const;
    bool vacio() const;
    void limpiar();

    // --- Persistencia (preorden plano: nivel|tipo|nombre|descripcion) ---
    void aPreordenPlano(std::vector<std::string>& out) const;
    bool reconstruirDesdePlano(const std::vector<std::string>& lineas);

    // Acceso de solo lectura a raices (para integracion/reportes).
    const std::vector<NodoGenerico*>& getRaices() const { return raices; }
};

#endif
