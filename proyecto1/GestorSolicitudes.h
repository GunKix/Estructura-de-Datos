#ifndef GESTOR_SOLICITUDES_H
#define GESTOR_SOLICITUDES_H

#include <queue>
#include <string>
#include <vector>

// Solicitud generada por un estudiante.
struct Solicitud {
    int id;                    // consecutivo automatico
    int carne;                 // estudiante solicitante
    std::string tipo;          // revision de tarea | consulta academica | habilitacion | reporte de error
    std::string descripcion;
    int prioridad;             // 1 = ALTA, 2 = NORMAL
    std::string fecha;
};

// Cola con prioridad implementada con DOS COLAS separadas (std::queue):
//   - colaAlta   : reportes de error y revisiones de tarea (prioridad 1)
//   - colaNormal : habilitaciones y consultas (prioridad 2)
// Al atender, siempre se sirve primero la cola de alta prioridad.
class GestorSolicitudes {
private:
    std::queue<Solicitud> colaAlta;
    std::queue<Solicitud> colaNormal;
    int siguienteId;

    static int prioridadDeTipo(const std::string& tipo);
    static std::queue<Solicitud> copiarCola(std::queue<Solicitud> q);

public:
    GestorSolicitudes();

    // Regla de prioridad documentada:
    //   ALTA(1): "reporte de error en material", "revision de tarea"
    //   NORMAL(2): "habilitacion de recurso", "consulta academica", otros
    Solicitud insertar(int carne, const std::string& tipo,
                       const std::string& descripcion, const std::string& fecha);

    bool atenderSiguiente(Solicitud& atendida);  // respeta prioridad
    bool verFrente(Solicitud& out) const;        // proxima a atender (frente de alta o normal)
    void mostrarTodas(std::vector<Solicitud>& out) const; // altas primero, en orden FIFO
    int contar() const;
    int contarAlta() const;
    int contarNormal() const;
    bool vacia() const;
    void limpiar();

    // Persistencia.
    std::vector<std::string> aLineasArchivo() const; // id|carne|tipo|descripcion|prioridad|fecha
    void cargarDesdeLineas(const std::vector<std::string>& lineas);

    static std::string textoPrioridad(int p);
};

#endif
