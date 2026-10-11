// Proyecto 1 - Sistema de administracion academica y navegacion de contenidos.
// Estructuras: LISTA (std::list) + ABB (propio) + ARBOL GENERICO (propio)
//              + PILA (std::stack) + COLA con prioridad (2x std::queue).
// Compilar: g++ -std=c++17 -o sistema main.cpp Estudiante.cpp ABB.cpp ArbolGenerico.cpp HistorialNavegacion.cpp GestorSolicitudes.cpp Utilidades.cpp SistemaAcademico.cpp

#include <iostream>
#include <vector>
#include "SistemaAcademico.h"
#include "Utilidades.h"

using namespace std;
using namespace Utilidades;

static void mostrarEstudiante(const Estudiante& e) {
    cout << "  " << e.toString() << "\n";
}

// ---------- Submenu 1: estudiantes ----------
static void menuEstudiantes(SistemaAcademico& s) {
    while (true) {
        cout << "\n===== MANEJO DE ESTUDIANTES (Lista + ABB) =====\n"
             << "1. Registrar estudiante\n"
             << "2. Modificar estudiante\n"
             << "3. Eliminar estudiante\n"
             << "4. Mostrar todos\n"
             << "5. Buscar por carne (usa ABB)\n"
             << "6. Buscar por nombre (usa Lista)\n"
             << "7. Buscar por correo (usa Lista)\n"
             << "0. Volver\n";
        int op = leerEnteroRango("Opcion: ", 0, 7);
        if (op == 0) return;
        if (op == 1) {
            int c = leerCarne("Carne: ");
            string n = leerLineaNoVacia("Nombre: ");
            string co = leerCorreo("Correo institucional: ");
            string ca = leerLineaNoVacia("Carrera: ");
            if (s.registrarEstudiante(Estudiante(c, n, co, ca)))
                cout << "Estudiante registrado (lista + ABB).\n";
            else
                cout << "No se pudo registrar (carne duplicado o datos invalidos).\n";
            pausar();
        } else if (op == 2) {
            int c = leerCarne("Carne a modificar: ");
            Estudiante actual;
            if (!s.buscarPorCarne(c, actual)) { cout << "No existe.\n"; pausar(); continue; }
            cout << "Actual: "; mostrarEstudiante(actual);
            string n = leerLineaNoVacia("Nuevo nombre: ");
            string co = leerCorreo("Nuevo correo: ");
            string ca = leerLineaNoVacia("Nueva carrera: ");
            if (s.modificarEstudiante(Estudiante(c, n, co, ca))) cout << "Modificado.\n";
            else cout << "Error al modificar.\n";
            pausar();
        } else if (op == 3) {
            int c = leerCarne("Carne a eliminar: ");
            if (s.eliminarEstudiante(c)) cout << "Eliminado de lista y ABB.\n";
            else cout << "No existe.\n";
            pausar();
        } else if (op == 4) {
            vector<Estudiante> v; s.listarEstudiantes(v);
            cout << "--- Estudiantes (" << v.size() << ") ---\n";
            for (auto& e : v) mostrarEstudiante(e);
            if (v.empty()) cout << "(vacio)\n";
            pausar();
        } else if (op == 5) {
            int c = leerCarne("Carne: ");
            Estudiante e;
            if (s.buscarPorCarne(c, e)) { cout << "Encontrado: "; mostrarEstudiante(e); }
            else cout << "No encontrado.\n";
            pausar();
        } else if (op == 6) {
            string n = leerLineaNoVacia("Nombre (o parte): ");
            Estudiante e;
            if (s.buscarPorNombre(n, e)) { cout << "Encontrado: "; mostrarEstudiante(e); }
            else cout << "No encontrado.\n";
            pausar();
        } else {
            string co = leerLineaNoVacia("Correo: ");
            Estudiante e;
            if (s.buscarPorCorreo(co, e)) { cout << "Encontrado: "; mostrarEstudiante(e); }
            else cout << "No encontrado.\n";
            pausar();
        }
    }
}

