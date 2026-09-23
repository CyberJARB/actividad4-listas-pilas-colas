/**
 * test_pila.cpp — Tests unitarios de la Parte 2 (std::stack)
 *
 * Incluye directamente src/pila_rpn.cpp (con TESTING definido para excluir
 * su main) de forma que se prueba el código real, no una copia.
 *
 * Verifica:
 *   - Orden LIFO con los valores del enunciado (10, 20, 30, 40).
 *   - La función balanceado() con los assert del enunciado y casos extra.
 *   - La evaluación de expresiones RPN y sus errores.
 *
 * Compilar (desde actividad4/):
 *   g++ -std=c++23 -o test_pila tests/test_pila.cpp -lstdc++exp
 */
// Los assert deben comprobarse siempre, también en Release (donde CMake
// define NDEBUG y los desactivaría).
#undef NDEBUG
#include <cassert>

#define TESTING
#include "../src/pila_rpn.cpp"

#include <stack>
#include <stdexcept>
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
void test_operaciones_basicas() {
    std::println("--- test_operaciones_basicas ---");

    std::stack<int> pila;
    TEST(pila.empty());         // la pila recién creada está vacía

    pila.push(10);
    pila.push(20);
    pila.push(30);
    pila.push(40);
    TEST(pila.size() == 4);

    // LIFO: la cima va siendo 40, 30, 20, 10
    TEST(pila.top() == 40);
    pila.pop();
    TEST(pila.top() == 30);
    pila.pop();
    TEST(pila.top() == 20);
    pila.pop();
    TEST(pila.top() == 10);
    pila.pop();
    TEST(pila.empty());
}

// -----------------------------------------------------------------------
void test_asserts_enunciado() {
    std::println("--- test_asserts_enunciado ---");

    // Tests unitarios exactamente como aparecen en el enunciado
    assert(balanceado("(())") == true);
    assert(balanceado("(()") == false);
    assert(balanceado("())") == false);
    assert(balanceado("()()") == true);

    // Si algún assert fallara, el programa se abortaría antes de llegar aquí
    std::println("  [PASS] los 4 assert del enunciado");
    ++tests_ok;
}

// -----------------------------------------------------------------------
void test_parentesis_balanceados() {
    std::println("--- test_parentesis_balanceados ---");

    TEST(balanceado("(())")    == true);
    TEST(balanceado("(()")     == false);
    TEST(balanceado("())")     == false);
    TEST(balanceado("()()")    == true);

    // Casos extra
    TEST(balanceado("")        == true);   // sin paréntesis: nada pendiente
    TEST(balanceado("((()))")  == true);
    TEST(balanceado(")(")      == false);  // cierre antes de apertura
    TEST(balanceado("(a+b)*c") == true);   // se ignoran los demás caracteres
}

// -----------------------------------------------------------------------
void test_rpn() {
    std::println("--- test_rpn ---");

    TEST(evaluarRPN("3 4 +") == 7);
    TEST(evaluarRPN("3 4 + 2 *") == 14);
    TEST(evaluarRPN("5 1 2 + 4 * + 3 -") == 14);
    TEST(evaluarRPN("10 3 -") == 7);        // el orden de los operandos importa
    TEST(evaluarRPN("20 4 /") == 5);
    TEST(evaluarRPN("-3 2 *") == -6);       // números negativos

    // Expresiones inválidas → std::invalid_argument
    bool lanzada = false;
    try { (void)evaluarRPN("2 +"); } catch (const std::invalid_argument&) { lanzada = true; }
    TEST(lanzada);                          // faltan operandos

    lanzada = false;
    try { (void)evaluarRPN("1 2 3 +"); } catch (const std::invalid_argument&) { lanzada = true; }
    TEST(lanzada);                          // sobran operandos

    lanzada = false;
    try { (void)evaluarRPN("4 0 /"); } catch (const std::invalid_argument&) { lanzada = true; }
    TEST(lanzada);                          // división entre cero

    lanzada = false;
    try { (void)evaluarRPN("2 x +"); } catch (const std::invalid_argument&) { lanzada = true; }
    TEST(lanzada);                          // token no válido
}

// -----------------------------------------------------------------------
int main() {
    std::println("=== Tests: std::stack ===\n");

    test_operaciones_basicas();
    test_asserts_enunciado();
    test_parentesis_balanceados();
    test_rpn();

    return resumen();
}
