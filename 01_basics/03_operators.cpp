/*
================================================================================
  LESSON 3: Operators — Complete Guide
================================================================================

  THEORY:
  -------
  An OPERATOR is a symbol that tells the compiler to perform a specific
  mathematical, logical, or relational operation.

  OPERATOR PRECEDENCE (high to low, simplified):
  -----------------------------------------------
  1.  ()                    — parentheses (highest)
  2.  ++, -- (postfix)      — post-increment/decrement
  3.  ++, -- (prefix), !, ~ — pre-increment, logical NOT, bitwise NOT
  4.  *, /, %               — multiplication, division, modulo
  5.  +, -                  — addition, subtraction
  6.  <<, >>                — bitwise shift
  7.  <, <=, >, >=          — relational
  8.  ==, !=                — equality
  9.  &                     — bitwise AND
  10. ^                     — bitwise XOR
  11. |                     — bitwise OR
  12. &&                    — logical AND
  13. ||                    — logical OR
  14. ?:                    — ternary
  15. =, +=, -=, etc.       — assignment (lowest)

  ASSOCIATIVITY:
  --------------
  Most operators are LEFT-TO-RIGHT: a - b - c = (a - b) - c
  Assignment is RIGHT-TO-LEFT:      a = b = c means a = (b = c)

================================================================================
*/

#include <iostream>
#include <cmath>   // for pow(), sqrt(), abs()

