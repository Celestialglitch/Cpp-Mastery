/*
================================================================================
  LESSON 2: Variables, Data Types, and Memory
================================================================================

  THEORY:
  -------
  A VARIABLE is a named location in memory that holds a value.
  Think of RAM as a giant grid of boxes. Each box has an address (like a house
  number). A variable gives a human-readable NAME to one of those boxes.

  DECLARATION vs INITIALIZATION:
  --------------------------------
  Declaration:    int age;          // tells compiler "reserve space for an int named age"
  Initialization: age = 25;         // puts a value into that space
  Both at once:   int age = 25;     // declare AND initialize (always prefer this)

  TYPE SYSTEM:
  ------------
  C++ is STATICALLY TYPED — every variable has a fixed type decided at compile time.
  The type determines:
    1. How many bytes of memory to reserve
    2. How to interpret those bytes (int? float? char?)
    3. What operations are valid on it

  MEMORY LAYOUT (typical 64-bit system):
  ----------------------------------------
  Type          | Size    | Range
  --------------|---------|------------------------------------------
  bool          | 1 byte  | true (1) or false (0)
  char          | 1 byte  | -128 to 127 (or 0-255 unsigned)
  short         | 2 bytes | -32,768 to 32,767
  int           | 4 bytes | -2,147,483,648 to 2,147,483,647
  long          | 4/8 bytes| platform-dependent
  long long     | 8 bytes | -9.2×10^18 to 9.2×10^18
  float         | 4 bytes | ~7 decimal digits precision
  double        | 8 bytes | ~15 decimal digits precision
  long double   | 8/16 bytes| extended precision

  SIGNED vs UNSIGNED:
  -------------------
  signed int:   can hold negative and positive (-2B to +2B)
  unsigned int: only positive (0 to 4B) — doubles the positive range

================================================================================
*/

#include <iostream>
#include <string>
#include <limits>    // for numeric_limits — shows min/max of each type
#include <climits>   // INT_MAX, INT_MIN, etc.
#include <cfloat>    // FLT_MAX, DBL_MAX, etc.

