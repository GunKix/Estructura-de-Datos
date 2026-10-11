#ifndef HISTORIAL_NAVEGACION_H
#define HISTORIAL_NAVEGACION_H

#include <stack>
#include <map>
#include <vector>
#include <string>

// Un registro de navegacion (elemento de la pila de historial).
struct Visita {
    std::string contenido;   // nombre del nodo visitado (recurso/tema)
    std::string accion;      // ej: "visita", "inscripcion en actividad", "recurso completado"
    std::string fechaHora;   // texto libre de fecha/hora
};

// Historial por estudiante usando PILA (std::stack).
// Tambien maneja pila de acciones para "deshacer ultima accion academica".
class HistorialNavegacion {
private:
    std::map<int, std::stack<Visita>> historiales;       // carne -> pila de visitas
    std::map<int, std::stack<std::string>> acciones;    // carne -> pila de acciones academicas

public:
    // Registrar navegacion: push en la pila del estudiante.
    void registrarNavegacion(int carne, const std::string& contenido,
                             const std::string& accion, const std::string& fechaHora);
    // Registrar solo accion academica (para deshacer).
    void registrarAccion(int carne, const std::string& accion);

    bool tieneHistorial(int carne) const;
    bool mostrarHistorial(int carne, std::vector<Visita>& out) const; // out: tope primero
    // Retroceder: pop del historial (sale del elemento actual y vuelve al anterior).
    bool retroceder(int carne, Visita& saliente);
    void limpiarHistorial(int carne);
    void limpiarTodo();
    // Deshacer ultima accion academica.
    bool deshacerAccion(int carne, std::string& accionDeshecha);

    int tamanoHistorial(int carne) const;
    int tamanoAcciones(int carne) const;
    std::vector<int> carnesConHistorial() const;

    // Persistencia: una linea por visita, de FONDO->TOPE.
    std::vector<std::string> aLineasArchivo() const; // carne|contenido|accion|fechaHora
    void cargarDesdeLineas(const std::vector<std::string>& lineas);
};

#endif
