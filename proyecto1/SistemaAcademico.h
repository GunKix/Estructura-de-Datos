#ifndef SISTEMA_ACADEMICO_H
#define SISTEMA_ACADEMICO_H

#include <list>
#include <string>
#include "Estudiante.h"
#include "ABB.h"
#include "ArbolGenerico.h"
#include "HistorialNavegacion.h"
#include "GestorSolicitudes.h"

// Sistema integral: integra LISTA + ABB + ARBOL GENERICO + PILA + COLA.
// - Estudiantes: std::list (principal) + ABB (indice por carne).
// - Cursos: ArbolGenerico (bosque, cada raiz es un curso).
// - Navegacion: HistorialNavegacion (pilas por estudiante).
// - Solicitudes: GestorSolicitudes (dos colas con prioridad).
class SistemaAcademico {
private:
    std::list<Estudiante> listaEstudiantes;  // LISTA: coleccion dinamica principal
    ABB indiceABB;                            // ABB: indice por carne
    ArbolGenerico cursos;                     // ARBOL GENERICO: jerarquia de contenidos
    HistorialNavegacion historiales;          // PILA: historial por estudiante
    GestorSolicitudes solicitudes;            // COLA: solicitudes con prioridad

    // Helpers de lista
    std::list<Estudiante>::iterator buscarEnLista(int carne);
    bool carneExiste(int carne) const;

public:
    SistemaAcademico();

    // ===== 1. Estudiantes (lista + ABB sincronizados) =====
    bool registrarEstudiante(const Estudiante& e);
    bool modificarEstudiante(const Estudiante& e);
    bool eliminarEstudiante(int carne);
    bool buscarPorCarne(int carne, Estudiante& out) const; // via ABB (eficiente)
    bool buscarPorNombre(const std::string& nombre, Estudiante& out) const; // via lista
    bool buscarPorCorreo(const std::string& correo, Estudiante& out) const;  // via lista
    void listarEstudiantes(std::vector<Estudiante>& out) const;
    int cantidadEstudiantes() const;

    // ===== 2. Cursos (arbol generico) =====
    ArbolGenerico& getCursos() { return cursos; }
    const ArbolGenerico& getCursos() const { return cursos; }

    // ===== 3. Navegacion (pila) - integrada: valida estudiante (ABB) + nodo (arbol) =====
    // Retorna false si el estudiante o el contenido no existen.
    bool registrarNavegacion(int carne, const std::string& nombreNodo, const std::string& accion);
    HistorialNavegacion& getHistoriales() { return historiales; }

    // ===== 4. Solicitudes (cola) - integrada =====
    bool crearSolicitud(int carne, const std::string& tipo,
                        const std::string& descripcion, Solicitud& out);
    GestorSolicitudes& getSolicitudes() { return solicitudes; }

    // ===== 5. ABB =====
    const ABB& getABB() const { return indiceABB; }

    // ===== Reportes (10 obligatorios) =====
    void generarReportes(std::ostream& os, int carneHistorial = -1) const;
    void exportarReportes(const std::string& archivo, int carneHistorial = -1) const;

    // ===== Persistencia =====
    bool guardarDatos(const std::string& carpeta = ".") const;
    bool cargarDatos(const std::string& carpeta = ".");

    // Datos de ejemplo (escenario de uso del enunciado).
    void cargarDatosEjemplo();

    // Reconstruye el ABB desde la lista (se usa tras cargar/modificar).
    void reconstruirABB();
};

#endif
