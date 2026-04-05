/*
================================================================================
  LESSON 4: Input and Output — Complete Guide
================================================================================

  THEORY:
  -------
  C++ I/O is built around STREAMS — sequences of characters flowing in/out.

  STANDARD STREAMS:
  -----------------
  std::cin   — standard input  (keyboard by default)
  std::cout  — standard output (terminal by default)
  std::cerr  — standard error  (terminal, unbuffered)
  std::clog  — standard log    (terminal, buffered)

  BUFFERING:
  ----------
  Output is often BUFFERED — characters are collected in memory and written
  to the terminal in batches (more efficient).
  - std::endl flushes the buffer immediately (slower but ensures output appears)
  - '\n' just adds a newline without flushing (faster)
  - std::flush explicitly flushes without newline

  STREAM STATE:
  -------------
  After reading, cin can be in a "fail" state if input doesn't match expected type.
  Always check and clear stream state when reading user input.

  STREAM MANIPULATORS (from <iomanip>):
  --------------------------------------
  std::setw(n)        — set field width
  std::setprecision(n)— set decimal precision
  std::fixed          — fixed-point notation
  std::scientific     — scientific notation
  std::left/right     — alignment
  std::setfill(c)     — fill character
  std::hex/oct/dec    — number base

================================================================================
*/

#include <iostream>
#include <iomanip>   // for formatting manipulators
#include <string>
#include <limits>    // for numeric_limits

