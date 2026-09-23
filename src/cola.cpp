/**
 * cola.cpp — Parte 3: Simulación de cola de tareas con std::queue
 *
 * Las tareas llegan en un orden y se procesan en el mismo orden (FIFO).
 * Nunca se altera el orden: ninguna tarea se "adelanta" a otra que llegó antes.
 *
 * Operaciones usadas (todas O(1)):
 *   push()  → encolar una tarea al final
 *   front() → consultar la tarea más antigua (sin eliminarla)
 *   pop()   → eliminar la tarea del frente tras procesarla
 *   empty() → comprobar si quedan tareas pendientes
 *
 * Compilar:
 *   g++ -std=c++23 -o cola cola.cpp -lstdc++exp
 */
#include <queue>
#include <vector>
#include <print>

// -----------------------------------------------------------------------
// Procesa todas las tareas de la cola en orden FIFO, mostrando cada una,
// y devuelve el orden en que se han procesado. La cola queda vacía.
// Coste: O(n), una operación front() + pop() O(1) por tarea.
// -----------------------------------------------------------------------
std::vector<int> procesarCola(std::queue<int>& cola, bool mostrar = true) {
    std::vector<int> procesadas;

    // 3. Mientras la cola no esté vacía
    while (!cola.empty()) {
        // 4. Mostrar la tarea que se va a procesar
        if (mostrar) {
            std::println("Procesando tarea: {}", cola.front());
        }
        procesadas.push_back(cola.front());
        // 5. Eliminar esa tarea de la cola
        cola.pop();
    }
    return procesadas;
}

#ifndef TESTING
int main() {
    std::println("=== Cola de tareas — orden FIFO ===\n");

    // 1. Cola de enteros donde cada número representa una tarea
    std::queue<int> cola;

    // 2. Añadir las tareas en este orden: 10, 20, 30, 40
    for (int tarea : {10, 20, 30, 40}) {
        cola.push(tarea);
    }

    std::println("Cola inicial:");
    std::println("front → [10, 20, 30, 40] → back\n");

    std::println("Procesamiento:");
    procesarCola(cola);

    std::println("\nCola vacía: {} — todas las tareas procesadas sin alterar el orden.",
                 cola.empty());
    return 0;
}
#endif
