cmake_minimum_required(VERSION 3.28)

project(actividad4_listas_pilas_colas
   VERSION 0.1.0
   DESCRIPTION "Actividad 4: listas (std::vector), pilas (std::stack) y colas (std::queue)"
   LANGUAGES CXX
)

enable_testing()

#-------------------------------------------------------------------
# ejecutables (cada parte se añade solo si sus ficheros ya existen,
# así la rama main compila aunque todavía no estén todas las partes):
#   vector, pila, cola                    → programas de src/
#   test_vector, test_pila, test_cola     → tests unitarios de tests/

set(TARGETS)

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/vector.cpp)
   add_executable(vector src/vector.cpp)
   add_executable(test_vector tests/test_vector.cpp)
   add_test(NAME test_vector COMMAND test_vector)
   list(APPEND TARGETS vector test_vector)
endif()

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/pila_rpn.cpp)
   add_executable(pila src/pila_rpn.cpp)
   add_executable(test_pila tests/test_pila.cpp)
   add_test(NAME test_pila COMMAND test_pila)
   list(APPEND TARGETS pila test_pila)
endif()

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/cola.cpp)
   add_executable(cola src/cola.cpp)
   add_executable(test_cola tests/test_cola.cpp)
   add_test(NAME test_cola COMMAND test_cola)
   list(APPEND TARGETS cola test_cola)
endif()

foreach(TARGET ${TARGETS})
   target_compile_features(${TARGET} PRIVATE cxx_std_23)

   set_target_properties(${TARGET} PROPERTIES
      CXX_EXTENSIONS OFF
      RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/debug
      RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/release
   )

   #----------------------------------------------------------------
   # política de avisos (y std::print / std::println en GCC):

   if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
      target_compile_options(${TARGET} PRIVATE /W3 /WX)
   elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
      target_compile_options(${TARGET} PRIVATE -Wall -Wextra -pedantic)
      target_link_libraries(${TARGET} PRIVATE -lstdc++exp)
   endif()
endforeach()

#-------------------------------------------------------------------
# integración de bibliotecas de terceros:

#find_package(nlohmann_json REQUIRED)
#target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json)

#-------------------------------------------------------------------
# mensajes a emitir en construcción:

message("-- CMake Generator: ${CMAKE_GENERATOR}")
message("-- Ejecutables: ${TARGETS}")

if (TARGETS)
   list(GET TARGETS 0 PRIMERO)

   get_target_property(CFEATURES ${PRIMERO} COMPILE_FEATURES)
   message("-- Target compile features: ${CFEATURES}")

   get_target_property(COPTIONS ${PRIMERO} COMPILE_OPTIONS)
   message("-- Target compile options: ${COPTIONS}")
endif()
