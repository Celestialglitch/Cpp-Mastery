/*
================================================================================
  LESSON 19: Exception Handling — Complete Guide
================================================================================

  THEORY:
  -------
  Exceptions are a mechanism for handling ERRORS at runtime.
  When something goes wrong, you THROW an exception.
  Code that can handle it CATCHES it.

  WITHOUT EXCEPTIONS:
  -------------------
  Old approach: return error codes.
  Problems:
  - Easy to ignore return values
  - Error handling mixed with normal logic
  - Hard to propagate errors up the call stack

  WITH EXCEPTIONS:
  ----------------
  - Errors cannot be silently ignored
  - Normal code and error handling are separated
  - Exceptions automatically propagate up the call stack
  - RAII ensures cleanup even when exceptions occur

  EXECUTION FLOW:
  ---------------
  try {
      statement1;
      throw SomeException();  // ← execution jumps to catch
      statement2;             // ← NEVER executed
  } catch (SomeException& e) {
      // handle it
  }
  // execution continues here

  STACK UNWINDING:
  ----------------
  When an exception is thrown, C++ UNWINDS the stack:
  1. Exits current scope
  2. Destroys all local objects (calls destructors) — RAII works!
  3. Moves to enclosing scope
  4. Repeats until a matching catch is found
  5. If no catch found → std::terminate() → program crashes

  EXCEPTION SAFETY LEVELS:
  -------------------------
  No-throw guarantee:    function never throws (noexcept)
  Strong guarantee:      if throws, state unchanged (like it never happened)
  Basic guarantee:       if throws, no leaks, object in valid state
  No guarantee:          anything can happen (avoid this!)

================================================================================
*/

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <memory>
#include <fstream>

// =========================================================================
// CUSTOM EXCEPTION HIERARCHY
// =========================================================================

// Base exception class:
class AppException : public std::exception {
protected:
    std::string message;
    std::string context;

public:
    AppException(std::string msg, std::string ctx = "")
        : message(msg), context(ctx) {}

    const char* what() const noexcept override {
        return message.c_str();
    }

    std::string get_context() const { return context; }
};

// Derived exceptions:
class ValidationError : public AppException {
public:
    std::string field;
    ValidationError(std::string field, std::string msg)
        : AppException("Validation failed for '" + field + "': " + msg), field(field) {}
};

class DatabaseError : public AppException {
public:
    int error_code;
    DatabaseError(int code, std::string msg)
        : AppException(msg, "Database"), error_code(code) {}
};

class NetworkError : public AppException {
public:
    NetworkError(std::string msg) : AppException(msg, "Network") {}
};

// =========================================================================
// FUNCTIONS THAT THROW
// =========================================================================

double safe_divide(double a, double b) {
    if (b == 0.0) {
        throw std::invalid_argument("Division by zero: " + std::to_string(a) + " / 0");
    }
    return a / b;
}

int parse_age(const std::string& s) {
    try {
        int age = std::stoi(s);  // throws std::invalid_argument if not a number
        if (age < 0 || age > 150) {
            throw ValidationError("age", "Must be between 0 and 150, got " + s);
        }
        return age;
    } catch (const std::invalid_argument&) {
        throw ValidationError("age", "Must be a number, got '" + s + "'");
    }
}

std::vector<int>& get_element(std::vector<int>& v, int index) {
    if (index < 0 || index >= (int)v.size()) {
        throw std::out_of_range(
            "Index " + std::to_string(index) +
            " out of range [0, " + std::to_string(v.size()-1) + "]"
        );
    }
    return v;
}

// =========================================================================
// RAII CLASS — demonstrates cleanup during stack unwinding
// =========================================================================

class FileGuard {
    std::string filename;
    bool opened;
public:
    FileGuard(const std::string& name) : filename(name), opened(true) {
        std::cout << "  [FileGuard] Opened: " << filename << std::endl;
    }
    ~FileGuard() {
        if (opened) {
            std::cout << "  [FileGuard] Closed: " << filename << " (destructor called)" << std::endl;
        }
    }
};

