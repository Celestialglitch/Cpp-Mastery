/*
================================================================================
  LESSON 1: Hello World — How a C++ Program Works
================================================================================

  THEORY:
  -------
  A C++ program is a set of instructions written in text files (.cpp).
  Before your computer can run it, it must be COMPILED — translated from
  human-readable C++ into machine code (binary 0s and 1s).

  THE COMPILATION PIPELINE:
  --------------------------
  Source code (.cpp)
       ↓  [Preprocessor]  — handles #include, #define, macros
  Preprocessed code
       ↓  [Compiler]      — translates to assembly/object code
  Object file (.o / .obj)
       ↓  [Linker]        — combines object files + libraries into executable
  Executable (.exe / a.out)

  EVERY C++ PROGRAM MUST HAVE:
  -----------------------------
  1. At least one #include (usually <iostream> for I/O)
  2. A main() function — this is where execution STARTS
  3. main() must return an int (0 = success, non-zero = error)

  NAMESPACES:
  -----------
  C++ organizes code into "namespaces" to avoid name collisions.
  The standard library lives in the 'std' namespace.
  So 'cout' is actually 'std::cout' — cout inside the std namespace.

  You can write 'using namespace std;' to avoid typing std:: everywhere,
  but in professional code, explicit std:: is preferred to avoid conflicts.

  EXECUTION FLOW:
  ---------------
  1. OS loads your executable into memory
  2. Runtime setup (stack, heap initialization)
  3. main() is called
  4. Statements execute TOP TO BOTTOM, one by one
  5. main() returns 0
  6. OS cleans up resources

================================================================================
*/

#include <iostream>   // Preprocessor directive: copy-paste iostream header here
                      // iostream provides: cout, cin, cerr, clog, endl

// Program entry point. 'int' = return type. 'main' = special name OS looks for.
int main() {

    // ------------------------------------------------------------------
    // std::cout — Standard Character Output
    // ------------------------------------------------------------------
    // 'std'  = namespace
    // '::'   = scope resolution operator (access something inside namespace)
    // 'cout' = character output stream (connected to your terminal)
    // '<<'   = stream insertion operator (send data INTO the stream)
    // '"Hello, World!"' = a string literal (text in double quotes)
    // 'std::endl' = end-of-line + flush the buffer

    std::cout << "Hello, World!" << std::endl;

    // ------------------------------------------------------------------
    // CHAINING the << operator
    // ------------------------------------------------------------------
    // You can chain multiple << to print several things on one line.
    // Each << sends the next piece to cout.
    std::cout << "My name is " << "Learner" << " and I am learning C++" << std::endl;

    // ------------------------------------------------------------------
    // '\n' vs std::endl
    // ------------------------------------------------------------------
    // Both move to the next line, but:
    // '\n'      = just a newline character (FAST)
    // std::endl = newline + flushes the output buffer (SLOWER)
    // Use '\n' in performance-sensitive code, endl when you need immediate output.

    std::cout << "Line 1\n";
    std::cout << "Line 2\n";
    std::cout << "Line 3" << std::endl;  // flush here

    // ------------------------------------------------------------------
    // Printing numbers and expressions directly
    // ------------------------------------------------------------------
    std::cout << "2 + 3 = " << 2 + 3 << std::endl;       // 5
    std::cout << "10 / 4 = " << 10 / 4 << std::endl;     // 2 (integer division!)
    std::cout << "10.0/4 = " << 10.0 / 4 << std::endl;   // 2.5 (float division)

    // ------------------------------------------------------------------
    // std::cerr — Standard Error Output
    // ------------------------------------------------------------------
    // Use cerr for error messages. It's unbuffered (immediate output).
    // In terminals, both cout and cerr appear on screen by default,
    // but they can be redirected separately.
    std::cerr << "This is an error message (goes to stderr)" << std::endl;

    // ------------------------------------------------------------------
    // RETURN VALUE of main()
    // ------------------------------------------------------------------
    // 0 = success (EXIT_SUCCESS)
    // Any non-zero = failure (EXIT_FAILURE)
    // The OS/shell can read this value after your program exits.
    // In bash: echo $?   (shows last program's return code)
    return 0;
}

/*
  HOW TO COMPILE AND RUN:
  -----------------------
  Windows (MinGW/MSVC):
    g++ 01_hello_world.cpp -o hello.exe
    hello.exe

  Linux/Mac:
    g++ 01_hello_world.cpp -o hello
    ./hello

  With C++17 standard (recommended):
    g++ -std=c++17 01_hello_world.cpp -o hello

  EXPECTED OUTPUT:
  ----------------
  Hello, World!
  My name is Learner and I am learning C++
  Line 1
  Line 2
  Line 3
  2 + 3 = 5
  10 / 4 = 2
  10.0/4 = 2.5
  This is an error message (goes to stderr)
*/
