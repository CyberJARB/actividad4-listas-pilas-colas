# Actividad 4 — Listas, Pilas y Colas · Peer Coding · Introducción a DevOps

Programación con Estructuras Lineales — Universidad Europea

## Estructura

```
actividad4/
├─ src/
│   ├─ vector.cpp      Parte 1: std::vector — inserciones y coste (+ recursiva vs iterativa)
│   ├─ pila_rpn.cpp    Parte 2: std::stack — operaciones básicas, paréntesis balanceados, RPN
│   ├─ cola.cpp        Parte 3: std::queue — cola de tareas FIFO
├─ tests/
│   ├─ test_vector.cpp
│   ├─ test_pila.cpp
│   ├─ test_cola.cpp
├─ CMakeLists.txt
└─ README.md
```

Los tests incluyen directamente el fichero de `src/` correspondiente con `TESTING`
definido (lo que excluye su `main`), de modo que prueban el código real y no una copia.

## Compilar y ejecutar

Requiere un compilador con C++23 (se usa `std::println`) y CMake ≥ 3.28.
Desde la carpeta `actividad4/`:

```bash
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build        # ejecuta los 3 tests unitarios
```

Los ejecutables quedan en `build/debug/` (o `build/release/` con `-DCMAKE_BUILD_TYPE=Release`).

Sin CMake, compilando a mano (con GCC 14/15 hay que enlazar `-lstdc++exp`):

```bash
# Programas
g++ -std=c++23 -Wall -Wextra -o vector src/vector.cpp   -lstdc++exp
g++ -std=c++23 -Wall -Wextra -o pila   src/pila_rpn.cpp -lstdc++exp
g++ -std=c++23 -Wall -Wextra -o cola   src/cola.cpp     -lstdc++exp

# Tests unitarios
g++ -std=c++23 -Wall -Wextra -o test_vector tests/test_vector.cpp -lstdc++exp && ./test_vector
g++ -std=c++23 -Wall -Wextra -o test_pila   tests/test_pila.cpp   -lstdc++exp && ./test_pila
g++ -std=c++23 -Wall -Wextra -o test_cola   tests/test_cola.cpp   -lstdc++exp && ./test_cola
```

Cada test termina con `EXITO: todos los tests han pasado correctamente.` y devuelve
código 0 (1 si falla alguno), lo que permite usarlos en validación automática.

## Parte 1 — Análisis de coste de las inserciones (n = 5)

Vector inicial: `{10, 20, 30, 40, 50}`. Cada inserción se analiza sobre el vector original.

| Operación                  | Desplazados | Coste exacto (n = 5)            | Complejidad     |
|----------------------------|:-----------:|---------------------------------|-----------------|
| Insertar 5 al principio    | 5           | 5 desplazamientos + 1 escritura | O(n)            |
| Insertar 35 en posición 3  | 2 (40, 50)  | 2 desplazamientos + 1 escritura | O(n)            |
| Insertar 60 al final       | 0           | 0 desplazamientos + 1 escritura | O(1) amortizado |

En general, insertar en la posición `p` desplaza `n − p` elementos. Al final es O(1)
*amortizado*: cuando `size == capacity` el vector reserva más memoria y copia los `n`
elementos, pero como la capacidad crece de forma geométrica ese coste repartido entre
muchas inserciones es constante.

Si las tres inserciones se aplican **en secuencia** sobre el mismo vector, `n` crece
(5 → 6 → 7) y los desplazamientos son 5, 3 y 0. Resultado final:
`{5, 10, 20, 35, 30, 40, 50, 60}`.

## Análisis de eficiencia

| Estructura    | Operación                  | Coste           | Motivo                                        |
|---------------|----------------------------|-----------------|-----------------------------------------------|
| `std::vector` | acceso `v[i]`              | O(1)            | memoria contigua                              |
| `std::vector` | insertar/borrar al inicio  | O(n)            | hay que desplazar todos los elementos         |
| `std::vector` | insertar/borrar en medio   | O(n)            | se desplazan los `n − p` elementos siguientes |
| `std::vector` | `push_back` / `pop_back`   | O(1) amortizado | no hay desplazamientos                        |
| `std::stack`  | `push` / `pop` / `top`     | O(1)            | solo se opera sobre la cima                   |
| `std::queue`  | `push` / `pop` / `front`   | O(1)            | se inserta al final y se extrae del frente    |
| Paréntesis    | `balanceado(texto)`        | O(n)            | un recorrido, operaciones O(1) por carácter   |
| RPN           | `evaluarRPN(expresion)`    | O(n)            | un recorrido, operaciones O(1) por token      |

**Recursiva vs iterativa** (`sumaRecursiva` / `sumaIterativa` en `vector.cpp`):
ambas son O(n) en tiempo; la recursiva usa O(n) de memoria (una llamada en la pila
del programa por elemento) y la iterativa O(1).

## Parte 2 — Pila

- **2.1** Se apilan 10, 20, 30, 40 y se muestra la cima y se desapila hasta vaciarla
  (orden de salida: 40, 30, 20, 10 → LIFO).
- **2.2** `balanceado()` sigue exactamente el algoritmo del enunciado: `'('` → push;
  `')'` → si la pila está vacía, `false`; si no, pop; al final, correcto si la pila
  está vacía.
- **RPN** `evaluarRPN()` evalúa expresiones en notación polaca inversa (número → push;
  operador → pop b, pop a, push a∘b) y muestra el estado de la pila tras cada token.
  Lanza `std::invalid_argument` si faltan/sobran operandos, hay un token no válido o
  una división entre cero.

## Parte 3 — Cola

Se encolan las tareas 10, 20, 30, 40 y, mientras la cola no esté vacía, se muestra
`front()` y se hace `pop()`. Se procesan en el mismo orden de llegada (FIFO) y la cola
termina vacía.

## Organización del trabajo

Autor: Jose Andres Ruiz Bautista (224H1795).

La actividad está planteada en pareja, pero la he realizado de forma individual
asumiendo los dos roles en cada parte: primero como programador (código en `src/`) y
después como tester (tests en `tests/` y revisión del enunciado y del análisis de costes).

## Flujo de trabajo con Git

1. Cada parte se desarrolla en su propia rama: `parte1-vector`, `parte2-pila`, `parte3-cola`.
2. Al terminarla se abre un Pull Request hacia `main`.
3. En el papel de tester se compila la rama, se ejecuta `ctest` y se revisan los cambios.
4. Solo se fusiona en `main` si todos los tests terminan con `EXITO`.
