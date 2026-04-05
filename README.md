# C++ Mastery — Learn by Reading and Running

Each file is a self-contained lesson. Read the theory at the top, then trace
through the execution flow in main(). Compile and run each one.

## How to compile any file

```
g++ -std=c++17 filename.cpp -o output && ./output
```

For concurrency (lesson 20):

```
g++ -std=c++17 20_concurrency.cpp -o concurrency -lpthread
```

## Lesson Map

### 01_basics/

| File                       | Topic                                                            |
| -------------------------- | ---------------------------------------------------------------- |
| 01_hello_world.cpp         | Compilation pipeline, main(), cout, cerr                         |
| 02_variables_and_types.cpp | All types, memory sizes, auto, scope, casting                    |
| 03_operators.cpp           | Arithmetic, bitwise, logical, precedence                         |
| 04_input_output.cpp        | cin, cout, formatting, iomanip, validation                       |
| 05_conditionals.cpp        | if/else, switch, ternary, truthy/falsy                           |
| 06_loops.cpp               | for, while, do-while, break, continue, patterns                  |
| 07_functions.cpp           | Pass by value/ref/ptr, recursion, overloading, function pointers |
| 08_arrays_and_strings.cpp  | Arrays, 2D arrays, std::string, manipulation                     |

### 02_pointers_memory/

| File                     | Topic                                                        |
| ------------------------ | ------------------------------------------------------------ |
| 09_pointers.cpp          | Addresses, dereferencing, pointer arithmetic, const pointers |
| 10_memory_management.cpp | Stack vs heap, new/delete, smart pointers, RAII              |

### 03_oop/

| File                  | Topic                                                                   |
| --------------------- | ----------------------------------------------------------------------- |
| 11_classes_basics.cpp | Classes, constructors, destructors, encapsulation, operator overloading |
| 12_inheritance.cpp    | Inheritance, virtual functions, polymorphism, abstract classes          |
| 13_smart_pointers.cpp | unique_ptr, shared_ptr, weak_ptr, custom deleters                       |

### 04_stl/

| File             | Topic                                              |
| ---------------- | -------------------------------------------------- |
| 14_vectors.cpp   | vector internals, algorithms, 2D vectors           |
| 15_maps_sets.cpp | map, unordered_map, set, stack, queue, deque, list |

### 05_modern_cpp/

| File             | Topic                                                           |
| ---------------- | --------------------------------------------------------------- |
| 16_lambdas.cpp   | Lambdas, closures, captures, std::function, structured bindings |
| 17_templates.cpp | Function/class templates, specialization, variadic, type traits |

### 06_advanced/

| File               | Topic                                                    |
| ------------------ | -------------------------------------------------------- |
| 18_file_io.cpp     | ifstream, ofstream, CSV, binary, string streams          |
| 19_exceptions.cpp  | try/catch, custom exceptions, RAII, noexcept             |
| 20_concurrency.cpp | threads, mutex, atomic, condition_variable, async/future |
