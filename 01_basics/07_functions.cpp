/*
================================================================================
  LESSON 7: Functions — Complete Guide
================================================================================

  THEORY:
  A function is a NAMED, REUSABLE block of code that performs a specific task.

  WHY FUNCTIONS?
  1. DRY (Don't Repeat Yourself) — write once, call many times
  2. Abstraction — hide complexity behind a name
  3. Testability — test each function independently
  4. Readability — code reads like English: calculate_tax(), send_email()

  FUNCTION ANATOMY:
  return_type  name  (parameter_list)  {
      body
      return value;
  }

  CALL STACK:
  When you call a function, the CPU:
  1. Saves current position (return address) on the STACK
  2. Allocates space for function's local variables on the stack
  3. Jumps to function code
  4. Executes function body
  5. Returns value (if any)
  6. Pops stack frame, returns to saved position

  PARAMETER PASSING MODES:
  By VALUE:     function gets a COPY — original unchanged
  By REFERENCE: function gets the ACTUAL variable — can modify original
  By POINTER:   function gets address — can modify original via dereference
  By const ref: read-only access to original — no copy, no modification
================================================================================
*/

#include <iostream>
#include <string>
#include <vector>

// =========================================================================
// BASIC FUNCTIONS
// =========================================================================

void print_separator(char c = '-', int width = 40) {
    for (int i = 0; i < width; i++) std::cout << c;
    std::cout << std::endl;
}

int add(int a, int b) { return a + b; }

double circle_area(double radius) {
    const double PI = 3.14159265358979;
    return PI * radius * radius;
}

// =========================================================================
// PASS BY VALUE vs REFERENCE vs POINTER
// =========================================================================

void double_by_value(int x) {
    x *= 2;
    std::cout << "  Inside by_value: x = " << x << std::endl;
}

void double_by_reference(int& x) {
    x *= 2;
    std::cout << "  Inside by_ref: x = " << x << std::endl;
}

void double_by_pointer(int* x) {
    *x *= 2;
    std::cout << "  Inside by_ptr: *x = " << *x << std::endl;
}

void print_vector(const std::vector<int>& v) {
    for (int x : v) std::cout << x << " ";
    std::cout << std::endl;
}

void swap_values(int& a, int& b) {
    int temp = a; a = b; b = temp;
}

// =========================================================================
// DEFAULT PARAMETERS
// =========================================================================

void greet(std::string name, std::string greeting = "Hello", char punct = '!') {
    std::cout << greeting << ", " << name << punct << std::endl;
}

double power(double base, int exp = 2) {
    double result = 1.0;
    for (int i = 0; i < exp; i++) result *= base;
    return result;
}

// =========================================================================
// FUNCTION OVERLOADING
// =========================================================================

int    max_val(int a, int b)       { return a > b ? a : b; }
double max_val(double a, double b) { return a > b ? a : b; }
int    max_val(int a, int b, int c){ return max_val(max_val(a, b), c); }

void print_type(int x)         { std::cout << "int: "    << x << std::endl; }
void print_type(double x)      { std::cout << "double: " << x << std::endl; }
void print_type(std::string x) { std::cout << "string: " << x << std::endl; }

// =========================================================================
// RECURSION
// =========================================================================

// Factorial: n! = n * (n-1) * ... * 1,  0! = 1
// Call stack for factorial(4):
//   factorial(4) → 4 * factorial(3)
//     factorial(3) → 3 * factorial(2)
//       factorial(2) → 2 * factorial(1)
//         factorial(1) → 1 * factorial(0)
//           factorial(0) → returns 1  (BASE CASE)
//         returns 1*1 = 1
//       returns 2*1 = 2
//     returns 3*2 = 6
//   returns 4*6 = 24
long long factorial(int n) {
    if (n <= 0) return 1;        // base case — stops recursion
    return n * factorial(n - 1); // recursive case
}

