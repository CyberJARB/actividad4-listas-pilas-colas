/**
 * pila_rpn.cpp — Parte 2: Pila (std::stack)
 *
 * 2.1  Operaciones básicas: push / top / pop hasta vaciar la pila.
 * 2.2  Verificación de paréntesis balanceados usando una pila de chars.
 * RPN  Evaluación de expresiones en Notación Polaca Inversa (Reverse Polish
 *      Notation) mostrando el estado de la pila tras cada token.
 *
 * La pila sigue el principio LIFO: el último en entrar es el primero en salir.
 * push(), pop() y top() son O(1).
 *
 * Compilar:
 *   g++ -std=c++23 -o pila pila_rpn.cpp -lstdc++exp
 */
#include <stack>
#include <string>
#include <sstream>
#include <stdexcept>
#include <print>

// -----------------------------------------------------------------------
// Parte 2.2 — Comprueba si los paréntesis de 'texto' están balanceados.
//
// Algoritmo:
//   - '(' → apilar (push)
//   - ')' → si pila vacía: error; si no: desapilar (pop)
//   - Al final: pila vacía = correcto, pila no vacía = incorrecto
// Coste: O(n), cada carácter se procesa una vez con operaciones O(1).
// -----------------------------------------------------------------------
bool balanceado(const std::string& texto) {

    std::stack<char> pila;

    for (char c : texto) {

        if (c == '(') {
            pila.push(c);               // paréntesis abierto pendiente de cerrar
        }
        else if (c == ')') {

            if (pila.empty()) {
                return false;           // cierre sin apertura correspondiente
            }

            pila.pop();                 // emparejamos con el último '(' apilado
        }
    }

    return pila.empty();                // vacía = todos los '(' tienen su ')'
}

// -----------------------------------------------------------------------
// Devuelve el contenido de la pila como texto, de la base a la cima,
// p. ej. "[ 3 4 ] <- cima". Se trabaja sobre una copia para no alterarla.
// -----------------------------------------------------------------------
std::string pilaComoTexto(std::stack<int> copia) {
    std::string elementos;
    while (!copia.empty()) {
        elementos = std::to_string(copia.top()) + " " + elementos;
        copia.pop();
    }
    return "[ " + elementos + "] <- cima";
}

// -----------------------------------------------------------------------
// RPN — Evalúa una expresión en notación polaca inversa con tokens separados
// por espacios, p. ej. "3 4 + 2 *" = (3 + 4) * 2 = 14.
//
// Algoritmo:
//   - Número   → push
//   - Operador → pop b, pop a, push (a op b)
//   - Al final debe quedar exactamente un valor: el resultado.
// Lanza std::invalid_argument si la expresión está mal formada o hay
// una división entre cero. Si 'traza' es true, muestra la pila tras cada token.
// Coste: O(n) en número de tokens.
// -----------------------------------------------------------------------
int evaluarRPN(const std::string& expresion, bool traza = false) {
    std::stack<int> pila;
    std::istringstream entrada(expresion);
    std::string token;

    while (entrada >> token) {
        bool esOperador = token.size() == 1 &&
                          (token[0] == '+' || token[0] == '-' ||
                           token[0] == '*' || token[0] == '/');

        if (esOperador) {
            if (pila.size() < 2) {
                throw std::invalid_argument("Faltan operandos para '" + token + "'");
            }
            int b = pila.top(); pila.pop();     // el operando derecho está en la cima
            int a = pila.top(); pila.pop();

            switch (token[0]) {
                case '+': pila.push(a + b); break;
                case '-': pila.push(a - b); break;
                case '*': pila.push(a * b); break;
                case '/':
                    if (b == 0) {
                        throw std::invalid_argument("Division entre cero");
                    }
                    pila.push(a / b);
                    break;
            }
        } else {
            try {
                std::size_t leidos = 0;
                int valor = std::stoi(token, &leidos);
                if (leidos != token.size()) {
                    throw std::invalid_argument("");
                }
                pila.push(valor);
            } catch (const std::logic_error&) {
                throw std::invalid_argument("Token no valido: '" + token + "'");
            }
        }

        if (traza) {
            std::println("    token {:>3}  →  pila {}", token, pilaComoTexto(pila));
        }
    }

    if (pila.size() != 1) {
        throw std::invalid_argument("Expresion RPN mal formada");
    }
    return pila.top();
}

// -----------------------------------------------------------------------
// Parte 2.1 — Operaciones básicas con std::stack<int>
// -----------------------------------------------------------------------
void demoOperacionesBasicas() {
    std::println("=== Parte 2.1: Operaciones básicas con std::stack ===\n");

    // 1. Crear una pila de enteros
    std::stack<int> pila;

    // 2. Añadir los elementos en este orden: 10, 20, 30, 40
    for (int valor : {10, 20, 30, 40}) {
        pila.push(valor);
        std::println("  push({})", valor);
    }

    // 3. Mostrar la cima  4. pop()  5. Volver a mostrar la cima
    // 6. Repetir hasta que la pila esté vacía
    std::println("\n  --- Vaciando la pila (LIFO) ---");
    while (!pila.empty()) {
        std::println("  cima (top) = {}  →  pop()", pila.top());
        pila.pop();
    }

    std::println("  Pila vacía: {}\n", pila.empty());
}

// -----------------------------------------------------------------------
// Parte 2.2 — Verificación de paréntesis balanceados
// -----------------------------------------------------------------------
void demoParentesisBalanceados() {
    std::println("=== Parte 2.2: Verificación de paréntesis balanceados ===\n");

    // Pares {expresión, resultado esperado} del enunciado
    const std::pair<std::string, bool> casos[] = {
        {"(())", true },
        {"(()",  false},
        {"())",  false},
        {"()()", true },
    };

    for (const auto& [expr, esperado] : casos) {
        bool resultado = balanceado(expr);
        std::println("  {:6} → {:10}  {}", expr,
                     resultado ? "correcto" : "incorrecto",
                     resultado == esperado ? "[OK]" : "[FALLO]");
    }
}

// -----------------------------------------------------------------------
// RPN — Evaluación con trazabilidad de la pila tras cada token
// -----------------------------------------------------------------------
void demoRPN() {
    std::println("\n=== RPN: evaluación de expresiones con pila ===");

    const std::string expresiones[] = {
        "3 4 + 2 *",          // (3 + 4) * 2        = 14
        "5 1 2 + 4 * + 3 -",  // 5 + (1 + 2) * 4 - 3 = 14
    };

    for (const auto& expr : expresiones) {
        std::println("\n  Expresión: \"{}\"", expr);
        int resultado = evaluarRPN(expr, true);
        std::println("  Resultado = {}", resultado);
    }

    // Expresión mal formada: se captura la excepción
    std::println("\n  Expresión: \"2 +\"");
    try {
        evaluarRPN("2 +");
    } catch (const std::invalid_argument& e) {
        std::println("  Error capturado: {}", e.what());
    }
}

#ifndef TESTING
int main() {
    demoOperacionesBasicas();
    demoParentesisBalanceados();
    demoRPN();
    return 0;
}
#endif
