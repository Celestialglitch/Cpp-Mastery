/*
================================================================================
  LESSON 10: Memory Management — Stack vs Heap
================================================================================

  THEORY — MEMORY REGIONS:
  -------------------------
  A running C++ program uses several memory regions:

  HIGH ADDRESS
  ┌─────────────────┐
  │   Stack         │ ← grows downward
  │   (local vars)  │   fast, automatic, limited (~1-8 MB)
  ├─────────────────┤
  │   (free space)  │
  ├─────────────────┤
  │   Heap          │ ← grows upward
  │   (dynamic mem) │   slower, manual, large (limited by RAM)
  ├─────────────────┤
  │   BSS segment   │ ← uninitialized global/static vars
  ├─────────────────┤
  │   Data segment  │ ← initialized global/static vars
  ├─────────────────┤
  │   Text segment  │ ← your compiled code (read-only)
  LOW ADDRESS

  STACK:
  ------
  - Automatic: variables created/destroyed automatically
  - Fast: just move a stack pointer register
  - Limited: typically 1-8 MB (stack overflow = crash)
  - LIFO: Last In, First Out
  - All local variables, function parameters, return addresses

  HEAP:
  -----
  - Manual: YOU control allocation and deallocation
  - Slower: OS must find free block, update bookkeeping
  - Large: limited only by available RAM
  - Fragmentation: holes can appear over time
  - Use 'new' to allocate, 'delete' to free

  RAII (Resource Acquisition Is Initialization):
  -----------------------------------------------
  The C++ idiom for safe resource management:
  - Acquire resource in constructor
  - Release resource in destructor
  - When object goes out of scope, destructor runs automatically
  - Smart pointers implement RAII for heap memory

================================================================================
*/

#include <iostream>
#include <memory>    // smart pointers
#include <string>
#include <vector>

// =========================================================================
// HELPER CLASS to visualize construction/destruction
// =========================================================================

class Resource {
public:
    std::string name;
    int* data;

    Resource(std::string n, int val) : name(n) {
        data = new int(val);  // allocate on heap
        std::cout << "  [+] Resource '" << name << "' created, data=" << *data << std::endl;
    }

    ~Resource() {
        std::cout << "  [-] Resource '" << name << "' destroyed, data=" << *data << std::endl;
        delete data;  // free heap memory
        data = nullptr;
    }
};

// =========================================================================
// SECTION: STACK DEMO
// =========================================================================

void stack_demo() {
    std::cout << "\n--- Inside stack_demo() ---" << std::endl;
    int local_a = 10;   // on stack
    int local_b = 20;   // on stack
    Resource r("stack_resource", 42);  // on stack (but r.data is on heap!)

    std::cout << "local_a=" << local_a << " local_b=" << local_b << std::endl;
    std::cout << "--- Leaving stack_demo() — locals destroyed ---" << std::endl;
    // local_a, local_b, r all destroyed here automatically
    // r's destructor frees r.data from heap
}

