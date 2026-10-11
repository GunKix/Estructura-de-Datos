#include "SistemaAcademico.h"
#include "Utilidades.h"
#include <fstream>
#include <iostream>

SistemaAcademico::SistemaAcademico() {}

std::list<Estudiante>::iterator SistemaAcademico::buscarEnLista(int carne) {
    for (auto it = listaEstudiantes.begin(); it != listaEstudiantes.end(); ++it)
        if (it->carne == carne) return it;
    return listaEstudiantes.end();
}

bool SistemaAcademico::carneExiste(int carne) const {
    return indiceABB.existe(carne);
}

// ---------- Estudiantes ----------
bool SistemaAcademico::registrarEstudiante(const Estudiante& e) {
    if (e.carne <= 0 || e.nombre.empty()) return false;
    if (carneExiste(e.carne)) return false;      // clave duplicada
    listaEstudiantes.push_back(e);               // LISTA
    indiceABB.insertar(e);                       // ABB (indice)
    return true;
}

bool SistemaAcademico::modificarEstudiante(const Estudiante& e) {
    auto it = buscarEnLista(e.carne);
    if (it == listaEstudiantes.end()) return false;
    *it = e;                 // actualiza lista
    indiceABB.modificar(e);  // actualiza indice
    return true;
}

bool SistemaAcademico::eliminarEstudiante(int carne) {
    auto it = buscarEnLista(carne);
    if (it == listaEstudiantes.end()) return false;
    listaEstudiantes.erase(it);      // sale de la lista
    indiceABB.eliminar(carne);       // sale del indice
    historiales.limpiarHistorial(carne); // limpia su historial (coherencia)
    return true;
}

bool SistemaAcademico::buscarPorCarne(int carne, Estudiante& out) const {
    return indiceABB.buscar(carne, out); // busqueda eficiente O(log n) promedio
}

bool SistemaAcademico::buscarPorNombre(const std::string& nombre, Estudiante& out) const {
    std::string q = Utilidades::aMinusculas(nombre);
    for (auto& e : listaEstudiantes) {
        if (Utilidades::aMinusculas(e.nombre).find(q) != std::string::npos) {
            out = e;
            return true;
        }
    }
    return false;
}

bool SistemaAcademico::buscarPorCorreo(const std::string& correo, Estudiante& out) const {
    std::string q = Utilidades::aMinusculas(correo);
    for (auto& e : listaEstudiantes) {
        if (Utilidades::aMinusculas(e.correo) == q) { out = e; return true; }
    }
    return false;
}

void SistemaAcademico::listarEstudiantes(std::vector<Estudiante>& out) const {
    out.clear();
    for (auto& e : listaEstudiantes) out.push_back(e);
}

int SistemaAcademico::cantidadEstudiantes() const { return (int)listaEstudiantes.size(); }

// ---------- Navegacion integrada ----------
bool SistemaAcademico::registrarNavegacion(int carne, const std::string& nombreNodo,
                                           const std::string& accion) {
    Estudiante e;
    if (!indiceABB.buscar(carne, e)) return false;  // estudiante debe existir (ABB)
    if (!cursos.existe(nombreNodo)) return false;    // contenido debe existir (arbol)
    historiales.registrarNavegacion(carne, nombreNodo, accion, Utilidades::fechaActual());
    return true;
}

// ---------- Solicitudes integradas ----------
bool SistemaAcademico::crearSolicitud(int carne, const std::string& tipo,
                                      const std::string& descripcion, Solicitud& out) {
    Estudiante e;
    if (!indiceABB.buscar(carne, e)) return false; // solicitante debe existir (ABB)
    out = solicitudes.insertar(carne, tipo, descripcion, Utilidades::fechaActual());
    return true;
}

void SistemaAcademico::reconstruirABB() {
    indiceABB.limpiar();
    for (auto& e : listaEstudiantes) indiceABB.insertar(e);
}