// Fibonacci: F(n) = F(n-1) + F(n-2),  F(0)=0, F(1)=1
int fibonacci(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// Sum of digits: sumDigits(123) = 1+2+3 = 6
int sum_digits(int n) {
    if (n < 10) return n;
    return (n % 10) + sum_digits(n / 10);
}

// =========================================================================
// INLINE FUNCTIONS
// =========================================================================

inline int square(int x)    { return x * x; }
inline bool is_even(int x)  { return x % 2 == 0; }

// =========================================================================
// FUNCTION POINTERS
// =========================================================================

int add_fn(int a, int b)      { return a + b; }
int subtract_fn(int a, int b) { return a - b; }
int multiply_fn(int a, int b) { return a * b; }

int apply(int a, int b, int (*operation)(int, int)) {
    return operation(a, b);
}

// =========================================================================
// FORWARD DECLARATION (define after main)
// =========================================================================

bool is_prime(int n);
std::string repeat_string(std::string s, int times);

// =========================================================================
// MAIN
// =========================================================================

int main() {

    print_separator('=', 50);
    std::cout << "FUNCTIONS DEMO" << std::endl;
    print_separator('=', 50);

    // --- Basic calls ---
    std::cout << "\n--- Basic ---" << std::endl;
    std::cout << "add(3, 7) = " << add(3, 7) << std::endl;
    std::cout << "circle_area(5) = " << circle_area(5) << std::endl;
    print_separator();
    print_separator('*');
    print_separator('#', 20);

    // --- Parameter passing ---
    std::cout << "\n--- Parameter passing ---" << std::endl;
    int num = 10;
    std::cout << "Before: num = " << num << std::endl;

    double_by_value(num);
    std::cout << "After by_value: num = " << num << std::endl;  // still 10!

    double_by_reference(num);
    std::cout << "After by_ref: num = " << num << std::endl;    // now 20!

    double_by_pointer(&num);
    std::cout << "After by_ptr: num = " << num << std::endl;    // now 40!

    int a = 5, b = 8;
    std::cout << "Before swap: a=" << a << " b=" << b << std::endl;
    swap_values(a, b);
    std::cout << "After swap:  a=" << a << " b=" << b << std::endl;

    std::vector<int> v = {1, 2, 3, 4, 5};
    std::cout << "Vector: ";
    print_vector(v);

    // --- Default parameters ---
    std::cout << "\n--- Default parameters ---" << std::endl;
    greet("Alice");
    greet("Bob", "Hi");
    greet("Charlie", "Hey", '.');
    std::cout << "power(3)    = " << power(3)    << std::endl;
    std::cout << "power(2, 8) = " << power(2, 8) << std::endl;

    // --- Overloading ---
    std::cout << "\n--- Overloading ---" << std::endl;
    std::cout << "max_val(3, 7)     = " << max_val(3, 7)     << std::endl;
    std::cout << "max_val(3.5, 2.1) = " << max_val(3.5, 2.1) << std::endl;
    std::cout << "max_val(3, 7, 5)  = " << max_val(3, 7, 5)  << std::endl;
    print_type(42);
    print_type(3.14);
    print_type(std::string("hello"));

    // --- Recursion ---
    std::cout << "\n--- Recursion (factorial) ---" << std::endl;
    for (int i = 0; i <= 10; i++) {
        std::cout << i << "! = " << factorial(i) << std::endl;
    }

    std::cout << "\nFibonacci: ";
    for (int i = 0; i <= 10; i++) std::cout << fibonacci(i) << " ";
    std::cout << std::endl;

    std::cout << "sum_digits(12345) = " << sum_digits(12345) << std::endl;

    // --- Inline ---
    std::cout << "\n--- Inline ---" << std::endl;
    std::cout << "square(7) = " << square(7) << std::endl;
    std::cout << "is_even(4) = " << is_even(4) << std::endl;

    // --- Function pointers ---
    std::cout << "\n--- Function pointers ---" << std::endl;
    int (*op)(int, int) = add_fn;
    std::cout << "add(5, 3) = " << op(5, 3) << std::endl;
    op = subtract_fn;
    std::cout << "sub(5, 3) = " << op(5, 3) << std::endl;
    std::cout << "apply(10, 4, multiply) = " << apply(10, 4, multiply_fn) << std::endl;

    int (*ops[])(int, int) = {add_fn, subtract_fn, multiply_fn};
    std::string op_names[] = {"add", "sub", "mul"};
    for (int i = 0; i < 3; i++) {
        std::cout << op_names[i] << "(6, 2) = " << ops[i](6, 2) << std::endl;
    }

    // --- Forward declared ---
    std::cout << "\n--- Forward declared ---" << std::endl;
    std::cout << "is_prime(17) = " << is_prime(17) << std::endl;
    std::cout << "is_prime(18) = " << is_prime(18) << std::endl;
    std::cout << repeat_string("ab", 4) << std::endl;

    return 0;
}

bool is_prime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

std::string repeat_string(std::string s, int times) {
    std::string result = "";
    for (int i = 0; i < times; i++) result += s;
    return result;
}
