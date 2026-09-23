/**
 * test_vector.cpp — Tests unitarios de la Parte 1 (std::vector)
 *
 * Incluye directamente src/vector.cpp (con TESTING definido para excluir su
 * main) de forma que se prueba el código real, no una copia.
 *
 * Verifica:
 *   - Inserción al principio, en la posición 3 y al final.
 *   - Número de elementos desplazados en cada inserción (n = 5).
 *   - Tamaño y valores en las posiciones esperadas.
 *   - Inserciones en secuencia, borrado y vaciado.
 *   - Suma recursiva e iterativa dan el mismo resultado.
 *
 * Compilar (desde actividad4/):
 *   g++ -std=c++23 -o test_vector tests/test_vector.cpp -lstdc++exp
 */
#define TESTING
#include "../src/vector.cpp"

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
void test_insercion_al_principio() {
    std::println("--- test_insercion_al_principio ---");

    std::vector<int> v = {10, 20, 30, 40, 50};
    std::size_t desplazados = insertarYContar(v, 0, 5);

    TEST(desplazados == 5);     // se mueven los 5 elementos → O(n)
    TEST(v.size() == 6);
    TEST(v[0] == 5);
    TEST(v[1] == 10);
    TEST(v[5] == 50);
}

// -----------------------------------------------------------------------
void test_insercion_en_posicion_3() {
    std::println("--- test_insercion_en_posicion_3 ---");

    std::vector<int> v = {10, 20, 30, 40, 50};
    std::size_t desplazados = insertarYContar(v, 3, 35);

    TEST(desplazados == 2);     // se mueven 40 y 50
    TEST(v.size() == 6);
    TEST(v[2] == 30);           // lo anterior a la posición 3 no cambia
    TEST(v[3] == 35);
    TEST(v[4] == 40);
    TEST(v[5] == 50);
}

// -----------------------------------------------------------------------
void test_insercion_al_final() {
    std::println("--- test_insercion_al_final ---");

    std::vector<int> v = {10, 20, 30, 40, 50};
    std::size_t desplazados = insertarYContar(v, v.size(), 60);

    TEST(desplazados == 0);     // no se mueve nada → O(1) amortizado
    TEST(v.size() == 6);
    TEST(v.back() == 60);
    TEST(v[0] == 10);
    TEST(v[4] == 50);
}

// -----------------------------------------------------------------------
void test_inserciones_en_secuencia() {
    std::println("--- test_inserciones_en_secuencia ---");

    std::vector<int> v = {10, 20, 30, 40, 50};
    TEST(insertarYContar(v, 0, 5) == 5);    // n = 5
    TEST(insertarYContar(v, 3, 35) == 3);   // n = 6: se mueven 30, 40, 50
    v.push_back(60);

    const std::vector<int> esperado = {5, 10, 20, 35, 30, 40, 50, 60};
    TEST(v == esperado);
}

// -----------------------------------------------------------------------
void test_borrado_y_vaciado() {
    std::println("--- test_borrado_y_vaciado ---");

    std::vector<int> v = {1, 2, 3, 4, 5};
    v.erase(v.begin() + 2);     // eliminar el elemento en posición 2 (valor 3)
    TEST(v.size() == 4);
    TEST(v[2] == 4);            // el 4 ocupa ahora la posición 2

    v.clear();
    TEST(v.empty());
    TEST(v.size() == 0);
}

// -----------------------------------------------------------------------
void test_recursiva_vs_iterativa() {
    std::println("--- test_recursiva_vs_iterativa ---");

    const std::vector<int> v = {10, 20, 30, 40, 50};
    const std::vector<int> vacio;
    TEST(sumaRecursiva(v) == 150);
    TEST(sumaIterativa(v) == 150);
    TEST(sumaRecursiva(vacio) == 0);        // caso base: vector vacío
    TEST(sumaIterativa(vacio) == 0);
}

// -----------------------------------------------------------------------
int main() {
    std::println("=== Tests: std::vector ===\n");

    test_insercion_al_principio();
    test_insercion_en_posicion_3();
    test_insercion_al_final();
    test_inserciones_en_secuencia();
    test_borrado_y_vaciado();
    test_recursiva_vs_iterativa();

    return resumen();
}