// ---------- Reportes ----------
void SistemaAcademico::generarReportes(std::ostream& os, int carneHistorial) const {
    os << "========== REPORTES DEL SISTEMA ==========\n\n";

    // 1. Lista completa de estudiantes.
    os << "1) LISTA COMPLETA DE ESTUDIANTES (" << listaEstudiantes.size() << "):\n";
    if (listaEstudiantes.empty()) os << "   (vacio)\n";
    for (auto& e : listaEstudiantes) os << "   - " << e.toString() << "\n";

    // 2. ABB inorden.
    os << "\n2) RECORRIDO ABB EN INORDEN (ordenado por carne):\n";
    std::vector<Estudiante> ino;
    indiceABB.inorden(ino);
    if (ino.empty()) os << "   (vacio)\n";
    for (auto& e : ino) os << "   - " << e.toString() << "\n";

    // 3. Estructura jerarquica.
    os << "\n3) ESTRUCTURA JERARQUICA COMPLETA:\n";
    cursos.mostrar(os);

    // 4. Historial de un estudiante.
    os << "\n4) HISTORIAL DE NAVEGACION";
    if (carneHistorial > 0) {
        os << " (carne " << carneHistorial << "):\n";
        std::vector<Visita> h;
        if (historiales.mostrarHistorial(carneHistorial, h)) {
            for (auto& v : h)
                os << "   - [" << v.fechaHora << "] " << v.contenido << " | " << v.accion << "\n";
        } else os << "   (sin historial)\n";
    } else os << ":\n   (indique un carne para ver su historial)\n";

    // 5. Solicitudes pendientes.
    os << "\n5) SOLICITUDES PENDIENTES (" << solicitudes.contar() << "):\n";
    std::vector<Solicitud> sols;
    solicitudes.mostrarTodas(sols);
    if (sols.empty()) os << "   (vacio)\n";
    for (auto& s : sols)
        os << "   - #" << s.id << " [" << GestorSolicitudes::textoPrioridad(s.prioridad)
           << "] carne " << s.carne << " | " << s.tipo << " | " << s.descripcion << "\n";

    // 6 y 7. Nodos y altura del generico.
    os << "\n6) CANTIDAD DE NODOS DEL ARBOL GENERICO: " << cursos.contarNodos() << "\n";
    os << "7) ALTURA DEL ARBOL GENERICO: " << cursos.altura() << "\n";

    // 8. Cantidad estudiantes.
    os << "8) CANTIDAD DE ESTUDIANTES REGISTRADOS: " << listaEstudiantes.size() << "\n";

    // 9. Menor y mayor.
    os << "9) MENOR Y MAYOR CARNE:\n";
    Estudiante mn, mx;
    if (indiceABB.minimo(mn)) os << "   Menor: " << mn.toString() << "\n";
    else os << "   Menor: (vacio)\n";
    if (indiceABB.maximo(mx)) os << "   Mayor: " << mx.toString() << "\n";
    else os << "   Mayor: (vacio)\n";

    // 10. Hojas.
    os << "10) RECURSOS HOJA DEL ARBOL:\n";
    std::vector<NodoGenerico*> hojas;
    cursos.obtenerHojas(hojas);
    if (hojas.empty()) os << "   (vacio)\n";
    for (auto n : hojas) os << "   - [" << n->tipo << "] " << n->nombre << "\n";

    os << "\n==========================================\n";
}

void SistemaAcademico::exportarReportes(const std::string& archivo, int carneHistorial) const {
    std::ofstream f(archivo);
    if (f.is_open()) generarReportes(f, carneHistorial);
}

// ---------- Persistencia ----------
static bool escribirLineas(const std::string& path, const std::vector<std::string>& lineas) {
    std::ofstream f(path);
    if (!f.is_open()) return false;
    for (auto& l : lineas) f << l << "\n";
    return true;
}
static bool leerLineas(const std::string& path, std::vector<std::string>& lineas) {
    std::ifstream f(path);
    if (!f.is_open()) return false;
    lineas.clear();
    std::string l;
    while (std::getline(f, l)) {
        if (!l.empty() && l.back() == '\r') l.pop_back();
        if (!l.empty()) lineas.push_back(l);
    }
    return true;
}