void risky_operation(bool should_throw) {
    FileGuard fg("important.txt");  // RAII — will be cleaned up even if exception thrown
    std::cout << "  Doing work..." << std::endl;
    if (should_throw) {
        throw std::runtime_error("Something went wrong!");
    }
    std::cout << "  Work done successfully." << std::endl;
}

// =========================================================================
// MAIN
// =========================================================================

int main() {

    // =========================================================================
    // SECTION 1: BASIC TRY-CATCH
    // =========================================================================

    std::cout << "=== BASIC TRY-CATCH ===" << std::endl;

    // Normal execution:
    try {
        double r = safe_divide(10, 2);
        std::cout << "10 / 2 = " << r << std::endl;
        std::cout << "No exception thrown." << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cout << "Caught: " << e.what() << std::endl;
    }

    // Exception thrown:
    try {
        std::cout << "\nAttempting 5 / 0..." << std::endl;
        double r = safe_divide(5, 0);  // throws!
        std::cout << "Result: " << r << std::endl;  // NEVER reached
        std::cout << "This line never executes." << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cout << "Caught invalid_argument: " << e.what() << std::endl;
    }
    std::cout << "Execution continues after try-catch." << std::endl;

    // =========================================================================
    // SECTION 2: MULTIPLE CATCH BLOCKS
    // =========================================================================

    std::cout << "\n=== MULTIPLE CATCH ===" << std::endl;

    auto test_exception = [](int choice) {
        try {
            switch (choice) {
                case 1: throw std::invalid_argument("Bad argument");
                case 2: throw std::out_of_range("Out of range");
                case 3: throw std::runtime_error("Runtime error");
                case 4: throw ValidationError("email", "Invalid format");
                case 5: throw 42;  // throwing a non-exception type (bad practice!)
                default: std::cout << "No exception" << std::endl;
            }
        } catch (const ValidationError& e) {
            // Most specific first!
            std::cout << "ValidationError: " << e.what() << " (field: " << e.field << ")" << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cout << "invalid_argument: " << e.what() << std::endl;
        } catch (const std::out_of_range& e) {
            std::cout << "out_of_range: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            // Catches any std::exception (base class)
            std::cout << "std::exception: " << e.what() << std::endl;
        } catch (...) {
            // Catches ANYTHING (last resort)
            std::cout << "Unknown exception caught!" << std::endl;
        }
    };

    for (int i = 0; i <= 5; i++) {
        std::cout << "Choice " << i << ": ";
        test_exception(i);
    }

    // =========================================================================
    // SECTION 3: CUSTOM EXCEPTIONS
    // =========================================================================

    std::cout << "\n=== CUSTOM EXCEPTIONS ===" << std::endl;

    // Validation:
    std::vector<std::string> test_ages = {"25", "-5", "200", "abc", "30"};
    for (const auto& s : test_ages) {
        try {
            int age = parse_age(s);
            std::cout << "'" << s << "' → age = " << age << std::endl;
        } catch (const ValidationError& e) {
            std::cout << "'" << s << "' → ERROR: " << e.what() << std::endl;
        }
    }

    // =========================================================================
    // SECTION 4: RAII AND STACK UNWINDING
    // =========================================================================

    std::cout << "\n=== RAII AND STACK UNWINDING ===" << std::endl;

    // Without exception:
    std::cout << "--- No exception ---" << std::endl;
    try {
        risky_operation(false);
    } catch (const std::exception& e) {
        std::cout << "Caught: " << e.what() << std::endl;
    }

    // With exception — RAII still cleans up!
    std::cout << "\n--- With exception ---" << std::endl;
    try {
        risky_operation(true);
    } catch (const std::exception& e) {
        std::cout << "Caught: " << e.what() << std::endl;
    }
    std::cout << "FileGuard was properly cleaned up even with exception!" << std::endl;

    // =========================================================================
    // SECTION 5: RETHROWING
    // =========================================================================

    std::cout << "\n=== RETHROWING ===" << std::endl;

    auto log_and_rethrow = [](auto fn) {
        try {
            fn();
        } catch (const std::exception& e) {
            std::cout << "  [LOG] Exception caught: " << e.what() << std::endl;
            throw;  // rethrow the SAME exception (preserves type and message)
        }
    };

    try {
        log_and_rethrow([]() {
            throw std::runtime_error("Original error");
        });
    } catch (const std::runtime_error& e) {
        std::cout << "Outer catch: " << e.what() << std::endl;
    }

    // =========================================================================
    // SECTION 6: noexcept
    // =========================================================================

    std::cout << "\n=== noexcept ===" << std::endl;

    // noexcept: promise this function won't throw
    // If it does throw, std::terminate() is called immediately
    auto safe_add = [](int a, int b) noexcept -> int {
        return a + b;
    };

    std::cout << "safe_add(3, 4) = " << safe_add(3, 4) << std::endl;

    // Check if function is noexcept:
    std::cout << "safe_add is noexcept: " << noexcept(safe_add(1, 2)) << std::endl;

    // noexcept(false) — may throw (default):
    auto may_throw = [](int x) -> int {
        if (x < 0) throw std::invalid_argument("negative");
        return x;
    };

    // =========================================================================
    // SECTION 7: EXCEPTION IN CONSTRUCTORS
    // =========================================================================

    std::cout << "\n=== EXCEPTIONS IN CONSTRUCTORS ===" << std::endl;

    class SafeArray {
        int* data;
        int size;
    public:
        SafeArray(int n) : data(nullptr), size(n) {
            if (n <= 0) throw std::invalid_argument("Size must be positive");
            if (n > 1000000) throw std::bad_alloc();
            data = new int[n]();
            std::cout << "SafeArray(" << n << ") created" << std::endl;
        }
        ~SafeArray() {
            delete[] data;
            std::cout << "SafeArray destroyed" << std::endl;
        }
        int& operator[](int i) {
            if (i < 0 || i >= size) throw std::out_of_range("Index out of range");
            return data[i];
        }
    };

    // Valid construction:
    try {
        SafeArray arr(5);
        arr[0] = 10;
        arr[4] = 50;
        std::cout << "arr[0]=" << arr[0] << " arr[4]=" << arr[4] << std::endl;
        arr[10] = 99;  // throws!
    } catch (const std::out_of_range& e) {
        std::cout << "Caught: " << e.what() << std::endl;
    }

    // Invalid construction:
    try {
        SafeArray bad(-5);  // throws in constructor
    } catch (const std::invalid_argument& e) {
        std::cout << "Bad construction: " << e.what() << std::endl;
    }

    // =========================================================================
    // SECTION 8: STANDARD EXCEPTION HIERARCHY
    // =========================================================================

    std::cout << "\n=== STANDARD EXCEPTIONS ===" << std::endl;
    std::cout << "std::exception" << std::endl;
    std::cout << "├── std::logic_error" << std::endl;
    std::cout << "│   ├── std::invalid_argument" << std::endl;
    std::cout << "│   ├── std::domain_error" << std::endl;
    std::cout << "│   ├── std::length_error" << std::endl;
    std::cout << "│   └── std::out_of_range" << std::endl;
    std::cout << "├── std::runtime_error" << std::endl;
    std::cout << "│   ├── std::range_error" << std::endl;
    std::cout << "│   ├── std::overflow_error" << std::endl;
    std::cout << "│   └── std::underflow_error" << std::endl;
    std::cout << "├── std::bad_alloc" << std::endl;
    std::cout << "├── std::bad_cast" << std::endl;
    std::cout << "└── std::bad_typeid" << std::endl;

    return 0;
}
