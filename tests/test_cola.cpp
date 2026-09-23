/**
 * test_cola.cpp — Tests unitarios de la Parte 3 (std::queue)
 *
 * Incluye directamente src/cola.cpp (con TESTING definido para excluir su
 * main) de forma que se prueba el código real, no una copia.
 *
 * Verifica:
 *   - El código de tests del enunciado (con assert).
 *   - Orden FIFO: las tareas se procesan en el orden de llegada.
 *   - La cola se vacía correctamente tras procesar todas las tareas.
 *   - empty() y size() reflejan el estado real de la cola.
 *
 * Compilar (desde actividad4/):
 *   g++ -std=c++23 -o test_cola tests/test_cola.cpp -lstdc++exp
 */
// Los assert deben comprobarse siempre, también en Release (donde CMake
// define NDEBUG y los desactivaría).
#undef NDEBUG
#include <cassert>

#define TESTING
#include "../src/cola.cpp"

#include <queue>
#include <vector>
#include <print>

static int tests_ok   = 0;
static int tests_fail = 0;

// Macro que evalúa una condición, imprime PASS o FAIL y actualiza contadores
#define TEST(condicion) \
    do { \
        if (condicion) { \
            ++tests_ok; \
            std::println("  [PASS] {}", #condicion); \
        } else { \
            ++tests_fail; \
            std::println("  [FAIL] {}", #condicion); \
        } \
    } while (false)

// Muestra el resumen final y devuelve el código de salida
int resumen() {
    std::println("\nResultado: {} pasados, {} fallidos", tests_ok, tests_fail);
    if (tests_fail == 0) {
        std::println("EXITO: todos los tests han pasado correctamente.");
        return 0;
    }
    std::println("ERROR: hay tests que fallan.");
    return 1;
}

// -----------------------------------------------------------------------
void test_asserts_enunciado() {
    std::println("--- test_asserts_enunciado ---");

    // Tests unitarios exactamente como aparecen en el enunciado
    std::queue<int> q;
    q.push(1);
    q.push(2);

    assert(q.front() == 1);
    q.pop();
    assert(q.front() == 2);
    q.pop();
    assert(q.empty());

    // Si algún assert fallara, el programa se abortaría antes de llegar aquí
    std::println("  [PASS] los 3 assert del enunciado");
    ++tests_ok;
}

// -----------------------------------------------------------------------
void test_procesar_tareas_enunciado() {
    std::println("--- test_procesar_tareas_enunciado ---");

    std::queue<int> cola;
    for (int tarea : {10, 20, 30, 40}) {
        cola.push(tarea);
    }
    TEST(cola.size() == 4);
    TEST(cola.front() == 10);   // la primera en llegar está al frente
    TEST(cola.back() == 40);    // la última en llegar está al final

    std::vector<int> orden = procesarCola(cola, false);

    const std::vector<int> esperado = {10, 20, 30, 40};
    TEST(orden == esperado);    // nunca se altera el orden (FIFO)
    TEST(cola.empty());         // la cola se vacía correctamente
    TEST(cola.size() == 0);
}

// -----------------------------------------------------------------------
void test_cola_vacia() {
    std::println("--- test_cola_vacia ---");

    std::queue<int> cola;
    TEST(cola.empty());
    TEST(procesarCola(cola, false).empty());   // procesar una cola vacía no hace nada
    TEST(cola.empty());
}

// -----------------------------------------------------------------------
void test_tamanio() {
    std::println("--- test_tamanio ---");

    std::queue<int> q;
    q.push(1);
    q.push(2);
    TEST(q.size() == 2);

    q.pop();
    TEST(q.size() == 1);

    q.pop();
    TEST(q.empty());
}

// -----------------------------------------------------------------------
int main() {
    std::println("=== Tests: std::queue ===\n");

    test_asserts_enunciado();
    test_procesar_tareas_enunciado();
    test_cola_vacia();
    test_tamanio();

    return resumen();
}
