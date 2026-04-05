/*
================================================================================
  LESSON 9: Pointers — The Heart of C++
================================================================================

  THEORY:
  -------
  Every variable lives at a specific MEMORY ADDRESS in RAM.
  A POINTER is a variable that stores a memory address.

  Think of RAM as a street of houses:
  - Each house = 1 byte of memory
  - Each house has an address (like 0x7ffd1234)
  - A variable is a house (or group of houses) with a name
  - A pointer is a piece of paper with an address written on it

  POINTER SYNTAX:
  ---------------
  int* ptr;       // declare: ptr is a pointer to int
  ptr = &x;       // assign: ptr holds address of x
  *ptr = 5;       // dereference: go to address, write 5 there
  int y = *ptr;   // dereference: go to address, read value

  OPERATORS:
  ----------
  &  = "address of" operator — gives you the address of a variable
  *  = dereference operator  — follows the pointer to get/set the value
  -> = member access through pointer (for structs/classes)

  WHY POINTERS?
  -------------
  1. Pass large objects to functions without copying (efficiency)
  2. Modify variables from inside functions
  3. Dynamic memory allocation (heap)
  4. Build data structures: linked lists, trees, graphs
  5. Polymorphism in OOP
  6. Interfacing with hardware/OS

================================================================================
*/

#include <iostream>
#include <string>

