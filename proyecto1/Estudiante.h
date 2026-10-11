#ifndef ESTUDIANTE_H
#define ESTUDIANTE_H

#include <string>
#include <iostream>

// Estructura que representa a un estudiante.
// Se almacena en: std::list (coleccion principal) + ABB (indice por carne).
struct Estudiante {
    int carne;                 // clave unica (numero de carne)
    std::string nombre;        // nombre completo
    std::string correo;        // correo institucional
    std::string carrera;       // carrera del estudiante

    Estudiante();
    Estudiante(int c, const std::string& n, const std::string& co, const std::string& ca);

    std::string toString() const;
    std::string aLineaArchivo() const;              // formato: carne|nombre|correo|carrera
    static bool desdeLineaArchivo(const std::string& linea, Estudiante& out);
};

#endif