bool SistemaAcademico::guardarDatos(const std::string& carpeta) const {
    std::string sep = (carpeta == "." || carpeta.empty()) ? "" : carpeta + "/";
    // estudiantes
    std::vector<std::string> le;
    for (auto& e : listaEstudiantes) le.push_back(e.aLineaArchivo());
    if (!escribirLineas(sep + "estudiantes.txt", le)) return false;
    // cursos (preorden plano)
    std::vector<std::string> lc;
    cursos.aPreordenPlano(lc);
    if (!escribirLineas(sep + "cursos.txt", lc)) return false;
    // historial
    if (!escribirLineas(sep + "historial.txt", historiales.aLineasArchivo())) return false;
    // solicitudes
    if (!escribirLineas(sep + "solicitudes.txt", solicitudes.aLineasArchivo())) return false;
    return true;
}

bool SistemaAcademico::cargarDatos(const std::string& carpeta) {
    std::string sep = (carpeta == "." || carpeta.empty()) ? "" : carpeta + "/";
    std::vector<std::string> le;
    if (leerLineas(sep + "estudiantes.txt", le)) {
        listaEstudiantes.clear();
        for (auto& l : le) {
            Estudiante e;
            if (Estudiante::desdeLineaArchivo(l, e)) listaEstudiantes.push_back(e);
        }
        reconstruirABB();
    }
    std::vector<std::string> lc;
    if (leerLineas(sep + "cursos.txt", lc)) cursos.reconstruirDesdePlano(lc);
    std::vector<std::string> lh;
    if (leerLineas(sep + "historial.txt", lh)) historiales.cargarDesdeLineas(lh);
    std::vector<std::string> ls;
    if (leerLineas(sep + "solicitudes.txt", ls)) solicitudes.cargarDesdeLineas(ls);
    return true;
}

void SistemaAcademico::cargarDatosEjemplo() {
    listaEstudiantes.clear(); indiceABB.limpiar(); cursos.limpiar();
    historiales.limpiarTodo(); solicitudes.limpiar();
    registrarEstudiante(Estudiante(20240001, "Ana Perez", "ana.perez@institucion.ac.cr", "Ingenieria"));
    registrarEstudiante(Estudiante(20240002, "Luis Gomez", "luis.gomez@institucion.ac.cr", "Computacion"));
    registrarEstudiante(Estudiante(20240003, "Maria Ruiz", "maria.ruiz@institucion.ac.cr", "Electronica"));
    cursos.crearCurso("Estructuras de Datos", "Curso base de ejemplo");
    cursos.agregarHijo("Estructuras de Datos", "Unidad 1: Fundamentos", "Unidad");
    cursos.agregarHijo("Unidad 1: Fundamentos", "Tema 1.1: Tipos abstractos", "Tema");
    cursos.agregarHijo("Tema 1.1: Tipos abstractos", "Recurso: PDF de teoria", "Recurso", "PDF introductorio");
    cursos.agregarHijo("Tema 1.1: Tipos abstractos", "Recurso: Video introductorio", "Recurso", "Video inicial");
    cursos.agregarHijo("Unidad 1: Fundamentos", "Tema 1.2: Complejidad", "Tema");
    cursos.agregarHijo("Tema 1.2: Complejidad", "Actividad: Quiz", "Actividad", "Quiz semanal");
    cursos.agregarHijo("Estructuras de Datos", "Unidad 2: Estructuras lineales", "Unidad");
    cursos.agregarHijo("Unidad 2: Estructuras lineales", "Tema 2.1: Listas", "Tema");
    cursos.agregarHijo("Unidad 2: Estructuras lineales", "Tema 2.2: Pilas", "Tema");
    cursos.agregarHijo("Unidad 2: Estructuras lineales", "Tema 2.3: Colas", "Tema");
    registrarNavegacion(20240001, "Recurso: PDF de teoria", "visita");
    registrarNavegacion(20240001, "Tema 1.2: Complejidad", "visita");
    Solicitud s;
    crearSolicitud(20240001, "reporte de error en material", "El PDF no abre en el tema 1.1", s);
    crearSolicitud(20240002, "consulta academica", "Duda sobre complejidad", s);
}