int main() {

    // =========================================================================
    // SECTION 1: STACK MEMORY
    // =========================================================================

    std::cout << "=== STACK MEMORY ===" << std::endl;

    {
        int x = 10;  // created on stack
        int y = 20;
        std::cout << "x=" << x << " at " << &x << std::endl;
        std::cout << "y=" << y << " at " << &y << std::endl;
        // x and y destroyed when this block ends
    }
    // x and y are GONE here

    stack_demo();
    std::cout << "Back in main — stack_demo locals are gone." << std::endl;

    // Stack overflow example (don't run this!):
    // void infinite_recursion() { infinite_recursion(); }
    // infinite_recursion();  // CRASH: stack overflow

    // =========================================================================
    // SECTION 2: HEAP MEMORY — new and delete
    // =========================================================================

    std::cout << "\n=== HEAP MEMORY ===" << std::endl;

    // Allocate a single int:
    int* heap_int = new int;      // allocate (uninitialized)
    *heap_int = 42;               // initialize
    std::cout << "heap_int = " << *heap_int << " at " << heap_int << std::endl;
    delete heap_int;              // FREE — must do this!
    heap_int = nullptr;           // good practice: null after delete

    // Allocate with initialization:
    int* heap_val = new int(100);
    std::cout << "heap_val = " << *heap_val << std::endl;
    delete heap_val;
    heap_val = nullptr;

    // Allocate a double:
    double* heap_d = new double(3.14);
    std::cout << "heap_d = " << *heap_d << std::endl;
    delete heap_d;

    // Allocate an object:
    Resource* heap_res = new Resource("heap_resource", 99);
    std::cout << "Using heap resource: " << heap_res->name << std::endl;
    delete heap_res;  // calls destructor, then frees memory
    heap_res = nullptr;

    // =========================================================================
    // SECTION 3: DYNAMIC ARRAYS
    // =========================================================================

    std::cout << "\n=== DYNAMIC ARRAYS ===" << std::endl;

    // Size determined at runtime:
    int size = 5;
    int* dyn_arr = new int[size];  // allocate array on heap

    // Initialize:
    for (int i = 0; i < size; i++) {
        dyn_arr[i] = (i + 1) * 10;
    }

    // Use:
    std::cout << "Dynamic array: ";
    for (int i = 0; i < size; i++) {
        std::cout << dyn_arr[i] << " ";
    }
    std::cout << std::endl;

    // MUST use delete[] for arrays (not delete):
    delete[] dyn_arr;
    dyn_arr = nullptr;

    // 2D dynamic array:
    int rows = 3, cols = 4;
    int** matrix = new int*[rows];  // array of pointers
    for (int i = 0; i < rows; i++) {
        matrix[i] = new int[cols];  // each row is a separate array
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = i * cols + j;
        }
    }

    std::cout << "2D dynamic array:" << std::endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout.width(4);
            std::cout << matrix[i][j];
        }
        std::cout << std::endl;
    }

    // Free 2D array (reverse order):
    for (int i = 0; i < rows; i++) {
        delete[] matrix[i];  // free each row
    }
    delete[] matrix;  // free array of pointers
    matrix = nullptr;

    // =========================================================================
    // SECTION 4: MEMORY BUGS (explained)
    // =========================================================================

    std::cout << "\n=== MEMORY BUGS ===" << std::endl;

    // --- MEMORY LEAK ---
    // Allocate but never delete:
    // int* leak = new int(5);
    // (program ends, OS reclaims, but in long-running programs this accumulates!)
    std::cout << "Memory leak: allocate without delete (see comment)" << std::endl;

    // --- DANGLING POINTER ---
    int* dangling = new int(42);
    std::cout << "Before delete: " << *dangling << std::endl;
    delete dangling;
    dangling = nullptr;  // FIX: null it after delete
    // *dangling = 5;  // UNDEFINED BEHAVIOR if not nulled!
    if (dangling == nullptr) {
        std::cout << "Dangling pointer safely nulled." << std::endl;
    }

    // --- DOUBLE DELETE ---
    // int* p = new int(5);
    // delete p;
    // delete p;  // CRASH! heap corruption

    // --- BUFFER OVERFLOW ---
    // int* arr = new int[3];
    // arr[5] = 10;  // writing past end — corrupts heap!

    // =========================================================================
    // SECTION 5: SMART POINTERS (RAII — the right way)
    // =========================================================================

    std::cout << "\n=== SMART POINTERS ===" << std::endl;

    // unique_ptr: single owner, auto-deleted when out of scope
    {
        std::unique_ptr<Resource> uptr = std::make_unique<Resource>("unique", 1);
        std::cout << "Using: " << uptr->name << std::endl;
        // No delete needed! Destructor called automatically when uptr goes out of scope
    }
    std::cout << "unique_ptr scope ended — resource freed." << std::endl;

    // shared_ptr: multiple owners, deleted when last owner gone
    {
        std::shared_ptr<Resource> sp1 = std::make_shared<Resource>("shared", 2);
        std::cout << "ref count = " << sp1.use_count() << std::endl;  // 1
        {
            std::shared_ptr<Resource> sp2 = sp1;  // share ownership
            std::cout << "ref count = " << sp1.use_count() << std::endl;  // 2
        }  // sp2 gone, count drops to 1
        std::cout << "ref count = " << sp1.use_count() << std::endl;  // 1
    }  // sp1 gone, count = 0, resource freed
    std::cout << "shared_ptr scope ended." << std::endl;

    // unique_ptr with array:
    {
        std::unique_ptr<int[]> arr = std::make_unique<int[]>(5);
        for (int i = 0; i < 5; i++) arr[i] = i * i;
        std::cout << "Smart array: ";
        for (int i = 0; i < 5; i++) std::cout << arr[i] << " ";
        std::cout << std::endl;
        // delete[] called automatically
    }

    // =========================================================================
    // SECTION 6: PLACEMENT NEW (advanced)
    // =========================================================================

    std::cout << "\n=== PLACEMENT NEW ===" << std::endl;

    // Construct object at a specific memory location:
    alignas(int) char buffer[sizeof(int)];  // raw memory
    int* placed = new (buffer) int(777);    // construct int at buffer's address
    std::cout << "Placed int: " << *placed << std::endl;
    placed->~int();  // explicitly call destructor (needed for placement new)

    // =========================================================================
    // SECTION 7: MEMORY PROFILING TIPS
    // =========================================================================

    std::cout << "\n=== MEMORY TIPS ===" << std::endl;
    std::cout << "Tools to detect memory issues:" << std::endl;
    std::cout << "  Valgrind:    valgrind --leak-check=full ./program" << std::endl;
    std::cout << "  AddressSanitizer: g++ -fsanitize=address ./program" << std::endl;
    std::cout << "  Visual Studio: built-in memory diagnostics" << std::endl;
    std::cout << "  Dr. Memory:  Windows alternative to Valgrind" << std::endl;

    std::cout << "\nGolden rules:" << std::endl;
    std::cout << "  1. Every new  → exactly one delete"  << std::endl;
    std::cout << "  2. Every new[] → exactly one delete[]" << std::endl;
    std::cout << "  3. Prefer smart pointers over raw new/delete" << std::endl;
    std::cout << "  4. Null pointers after delete" << std::endl;
    std::cout << "  5. Never dereference null or dangling pointers" << std::endl;

    return 0;
}