int main() {

    // =========================================================================
    // SECTION 1: INTEGER TYPES
    // =========================================================================

    // Basic int — most common integer type
    int age = 25;
    int temperature = -10;    // negative values work fine
    // int zero = 0;  // valid but unused in this demo

    std::cout << "=== INTEGER TYPES ===" << std::endl;
    std::cout << "age         = " << age         << std::endl;
    std::cout << "temperature = " << temperature << std::endl;

    // short — smaller range, saves memory in large arrays
    short small_num = 32767;   // max value for short
    // OVERFLOW demo: 32767 + 1 exceeds short's max, wraps to -32768
    // We cast explicitly so the compiler knows we intend this:
    short overflow  = static_cast<short>(32767 + 1);
    std::cout << "short max      = " << small_num << std::endl;
    std::cout << "short overflow = " << overflow  << std::endl;  // -32768 (wraps!)

    // long long — for very large numbers
    long long population = 8000000000LL;  // 8 billion (LL suffix = long long literal)
    std::cout << "World population = " << population << std::endl;

    // unsigned — only positive, double the range
    unsigned int positive_only = 4294967295U;  // max unsigned int (U suffix)
    std::cout << "Max unsigned int = " << positive_only << std::endl;

    // Showing limits:
    std::cout << "\nINT_MIN = " << INT_MIN << std::endl;  // -2147483648
    std::cout << "INT_MAX = " << INT_MAX << std::endl;    // 2147483647
    std::cout << "LLONG_MAX = " << LLONG_MAX << std::endl;

    // =========================================================================
    // SECTION 2: FLOATING POINT TYPES
    // =========================================================================

    std::cout << "\n=== FLOATING POINT ===" << std::endl;

    float  f = 3.14f;          // 'f' suffix = float literal (without it, it's double)
    double d = 3.14159265358979;  // more precise
    long double ld = 3.14159265358979323846L;  // even more precise

    std::cout << "float:       " << f  << std::endl;
    std::cout << "double:      " << d  << std::endl;
    std::cout << "long double: " << ld << std::endl;

    // Precision demonstration:
    std::cout.precision(20);  // show 20 significant digits
    std::cout << "float   precise: " << f  << std::endl;  // imprecise after ~7 digits
    std::cout << "double  precise: " << d  << std::endl;  // imprecise after ~15 digits
    std::cout.precision(6);   // reset to default

    // Special float values:
    float pos_inf = 1.0f / 0.0f;   // positive infinity
    float neg_inf = -1.0f / 0.0f;  // negative infinity
    float nan_val = 0.0f / 0.0f;   // NaN (Not a Number)
    std::cout << "1/0   = " << pos_inf << std::endl;  // inf
    std::cout << "-1/0  = " << neg_inf << std::endl;  // -inf
    std::cout << "0/0   = " << nan_val << std::endl;  // nan

    // FLOATING POINT COMPARISON TRAP:
    double a = 0.1 + 0.2;
    double b = 0.3;
    std::cout << "\n0.1 + 0.2 == 0.3 ? " << (a == b) << std::endl;  // 0 (FALSE!)
    std::cout << "0.1 + 0.2 = " << a << std::endl;  // 0.30000000000000004
    // LESSON: Never compare floats with ==. Use a tolerance (epsilon):
    double epsilon = 1e-9;
    std::cout << "Within epsilon? " << (std::abs(a - b) < epsilon) << std::endl;  // 1 (true)

    // =========================================================================
    // SECTION 3: CHARACTER TYPE
    // =========================================================================

    std::cout << "\n=== CHAR ===" << std::endl;

    char letter = 'A';         // single quotes for char
    char digit  = '7';         // '7' is char 55 in ASCII, NOT the number 7
    char newline = '\n';       // escape sequence
    char tab     = '\t';

    std::cout << "letter = " << letter << std::endl;  // A
    std::cout << "digit  = " << digit  << std::endl;  // 7

    // char is just a small integer — ASCII value:
    std::cout << "ASCII of 'A' = " << (int)letter << std::endl;  // 65
    std::cout << "ASCII of 'a' = " << (int)'a'    << std::endl;  // 97
    std::cout << "ASCII of '0' = " << (int)'0'    << std::endl;  // 48

    // Arithmetic on chars:
    char next = letter + 1;   // 'A' + 1 = 'B'
    std::cout << "Next letter: " << next << std::endl;  // B

    // Uppercase to lowercase:
    char upper = 'G';
    char lower = upper + 32;  // 'G'(71) + 32 = 'g'(103)
    std::cout << "Lower: " << lower << std::endl;  // g

    // =========================================================================
    // SECTION 4: BOOLEAN TYPE
    // =========================================================================

    std::cout << "\n=== BOOL ===" << std::endl;

    bool is_raining = false;
    bool is_sunny   = true;
    bool result     = (5 > 3);  // expression evaluates to true

    std::cout << "is_raining = " << is_raining << std::endl;  // 0
    std::cout << "is_sunny   = " << is_sunny   << std::endl;  // 1
    std::cout << "5 > 3      = " << result     << std::endl;  // 1

    // Print as text:
    std::cout << std::boolalpha;  // makes bool print as "true"/"false"
    std::cout << "is_sunny = " << is_sunny << std::endl;  // true
    std::cout << std::noboolalpha;  // reset

    // Non-zero integers are truthy:
    bool from_int = 42;    // true (any non-zero)
    bool from_zero = 0;    // false
    std::cout << "bool(42) = " << from_int  << std::endl;  // 1
    std::cout << "bool(0)  = " << from_zero << std::endl;  // 0

    // =========================================================================
    // SECTION 5: STRING TYPE
    // =========================================================================

    std::cout << "\n=== STRING ===" << std::endl;

    std::string name    = "Alice";
    std::string empty   = "";
    std::string spaces  = "Hello World";

    // String is NOT a primitive — it's a class from the standard library.
    // It manages its own memory dynamically.

    std::cout << "name   = " << name   << std::endl;
    std::cout << "length = " << name.length() << std::endl;  // 5
    std::cout << "empty? = " << empty.empty() << std::endl;  // 1 (true)

    // Concatenation:
    std::string full = name + " is learning C++";
    std::cout << full << std::endl;

    // =========================================================================
    // SECTION 6: CONSTANTS
    // =========================================================================

    std::cout << "\n=== CONSTANTS ===" << std::endl;

    // const — value cannot change after initialization
    const double PI = 3.14159265358979;
    const int MAX_STUDENTS = 30;
    // PI = 3.0;  // COMPILE ERROR: assignment of read-only variable

    std::cout << "PI = " << PI << std::endl;
    std::cout << "MAX_STUDENTS = " << MAX_STUDENTS << std::endl;

    // constexpr — evaluated at COMPILE TIME (even faster)
    // constexpr int ARRAY_SIZE = 100;  // useful when declaring arrays: int arr[ARRAY_SIZE];
    constexpr double GRAVITY = 9.81;
    std::cout << "GRAVITY = " << GRAVITY << std::endl;

    // =========================================================================
    // SECTION 7: TYPE DEDUCTION WITH auto
    // =========================================================================

    std::cout << "\n=== AUTO ===" << std::endl;

    auto x = 42;           // int   (compiler sees integer literal)
    auto y = 3.14;         // double (compiler sees decimal literal)
    auto z = 3.14f;        // float  (f suffix)
    auto c = 'A';          // char
    auto s = std::string("hello");  // std::string
    auto b = true;         // bool

    // auto is NOT dynamic typing — type is fixed at compile time!
    // x = "hello";  // COMPILE ERROR: x is int, can't assign string

    std::cout << "auto x = " << x << " (int)"    << std::endl;
    std::cout << "auto y = " << y << " (double)"  << std::endl;
    std::cout << "auto z = " << z << " (float)"   << std::endl;
    std::cout << "auto c = " << c << " (char)"    << std::endl;
    std::cout << "auto s = " << s << " (string)"  << std::endl;
    std::cout << "auto b = " << b << " (bool)"    << std::endl;

    // =========================================================================
    // SECTION 8: TYPE CONVERSION
    // =========================================================================

    std::cout << "\n=== TYPE CONVERSION ===" << std::endl;

    // IMPLICIT conversion (automatic, may lose data):
    int   i = static_cast<int>(3.99);  // double → int: truncates to 3 (NOT rounded!)
    // float f2 = 100000000000.0;  // too big for float precision — just a note
    std::cout << "double 3.99 → int: " << i << std::endl;  // 3

    // EXPLICIT cast (you tell compiler to convert):
    double ratio = (double)7 / 2;   // C-style cast
    double ratio2 = static_cast<double>(7) / 2;  // C++ style (preferred)
    std::cout << "7/2 as int:    " << 7/2    << std::endl;  // 3
    std::cout << "7/2 as double: " << ratio  << std::endl;  // 3.5

    // static_cast is safer — checked at compile time:
    int big = 1000;
    char small = static_cast<char>(big);  // truncates — may lose data
    std::cout << "1000 as char: " << (int)small << std::endl;  // 232 (truncated)

    // =========================================================================
    // SECTION 9: sizeof operator
    // =========================================================================

    std::cout << "\n=== SIZEOF ===" << std::endl;
    std::cout << "bool:        " << sizeof(bool)        << " bytes" << std::endl;
    std::cout << "char:        " << sizeof(char)        << " bytes" << std::endl;
    std::cout << "short:       " << sizeof(short)       << " bytes" << std::endl;
    std::cout << "int:         " << sizeof(int)         << " bytes" << std::endl;
    std::cout << "long:        " << sizeof(long)        << " bytes" << std::endl;
    std::cout << "long long:   " << sizeof(long long)   << " bytes" << std::endl;
    std::cout << "float:       " << sizeof(float)       << " bytes" << std::endl;
    std::cout << "double:      " << sizeof(double)      << " bytes" << std::endl;
    std::cout << "std::string: " << sizeof(std::string) << " bytes" << std::endl;

    // sizeof a variable:
    int arr[10];
    std::cout << "int arr[10]: " << sizeof(arr) << " bytes" << std::endl;  // 40
    std::cout << "Elements:    " << sizeof(arr)/sizeof(arr[0]) << std::endl;  // 10

    // =========================================================================
    // SECTION 10: VARIABLE SCOPE
    // =========================================================================

    std::cout << "\n=== SCOPE ===" << std::endl;

    int outer = 10;
    {
        int inner = 20;  // only exists inside this block
        int outer = 99;  // SHADOWS the outer 'outer' — different variable!
        std::cout << "inner outer = " << outer << std::endl;  // 99
        std::cout << "inner = " << inner << std::endl;        // 20
    }
    // inner is GONE here — out of scope
    std::cout << "outer = " << outer << std::endl;  // 10 (original, unaffected)
    // std::cout << inner;  // COMPILE ERROR: inner not in scope

    return 0;
}

/*
  EXPECTED OUTPUT (partial):
  --------------------------
  === INTEGER TYPES ===
  age         = 25
  temperature = -10
  short max   = 32767
  short overflow = -32768
  World population = 8000000000
  ...
  0.1 + 0.2 == 0.3 ? 0
  0.1 + 0.2 = 0.3
  ...
  ASCII of 'A' = 65
  ...
  7/2 as int:    3
  7/2 as double: 3.5
*/