// ---------- Submenu 2: cursos (arbol generico) ----------
static void menuCursos(SistemaAcademico& s) {
    ArbolGenerico& a = s.getCursos();
    while (true) {
        cout << "\n===== CURSOS Y CONTENIDOS (Arbol generico) =====\n"
             << "1. Crear curso (raiz)\n"
             << "2. Agregar nodo hijo\n"
             << "3. Eliminar nodo hoja\n"
             << "4. Mostrar arbol completo\n"
             << "5. Buscar nodo por nombre\n"
             << "6. Mostrar nivel de un nodo\n"
             << "7. Contar descendientes\n"
             << "8. Altura del arbol\n"
             << "9. Mostrar nodos hoja\n"
             << "10. Ruta raiz -> nodo\n"
             << "0. Volver\n";
        int op = leerEnteroRango("Opcion: ", 0, 10);
        if (op == 0) return;
        if (op == 1) {
            string n = leerLineaNoVacia("Nombre del curso: ");
            string d = leerLineaNoVacia("Descripcion (o -): ");
            if (d == "-") d = "";
            cout << (a.crearCurso(n, d) ? "Curso creado.\n" : "No se pudo (duplicado o vacio).\n");
            pausar();
        } else if (op == 2) {
            string p = leerLineaNoVacia("Nombre del nodo padre: ");
            string h = leerLineaNoVacia("Nombre del hijo nuevo: ");
            string t = leerTipoNodo();
            string d = leerLineaNoVacia("Descripcion (o -): ");
            if (d == "-") d = "";
            cout << (a.agregarHijo(p, h, t, d) ? "Agregado.\n" : "No se pudo (padre inexistente o nombre duplicado).\n");
            pausar();
        } else if (op == 3) {
            string n = leerLineaNoVacia("Nodo hoja a eliminar: ");
            cout << (a.eliminarHoja(n) ? "Eliminado.\n" : "No se pudo (no existe o no es hoja).\n");
            pausar();
        } else if (op == 4) {
            cout << "--- Arbol ---\n"; a.mostrar(cout); pausar();
        } else if (op == 5) {
            string n = leerLineaNoVacia("Nombre a buscar: ");
            NodoGenerico* nd = a.buscar(n);
            if (nd) cout << "Encontrado: [" << nd->tipo << "] " << nd->nombre << "\n";
            else cout << "No encontrado.\n";
            pausar();
        } else if (op == 6) {
            string n = leerLineaNoVacia("Nodo: ");
            int nv = a.nivelDe(n);
            if (nv >= 0) cout << "Nivel: " << nv << " (raiz = 0).\n";
            else cout << "No existe.\n";
            pausar();
        } else if (op == 7) {
            string n = leerLineaNoVacia("Nodo: ");
            int c = a.contarDescendientes(n);
            if (c >= 0) cout << "Descendientes: " << c << "\n";
            else cout << "No existe.\n";
            pausar();
        } else if (op == 8) {
            cout << "Altura: " << a.altura() << "\n"; pausar();
        } else if (op == 9) {
            vector<NodoGenerico*> hojas; a.obtenerHojas(hojas);
            cout << "--- Hojas (" << hojas.size() << ") ---\n";
            for (auto n : hojas) cout << "  - [" << n->tipo << "] " << n->nombre << "\n";
            pausar();
        } else {
            string n = leerLineaNoVacia("Nodo destino: ");
            vector<string> ruta;
            if (a.rutaHasta(n, ruta)) {
                cout << "Ruta: ";
                for (size_t i = 0; i < ruta.size(); i++) {
                    if (i) cout << " -> ";
                    cout << ruta[i];
                }
                cout << "\n";
            } else cout << "No existe.\n";
            pausar();
        }
    }
}