int main() {

    // =========================================================================
    // SECTION 1: ARITHMETIC OPERATORS
    // =========================================================================

    std::cout << "=== ARITHMETIC ===" << std::endl;

    int a = 17, b = 5;

    std::cout << "a = " << a << ", b = " << b << std::endl;
    std::cout << "a + b = " << (a + b) << std::endl;   // 22
    std::cout << "a - b = " << (a - b) << std::endl;   // 12
    std::cout << "a * b = " << (a * b) << std::endl;   // 85
    std::cout << "a / b = " << (a / b) << std::endl;   // 3  ← INTEGER division (truncates)
    std::cout << "a % b = " << (a % b) << std::endl;   // 2  ← remainder: 17 = 3*5 + 2

    // INTEGER DIVISION TRAP — very common bug:
    std::cout << "\n--- Integer division trap ---" << std::endl;
    int x = 7, y = 2;
    std::cout << "7 / 2 (int)    = " << x / y           << std::endl;  // 3 (NOT 3.5!)
    std::cout << "7.0 / 2 (float)= " << 7.0 / 2         << std::endl;  // 3.5
    std::cout << "(double)7 / 2  = " << (double)x / y   << std::endl;  // 3.5

    // MODULO use cases:
    std::cout << "\n--- Modulo use cases ---" << std::endl;
    std::cout << "Is 17 even? " << (17 % 2 == 0 ? "yes" : "no") << std::endl;  // no
    std::cout << "Is 16 even? " << (16 % 2 == 0 ? "yes" : "no") << std::endl;  // yes
    std::cout << "17 % 5 = " << 17 % 5 << std::endl;  // 2 (clock arithmetic!)
    // Modulo with negative numbers (behavior is implementation-defined in C):
    std::cout << "-7 % 3 = " << -7 % 3 << std::endl;  // -1 in C++ (sign follows dividend)
    std::cout << "7 % -3 = " << 7 % -3 << std::endl;  // 1

    // Math functions from <cmath>:
    std::cout << "\n--- cmath functions ---" << std::endl;
    std::cout << "pow(2, 10)  = " << std::pow(2, 10)   << std::endl;  // 1024
    std::cout << "sqrt(144)   = " << std::sqrt(144)    << std::endl;  // 12
    std::cout << "abs(-42)    = " << std::abs(-42)     << std::endl;  // 42
    std::cout << "ceil(3.2)   = " << std::ceil(3.2)    << std::endl;  // 4
    std::cout << "floor(3.9)  = " << std::floor(3.9)   << std::endl;  // 3
    std::cout << "round(3.5)  = " << std::round(3.5)   << std::endl;  // 4
    std::cout << "log(2.718)  = " << std::log(2.71828) << std::endl;  // ~1 (natural log)
    std::cout << "log10(1000) = " << std::log10(1000)  << std::endl;  // 3

    // =========================================================================
    // SECTION 2: INCREMENT AND DECREMENT
    // =========================================================================

    std::cout << "\n=== INCREMENT / DECREMENT ===" << std::endl;

    int n = 5;

    // POST-increment: use value FIRST, then increment
    std::cout << "n = " << n << std::endl;          // 5
    std::cout << "n++ = " << n++ << std::endl;      // prints 5, then n becomes 6
    std::cout << "n after n++ = " << n << std::endl; // 6

    // PRE-increment: increment FIRST, then use value
    std::cout << "++n = " << ++n << std::endl;      // n becomes 7, prints 7
    std::cout << "n after ++n = " << n << std::endl; // 7

    // This distinction matters in expressions:
    int p = 5, q = 5;
    int r1 = p++ * 2;  // r1 = 5*2 = 10, then p becomes 6
    int r2 = ++q * 2;  // q becomes 6 first, then r2 = 6*2 = 12
    std::cout << "p++ * 2: r1=" << r1 << " p=" << p << std::endl;  // r1=10, p=6
    std::cout << "++q * 2: r2=" << r2 << " q=" << q << std::endl;  // r2=12, q=6

    // =========================================================================
    // SECTION 3: ASSIGNMENT OPERATORS
    // =========================================================================

    std::cout << "\n=== ASSIGNMENT ===" << std::endl;

    int val = 100;
    std::cout << "Start: val = " << val << std::endl;

    val += 20;   std::cout << "val += 20  → " << val << std::endl;  // 120
    val -= 15;   std::cout << "val -= 15  → " << val << std::endl;  // 105
    val *= 2;    std::cout << "val *= 2   → " << val << std::endl;  // 210
    val /= 3;    std::cout << "val /= 3   → " << val << std::endl;  // 70
    val %= 9;    std::cout << "val %= 9   → " << val << std::endl;  // 7

    // Chained assignment (right-to-left):
    int i, j, k;
    i = j = k = 42;  // k=42, then j=42, then i=42
    std::cout << "i=j=k=42: " << i << " " << j << " " << k << std::endl;

    // =========================================================================
    // SECTION 4: COMPARISON OPERATORS
    // =========================================================================

    std::cout << "\n=== COMPARISON ===" << std::endl;
    std::cout << std::boolalpha;  // print true/false instead of 1/0

    int m = 10, nn = 20;
    std::cout << "m=10, n=20" << std::endl;
    std::cout << "m == nn : " << (m == nn) << std::endl;  // false
    std::cout << "m != nn : " << (m != nn) << std::endl;  // true
    std::cout << "m <  nn : " << (m <  nn) << std::endl;  // true
    std::cout << "m >  nn : " << (m >  nn) << std::endl;  // false
    std::cout << "m <= nn : " << (m <= nn) << std::endl;  // true
    std::cout << "m >= nn : " << (m >= nn) << std::endl;  // false

    // COMMON MISTAKE: = vs ==
    // if (m = 5)  ← ASSIGNMENT (sets m to 5, condition is 5 = true)
    // if (m == 5) ← COMPARISON (checks if m equals 5)
    // Modern compilers warn about this. Always use == for comparison!

    // Comparing floats — NEVER use == directly:
    double f1 = 0.1 + 0.2;
    double f2 = 0.3;
    std::cout << "\nFloat comparison:" << std::endl;
    std::cout << "0.1+0.2 == 0.3 : " << (f1 == f2) << std::endl;  // false! (precision)
    std::cout << "Within 1e-9?   : " << (std::abs(f1 - f2) < 1e-9) << std::endl;  // true

    std::cout << std::noboolalpha;

    // =========================================================================
    // SECTION 5: LOGICAL OPERATORS
    // =========================================================================

    std::cout << "\n=== LOGICAL ===" << std::endl;

    bool t = true, f = false;

    // AND (&&): both must be true
    std::cout << "true  && true  = " << (t && t) << std::endl;  // 1
    std::cout << "true  && false = " << (t && f) << std::endl;  // 0
    std::cout << "false && true  = " << (f && t) << std::endl;  // 0
    std::cout << "false && false = " << (f && f) << std::endl;  // 0

    // OR (||): at least one must be true
    std::cout << "true  || false = " << (t || f) << std::endl;  // 1
    std::cout << "false || false = " << (f || f) << std::endl;  // 0

    // NOT (!): flips the value
    std::cout << "!true  = " << (!t) << std::endl;  // 0
    std::cout << "!false = " << (!f) << std::endl;  // 1

    // SHORT-CIRCUIT EVALUATION — very important!
    // && stops at first false (no need to check rest)
    // || stops at first true  (no need to check rest)
    int counter = 0;
    auto increment_and_return = [&counter](bool val) -> bool {
        counter++;
        return val;
    };

    counter = 0;
    bool r = increment_and_return(false) && increment_and_return(true);
    std::cout << "\nShort-circuit &&: counter=" << counter << std::endl;  // 1 (stopped early!)

    counter = 0;
    r = increment_and_return(true) || increment_and_return(false);
    std::cout << "Short-circuit ||: counter=" << counter << std::endl;  // 1 (stopped early!)

    // Practical use — null check before dereference:
    int* ptr = nullptr;
    // Safe: if ptr is null, second condition never evaluated (no crash):
    if (ptr != nullptr && *ptr > 0) {
        std::cout << "ptr is valid and positive" << std::endl;
    } else {
        std::cout << "ptr is null (safe check)" << std::endl;
    }

    // =========================================================================
    // SECTION 6: BITWISE OPERATORS
    // =========================================================================

    std::cout << "\n=== BITWISE ===" << std::endl;
    // These operate on individual BITS of integers.
    // Essential for: flags, permissions, hardware, compression, encryption.

    unsigned int A = 0b1010;  // binary literal: 10 in decimal
    unsigned int B = 0b1100;  // binary literal: 12 in decimal

    std::cout << "A = " << A << " (binary: 1010)" << std::endl;
    std::cout << "B = " << B << " (binary: 1100)" << std::endl;

    // AND: bit is 1 only if BOTH bits are 1
    //   1010
    // & 1100
    // ------
    //   1000 = 8
    std::cout << "A & B = " << (A & B) << " (expected 8)" << std::endl;

    // OR: bit is 1 if EITHER bit is 1
    //   1010
    // | 1100
    // ------
    //   1110 = 14
    std::cout << "A | B = " << (A | B) << " (expected 14)" << std::endl;

    // XOR: bit is 1 if bits are DIFFERENT
    //   1010
    // ^ 1100
    // ------
    //   0110 = 6
    std::cout << "A ^ B = " << (A ^ B) << " (expected 6)" << std::endl;

    // NOT: flips all bits
    std::cout << "~A    = " << (~A) << std::endl;  // large number (all bits flipped)

    // LEFT SHIFT (<<): multiply by 2^n
    std::cout << "A << 1 = " << (A << 1) << " (A * 2 = 20)" << std::endl;
    std::cout << "A << 2 = " << (A << 2) << " (A * 4 = 40)" << std::endl;

    // RIGHT SHIFT (>>): divide by 2^n
    std::cout << "A >> 1 = " << (A >> 1) << " (A / 2 = 5)" << std::endl;

    // PRACTICAL: Using bits as flags (common in systems programming)
    const int READ    = 0b001;  // 1
    const int WRITE   = 0b010;  // 2
    const int EXECUTE = 0b100;  // 4

    int permissions = READ | WRITE;  // user has read + write
    std::cout << "\nPermissions: " << permissions << std::endl;  // 3

    // Check if user has READ permission:
    if (permissions & READ) std::cout << "Has READ" << std::endl;
    if (permissions & WRITE) std::cout << "Has WRITE" << std::endl;
    if (!(permissions & EXECUTE)) std::cout << "No EXECUTE" << std::endl;

    // Add EXECUTE permission:
    permissions |= EXECUTE;
    if (permissions & EXECUTE) std::cout << "Now has EXECUTE" << std::endl;

    // Remove WRITE permission:
    permissions &= ~WRITE;  // AND with NOT WRITE
    if (!(permissions & WRITE)) std::cout << "WRITE removed" << std::endl;

    // =========================================================================
    // SECTION 7: TERNARY OPERATOR
    // =========================================================================

    std::cout << "\n=== TERNARY ===" << std::endl;
    // condition ? value_if_true : value_if_false

    int score = 75;
    std::string grade = (score >= 90) ? "A" :
                        (score >= 80) ? "B" :
                        (score >= 70) ? "C" :
                        (score >= 60) ? "D" : "F";
    std::cout << "Score " << score << " → Grade " << grade << std::endl;  // C

    int abs_val = (score < 0) ? -score : score;
    std::cout << "Abs of " << score << " = " << abs_val << std::endl;

    // =========================================================================
    // SECTION 8: OPERATOR PRECEDENCE EXAMPLES
    // =========================================================================

    std::cout << "\n=== PRECEDENCE ===" << std::endl;

    // Without parentheses — follows precedence rules:
    int result1 = 2 + 3 * 4;       // 14 (not 20! * before +)
    int result2 = (2 + 3) * 4;     // 20 (parentheses override)
    int result3 = 10 - 3 - 2;      // 5  (left-to-right: (10-3)-2)
    int result4 = 2 << 3 + 1;      // 2 << 4 = 32 (+ before <<)

    std::cout << "2 + 3 * 4   = " << result1 << std::endl;  // 14
    std::cout << "(2+3) * 4   = " << result2 << std::endl;  // 20
    std::cout << "10 - 3 - 2  = " << result3 << std::endl;  // 5
    std::cout << "2 << 3 + 1  = " << result4 << std::endl;  // 32

    // RULE: When in doubt, use parentheses. Clarity > cleverness.

    return 0;
}
