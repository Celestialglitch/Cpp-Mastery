/*
================================================================================
  LESSON 17: Templates — Generic Programming
================================================================================

  THEORY:
  -------
  Templates let you write code that works with ANY type.
  The compiler generates type-specific code at COMPILE TIME.
  This is called COMPILE-TIME POLYMORPHISM (vs runtime polymorphism with virtual).

  HOW IT WORKS:
  -------------
  When you write: template<typename T> T max(T a, T b) { ... }
  And call:       max(3, 7)
  The compiler generates: int max(int a, int b) { ... }
  And for:        max(3.5, 2.1)
  It generates:   double max(double a, double b) { ... }
  This is called TEMPLATE INSTANTIATION.

  TEMPLATE vs MACRO:
  ------------------
  Macros (#define) do text substitution — no type safety.
  Templates are type-safe and checked by the compiler.

  TWO KINDS:
  ----------
  Function templates: template<typename T> T func(T a) { ... }
  Class templates:    template<typename T> class MyClass { ... }

  CONCEPTS (C++20):
  -----------------
  Constraints on template parameters — specify what operations T must support.
  Gives better error messages than raw templates.

================================================================================
*/

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <type_traits>  // for type traits (C++11)
// NOTE: <concepts> is C++20. If your compiler only supports C++17, comment this out.
// #include <concepts>

// =========================================================================
// SECTION 1: FUNCTION TEMPLATES
// =========================================================================

// Basic function template:
template <typename T>
T max_val(T a, T b) {
    return (a > b) ? a : b;
}

// Multiple type parameters:
template <typename T, typename U>
void print_pair(const T& first, const U& second) {
    std::cout << "(" << first << ", " << second << ")" << std::endl;
}

// Template with non-type parameter:
template <typename T, int N>
void fill_and_print(T value) {
    T arr[N];
    for (int i = 0; i < N; i++) arr[i] = value;
    std::cout << "Array of " << N << " " << value << "s: ";
    for (int i = 0; i < N; i++) std::cout << arr[i] << " ";
    std::cout << std::endl;
}

// Template function for printing any container:
template <typename Container>
void print_container(const Container& c, const std::string& label = "") {
    if (!label.empty()) std::cout << label << ": ";
    for (const auto& elem : c) std::cout << elem << " ";
    std::cout << std::endl;
}

// Template swap:
template <typename T>
void my_swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

// =========================================================================
// SECTION 2: TEMPLATE SPECIALIZATION
// =========================================================================

// Primary template:
template <typename T>
std::string type_name() { return "unknown"; }

// Full specializations for specific types:
template <> std::string type_name<int>()         { return "int"; }
template <> std::string type_name<double>()      { return "double"; }
template <> std::string type_name<std::string>() { return "string"; }
template <> std::string type_name<bool>()        { return "bool"; }

// Specialization for behavior:
template <typename T>
void describe(T val) {
    std::cout << "Value: " << val << " (type: " << type_name<T>() << ")" << std::endl;
}

template <>
void describe<bool>(bool val) {
    std::cout << "Boolean: " << (val ? "TRUE" : "FALSE") << std::endl;
}

template <>
void describe<std::string>(std::string val) {
    std::cout << "String of length " << val.length() << ": \"" << val << "\"" << std::endl;
}

// =========================================================================
// SECTION 3: CLASS TEMPLATES
// =========================================================================

// Generic Stack:
template <typename T>
class Stack {
private:
    std::vector<T> data;
    size_t max_size;

public:
    Stack(size_t max = 100) : max_size(max) {}

    void push(const T& value) {
        if (data.size() >= max_size) {
            throw std::overflow_error("Stack overflow!");
        }
        data.push_back(value);
    }

    T pop() {
        if (data.empty()) throw std::underflow_error("Stack underflow!");
        T top = data.back();
        data.pop_back();
        return top;
    }

    const T& top() const {
        if (data.empty()) throw std::underflow_error("Stack is empty!");
        return data.back();
    }

    bool empty() const { return data.empty(); }
    size_t size() const { return data.size(); }

    void print() const {
        std::cout << "Stack [top→bottom]: ";
        for (int i = data.size()-1; i >= 0; i--) std::cout << data[i] << " ";
        std::cout << std::endl;
    }
};

// Generic Pair:
template <typename First, typename Second>
class Pair {
public:
    First  first;
    Second second;

    Pair(First f, Second s) : first(f), second(s) {}

    void swap() { my_swap(first, second); }  // only works if First == Second

    friend std::ostream& operator<<(std::ostream& os, const Pair& p) {
        os << "(" << p.first << ", " << p.second << ")";
        return os;
    }
};

// Generic Matrix:
template <typename T, int ROWS, int COLS>
class Matrix {
private:
    T data[ROWS][COLS];

public:
    Matrix() {
        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLS; j++)
                data[i][j] = T{};  // zero-initialize
    }

    T& at(int r, int c) { return data[r][c]; }
    const T& at(int r, int c) const { return data[r][c]; }

    void print() const {
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                std::cout.width(6);
                std::cout << data[i][j];
            }
            std::cout << std::endl;
        }
    }

    Matrix<T, ROWS, COLS> operator+(const Matrix& other) const {
        Matrix result;
        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLS; j++)
                result.data[i][j] = data[i][j] + other.data[i][j];
        return result;
    }
};

