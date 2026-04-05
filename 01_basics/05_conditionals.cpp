/*
================================================================================
  LESSON 5: Conditionals — Complete Guide
================================================================================

  THEORY:
  -------
  Conditionals let your program make DECISIONS — execute different code
  depending on whether conditions are true or false.

  EXECUTION FLOW:
  ---------------
  Without conditionals, code runs top-to-bottom linearly.
  Conditionals create BRANCHES — the program takes one path or another.

  if (condition) {
      // path A — runs if condition is true
  } else {
      // path B — runs if condition is false
  }
  // execution continues here regardless

  WHAT IS "TRUTHY" IN C++?
  -------------------------
  - 0, 0.0, nullptr, '\0' → false
  - Everything else        → true
  So: if (5) runs, if (0) doesn't.

  DANGLING ELSE PROBLEM:
  ----------------------
  else always binds to the NEAREST if.
  Always use braces {} to avoid ambiguity.

================================================================================
*/

#include <iostream>
#include <string>

int main() {

    // =========================================================================
    // SECTION 1: BASIC if / else if / else
    // =========================================================================

    std::cout << "=== BASIC IF/ELSE ===" << std::endl;

    int score = 75;

    // Execution flow:
    // 1. Check score >= 90 → false, skip
    // 2. Check score >= 80 → false, skip
    // 3. Check score >= 70 → TRUE, execute this block
    // 4. Skip remaining else-if and else
    // 5. Continue after the entire if-else chain

    if (score >= 90) {
        std::cout << "Grade: A (Excellent)" << std::endl;
    } else if (score >= 80) {
        std::cout << "Grade: B (Good)" << std::endl;
    } else if (score >= 70) {
        std::cout << "Grade: C (Average)" << std::endl;  // ← this runs
    } else if (score >= 60) {
        std::cout << "Grade: D (Below Average)" << std::endl;
    } else {
        std::cout << "Grade: F (Fail)" << std::endl;
    }

    std::cout << "After if-else chain." << std::endl;  // always runs

    // =========================================================================
    // SECTION 2: SINGLE-LINE if (no braces) — DANGEROUS
    // =========================================================================

    std::cout << "\n=== SINGLE LINE IF ===" << std::endl;

    int x = 5;
    if (x > 0) std::cout << "Positive" << std::endl;  // works but risky

    // THE DANGLING ELSE TRAP:
    int a = 1, b = 2;
    if (a > 0)
        if (b > 0)
            std::cout << "Both positive" << std::endl;
    else
        // This else belongs to the INNER if (b > 0), NOT the outer if (a > 0)!
        // This is confusing. ALWAYS use braces.
        std::cout << "b is not positive" << std::endl;

    // SAFE version with braces:
    if (a > 0) {
        if (b > 0) {
            std::cout << "Both positive (safe)" << std::endl;
        }
    } else {
        std::cout << "a is not positive" << std::endl;
    }

    // =========================================================================
    // SECTION 3: NESTED if
    // =========================================================================

    std::cout << "\n=== NESTED IF ===" << std::endl;

    int age = 20;
    bool has_ticket = true;
    bool is_vip = false;

    if (age >= 18) {
        std::cout << "Age check passed." << std::endl;
        if (has_ticket) {
            std::cout << "Ticket check passed." << std::endl;
            if (is_vip) {
                std::cout << "VIP access granted!" << std::endl;
            } else {
                std::cout << "Standard access granted." << std::endl;  // ← runs
            }
        } else {
            std::cout << "No ticket — buy one first." << std::endl;
        }
    } else {
        std::cout << "Must be 18+ to enter." << std::endl;
    }

    // =========================================================================
    // SECTION 4: TRUTHY / FALSY values
    // =========================================================================

    std::cout << "\n=== TRUTHY/FALSY ===" << std::endl;

    // These all evaluate as FALSE:
    if (0)       std::cout << "0 is true"       << std::endl;
    else         std::cout << "0 is false"       << std::endl;  // ← runs

    if (0.0)     std::cout << "0.0 is true"     << std::endl;
    else         std::cout << "0.0 is false"     << std::endl;  // ← runs

    if (nullptr) std::cout << "nullptr is true" << std::endl;
    else         std::cout << "nullptr is false" << std::endl;  // ← runs

    if ("")      std::cout << "\"\" is true"    << std::endl;   // ← RUNS! empty string literal is a pointer (non-null)
    else         std::cout << "\"\" is false"   << std::endl;

    // These all evaluate as TRUE:
    if (1)       std::cout << "1 is true"       << std::endl;  // ← runs
    if (-1)      std::cout << "-1 is true"      << std::endl;  // ← runs
    if (42)      std::cout << "42 is true"      << std::endl;  // ← runs
    if (3.14)    std::cout << "3.14 is true"    << std::endl;  // ← runs

    // Checking pointer validity:
    int* ptr = nullptr;
    if (ptr) {
        std::cout << "ptr is valid" << std::endl;
    } else {
        std::cout << "ptr is null" << std::endl;  // ← runs
    }

    int val = 42;
    ptr = &val;
    if (ptr) {
        std::cout << "ptr is valid, value = " << *ptr << std::endl;  // ← runs
    }

    // =========================================================================
    // SECTION 5: TERNARY OPERATOR
    // =========================================================================

    std::cout << "\n=== TERNARY ===" << std::endl;
    // condition ? value_if_true : value_if_false
    // Returns a VALUE — can be used in expressions

    int n = -7;
    int abs_n = (n < 0) ? -n : n;
    std::cout << "abs(-7) = " << abs_n << std::endl;  // 7

    // Nested ternary (use sparingly — can get unreadable):
    int temp = 25;
    std::string weather = (temp > 30) ? "Hot" :
                          (temp > 20) ? "Warm" :
                          (temp > 10) ? "Cool" : "Cold";
    std::cout << temp << "°C is " << weather << std::endl;  // Warm

    // Ternary in output:
    bool logged_in = true;
    std::cout << "Status: " << (logged_in ? "Online" : "Offline") << std::endl;

    // =========================================================================
    // SECTION 6: SWITCH STATEMENT
    // =========================================================================

    std::cout << "\n=== SWITCH ===" << std::endl;

    // switch works with: int, char, enum (NOT float, string)
    // Execution flow:
    // 1. Evaluate expression
    // 2. Jump to matching case label
    // 3. Execute from there until 'break' or end of switch
    // 4. If no match, jump to 'default'

    int day = 3;
    std::cout << "Day " << day << " is: ";

    switch (day) {
        case 1:
            std::cout << "Monday" << std::endl;
            break;  // jump out of switch
        case 2:
            std::cout << "Tuesday" << std::endl;
            break;
        case 3:
            std::cout << "Wednesday" << std::endl;  // ← runs
            break;
        case 4:
            std::cout << "Thursday" << std::endl;
            break;
        case 5:
            std::cout << "Friday" << std::endl;
            break;
        case 6:
        case 7:
            // Both case 6 and 7 fall through to this code:
            std::cout << "Weekend!" << std::endl;
            break;
        default:
            std::cout << "Invalid day" << std::endl;
    }

    // FALL-THROUGH (intentional, without break):
    std::cout << "\n--- Fall-through demo ---" << std::endl;
    int level = 1;
    switch (level) {
        case 1:
            std::cout << "Level 1 bonus" << std::endl;
            // intentional fall-through
            [[fallthrough]];  // C++17: tells compiler this is intentional (suppresses warning)
        case 2:
            std::cout << "Level 2 bonus" << std::endl;
            [[fallthrough]];
        case 3:
            std::cout << "Level 3 bonus" << std::endl;
            break;
        default:
            std::cout << "No bonus" << std::endl;
    }
    // Output: Level 1, Level 2, Level 3 (all three because of fall-through)

    // Switch with char:
    char grade = 'B';
    switch (grade) {
        case 'A': case 'a':
            std::cout << "\nExcellent!" << std::endl;
            break;
        case 'B': case 'b':
            std::cout << "\nGood job!" << std::endl;  // ← runs
            break;
        case 'C': case 'c':
            std::cout << "\nAverage" << std::endl;
            break;
        default:
            std::cout << "\nTry harder" << std::endl;
    }

    // =========================================================================
    // SECTION 7: if constexpr (C++17) — compile-time branching
    // =========================================================================

    std::cout << "\n=== if constexpr ===" << std::endl;

    // Evaluated at COMPILE TIME — dead branch is completely removed from binary.
    // Used heavily in template programming.
    constexpr bool DEBUG_MODE = false;

    if constexpr (DEBUG_MODE) {
        std::cout << "Debug info..." << std::endl;  // compiled out entirely
    } else {
        std::cout << "Release mode" << std::endl;  // ← this is in the binary
    }

    // =========================================================================
    // SECTION 8: REAL-WORLD EXAMPLES
    // =========================================================================

    std::cout << "\n=== REAL EXAMPLES ===" << std::endl;

    // Login system:
    std::string username = "admin";
    std::string password = "secret123";
    int attempts = 0;
    const int MAX_ATTEMPTS = 3;

    // Simulating login check:
    std::string input_user = "admin";
    std::string input_pass = "secret123";

    if (attempts >= MAX_ATTEMPTS) {
        std::cout << "Account locked!" << std::endl;
    } else if (input_user == username && input_pass == password) {
        std::cout << "Login successful! Welcome, " << username << std::endl;
    } else {
        attempts++;
        std::cout << "Invalid credentials. Attempts: " << attempts << "/" << MAX_ATTEMPTS << std::endl;
    }

    // FizzBuzz (classic interview question):
    std::cout << "\nFizzBuzz 1-20:" << std::endl;
    for (int i = 1; i <= 20; i++) {
        if (i % 15 == 0)      std::cout << "FizzBuzz ";
        else if (i % 3 == 0)  std::cout << "Fizz ";
        else if (i % 5 == 0)  std::cout << "Buzz ";
        else                   std::cout << i << " ";
    }
    std::cout << std::endl;

    return 0;
}
