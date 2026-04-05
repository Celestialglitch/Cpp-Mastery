/*
================================================================================
  LESSON 6: Loops — Complete Guide
================================================================================

  THEORY:
  -------
  A loop repeats a block of code multiple times.
  Without loops, you'd have to write the same code hundreds of times.

  THREE TYPES:
  ------------
  1. for loop    — when you know HOW MANY times to repeat
  2. while loop  — when you repeat WHILE a condition is true (unknown count)
  3. do-while    — like while, but runs AT LEAST ONCE

  LOOP ANATOMY (for loop):
  -------------------------
  for (initialization; condition; update) {
      body
  }

  EXECUTION ORDER:
  ----------------
  1. initialization (runs ONCE at start)
  2. condition check (if false, exit loop)
  3. body executes
  4. update runs
  5. go back to step 2

  INFINITE LOOP:
  --------------
  A loop that never ends. Usually a bug, but sometimes intentional
  (game loops, server loops). Always have an exit condition!

================================================================================
*/

#include <iostream>
#include <string>
#include <vector>

int main() {

    // =========================================================================
    // SECTION 1: FOR LOOP
    // =========================================================================

    std::cout << "=== FOR LOOP ===" << std::endl;

    // Basic counting:
    std::cout << "Count 0-4: ";
    for (int i = 0; i < 5; i++) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    // After loop: i is DESTROYED (out of scope)

    // Count DOWN:
    std::cout << "Count 5-1: ";
    for (int i = 5; i >= 1; i--) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    // Step by 2:
    std::cout << "Even 0-10: ";
    for (int i = 0; i <= 10; i += 2) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    // Step by 3:
    std::cout << "Step 3:    ";
    for (int i = 0; i <= 15; i += 3) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    // Multiple variables in for loop:
    std::cout << "Two vars:  ";
    for (int i = 0, j = 10; i < j; i++, j--) {
        std::cout << "(" << i << "," << j << ") ";
    }
    std::cout << std::endl;

    // Infinite for loop (with break):
    std::cout << "Find first multiple of 7 > 50: ";
    for (int i = 51; ; i++) {  // no condition = infinite
        if (i % 7 == 0) {
            std::cout << i << std::endl;
            break;
        }
    }

    // =========================================================================
    // SECTION 2: WHILE LOOP
    // =========================================================================

    std::cout << "\n=== WHILE LOOP ===" << std::endl;

    // Basic while:
    int count = 1;
    std::cout << "Powers of 2: ";
    while (count <= 1024) {
        std::cout << count << " ";
        count *= 2;
    }
    std::cout << std::endl;

    // While with unknown iterations — reading until sentinel:
    // (simulating user input with a vector)
    std::vector<int> inputs = {5, 3, 8, -1, 2};  // -1 is the "stop" signal
    int idx = 0;
    int sum = 0;
    std::cout << "Sum until -1: ";
    while (inputs[idx] != -1) {
        std::cout << inputs[idx] << " ";
        sum += inputs[idx];
        idx++;
    }
    std::cout << "= " << sum << std::endl;

    // Collatz conjecture (unknown iterations):
    int n = 27;
    int steps = 0;
    std::cout << "Collatz(" << n << "): ";
    while (n != 1) {
        if (n % 2 == 0) n /= 2;
        else             n = 3 * n + 1;
        steps++;
    }
    std::cout << steps << " steps to reach 1" << std::endl;

    // =========================================================================
    // SECTION 3: DO-WHILE LOOP
    // =========================================================================

    std::cout << "\n=== DO-WHILE ===" << std::endl;

    // Runs body FIRST, then checks condition.
    // Guarantees at least ONE execution.

    int x = 10;
    do {
        std::cout << "x = " << x << std::endl;
        x++;
    } while (x < 5);  // false immediately, but body ran once!
    // Output: x = 10 (ran once even though 10 < 5 is false)

    // Classic use: menu-driven programs
    int choice;
    do {
        std::cout << "\nMenu: 1=Hello 2=Bye 0=Exit" << std::endl;
        std::cout << "Choice: ";
        // Simulating user input:
        static int sim_inputs[] = {1, 2, 0};
        static int sim_idx = 0;
        choice = sim_inputs[sim_idx++];
        std::cout << choice << std::endl;  // show simulated input

        switch (choice) {
            case 1: std::cout << "Hello!" << std::endl; break;
            case 2: std::cout << "Bye!"   << std::endl; break;
            case 0: std::cout << "Exiting..." << std::endl; break;
            default: std::cout << "Invalid choice" << std::endl;
        }
    } while (choice != 0);

    // =========================================================================
    // SECTION 4: BREAK and CONTINUE
    // =========================================================================

    std::cout << "\n=== BREAK ===" << std::endl;

    // break: EXIT the loop immediately
    std::cout << "Find first prime > 20: ";
    for (int num = 21; num < 100; num++) {
        bool is_prime = true;
        for (int d = 2; d * d <= num; d++) {
            if (num % d == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            std::cout << num << std::endl;
            break;  // found it, stop outer loop
        }
    }

    std::cout << "\n=== CONTINUE ===" << std::endl;

    // continue: SKIP rest of current iteration, go to next
    std::cout << "Odd numbers 1-10: ";
    for (int i = 1; i <= 10; i++) {
        if (i % 2 == 0) continue;  // skip even numbers
        std::cout << i << " ";
    }
    std::cout << std::endl;

    // Skip multiples of 3:
    std::cout << "1-15 skip multiples of 3: ";
    for (int i = 1; i <= 15; i++) {
        if (i % 3 == 0) continue;
        std::cout << i << " ";
    }
    std::cout << std::endl;

    // =========================================================================
    // SECTION 5: NESTED LOOPS
    // =========================================================================

    std::cout << "\n=== NESTED LOOPS ===" << std::endl;

    // Multiplication table:
    std::cout << "Multiplication table (1-5):" << std::endl;
    for (int row = 1; row <= 5; row++) {
        for (int col = 1; col <= 5; col++) {
            std::cout.width(4);
            std::cout << (row * col);
        }
        std::cout << std::endl;
    }

    // Pattern printing:
    std::cout << "\nTriangle pattern:" << std::endl;
    for (int i = 1; i <= 5; i++) {
        for (int j = 1; j <= i; j++) {
            std::cout << "* ";
        }
        std::cout << std::endl;
    }

    std::cout << "\nInverted triangle:" << std::endl;
    for (int i = 5; i >= 1; i--) {
        for (int j = 1; j <= i; j++) {
            std::cout << "* ";
        }
        std::cout << std::endl;
    }

    std::cout << "\nDiamond pattern:" << std::endl;
    int size = 5;
    for (int i = 1; i <= size; i++) {
        for (int s = size - i; s > 0; s--) std::cout << " ";
        for (int j = 1; j <= 2*i-1; j++) std::cout << "*";
        std::cout << std::endl;
    }
    for (int i = size-1; i >= 1; i--) {
        for (int s = size - i; s > 0; s--) std::cout << " ";
        for (int j = 1; j <= 2*i-1; j++) std::cout << "*";
        std::cout << std::endl;
    }

    // Breaking out of nested loops with goto (rare but valid):
    std::cout << "\nFind pair summing to 10:" << std::endl;
    bool found = false;
    for (int i = 1; i <= 9 && !found; i++) {
        for (int j = i+1; j <= 9 && !found; j++) {
            if (i + j == 10) {
                std::cout << i << " + " << j << " = 10" << std::endl;
                found = true;  // flag to break outer loop
            }
        }
    }

    // =========================================================================
    // SECTION 6: RANGE-BASED FOR (Modern C++)
    // =========================================================================

    std::cout << "\n=== RANGE-BASED FOR ===" << std::endl;

    // Works with arrays, vectors, strings, and any container:
    int arr[] = {10, 20, 30, 40, 50};
    std::cout << "Array: ";
    for (int val : arr) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // With vector:
    std::vector<std::string> names = {"Alice", "Bob", "Charlie"};
    for (const std::string& name : names) {  // const& avoids copying
        std::cout << "Hello, " << name << "!" << std::endl;
    }

    // Modifying elements (use reference &):
    std::vector<int> nums = {1, 2, 3, 4, 5};
    for (int& num : nums) {
        num *= 2;  // doubles each element in-place
    }
    std::cout << "Doubled: ";
    for (int n : nums) std::cout << n << " ";
    std::cout << std::endl;

    // With string (iterate characters):
    std::string word = "Hello";
    std::cout << "Characters: ";
    for (char c : word) {
        std::cout << c << "-";
    }
    std::cout << std::endl;

    // =========================================================================
    // SECTION 7: LOOP ALGORITHMS (common patterns)
    // =========================================================================

    std::cout << "\n=== COMMON PATTERNS ===" << std::endl;

    std::vector<int> data = {3, 7, 1, 9, 4, 6, 2, 8, 5};

    // Sum:
    int total = 0;
    for (int v : data) total += v;
    std::cout << "Sum: " << total << std::endl;

    // Max and Min:
    int max_val = data[0], min_val = data[0];
    for (int v : data) {
        if (v > max_val) max_val = v;
        if (v < min_val) min_val = v;
    }
    std::cout << "Max: " << max_val << " Min: " << min_val << std::endl;

    // Count occurrences:
    std::vector<int> grades = {85, 90, 85, 70, 90, 85, 95};
    int count_85 = 0;
    for (int g : grades) if (g == 85) count_85++;
    std::cout << "Count of 85: " << count_85 << std::endl;

    // Linear search:
    int target = 9;
    int found_idx = -1;
    for (int i = 0; i < (int)data.size(); i++) {
        if (data[i] == target) { found_idx = i; break; }
    }
    std::cout << "Found " << target << " at index: " << found_idx << std::endl;

    return 0;
}