// ---------- Submenu 3: navegacion (pila) ----------
static void menuNavegacion(SistemaAcademico& s) {
    while (true) {
        cout << "\n===== NAVEGACION ACADEMICA (Pila) =====\n"
             << "1. Registrar navegacion (estudiante visita contenido del arbol)\n"
             << "2. Registrar accion academica (inscripcion / completado)\n"
             << "3. Mostrar historial\n"
             << "4. Retroceder al elemento anterior (pop)\n"
             << "5. Deshacer ultima accion\n"
             << "6. Limpiar historial\n"
             << "0. Volver\n";
        int op = leerEnteroRango("Opcion: ", 0, 6);
        if (op == 0) return;
        if (op == 1) {
            int c = leerCarne("Carne del estudiante: ");
            string nodo = leerLineaNoVacia("Contenido visitado (nombre exacto del arbol): ");
            cout << "Accion: 1=visita  2=inscripcion en actividad  3=recurso completado\n";
            int t = leerEnteroRango("Accion (1-3): ", 1, 3);
            string acc = (t == 1) ? "visita" : (t == 2 ? "inscripcion en actividad" : "recurso completado");
            if (s.registrarNavegacion(c, nodo, acc)) cout << "Visita registrada en la pila.\n";
            else cout << "No se pudo (verifique carne y que el nodo exista en el arbol).\n";
            pausar();
        } else if (op == 2) {
            int c = leerCarne("Carne: ");
            Estudiante e;
            if (!s.buscarPorCarne(c, e)) { cout << "Estudiante no existe.\n"; pausar(); continue; }
            string acc = leerLineaNoVacia("Accion academica: ");
            s.getHistoriales().registrarAccion(c, acc);
            cout << "Accion registrada (se podra deshacer).\n";
            pausar();
        } else if (op == 3) {
            int c = leerCarne("Carne: ");
            vector<Visita> h;
            if (s.getHistoriales().mostrarHistorial(c, h)) {
                cout << "--- Historial (tope primero) ---\n";
                for (auto& v : h) cout << "  - [" << v.fechaHora << "] " << v.contenido << " | " << v.accion << "\n";
            } else cout << "(sin historial)\n";
            pausar();
        } else if (op == 4) {
            int c = leerCarne("Carne: ");
            Visita v;
            if (s.getHistoriales().retroceder(c, v)) cout << "Retrocedio. Salio: " << v.contenido << "\n";
            else cout << "No hay historial.\n";
            pausar();
        } else if (op == 5) {
            int c = leerCarne("Carne: ");
            string a;
            if (s.getHistoriales().deshacerAccion(c, a)) cout << "Se deshizo: " << a << "\n";
            else cout << "No hay acciones para deshacer.\n";
            pausar();
        } else {
            int c = leerCarne("Carne: ");
            s.getHistoriales().limpiarHistorial(c);
            cout << "Historial limpiado.\n";
            pausar();
        }
    }
}

// ---------- Submenu 4: solicitudes (cola) ----------
static void menuSolicitudes(SistemaAcademico& s) {
    while (true) {
        cout << "\n===== SOLICITUDES PENDIENTES (Cola con prioridad) =====\n"
             << "1. Insertar solicitud\n"
             << "2. Atender siguiente (respeta prioridad)\n"
             << "3. Ver frente\n"
             << "4. Mostrar todas\n"
             << "5. Contar en espera\n"
             << "0. Volver\n";
        int op = leerEnteroRango("Opcion: ", 0, 5);
        if (op == 0) return;
        if (op == 1) {
            int c = leerCarne("Carne del solicitante: ");
            string t = leerTipoSolicitud();
            string d = leerLineaNoVacia("Descripcion: ");
            Solicitud out;
            if (s.crearSolicitud(c, t, d, out))
                cout << "Solicitud #" << out.id << " creada con prioridad "
                     << GestorSolicitudes::textoPrioridad(out.prioridad) << ".\n";
            else cout << "No se pudo (estudiante inexistente).\n";
            pausar();
        } else if (op == 2) {
            Solicitud at;
            if (s.getSolicitudes().atenderSiguiente(at))
                cout << "Atendida #" << at.id << " | carne " << at.carne << " | " << at.tipo << "\n";
            else cout << "No hay solicitudes.\n";
            pausar();
        } else if (op == 3) {
            Solicitud f;
            if (s.getSolicitudes().verFrente(f))
                cout << "Frente: #" << f.id << " [" << GestorSolicitudes::textoPrioridad(f.prioridad)
                     << "] " << f.tipo << " | " << f.descripcion << "\n";
            else cout << "Cola vacia.\n";
            pausar();
        } else if (op == 4) {
            vector<Solicitud> v; s.getSolicitudes().mostrarTodas(v);
            cout << "--- Pendientes (" << v.size() << ") [altas primero] ---\n";
            for (auto& x : v)
                cout << "  #" << x.id << " [" << GestorSolicitudes::textoPrioridad(x.prioridad)
                     << "] carne " << x.carne << " | " << x.tipo << " | " << x.descripcion << "\n";
            if (v.empty()) cout << "(vacio)\n";
            pausar();
        } else {
            cout << "En espera: " << s.getSolicitudes().contar()
                 << " (alta: " << s.getSolicitudes().contarAlta()
                 << ", normal: " << s.getSolicitudes().contarNormal() << ")\n";
            pausar();
        }
    }
}

