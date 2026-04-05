/*
================================================================================
  LESSON 8: Arrays and Strings — Complete Guide
================================================================================

  THEORY — ARRAYS:
  ----------------
  An array is a CONTIGUOUS block of memory holding multiple values of the SAME type.
  "Contiguous" means all elements are stored next to each other in memory.

  Memory layout for int arr[5]:
  Address:  1000  1004  1008  1012  1016
  Value:    [10]  [20]  [30]  [40]  [50]
  Index:     [0]   [1]   [2]   [3]   [4]

  Each int = 4 bytes, so arr[1] is at address 1000 + 1*4 = 1004.
  This is why array indexing is O(1) — direct address calculation.

  ARRAY LIMITATIONS:
  ------------------
  - Fixed size (decided at compile time for stack arrays)
  - No bounds checking — accessing arr[10] on a 5-element array is UNDEFINED BEHAVIOR
  - Cannot be resized
  - Cannot be returned from functions directly (use std::vector instead)

  THEORY — STRINGS:
  -----------------
  Two kinds of strings in C++:
  1. C-style strings: char arrays ending with '\0' (null terminator)
     char name[] = "Alice";  // stored as: A l i c e \0
  2. std::string: a class that manages a char array for you (use this!)

================================================================================
*/

#include <iostream>
#include <string>
#include <algorithm>  // for sort, reverse, etc.
#include <cctype>     // for toupper, tolower, isdigit, isalpha

