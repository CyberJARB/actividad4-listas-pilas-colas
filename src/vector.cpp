/**
 * vector.cpp — Parte 1: Lista (std::vector) — inserción y análisis de coste
 *
 * Parte del vector v = {10, 20, 30, 40, 50} (n = 5) y realiza:
 *   1. Insertar 5 al principio.
 *   2. Insertar 35 en la posición 3.
 *   3. Insertar 60 al final.
 * Para cada inserción indica cuántos elementos se desplazan y el coste exacto
 * para n = 5 (cada inserción se analiza sobre el vector original de 5 elementos).
 * Después aplica las tres inserciones en secuencia sobre el mismo vector.
 *
 * Además (objetivo de la actividad): función recursiva simple comparada con
 * su versión iterativa (suma de los elementos del vector).
 *
 * Compilar:
 *   g++ -std=c++23 -o vector vector.cpp -lstdc++exp
 */
#include <vector>
#include <string_view>
#include <cstddef>
#include <print>

// -----------------------------------------------------------------------
// Inserta 'valor' en la posición 'pos' y devuelve cuántos elementos se han
// desplazado una posición a la derecha para hacerle hueco.
// Desplazamientos = n - pos  →  coste O(n) en el peor caso (pos = 0)
//                             →  coste O(1) amortizado si pos = n (al final)
// -----------------------------------------------------------------------
std::size_t insertarYContar(std::vector<int>& v, std::size_t pos, int valor) {
    std::size_t desplazados = v.size() - pos;   // elementos en [pos, n)
    v.insert(v.begin() + pos, valor);
    return desplazados;
}

// -----------------------------------------------------------------------
// Suma recursiva: suma(v, i) = v[i] + suma(v, i + 1);  caso base: i == n
// Tiempo O(n) y memoria O(n) (una llamada en la pila por cada elemento).
// -----------------------------------------------------------------------
int sumaRecursiva(const std::vector<int>& v, std::size_t i = 0) {
    if (i == v.size()) {
        return 0;                               // caso base: no quedan elementos
    }
    return v[i] + sumaRecursiva(v, i + 1);      // caso recursivo
}

// -----------------------------------------------------------------------
// Suma iterativa: mismo resultado, tiempo O(n) pero memoria O(1).
// -----------------------------------------------------------------------
int sumaIterativa(const std::vector<int>& v) {
    int total = 0;
    for (int x : v) {
        total += x;
    }
    return total;
}

// Muestra el contenido del vector con una etiqueta descriptiva
void mostrarVector(const std::vector<int>& v, std::string_view etiqueta) {
    std::print("{}: [ ", etiqueta);
    for (const auto& elem : v) {
        std::print("{} ", elem);
    }
    std::println("]  (tamaño = {})", v.size());
}

#ifndef TESTING
int main() {
    // Vector inicial con n = 5 elementos
    std::vector<int> v = {10, 20, 30, 40, 50};
    mostrarVector(v, "Estado inicial (n = 5)");

    // ================================================================
    // A) Análisis de cada inserción sobre una copia del vector original (n = 5)
    // ================================================================
    std::println("\n=== A) Coste de cada inserción para n = 5 ===");

    // Inserción 1: valor 5 al principio (pos 0)
    // Se desplazan TODOS los elementos: n - 0 = 5  →  O(n)
    {
        std::vector<int> copia = v;
        std::size_t d = insertarYContar(copia, 0, 5);
        std::println("\n[1] Insertar 5 al principio (posición 0)");
        std::println("    Elementos desplazados : {}  (10, 20, 30, 40, 50 se mueven una posición)", d);
        std::println("    Coste exacto n=5      : {} desplazamientos + 1 escritura = {} operaciones  → O(n)", d, d + 1);
        mostrarVector(copia, "    Resultado           ");
    }

    // Inserción 2: valor 35 en la posición 3
    // Se desplazan los elementos de las posiciones 3 y 4: n - 3 = 2  →  O(n)
    {
        std::vector<int> copia = v;
        std::size_t d = insertarYContar(copia, 3, 35);
        std::println("\n[2] Insertar 35 en la posición 3");
        std::println("    Elementos desplazados : {}  (40 y 50 se mueven una posición)", d);
        std::println("    Coste exacto n=5      : {} desplazamientos + 1 escritura = {} operaciones  → O(n)", d, d + 1);
        mostrarVector(copia, "    Resultado           ");
    }

    // Inserción 3: valor 60 al final (pos n)
    // No se desplaza ningún elemento: n - 5 = 0  →  O(1) amortizado
    {
        std::vector<int> copia = v;
        std::size_t d = insertarYContar(copia, copia.size(), 60);
        std::println("\n[3] Insertar 60 al final (posición 5)");
        std::println("    Elementos desplazados : {}  (no se mueve ningún elemento)", d);
        std::println("    Coste exacto n=5      : {} desplazamientos + 1 escritura = {} operación  → O(1) amortizado", d, d + 1);
        std::println("    (amortizado: si size == capacity, el vector reserva más memoria y copia");
        std::println("     los n elementos una vez; repartido entre muchas inserciones sigue siendo O(1))");
        mostrarVector(copia, "    Resultado           ");
    }

    // ================================================================
    // B) Las tres inserciones aplicadas en secuencia sobre el mismo vector
    //    (n crece tras cada inserción: 5 → 6 → 7 → 8)
    // ================================================================
    std::println("\n=== B) Inserciones en secuencia sobre el mismo vector ===");
    std::size_t d1 = insertarYContar(v, 0, 5);
    std::println("  insertar 5 en pos 0  (n=5) → desplazados: {}", d1);
    std::size_t d2 = insertarYContar(v, 3, 35);
    std::println("  insertar 35 en pos 3 (n=6) → desplazados: {}", d2);
    v.push_back(60);
    std::println("  push_back(60)        (n=7) → desplazados: 0");
    mostrarVector(v, "  Resultado final     ");

    // ================================================================
    // C) Función recursiva vs iterativa
    // ================================================================
    std::println("\n=== C) Suma de elementos: recursiva vs iterativa ===");
    std::println("  sumaRecursiva(v) = {}   → O(n) tiempo, O(n) memoria (pila de llamadas)", sumaRecursiva(v));
    std::println("  sumaIterativa(v) = {}   → O(n) tiempo, O(1) memoria", sumaIterativa(v));

    return 0;
}
#endif
