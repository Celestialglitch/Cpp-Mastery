/*
================================================================================
  LESSON 14: std::vector — Dynamic Arrays
================================================================================

  THEORY:
  -------
  std::vector is a DYNAMIC ARRAY — it can grow and shrink at runtime.
  It's the most used container in C++. When in doubt, use vector.

  INTERNAL MECHANICS:
  -------------------
  vector maintains:
  - data:     pointer to heap-allocated array
  - size:     number of elements currently stored
  - capacity: total allocated space (>= size)

  When you push_back and size == capacity:
  1. Allocate new array (typically 2x capacity)
  2. Copy/move all elements to new array
  3. Free old array
  This is O(n) but happens rarely → amortized O(1) per push_back.

  MEMORY LAYOUT:
  --------------
  vector<int> v = {1, 2, 3};
  Stack: [ptr | size=3 | cap=3]
  Heap:  [1][2][3]

  ITERATOR INVALIDATION:
  ----------------------
  After push_back (if reallocation occurs), ALL iterators are invalidated!
  After insert/erase, iterators at/after the point are invalidated.
  Always re-obtain iterators after modifying the vector.

================================================================================
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>    // for accumulate
#include <string>

int main() {

    // =========================================================================
    // SECTION 1: CREATING VECTORS
    // =========================================================================

    std::cout << "=== CREATING VECTORS ===" << std::endl;

    std::vector<int> empty;                          // empty
    std::vector<int> from_list = {5, 3, 8, 1, 9};  // from initializer list
    std::vector<int> n_copies(5, 42);               // 5 elements, all 42
    std::vector<int> n_zeros(10);                   // 10 elements, all 0
    std::vector<std::string> strings = {"hello", "world", "cpp"};

    // From another vector:
    std::vector<int> copy_of = from_list;           // copy
    std::vector<int> moved   = std::move(copy_of);  // move (copy_of is now empty)

    // From range:
    std::vector<int> sub(from_list.begin(), from_list.begin() + 3);  // first 3 elements

    std::cout << "from_list: ";
    for (int x : from_list) std::cout << x << " ";
    std::cout << std::endl;

    std::cout << "n_copies:  ";
    for (int x : n_copies) std::cout << x << " ";
    std::cout << std::endl;

    // =========================================================================
    // SECTION 2: SIZE AND CAPACITY
    // =========================================================================

    std::cout << "\n=== SIZE AND CAPACITY ===" << std::endl;

    std::vector<int> v;
    std::cout << "empty: size=" << v.size() << " cap=" << v.capacity() << std::endl;

    v.push_back(1);
    std::cout << "after push 1: size=" << v.size() << " cap=" << v.capacity() << std::endl;

    v.push_back(2);
    std::cout << "after push 2: size=" << v.size() << " cap=" << v.capacity() << std::endl;

    v.push_back(3);
    std::cout << "after push 3: size=" << v.size() << " cap=" << v.capacity() << std::endl;

    // Reserve: pre-allocate capacity (avoids reallocations)
    v.reserve(100);
    std::cout << "after reserve(100): size=" << v.size() << " cap=" << v.capacity() << std::endl;

    // Resize: change size (adds default-initialized elements if growing)
    v.resize(6, 99);  // grow to 6, fill new elements with 99
    std::cout << "after resize(6,99): ";
    for (int x : v) std::cout << x << " ";
    std::cout << std::endl;

    v.resize(2);  // shrink to 2 (elements beyond 2 are removed)
    std::cout << "after resize(2): ";
    for (int x : v) std::cout << x << " ";
    std::cout << std::endl;

    // shrink_to_fit: release excess capacity
    v.shrink_to_fit();
    std::cout << "after shrink_to_fit: cap=" << v.capacity() << std::endl;

    // =========================================================================
    // SECTION 3: ADDING AND REMOVING ELEMENTS
    // =========================================================================

    std::cout << "\n=== ADD/REMOVE ===" << std::endl;

    std::vector<int> nums = {10, 20, 30, 40, 50};

    // push_back: add to end (amortized O(1))
    nums.push_back(60);
    std::cout << "After push_back(60): ";
    for (int x : nums) std::cout << x << " ";
    std::cout << std::endl;

    // pop_back: remove from end (O(1))
    nums.pop_back();
    std::cout << "After pop_back: ";
    for (int x : nums) std::cout << x << " ";
    std::cout << std::endl;

    // emplace_back: construct in-place (more efficient than push_back for objects)
    std::vector<std::string> words;
    words.emplace_back("hello");   // constructs string directly in vector
    words.emplace_back("world");
    words.push_back("cpp");        // also works but may copy

    // insert: add at specific position (O(n) — shifts elements)
    auto it = nums.begin() + 2;  // iterator to index 2
    nums.insert(it, 99);          // insert 99 at index 2
    std::cout << "After insert(2, 99): ";
    for (int x : nums) std::cout << x << " ";
    std::cout << std::endl;

    // insert multiple:
    nums.insert(nums.begin(), {1, 2, 3});  // insert at beginning
    std::cout << "After insert 3 at front: ";
    for (int x : nums) std::cout << x << " ";
    std::cout << std::endl;

    // erase: remove at position (O(n) — shifts elements)
    nums.erase(nums.begin());  // remove first element
    std::cout << "After erase first: ";
    for (int x : nums) std::cout << x << " ";
    std::cout << std::endl;

    // erase range:
    nums.erase(nums.begin(), nums.begin() + 2);  // remove first 2
    std::cout << "After erase first 2: ";
    for (int x : nums) std::cout << x << " ";
    std::cout << std::endl;

    // clear: remove all elements (size=0, capacity unchanged)
    std::vector<int> temp = {1, 2, 3};
    temp.clear();
    std::cout << "After clear: size=" << temp.size() << std::endl;

    // =========================================================================
    // SECTION 4: ACCESSING ELEMENTS
    // =========================================================================

    std::cout << "\n=== ACCESS ===" << std::endl;

    std::vector<int> data = {10, 20, 30, 40, 50};

    // operator[] — no bounds check (fast but unsafe):
    std::cout << "data[0] = " << data[0] << std::endl;
    std::cout << "data[4] = " << data[4] << std::endl;
    // data[10] = 5;  // UNDEFINED BEHAVIOR — no crash guaranteed

    // at() — bounds checked (throws std::out_of_range if invalid):
    std::cout << "data.at(2) = " << data.at(2) << std::endl;
    try {
        std::cout << data.at(10) << std::endl;  // throws!
    } catch (const std::out_of_range& e) {
        std::cout << "Caught: " << e.what() << std::endl;
    }

    // front() and back():
    std::cout << "front = " << data.front() << std::endl;  // 10
    std::cout << "back  = " << data.back()  << std::endl;  // 50

    // data() — raw pointer to underlying array:
    int* raw = data.data();
    std::cout << "raw[0] = " << raw[0] << std::endl;  // 10

    // =========================================================================
    // SECTION 5: ITERATING
    // =========================================================================

    std::cout << "\n=== ITERATING ===" << std::endl;

    std::vector<int> v2 = {1, 2, 3, 4, 5};

    // Index-based:
    for (size_t i = 0; i < v2.size(); i++) std::cout << v2[i] << " ";
    std::cout << std::endl;

    // Range-based (cleanest):
    for (int x : v2) std::cout << x << " ";
    std::cout << std::endl;

    // Range-based with reference (modify in-place):
    for (int& x : v2) x *= 2;
    for (int x : v2) std::cout << x << " ";
    std::cout << std::endl;

    // Iterator:
    for (auto it2 = v2.begin(); it2 != v2.end(); ++it2) {
        std::cout << *it2 << " ";
    }
    std::cout << std::endl;

    // Reverse iterator:
    for (auto it2 = v2.rbegin(); it2 != v2.rend(); ++it2) {
        std::cout << *it2 << " ";
    }
    std::cout << std::endl;

    // =========================================================================
    // SECTION 6: ALGORITHMS
    // =========================================================================

    std::cout << "\n=== ALGORITHMS ===" << std::endl;

    std::vector<int> algo = {5, 2, 8, 1, 9, 3, 7, 4, 6};

    // Sort ascending:
    std::sort(algo.begin(), algo.end());
    std::cout << "Sorted asc: ";
    for (int x : algo) std::cout << x << " ";
    std::cout << std::endl;

    // Sort descending:
    std::sort(algo.begin(), algo.end(), std::greater<int>());
    std::cout << "Sorted desc: ";
    for (int x : algo) std::cout << x << " ";
    std::cout << std::endl;

    // Sort with custom comparator (by absolute value):
    std::vector<int> mixed = {-5, 3, -1, 4, -2};
    std::sort(mixed.begin(), mixed.end(), [](int a, int b) {
        return std::abs(a) < std::abs(b);
    });
    std::cout << "Sorted by abs: ";
    for (int x : mixed) std::cout << x << " ";
    std::cout << std::endl;

    // Binary search (requires sorted vector):
    std::sort(algo.begin(), algo.end());
    bool found = std::binary_search(algo.begin(), algo.end(), 5);
    std::cout << "Binary search 5: " << found << std::endl;

    // lower_bound / upper_bound:
    auto lb = std::lower_bound(algo.begin(), algo.end(), 5);  // first >= 5
    auto ub = std::upper_bound(algo.begin(), algo.end(), 5);  // first > 5
    std::cout << "lower_bound(5) index: " << (lb - algo.begin()) << std::endl;
    std::cout << "upper_bound(5) index: " << (ub - algo.begin()) << std::endl;

    // Find:
    auto fit = std::find(algo.begin(), algo.end(), 7);
    if (fit != algo.end()) std::cout << "Found 7 at index " << (fit - algo.begin()) << std::endl;

    // find_if:
    auto fit2 = std::find_if(algo.begin(), algo.end(), [](int x) { return x > 5; });
    if (fit2 != algo.end()) std::cout << "First > 5: " << *fit2 << std::endl;

    // count and count_if:
    std::vector<int> counts = {1, 2, 2, 3, 2, 4, 2};
    std::cout << "Count of 2: " << std::count(counts.begin(), counts.end(), 2) << std::endl;
    std::cout << "Count even: " << std::count_if(counts.begin(), counts.end(), [](int x){ return x%2==0; }) << std::endl;

    // accumulate (sum):
    int sum = std::accumulate(algo.begin(), algo.end(), 0);
    std::cout << "Sum: " << sum << std::endl;

    // min/max element:
    auto min_it = std::min_element(algo.begin(), algo.end());
    auto max_it = std::max_element(algo.begin(), algo.end());
    std::cout << "Min: " << *min_it << " Max: " << *max_it << std::endl;

    // reverse:
    std::reverse(algo.begin(), algo.end());
    std::cout << "Reversed: ";
    for (int x : algo) std::cout << x << " ";
    std::cout << std::endl;

    // unique (remove consecutive duplicates — sort first!):
    std::vector<int> dups = {1, 1, 2, 3, 3, 3, 4, 4, 5};
    auto new_end = std::unique(dups.begin(), dups.end());
    dups.erase(new_end, dups.end());
    std::cout << "After unique: ";
    for (int x : dups) std::cout << x << " ";
    std::cout << std::endl;

    // transform (apply function to each element):
    std::vector<int> src = {1, 2, 3, 4, 5};
    std::vector<int> dst(src.size());
    std::transform(src.begin(), src.end(), dst.begin(), [](int x) { return x * x; });
    std::cout << "Squares: ";
    for (int x : dst) std::cout << x << " ";
    std::cout << std::endl;

    // =========================================================================
    // SECTION 7: 2D VECTOR
    // =========================================================================

    std::cout << "\n=== 2D VECTOR ===" << std::endl;

    // Create 3x4 matrix filled with 0:
    int rows = 3, cols = 4;
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols, 0));

    // Fill with values:
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            matrix[i][j] = i * cols + j + 1;

    // Print:
    for (const auto& row : matrix) {
        for (int val : row) { std::cout.width(4); std::cout << val; }
        std::cout << std::endl;
    }

    // Dynamic rows (jagged array):
    std::vector<std::vector<int>> triangle;
    for (int i = 1; i <= 5; i++) {
        triangle.push_back(std::vector<int>(i, i));  // row i has i copies of i
    }
    std::cout << "Triangle:" << std::endl;
    for (const auto& row : triangle) {
        for (int x : row) std::cout << x << " ";
        std::cout << std::endl;
    }

    return 0;
}