// ---------- Submenu 5: indice ABB ----------
static void menuABB(SistemaAcademico& s) {
    while (true) {
        cout << "\n===== INDICE DE BUSQUEDA (ABB por carne) =====\n"
             << "1. Buscar por clave\n"
             << "2. Recorrido inorden\n"
             << "3. Recorrido preorden\n"
             << "4. Recorrido postorden\n"
             << "5. Menor y mayor\n"
             << "6. Altura y conteo\n"
             << "7. Eliminar nodo (3 casos)\n"
             << "0. Volver\n";
        int op = leerEnteroRango("Opcion: ", 0, 7);
        if (op == 0) return;
        const ABB& abb = s.getABB();
        if (op == 1) {
            int c = leerCarne("Carne: ");
            Estudiante e;
            if (abb.buscar(c, e)) { cout << "Encontrado: "; mostrarEstudiante(e); }
            else cout << "No encontrado.\n";
            pausar();
        } else if (op >= 2 && op <= 4) {
            vector<Estudiante> v;
            if (op == 2) abb.inorden(v);
            else if (op == 3) abb.preorden(v);
            else abb.postorden(v);
            cout << "--- Recorrido (" << v.size() << ") ---\n";
            for (auto& e : v) mostrarEstudiante(e);
            if (v.empty()) cout << "(vacio)\n";
            pausar();
        } else if (op == 5) {
            Estudiante mn, mx;
            if (abb.minimo(mn)) { cout << "Menor: "; mostrarEstudiante(mn); }
            else cout << "Vacio.\n";
            if (abb.maximo(mx)) { cout << "Mayor: "; mostrarEstudiante(mx); }
            pausar();
        } else if (op == 6) {
            cout << "Altura: " << abb.altura() << " | Nodos: " << abb.contarNodos() << "\n";
            pausar();
        } else {
            int c = leerCarne("Carne a eliminar: ");
            if (s.eliminarEstudiante(c)) cout << "Eliminado (ABB + lista, maneja hoja/1 hijo/2 hijos).\n";
            else cout << "No existe.\n";
            pausar();
        }
    }
}

// ---------- Submenu 6: reportes ----------
static void menuReportes(SistemaAcademico& s) {
    cout << "\n===== REPORTES =====\n";
    cout << "Carne para reporte de historial (0 = omitir): ";
    int c = leerEntero("Carne: ");
    if (c < 0) c = -1;
    s.generarReportes(cout, c <= 0 ? -1 : c);
    cout << "\nExportar a reportes.txt? 1=Si 0=No: ";
    int e = leerEnteroRango("", 0, 1);
    if (e == 1) {
        s.exportarReportes("reportes.txt", c <= 0 ? -1 : c);
        cout << "Exportado a reportes.txt\n";
    }
    pausar();
}

int main() {
    SistemaAcademico sistema;
    cout << "=================================================\n"
         << " SISTEMA DE ADMINISTRACION ACADEMICA (Proy. 1)\n"
         << " Lista + Pila + Cola + Arbol generico + ABB\n"
         << "=================================================\n";
    cout << "Cargar datos de ejemplo? 1=Si 0=No: ";
    int ej = leerEnteroRango("", 0, 1);
    if (ej == 1) { sistema.cargarDatosEjemplo(); cout << "Datos de ejemplo cargados.\n"; }

    while (true) {
        cout << "\n========== MENU PRINCIPAL ==========\n"
             << "1. Manejo de estudiantes\n"
             << "2. Manejo de cursos y contenidos\n"
             << "3. Navegacion academica\n"
             << "4. Solicitudes pendientes\n"
             << "5. Indice de busqueda\n"
             << "6. Reportes\n"
             << "7. Guardar datos\n"
             << "8. Cargar datos\n"
             << "9. Salir\n";
        int op = leerEnteroRango("Opcion: ", 1, 9);
        if (op == 1) menuEstudiantes(sistema);
        else if (op == 2) menuCursos(sistema);
        else if (op == 3) menuNavegacion(sistema);
        else if (op == 4) menuSolicitudes(sistema);
        else if (op == 5) menuABB(sistema);
        else if (op == 6) menuReportes(sistema);
        else if (op == 7) {
            cout << (sistema.guardarDatos(".") ? "Datos guardados.\n" : "Error al guardar.\n");
            pausar();
        } else if (op == 8) {
            sistema.cargarDatos(".");
            cout << "Datos cargados (si existen los .txt).\n";
            pausar();
        } else {
            cout << "Saliendo...\n";
            break;
        }
    }
    return 0;
}
