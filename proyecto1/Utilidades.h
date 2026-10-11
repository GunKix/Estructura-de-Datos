#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <string>
#include <vector>

// Funciones auxiliares: validacion de entradas y formato.
namespace Utilidades {
    int leerEntero(const std::string& mensaje);
    int leerEnteroRango(const std::string& mensaje, int min, int max);
    int leerCarne(const std::string& mensaje);   // entero positivo
    std::string leerLineaNoVacia(const std::string& mensaje);
    std::string leerCorreo(const std::string& mensaje); // valida que tenga @ y .
    std::string leerTipoNodo();        // Curso/Unidad/Tema/Recurso/Actividad
    std::string leerTipoSolicitud();  // 4 tipos del enunciado
    void pausar();
    void limpiarPantalla();
    std::string fechaActual();         // "YYYY-MM-DD HH:MM:SS"
    std::string aMinusculas(std::string s);
    std::string trim(const std::string& s);
    std::vector<std::string> dividir(const std::string& s, char sep);
    bool esSoloDigitos(const std::string& s);
}

#endif