// =========================================================================
// SECTION 4: VARIADIC TEMPLATES
// =========================================================================

// Base case (empty parameter pack):
void print_all() { std::cout << std::endl; }

// Recursive case:
template <typename T, typename... Args>
void print_all(T first, Args... rest) {
    std::cout << first;
    if (sizeof...(rest) > 0) std::cout << ", ";
    print_all(rest...);
}

// Sum of any number of arguments:
template <typename T>
T sum(T val) { return val; }

template <typename T, typename... Args>
T sum(T first, Args... rest) {
    return first + sum(rest...);
}

// =========================================================================
// SECTION 5: TYPE TRAITS
// =========================================================================

template <typename T>
void analyze_type() {
    std::cout << "Type analysis:" << std::endl;
    std::cout << "  is_integral:   " << std::is_integral<T>::value    << std::endl;
    std::cout << "  is_floating:   " << std::is_floating_point<T>::value << std::endl;
    std::cout << "  is_pointer:    " << std::is_pointer<T>::value     << std::endl;
    std::cout << "  is_class:      " << std::is_class<T>::value       << std::endl;
    std::cout << "  sizeof:        " << sizeof(T) << " bytes"         << std::endl;
}

// Conditional behavior based on type:
template <typename T>
void safe_print(T val) {
    if constexpr (std::is_floating_point<T>::value) {
        std::cout << std::fixed;
        std::cout.precision(4);
        std::cout << "float: " << val << std::endl;
        std::cout.unsetf(std::ios::fixed);
    } else if constexpr (std::is_integral<T>::value) {
        std::cout << "int: " << val << " (hex: " << std::hex << val << std::dec << ")" << std::endl;
    } else {
        std::cout << "other: " << val << std::endl;
    }
}

// =========================================================================
// MAIN
// =========================================================================

int main() {

    std::cout << "=== FUNCTION TEMPLATES ===" << std::endl;

    // Compiler deduces T from arguments:
    std::cout << "max_val(3, 7)       = " << max_val(3, 7)       << std::endl;
    std::cout << "max_val(3.5, 2.1)   = " << max_val(3.5, 2.1)   << std::endl;
    std::cout << "max_val('a', 'z')   = " << max_val('a', 'z')   << std::endl;
    std::cout << "max_val(\"ab\",\"cd\") = " << max_val(std::string("ab"), std::string("cd")) << std::endl;

    // Explicit type:
    std::cout << "max_val<double>(3, 7) = " << max_val<double>(3, 7) << std::endl;

    print_pair(42, "hello");
    print_pair(3.14, true);
    print_pair(std::string("name"), 100);

    fill_and_print<int, 5>(42);
    fill_and_print<char, 8>('*');

    std::vector<int> v = {1, 2, 3, 4, 5};
    std::string str = "hello";
    print_container(v, "vector");
    print_container(str, "string");

    int a = 5, b = 10;
    my_swap(a, b);
    std::cout << "After swap: a=" << a << " b=" << b << std::endl;

    std::cout << "\n=== SPECIALIZATION ===" << std::endl;
    describe(42);
    describe(3.14);
    describe(true);
    describe(std::string("hello world"));

    std::cout << "\n=== CLASS TEMPLATES ===" << std::endl;

    // Stack<int>:
    Stack<int> int_stack;
    int_stack.push(10);
    int_stack.push(20);
    int_stack.push(30);
    int_stack.print();
    std::cout << "Pop: " << int_stack.pop() << std::endl;
    int_stack.print();

    // Stack<string>:
    Stack<std::string> str_stack;
    str_stack.push("hello");
    str_stack.push("world");
    str_stack.push("cpp");
    str_stack.print();

    // Pair:
    Pair<std::string, int> person("Alice", 30);
    std::cout << "Person: " << person << std::endl;

    Pair<int, int> coords(3, 7);
    std::cout << "Coords: " << coords << std::endl;

    // Matrix:
    Matrix<int, 3, 3> m1, m2;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) {
            m1.at(i, j) = i * 3 + j + 1;
            m2.at(i, j) = (i == j) ? 1 : 0;  // identity matrix
        }

    std::cout << "Matrix m1:" << std::endl;
    m1.print();
    std::cout << "Matrix m2 (identity):" << std::endl;
    m2.print();
    std::cout << "m1 + m2:" << std::endl;
    (m1 + m2).print();

    std::cout << "\n=== VARIADIC TEMPLATES ===" << std::endl;
    std::cout << "print_all: ";
    print_all(1, 2.5, "three", true, 'x');

    std::cout << "sum(1,2,3,4,5) = " << sum(1, 2, 3, 4, 5) << std::endl;
    std::cout << "sum(1.1,2.2,3.3) = " << sum(1.1, 2.2, 3.3) << std::endl;

    std::cout << "\n=== TYPE TRAITS ===" << std::endl;
    std::cout << "int:" << std::endl;
    analyze_type<int>();
    std::cout << "double:" << std::endl;
    analyze_type<double>();
    std::cout << "std::string:" << std::endl;
    analyze_type<std::string>();

    std::cout << "\n=== if constexpr ===" << std::endl;
    safe_print(255);
    safe_print(3.14159);
    safe_print(std::string("hello"));

    return 0;
}