int main() {

    // =========================================================================
    // SECTION 1: ARRAY BASICS
    // =========================================================================

    std::cout << "=== ARRAY BASICS ===" << std::endl;

    // Declaration and initialization:
    int scores[5] = {90, 85, 78, 92, 88};

    // Partial initialization — rest filled with 0:
    int partial[5] = {1, 2};  // {1, 2, 0, 0, 0}

    // Zero-initialize all (two equivalent ways):
    int zeros[5]  = {};   // {0, 0, 0, 0, 0}
    int zeros2[5] = {0};  // same result

    // Size deduced from initializer (compiler counts the elements):
    int auto_size[] = {10, 20, 30, 40, 50};  // compiler knows size = 5

    // Suppress unused-variable warnings for demo variables:
    (void)partial; (void)zeros; (void)zeros2; (void)auto_size;

    // Access by index (0-based):
    std::cout << "scores[0] = " << scores[0] << std::endl;  // 90
    std::cout << "scores[4] = " << scores[4] << std::endl;  // 88

    // Modify:
    scores[2] = 95;
    std::cout << "scores[2] after modify = " << scores[2] << std::endl;

    // Get size:
    int size = sizeof(scores) / sizeof(scores[0]);
    std::cout << "Array size = " << size << std::endl;  // 5

    // Loop through:
    std::cout << "All scores: ";
    for (int i = 0; i < size; i++) {
        std::cout << scores[i] << " ";
    }
    std::cout << std::endl;

    // Range-based for:
    std::cout << "Range-based: ";
    for (int s : scores) std::cout << s << " ";
    std::cout << std::endl;

    // =========================================================================
    // SECTION 2: ARRAY ALGORITHMS
    // =========================================================================

    std::cout << "\n=== ARRAY ALGORITHMS ===" << std::endl;

    int data[] = {64, 34, 25, 12, 22, 11, 90};
    int n = sizeof(data) / sizeof(data[0]);

    // Sum and average:
    int sum = 0;
    for (int i = 0; i < n; i++) sum += data[i];
    double avg = (double)sum / n;
    std::cout << "Sum = " << sum << ", Avg = " << avg << std::endl;

    // Max and min:
    int max_val = data[0], min_val = data[0];
    for (int i = 1; i < n; i++) {
        if (data[i] > max_val) max_val = data[i];
        if (data[i] < min_val) min_val = data[i];
    }
    std::cout << "Max = " << max_val << ", Min = " << min_val << std::endl;

    // Linear search:
    int target = 25;
    int found = -1;
    for (int i = 0; i < n; i++) {
        if (data[i] == target) { found = i; break; }
    }
    std::cout << "Search " << target << ": index " << found << std::endl;

    // Bubble sort (classic, O(n^2)):
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int arr_n = 7;
    for (int i = 0; i < arr_n - 1; i++) {
        for (int j = 0; j < arr_n - i - 1; j++) {
            if (arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
    std::cout << "Sorted: ";
    for (int i = 0; i < arr_n; i++) std::cout << arr[i] << " ";
    std::cout << std::endl;

    // Reverse an array:
    int rev[] = {1, 2, 3, 4, 5};
    int rev_n = 5;
    for (int i = 0, j = rev_n-1; i < j; i++, j--) {
        int temp = rev[i]; rev[i] = rev[j]; rev[j] = temp;
    }
    std::cout << "Reversed: ";
    for (int x : rev) std::cout << x << " ";
    std::cout << std::endl;

    // =========================================================================
    // SECTION 3: 2D ARRAYS
    // =========================================================================

    std::cout << "\n=== 2D ARRAYS ===" << std::endl;

    // Matrix: rows x cols
    int matrix[3][4] = {
        {1,  2,  3,  4},   // row 0
        {5,  6,  7,  8},   // row 1
        {9, 10, 11, 12}    // row 2
    };

    // Access: matrix[row][col]
    std::cout << "matrix[1][2] = " << matrix[1][2] << std::endl;  // 7

    // Print matrix:
    std::cout << "Matrix:" << std::endl;
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 4; col++) {
            std::cout.width(4);
            std::cout << matrix[row][col];
        }
        std::cout << std::endl;
    }

    // Matrix transpose:
    int orig[3][3] = {{1,2,3},{4,5,6},{7,8,9}};
    int trans[3][3];
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            trans[j][i] = orig[i][j];

    std::cout << "Transposed:" << std::endl;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            std::cout.width(3);
            std::cout << trans[i][j];
        }
        std::cout << std::endl;
    }

    // =========================================================================
    // SECTION 4: C-STYLE STRINGS (char arrays)
    // =========================================================================

    std::cout << "\n=== C-STYLE STRINGS ===" << std::endl;

    // Stored as array of chars ending with '\0' (null terminator):
    char name[] = "Alice";  // A l i c e \0  (6 bytes, not 5!)
    char greeting[] = {'H', 'e', 'l', 'l', 'o', '\0'};  // explicit null

    std::cout << "name = " << name << std::endl;
    std::cout << "greeting = " << greeting << std::endl;

    // Access individual chars:
    std::cout << "name[0] = " << name[0] << std::endl;  // A

    // Iterate until null terminator:
    std::cout << "Chars: ";
    for (int i = 0; name[i] != '\0'; i++) {
        std::cout << name[i] << " ";
    }
    std::cout << std::endl;

    // =========================================================================
    // SECTION 5: std::string — THE RIGHT WAY
    // =========================================================================

    std::cout << "\n=== std::string ===" << std::endl;

    std::string s1 = "Hello";
    std::string s2 = "World";
    std::string s3;  // empty string

    // Length:
    std::cout << "s1.length() = " << s1.length() << std::endl;  // 5
    std::cout << "s1.size()   = " << s1.size()   << std::endl;  // same
    std::cout << "s3.empty()  = " << s3.empty()  << std::endl;  // 1 (true)

    // Concatenation:
    std::string full = s1 + " " + s2;
    std::cout << "full = " << full << std::endl;  // Hello World

    s1 += "!";
    std::cout << "s1 after += : " << s1 << std::endl;  // Hello!

    // Access characters:
    std::cout << "full[0] = " << full[0] << std::endl;  // H
    std::cout << "full.at(6) = " << full.at(6) << std::endl;  // W (bounds-checked)
    std::cout << "full.front() = " << full.front() << std::endl;  // H
    std::cout << "full.back()  = " << full.back()  << std::endl;  // d

    // Substring:
    std::string sub = full.substr(6, 5);  // start=6, length=5
    std::cout << "substr(6,5) = " << sub << std::endl;  // World

    // Find:
    size_t pos = full.find("World");
    std::cout << "find('World') = " << pos << std::endl;  // 6

    if (full.find("xyz") == std::string::npos) {
        std::cout << "'xyz' not found (npos)" << std::endl;
    }

    // Find from position:
    std::string text = "banana";
    size_t p = text.find('a');
    while (p != std::string::npos) {
        std::cout << "'a' at index " << p << std::endl;
        p = text.find('a', p + 1);  // search from next position
    }

    // Replace:
    std::string sentence = "I love cats and cats are great";
    size_t rpos = sentence.find("cats");
    while (rpos != std::string::npos) {
        sentence.replace(rpos, 4, "dogs");  // replace 4 chars at rpos with "dogs"
        rpos = sentence.find("cats", rpos + 4);
    }
    std::cout << "Replaced: " << sentence << std::endl;

    // Insert and erase:
    std::string word = "Hello World";
    word.insert(5, " Beautiful");  // insert at index 5
    std::cout << "After insert: " << word << std::endl;

    word.erase(5, 10);  // erase 10 chars starting at index 5
    std::cout << "After erase:  " << word << std::endl;

    // Compare:
    std::string a_str = "apple", b_str = "banana";
    std::cout << "apple == banana: " << (a_str == b_str) << std::endl;  // 0
    std::cout << "apple <  banana: " << (a_str <  b_str) << std::endl;  // 1 (lexicographic)

    // =========================================================================
    // SECTION 6: STRING MANIPULATION
    // =========================================================================

    std::cout << "\n=== STRING MANIPULATION ===" << std::endl;

    std::string str = "Hello, World! 123";

    // Convert to uppercase/lowercase:
    std::string upper = str, lower = str;
    for (char& c : upper) c = toupper(c);
    for (char& c : lower) c = tolower(c);
    std::cout << "Upper: " << upper << std::endl;
    std::cout << "Lower: " << lower << std::endl;

    // Count characters:
    int vowels = 0, digits = 0, spaces = 0;
    for (char c : str) {
        if (std::string("aeiouAEIOU").find(c) != std::string::npos) vowels++;
        if (isdigit(c)) digits++;
        if (isspace(c)) spaces++;
    }
    std::cout << "Vowels: " << vowels << " Digits: " << digits << " Spaces: " << spaces << std::endl;

    // Reverse a string:
    std::string rev_str = "Hello";
    std::reverse(rev_str.begin(), rev_str.end());
    std::cout << "Reversed: " << rev_str << std::endl;  // olleH

    // Check palindrome:
    auto is_palindrome = [](std::string s) {
        std::string rev = s;
        std::reverse(rev.begin(), rev.end());
        return s == rev;
    };
    std::cout << "racecar palindrome: " << is_palindrome("racecar") << std::endl;  // 1
    std::cout << "hello palindrome:   " << is_palindrome("hello")   << std::endl;  // 0

    // Split string by delimiter (manual):
    std::string csv = "Alice,30,Engineer";
    std::vector<std::string> tokens;
    std::string token;
    for (char c : csv) {
        if (c == ',') {
            tokens.push_back(token);
            token = "";
        } else {
            token += c;
        }
    }
    tokens.push_back(token);  // last token
    std::cout << "CSV tokens: ";
    for (const std::string& t : tokens) std::cout << "[" << t << "] ";
    std::cout << std::endl;

    // String to number:
    std::string num_str = "42";
    int num = std::stoi(num_str);       // string to int
    double dbl = std::stod("3.14");     // string to double
    std::cout << "stoi: " << num << " stod: " << dbl << std::endl;

    // Number to string:
    std::string from_num = std::to_string(12345);
    std::string from_dbl = std::to_string(3.14);
    std::cout << "to_string: " << from_num << " " << from_dbl << std::endl;

    return 0;
}