int main() {

    // =========================================================================
    // SECTION 1: WHAT IS A MEMORY ADDRESS?
    // =========================================================================

    std::cout << "=== MEMORY ADDRESSES ===" << std::endl;

    int x = 42;
    double d = 3.14;
    char c = 'A';

    // & gives the address of a variable:
    std::cout << "x = " << x << " lives at address: " << &x << std::endl;
    std::cout << "d = " << d << " lives at address: " << &d << std::endl;
    std::cout << "c = " << c << " lives at address: " << (void*)&c << std::endl;

    // Addresses are just numbers (hexadecimal):
    std::cout << "Address as integer: " << (long long)&x << std::endl;

    // Adjacent variables are close in memory:
    int a = 1, b = 2, e = 3;
    std::cout << "\nAddresses of a, b, e:" << std::endl;
    std::cout << "&a = " << &a << std::endl;
    std::cout << "&b = " << &b << std::endl;
    std::cout << "&e = " << &e << std::endl;
    // Note: they may be 4 bytes apart (size of int)

    // =========================================================================
    // SECTION 2: DECLARING AND USING POINTERS
    // =========================================================================

    std::cout << "\n=== POINTER BASICS ===" << std::endl;

    int val = 100;
    int* ptr = &val;  // ptr holds the address of val

    std::cout << "val       = " << val  << std::endl;   // 100
    std::cout << "&val      = " << &val << std::endl;   // address of val
    std::cout << "ptr       = " << ptr  << std::endl;   // same address
    std::cout << "*ptr      = " << *ptr << std::endl;   // 100 (dereference)
    std::cout << "&ptr      = " << &ptr << std::endl;   // address of the pointer itself

    // Modifying through pointer:
    *ptr = 999;  // go to address in ptr, write 999
    std::cout << "\nAfter *ptr = 999:" << std::endl;
    std::cout << "val  = " << val  << std::endl;  // 999! val changed!
    std::cout << "*ptr = " << *ptr << std::endl;  // 999

    // Pointer and variable are linked — same memory location:
    val = 777;
    std::cout << "\nAfter val = 777:" << std::endl;
    std::cout << "*ptr = " << *ptr << std::endl;  // 777 (reflects change)

    // =========================================================================
    // SECTION 3: POINTER TYPES MUST MATCH
    // =========================================================================

    std::cout << "\n=== POINTER TYPES ===" << std::endl;

    int    i_val = 10;
    double d_val = 3.14;
    char   c_val = 'Z';

    int*    i_ptr = &i_val;
    double* d_ptr = &d_val;
    char*   c_ptr = &c_val;

    std::cout << "*i_ptr = " << *i_ptr << std::endl;
    std::cout << "*d_ptr = " << *d_ptr << std::endl;
    std::cout << "*c_ptr = " << *c_ptr << std::endl;

    // Type matters for pointer arithmetic and dereferencing:
    // int* wrong = &d_val;  // COMPILE ERROR: type mismatch

    // void* — generic pointer (no type, can't dereference directly):
    void* generic = &i_val;
    // *generic = 5;  // ERROR: can't dereference void*
    int* back = (int*)generic;  // cast back to use it
    std::cout << "*back = " << *back << std::endl;

    // =========================================================================
    // SECTION 4: NULL POINTER
    // =========================================================================

    std::cout << "\n=== NULL POINTER ===" << std::endl;

    int* null_ptr = nullptr;  // points to nothing (address 0)

    std::cout << "null_ptr = " << null_ptr << std::endl;  // 0 or 0x0

    // ALWAYS check before dereferencing:
    if (null_ptr == nullptr) {
        std::cout << "Pointer is null — safe!" << std::endl;
    }

    // NEVER do this — it crashes:
    // *null_ptr = 5;  // SEGFAULT! Writing to address 0

    // Assign a real address:
    int real_val = 42;
    null_ptr = &real_val;
    if (null_ptr != nullptr) {
        std::cout << "Now valid: *null_ptr = " << *null_ptr << std::endl;
    }

    // =========================================================================
    // SECTION 5: POINTER ARITHMETIC
    // =========================================================================

    std::cout << "\n=== POINTER ARITHMETIC ===" << std::endl;

    // Pointers can be incremented/decremented.
    // Moving by 1 moves by sizeof(type) bytes.

    int arr[] = {10, 20, 30, 40, 50};
    int* p = arr;  // points to arr[0]

    std::cout << "p   points to arr[0] = " << *p     << std::endl;  // 10
    std::cout << "p+1 points to arr[1] = " << *(p+1) << std::endl;  // 20
    std::cout << "p+2 points to arr[2] = " << *(p+2) << std::endl;  // 30
    std::cout << "p+4 points to arr[4] = " << *(p+4) << std::endl;  // 50

    // Moving the pointer:
    p++;  // now points to arr[1]
    std::cout << "\nAfter p++: *p = " << *p << std::endl;  // 20

    p += 2;  // now points to arr[3]
    std::cout << "After p+=2: *p = " << *p << std::endl;  // 40

    p--;  // back to arr[2]
    std::cout << "After p--: *p = " << *p << std::endl;  // 30

    // Pointer difference (how many elements apart):
    int* start = arr;
    int* end   = arr + 5;
    std::cout << "end - start = " << (end - start) << " elements" << std::endl;  // 5

    // Iterating array with pointer:
    std::cout << "Array via pointer: ";
    for (int* q = arr; q < arr + 5; q++) {
        std::cout << *q << " ";
    }
    std::cout << std::endl;

    // Array name IS a pointer to first element:
    std::cout << "arr == &arr[0]: " << (arr == &arr[0]) << std::endl;  // 1 (true)
    std::cout << "*arr = " << *arr << std::endl;  // 10

    // =========================================================================
    // SECTION 6: CONST POINTERS
    // =========================================================================

    std::cout << "\n=== CONST POINTERS ===" << std::endl;

    int x1 = 10, x2 = 20;

    // 1. Pointer to const int — can't change VALUE through pointer
    const int* ptr1 = &x1;
    // *ptr1 = 50;  // ERROR! value is const
    ptr1 = &x2;    // OK — can change where pointer points
    std::cout << "ptr1 now points to x2: " << *ptr1 << std::endl;

    // 2. Const pointer to int — can't change WHERE pointer points
    int* const ptr2 = &x1;
    *ptr2 = 50;    // OK — can change the value
    // ptr2 = &x2; // ERROR! pointer itself is const
    std::cout << "x1 changed via ptr2: " << x1 << std::endl;

    // 3. Const pointer to const int — can't change either
    const int* const ptr3 = &x2;
    // *ptr3 = 5;  // ERROR
    // ptr3 = &x1; // ERROR
    std::cout << "ptr3 (read-only): " << *ptr3 << std::endl;

    // =========================================================================
    // SECTION 7: POINTER TO POINTER
    // =========================================================================

    std::cout << "\n=== POINTER TO POINTER ===" << std::endl;

    int   v   = 42;
    int*  p1  = &v;    // pointer to int
    int** p2  = &p1;   // pointer to pointer to int
    int*** p3 = &p2;   // pointer to pointer to pointer to int

    std::cout << "v    = " << v     << std::endl;  // 42
    std::cout << "*p1  = " << *p1   << std::endl;  // 42
    std::cout << "**p2 = " << **p2  << std::endl;  // 42
    std::cout << "***p3= " << ***p3 << std::endl;  // 42

    // Modifying through double pointer:
    **p2 = 100;
    std::cout << "After **p2 = 100: v = " << v << std::endl;  // 100

    // =========================================================================
    // SECTION 8: POINTERS AND FUNCTIONS
    // =========================================================================

    std::cout << "\n=== POINTERS IN FUNCTIONS ===" << std::endl;

    // Swap using pointers:
    auto swap_ptr = [](int* a, int* b) {
        int temp = *a;
        *a = *b;
        *b = temp;
    };

    int m = 5, n = 8;
    std::cout << "Before swap: m=" << m << " n=" << n << std::endl;
    swap_ptr(&m, &n);
    std::cout << "After swap:  m=" << m << " n=" << n << std::endl;

    // Function returning pointer (be careful — don't return pointer to local!):
    auto find_max = [](int* arr, int size) -> int* {
        int* max_ptr = arr;
        for (int i = 1; i < size; i++) {
            if (arr[i] > *max_ptr) max_ptr = &arr[i];
        }
        return max_ptr;  // OK — points into arr which lives in caller's scope
    };

    int nums[] = {3, 7, 1, 9, 4};
    int* max_p = find_max(nums, 5);
    std::cout << "Max = " << *max_p << " at index " << (max_p - nums) << std::endl;

    // DANGER — returning pointer to local variable:
    // int* bad_function() {
    //     int local = 42;
    //     return &local;  // UNDEFINED BEHAVIOR! local destroyed after return
    // }

    // =========================================================================
    // SECTION 9: COMMON POINTER BUGS
    // =========================================================================

    std::cout << "\n=== COMMON BUGS (shown safely) ===" << std::endl;

    // 1. Uninitialized pointer (wild pointer):
    // int* wild;       // contains garbage address
    // *wild = 5;       // CRASH — writing to random memory!
    // FIX: always initialize: int* wild = nullptr;

    // 2. Dangling pointer (pointing to freed/destroyed memory):
    // int* dangling;
    // { int temp = 5; dangling = &temp; }  // temp destroyed here
    // *dangling = 10;  // UNDEFINED BEHAVIOR — temp is gone!

    // 3. Memory leak (heap memory not freed):
    // int* leak = new int(5);
    // (never deleted — memory lost until program ends)

    // 4. Double delete:
    // int* p = new int(5);
    // delete p;
    // delete p;  // CRASH — already freed!

    std::cout << "See comments for common pointer bugs to avoid." << std::endl;

    return 0;
}