int main() {

    // =========================================================================
    // SECTION 1: BASIC OUTPUT
    // =========================================================================

    std::cout << "=== BASIC OUTPUT ===" << std::endl;

    // Printing different types:
    std::cout << "Integer: "    << 42          << std::endl;
    std::cout << "Float: "      << 3.14        << std::endl;
    std::cout << "Character: "  << 'A'         << std::endl;
    std::cout << "Boolean: "    << true        << std::endl;  // prints 1
    std::cout << "String: "     << "Hello"     << std::endl;

    // Multiple values on one line:
    int age = 25;
    std::string name = "Alice";
    std::cout << name << " is " << age << " years old." << std::endl;

    // =========================================================================
    // SECTION 2: ESCAPE SEQUENCES
    // =========================================================================

    std::cout << "\n=== ESCAPE SEQUENCES ===" << std::endl;

    std::cout << "Newline:    line1\nline2"     << std::endl;
    std::cout << "Tab:        col1\tcol2\tcol3" << std::endl;
    std::cout << "Backslash:  C:\\Users\\Alice"  << std::endl;
    std::cout << "Quote:      \"Hello World\""  << std::endl;
    std::cout << "Single:     \'A\'"            << std::endl;
    std::cout << "Bell:       \a"               << std::endl;  // may beep
    std::cout << "Carriage:   overwrite\rNEW"   << std::endl;  // \r goes to line start

    // =========================================================================
    // SECTION 3: OUTPUT FORMATTING with <iomanip>
    // =========================================================================

    std::cout << "\n=== FORMATTING ===" << std::endl;

    // --- Number bases ---
    int num = 255;
    std::cout << "Decimal:     " << std::dec << num << std::endl;  // 255
    std::cout << "Hexadecimal: " << std::hex << num << std::endl;  // ff
    std::cout << "Octal:       " << std::oct << num << std::endl;  // 377
    std::cout << std::dec;  // reset to decimal

    // Show base prefix:
    std::cout << std::showbase;
    std::cout << "Hex with prefix: " << std::hex << num << std::endl;  // 0xff
    std::cout << "Oct with prefix: " << std::oct << num << std::endl;  // 0377
    std::cout << std::noshowbase << std::dec;

    // --- Floating point precision ---
    double pi = 3.14159265358979323846;

    std::cout << "\nDefault:     " << pi << std::endl;             // 3.14159
    std::cout << "precision(2):" << std::setprecision(2) << pi << std::endl;  // 3.1
    std::cout << "precision(10):" << std::setprecision(10) << pi << std::endl;
    std::cout << std::setprecision(6);  // reset

    // Fixed vs scientific:
    double big = 123456789.0;
    double small_d = 0.000001234;
    std::cout << "\nFixed:      " << std::fixed      << std::setprecision(2) << big   << std::endl;
    std::cout << "Scientific: " << std::scientific  << std::setprecision(2) << big   << std::endl;
    std::cout << "Scientific: " << std::scientific  << std::setprecision(4) << small_d << std::endl;
    std::cout << std::defaultfloat << std::setprecision(6);  // reset

    // --- Field width and alignment ---
    std::cout << "\n--- Table formatting ---" << std::endl;
    std::cout << std::left;  // left-align
    std::cout << std::setw(15) << "Name"
              << std::setw(8)  << "Age"
              << std::setw(10) << "Score" << std::endl;
    std::cout << std::string(33, '-') << std::endl;
    std::cout << std::setw(15) << "Alice"
              << std::setw(8)  << 25
              << std::setw(10) << 95.5 << std::endl;
    std::cout << std::setw(15) << "Bob"
              << std::setw(8)  << 30
              << std::setw(10) << 87.3 << std::endl;
    std::cout << std::setw(15) << "Charlie"
              << std::setw(8)  << 22
              << std::setw(10) << 91.0 << std::endl;

    // Right-align numbers:
    std::cout << "\n--- Right-aligned numbers ---" << std::endl;
    std::cout << std::right;
    std::cout << std::setw(10) << 1      << std::endl;
    std::cout << std::setw(10) << 100    << std::endl;
    std::cout << std::setw(10) << 10000  << std::endl;

    // Fill character:
    std::cout << "\n--- Fill character ---" << std::endl;
    std::cout << std::setfill('0') << std::setw(6) << 42   << std::endl;  // 000042
    std::cout << std::setfill('*') << std::setw(10) << "hi" << std::endl; // ********hi
    std::cout << std::setfill(' ');  // reset fill

    // =========================================================================
    // SECTION 4: READING INPUT with cin
    // =========================================================================

    std::cout << "\n=== INPUT ===" << std::endl;
    std::cout << "NOTE: This section asks for keyboard input." << std::endl;
    std::cout << "      Follow each prompt and press Enter." << std::endl;

    // --- Reading a single value ---
    int user_age;
    std::cout << "\nEnter your age (type a number, e.g. 25, then press Enter): ";
    std::cin >> user_age;
    std::cout << "You entered: " << user_age << std::endl;

    // --- Reading multiple values ---
    int x, y;
    std::cout << "\nEnter two integers separated by space (e.g. 5 10), then Enter: ";
    std::cin >> x >> y;
    std::cout << "x=" << x << " y=" << y << " sum=" << (x+y) << std::endl;

    // --- Reading a word (stops at whitespace) ---
    std::string word;
    std::cout << "\nEnter one word (no spaces), then Enter: ";
    std::cin >> word;
    std::cout << "Word: " << word << std::endl;

    // --- Reading a full line (including spaces) ---
    // IMPORTANT: After cin >>, there's a leftover '\n' in the buffer.
    // getline reads until '\n', so it would immediately return empty string.
    // Fix: use cin.ignore() to discard the leftover newline.
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string full_line;
    std::cout << "\nEnter a full sentence (spaces allowed), then Enter: ";
    std::getline(std::cin, full_line);
    std::cout << "You said: " << full_line << std::endl;

    // =========================================================================
    // SECTION 5: INPUT VALIDATION
    // =========================================================================

    std::cout << "\n=== INPUT VALIDATION ===" << std::endl;
    std::cout << "Enter a positive integer (try typing 'abc' first to see error handling): " << std::endl;

    int valid_num;
    while (true) {
        std::cout << "Enter a positive integer: ";
        std::cin >> valid_num;

        if (std::cin.fail()) {
            // Input was not an integer
            std::cin.clear();  // clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // discard bad input
            std::cout << "Invalid input! Please enter a number." << std::endl;
        } else if (valid_num <= 0) {
            std::cout << "Must be positive! Try again." << std::endl;
        } else {
            break;  // valid input, exit loop
        }
    }
    std::cout << "Valid input: " << valid_num << std::endl;

    // =========================================================================
    // SECTION 6: cerr and clog
    // =========================================================================

    std::cout << "\n=== cerr and clog ===" << std::endl;

    // cerr: unbuffered error output (appears immediately)
    std::cerr << "ERROR: Something went wrong!" << std::endl;

    // clog: buffered log output
    std::clog << "LOG: Program started successfully." << std::endl;

    // In production, you'd redirect these:
    // ./program 2>errors.log    (redirect stderr to file)
    // ./program >output.log 2>&1 (redirect both to same file)

    // =========================================================================
    // SECTION 7: READING NUMBERS IN DIFFERENT BASES
    // =========================================================================

    std::cout << "\n=== READING HEX/OCT ===" << std::endl;

    int hex_val, oct_val;
    std::cout << "Enter a hex number (e.g. ff): ";
    std::cin >> std::hex >> hex_val;
    std::cout << "Hex ff = decimal " << std::dec << hex_val << std::endl;

    return 0;
}

/*
  EXPECTED OUTPUT (partial):
  --------------------------
  === BASIC OUTPUT ===
  Integer: 42
  Float: 3.14
  Character: A
  Boolean: 1
  String: Hello
  Alice is 25 years old.

  === FORMATTING ===
  Decimal:     255
  Hexadecimal: ff
  Octal:       377

  --- Table formatting ---
  Name           Age     Score
  ---------------------------------
  Alice          25      95.5
  Bob            30      87.3
  Charlie        22      91
*/
