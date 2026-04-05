/*
================================================================================
  LESSON 16: Lambdas, auto, and Modern C++ Features
================================================================================

  THEORY — LAMBDAS:
  -----------------
  A lambda is an ANONYMOUS FUNCTION defined inline where it's used.
  It's syntactic sugar for a compiler-generated functor (function object).

  SYNTAX:
  -------
  [capture](parameters) -> return_type { body }

  CAPTURE CLAUSE:
  ---------------
  []        — capture nothing
  [x]       — capture x by value (copy)
  [&x]      — capture x by reference
  [=]       — capture ALL used variables by value
  [&]       — capture ALL used variables by reference
  [=, &x]   — capture all by value, but x by reference
  [this]    — capture the current object (in member functions)

  WHAT HAPPENS INTERNALLY:
  ------------------------
  The compiler generates a class with:
  - Captured variables as data members
  - operator() implementing the lambda body
  So [x](int y){ return x+y; } becomes roughly:
  struct Lambda { int x; int operator()(int y){ return x+y; } };

  CLOSURES:
  ---------
  A lambda + its captured environment = a CLOSURE.
  The closure "closes over" variables from the surrounding scope.

================================================================================
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include <map>
#include <numeric>
#include <optional>

int main() {

    // =========================================================================
    // SECTION 1: BASIC LAMBDA SYNTAX
    // =========================================================================

    std::cout << "=== BASIC LAMBDAS ===" << std::endl;

    // Simplest lambda — no capture, no params, no return:
    auto say_hello = []() {
        std::cout << "Hello from lambda!" << std::endl;
    };
    say_hello();  // call it like a function

    // With parameters:
    auto add = [](int a, int b) {
        return a + b;  // return type deduced as int
    };
    std::cout << "add(3, 4) = " << add(3, 4) << std::endl;

    // Explicit return type:
    auto divide = [](double a, double b) -> double {
        if (b == 0) return 0;
        return a / b;
    };
    std::cout << "divide(10, 3) = " << divide(10, 3) << std::endl;

    // Immediately invoked lambda (IIFE):
    int result = [](int x, int y) { return x * y; }(5, 6);
    std::cout << "IIFE result = " << result << std::endl;

    // Lambda stored in auto:
    auto greet = [](std::string name) {
        std::cout << "Hello, " << name << "!" << std::endl;
    };
    greet("Alice");
    greet("Bob");

    // =========================================================================
    // SECTION 2: CAPTURE CLAUSE
    // =========================================================================

    std::cout << "\n=== CAPTURE ===" << std::endl;

    int x = 10, y = 20;
    std::string prefix = "Value: ";

    // Capture by VALUE (copy at time of lambda creation):
    auto capture_val = [x, y, prefix]() {
        std::cout << prefix << x << " and " << y << std::endl;
        // x = 99;  // ERROR! captured by value — read-only
    };
    x = 999;  // change x AFTER lambda creation
    capture_val();  // still prints 10 (captured the old value!)
    x = 10;  // reset

    // Capture by REFERENCE (live reference to original):
    auto capture_ref = [&x, &y]() {
        x += 5;  // modifies the original x!
        y += 5;
        std::cout << "x=" << x << " y=" << y << std::endl;
    };
    capture_ref();
    std::cout << "After lambda: x=" << x << " y=" << y << std::endl;  // 15, 25

    // Capture ALL by value:
    auto all_val = [=]() {
        std::cout << "All by val: x=" << x << " y=" << y << std::endl;
    };
    all_val();

    // Capture ALL by reference:
    auto all_ref = [&]() {
        x = 100; y = 200;
    };
    all_ref();
    std::cout << "After all_ref: x=" << x << " y=" << y << std::endl;

    // mutable lambda — allows modifying value-captured variables:
    int counter = 0;
    auto increment = [counter]() mutable {
        counter++;  // modifies the COPY, not the original
        std::cout << "Inside: counter=" << counter << std::endl;
    };
    increment();  // Inside: counter=1
    increment();  // Inside: counter=2
    std::cout << "Outside: counter=" << counter << std::endl;  // still 0!

    // =========================================================================
    // SECTION 3: LAMBDAS WITH STL ALGORITHMS
    // =========================================================================

    std::cout << "\n=== LAMBDAS WITH STL ===" << std::endl;

    std::vector<int> nums = {5, 2, 8, 1, 9, 3, 7, 4, 6};

    // sort with custom comparator:
    std::sort(nums.begin(), nums.end(), [](int a, int b) { return a < b; });
    std::cout << "Sorted: ";
    for (int n : nums) std::cout << n << " ";
    std::cout << std::endl;

    // sort descending:
    std::sort(nums.begin(), nums.end(), [](int a, int b) { return a > b; });
    std::cout << "Desc:   ";
    for (int n : nums) std::cout << n << " ";
    std::cout << std::endl;

    // for_each:
    std::cout << "Doubled: ";
    std::for_each(nums.begin(), nums.end(), [](int n) {
        std::cout << n * 2 << " ";
    });
    std::cout << std::endl;

    // find_if:
    auto it = std::find_if(nums.begin(), nums.end(), [](int n) { return n > 5; });
    if (it != nums.end()) std::cout << "First > 5: " << *it << std::endl;

    // count_if:
    int even_count = std::count_if(nums.begin(), nums.end(), [](int n) { return n % 2 == 0; });
    std::cout << "Even count: " << even_count << std::endl;

    // transform:
    std::vector<int> squares(nums.size());
    std::transform(nums.begin(), nums.end(), squares.begin(), [](int n) { return n * n; });
    std::cout << "Squares: ";
    for (int n : squares) std::cout << n << " ";
    std::cout << std::endl;

    // remove_if (erase-remove idiom):
    nums.erase(
        std::remove_if(nums.begin(), nums.end(), [](int n) { return n < 4; }),
        nums.end()
    );
    std::cout << "After remove < 4: ";
    for (int n : nums) std::cout << n << " ";
    std::cout << std::endl;

    // accumulate with lambda:
    std::vector<int> data = {1, 2, 3, 4, 5};
    int product = std::accumulate(data.begin(), data.end(), 1, [](int acc, int x) {
        return acc * x;
    });
    std::cout << "Product: " << product << std::endl;  // 120

    // Sort vector of structs:
    struct Person { std::string name; int age; };
    std::vector<Person> people = {{"Alice", 30}, {"Bob", 25}, {"Charlie", 35}};
    std::sort(people.begin(), people.end(), [](const Person& a, const Person& b) {
        return a.age < b.age;  // sort by age
    });
    std::cout << "Sorted by age: ";
    for (const auto& p : people) std::cout << p.name << "(" << p.age << ") ";
    std::cout << std::endl;

    // =========================================================================
    // SECTION 4: std::function — store any callable
    // =========================================================================

    std::cout << "\n=== std::function ===" << std::endl;

    // std::function<return_type(param_types)> can hold:
    // - lambdas
    // - regular functions
    // - function objects (functors)
    // - std::bind results

    std::function<int(int, int)> op;

    op = [](int a, int b) { return a + b; };
    std::cout << "add: " << op(3, 4) << std::endl;

    op = [](int a, int b) { return a * b; };
    std::cout << "mul: " << op(3, 4) << std::endl;

    // Map of operations:
    std::map<std::string, std::function<double(double, double)>> operations;
    operations["add"] = [](double a, double b) { return a + b; };
    operations["sub"] = [](double a, double b) { return a - b; };
    operations["mul"] = [](double a, double b) { return a * b; };
    operations["div"] = [](double a, double b) { return b != 0 ? a/b : 0; };

    for (const auto& [name, fn] : operations) {
        std::cout << name << "(10, 3) = " << fn(10, 3) << std::endl;
    }

    // Higher-order function (function that takes/returns functions):
    auto make_multiplier = [](int factor) {
        return [factor](int x) { return x * factor; };  // returns a lambda!
    };

    auto double_it = make_multiplier(2);
    auto triple_it = make_multiplier(3);

    std::cout << "double(5) = " << double_it(5) << std::endl;  // 10
    std::cout << "triple(5) = " << triple_it(5) << std::endl;  // 15

    // =========================================================================
    // SECTION 5: GENERIC LAMBDAS (C++14)
    // =========================================================================

    std::cout << "\n=== GENERIC LAMBDAS ===" << std::endl;

    // 'auto' parameters make lambda work with any type:
    auto print_any = [](auto val) {
        std::cout << val << std::endl;
    };
    print_any(42);
    print_any(3.14);
    print_any(std::string("hello"));
    print_any('A');

    auto max_any = [](auto a, auto b) {
        return a > b ? a : b;
    };
    std::cout << "max(3, 7) = " << max_any(3, 7) << std::endl;
    std::cout << "max(3.5, 2.1) = " << max_any(3.5, 2.1) << std::endl;

    // =========================================================================
    // SECTION 6: OTHER MODERN C++ FEATURES
    // =========================================================================

    std::cout << "\n=== MODERN C++ FEATURES ===" << std::endl;

    // --- auto type deduction ---
    auto i = 42;           // int
    auto d = 3.14;         // double
    auto s = std::string("hello");
    auto v = std::vector<int>{1, 2, 3};

    // auto in range-based for:
    for (const auto& elem : v) std::cout << elem << " ";
    std::cout << std::endl;

    // --- Structured bindings (C++17) ---
    std::pair<std::string, int> person = {"Alice", 30};
    auto [name, age] = person;
    std::cout << name << " is " << age << std::endl;

    std::map<std::string, int> scores = {{"Alice", 95}, {"Bob", 87}};
    for (const auto& [student, score] : scores) {
        std::cout << student << ": " << score << std::endl;
    }

    // --- if with initializer (C++17) ---
    if (auto it2 = scores.find("Alice"); it2 != scores.end()) {
        std::cout << "Found Alice: " << it2->second << std::endl;
    }

    // --- Fold expressions (C++17) ---
    auto sum_all = [](auto... args) { return (args + ...); };
    std::cout << "sum(1,2,3,4,5) = " << sum_all(1, 2, 3, 4, 5) << std::endl;

    // --- std::optional (C++17) ---
    auto safe_divide = [](double a, double b) -> std::optional<double> {
        if (b == 0) return std::nullopt;
        return a / b;
    };

    auto r1 = safe_divide(10, 2);
    auto r2 = safe_divide(10, 0);

    if (r1) std::cout << "10/2 = " << *r1 << std::endl;
    if (!r2) std::cout << "10/0 = undefined" << std::endl;

    // value_or: get value or default:
    std::cout << "10/0 or -1: " << r2.value_or(-1) << std::endl;

    return 0;
}
