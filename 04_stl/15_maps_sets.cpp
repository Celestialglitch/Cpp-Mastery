/*
================================================================================
  LESSON 15: map, unordered_map, set, and other STL containers
================================================================================

  THEORY — STL CONTAINERS:
  -------------------------
  The STL provides many container types. Choose based on your needs:

  SEQUENCE CONTAINERS (ordered by position):
  -------------------------------------------
  vector       — dynamic array, O(1) random access, O(n) insert/erase middle
  deque        — double-ended queue, O(1) push/pop front and back
  list         — doubly-linked list, O(1) insert/erase anywhere, O(n) access
  array        — fixed-size array (stack), O(1) access
  forward_list — singly-linked list

  ASSOCIATIVE CONTAINERS (ordered by key, tree-based):
  ------------------------------------------------------
  map          — key-value pairs, sorted by key, O(log n) operations
  set          — unique sorted values, O(log n) operations
  multimap     — map allowing duplicate keys
  multiset     — set allowing duplicates

  UNORDERED CONTAINERS (hash-based, no order):
  ---------------------------------------------
  unordered_map    — key-value pairs, O(1) average operations
  unordered_set    — unique values, O(1) average operations
  unordered_multimap, unordered_multiset

  CONTAINER ADAPTERS:
  -------------------
  stack   — LIFO (last in, first out)
  queue   — FIFO (first in, first out)
  priority_queue — heap-based, always gives max (or min) element

================================================================================
*/

#include <iostream>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <stack>
#include <queue>
#include <deque>
#include <list>
#include <string>
#include <vector>
#include <algorithm>

int main() {

    // =========================================================================
    // SECTION 1: std::map
    // =========================================================================

    std::cout << "=== std::map ===" << std::endl;
    // Internally a RED-BLACK TREE. Keys always sorted. O(log n) operations.

    std::map<std::string, int> ages;

    // Insert methods:
    ages["Alice"]   = 30;                          // operator[] (creates if not exists)
    ages["Bob"]     = 25;
    ages.insert({"Charlie", 35});                  // insert pair
    ages.insert(std::make_pair("Dave", 28));        // make_pair
    ages.emplace("Eve", 22);                       // construct in-place (most efficient)

    // Iteration (always sorted by key):
    std::cout << "All entries (sorted by key):" << std::endl;
    for (const auto& [key, value] : ages) {        // structured binding (C++17)
        std::cout << "  " << key << " -> " << value << std::endl;
    }

    // Access:
    std::cout << "Alice's age: " << ages["Alice"] << std::endl;
    // WARNING: ages["Unknown"] creates entry with value 0!
    // Use find() to check without creating:
    auto it = ages.find("Bob");
    if (it != ages.end()) {
        std::cout << "Bob found: " << it->second << std::endl;
        // it->first = key, it->second = value
    }

    // count() — returns 0 or 1 for map (1 if key exists):
    std::cout << "Has Frank? " << ages.count("Frank") << std::endl;  // 0

    // contains() — C++20:
    // std::cout << ages.contains("Alice") << std::endl;

    // Modify:
    ages["Alice"] = 31;
    std::cout << "Alice updated: " << ages["Alice"] << std::endl;

    // Erase:
    ages.erase("Dave");
    std::cout << "After erase Dave, size: " << ages.size() << std::endl;

    // Erase by iterator:
    auto it2 = ages.find("Eve");
    if (it2 != ages.end()) ages.erase(it2);

    // Lower/upper bound:
    std::map<int, std::string> scores = {{60,"D"},{70,"C"},{80,"B"},{90,"A"}};
    auto lb = scores.lower_bound(75);  // first key >= 75
    std::cout << "lower_bound(75): " << lb->first << " -> " << lb->second << std::endl;

    // =========================================================================
    // SECTION 2: std::unordered_map
    // =========================================================================

    std::cout << "\n=== std::unordered_map ===" << std::endl;
    // Hash table. O(1) average, O(n) worst case. No ordering.

    std::unordered_map<std::string, int> word_count;

    std::string text = "the quick brown fox jumps over the lazy dog the fox";
    std::string current_word;  // renamed to avoid shadowing outer 'word' variable
    for (char c : text) {
        if (c == ' ') {
            if (!current_word.empty()) {
                word_count[current_word]++;
                current_word = "";
            }
        } else {
            current_word += c;
        }
    }
    if (!current_word.empty()) word_count[current_word]++;

    std::cout << "Word frequencies:" << std::endl;
    for (const auto& [w, count] : word_count) {
        std::cout << "  " << w << ": " << count << std::endl;
    }

    // Find most frequent word:
    std::string most_frequent;
    int max_count = 0;
    for (const auto& [w, count] : word_count) {
        if (count > max_count) { max_count = count; most_frequent = w; }
    }
    std::cout << "Most frequent: '" << most_frequent << "' (" << max_count << " times)" << std::endl;

    // Performance: reserve buckets to avoid rehashing:
    std::unordered_map<int, int> fast_map;
    fast_map.reserve(1000);  // pre-allocate for 1000 elements
    fast_map.max_load_factor(0.25);  // keep load factor low for faster lookups

    // =========================================================================
    // SECTION 3: std::set
    // =========================================================================

    std::cout << "\n=== std::set ===" << std::endl;
    // Sorted unique values. Red-black tree. O(log n) operations.

    std::set<int> s = {5, 3, 8, 1, 3, 5, 9, 2};  // duplicates removed!
    std::cout << "Set (unique, sorted): ";
    for (int x : s) std::cout << x << " ";  // 1 2 3 5 8 9
    std::cout << std::endl;

    // Insert:
    s.insert(4);
    s.insert(5);  // already exists, ignored
    std::cout << "After insert 4,5: ";
    for (int x : s) std::cout << x << " ";
    std::cout << std::endl;

    // Check membership:
    std::cout << "Contains 8? " << s.count(8) << std::endl;  // 1
    std::cout << "Contains 7? " << s.count(7) << std::endl;  // 0

    // Erase:
    s.erase(3);
    std::cout << "After erase 3: ";
    for (int x : s) std::cout << x << " ";
    std::cout << std::endl;

    // Set operations:
    std::set<int> A = {1, 2, 3, 4, 5};
    std::set<int> B = {3, 4, 5, 6, 7};

    // Intersection:
    std::vector<int> intersection;
    std::set_intersection(A.begin(), A.end(), B.begin(), B.end(),
                          std::back_inserter(intersection));
    std::cout << "A ∩ B: ";
    for (int x : intersection) std::cout << x << " ";
    std::cout << std::endl;

    // Union:
    std::vector<int> union_set;
    std::set_union(A.begin(), A.end(), B.begin(), B.end(),
                   std::back_inserter(union_set));
    std::cout << "A ∪ B: ";
    for (int x : union_set) std::cout << x << " ";
    std::cout << std::endl;

    // Difference:
    std::vector<int> diff;
    std::set_difference(A.begin(), A.end(), B.begin(), B.end(),
                        std::back_inserter(diff));
    std::cout << "A - B: ";
    for (int x : diff) std::cout << x << " ";
    std::cout << std::endl;

    // Remove duplicates from vector using set:
    std::vector<int> dupes = {1, 2, 2, 3, 3, 3, 4, 1};
    std::set<int> unique_set(dupes.begin(), dupes.end());
    std::vector<int> unique_vec(unique_set.begin(), unique_set.end());
    std::cout << "Unique: ";
    for (int x : unique_vec) std::cout << x << " ";
    std::cout << std::endl;

    // =========================================================================
    // SECTION 4: std::stack
    // =========================================================================

    std::cout << "\n=== std::stack (LIFO) ===" << std::endl;

    std::stack<int> stk;
    stk.push(10);
    stk.push(20);
    stk.push(30);

    std::cout << "Top: " << stk.top() << std::endl;  // 30
    std::cout << "Size: " << stk.size() << std::endl;

    while (!stk.empty()) {
        std::cout << stk.top() << " ";
        stk.pop();
    }
    std::cout << std::endl;  // 30 20 10

    // Use case: balanced parentheses checker
    auto is_balanced = [](const std::string& expr) -> bool {
        std::stack<char> st;
        for (char c : expr) {
            if (c == '(' || c == '[' || c == '{') {
                st.push(c);
            } else if (c == ')' || c == ']' || c == '}') {
                if (st.empty()) return false;
                char top = st.top(); st.pop();
                if ((c == ')' && top != '(') ||
                    (c == ']' && top != '[') ||
                    (c == '}' && top != '{')) return false;
            }
        }
        return st.empty();
    };

    std::cout << "({[]}) balanced: " << is_balanced("({[]})") << std::endl;  // 1
    std::cout << "({[)}) balanced: " << is_balanced("({[)})") << std::endl;  // 0

    // =========================================================================
    // SECTION 5: std::queue
    // =========================================================================

    std::cout << "\n=== std::queue (FIFO) ===" << std::endl;

    std::queue<std::string> q;
    q.push("Task 1");
    q.push("Task 2");
    q.push("Task 3");

    std::cout << "Front: " << q.front() << std::endl;  // Task 1
    std::cout << "Back:  " << q.back()  << std::endl;  // Task 3

    while (!q.empty()) {
        std::cout << "Processing: " << q.front() << std::endl;
        q.pop();
    }

    // =========================================================================
    // SECTION 6: std::priority_queue
    // =========================================================================

    std::cout << "\n=== std::priority_queue ===" << std::endl;

    // Max-heap by default (largest element at top):
    std::priority_queue<int> pq;
    pq.push(30);
    pq.push(10);
    pq.push(50);
    pq.push(20);

    std::cout << "Max-heap order: ";
    while (!pq.empty()) {
        std::cout << pq.top() << " ";
        pq.pop();
    }
    std::cout << std::endl;  // 50 30 20 10

    // Min-heap:
    std::priority_queue<int, std::vector<int>, std::greater<int>> min_pq;
    min_pq.push(30); min_pq.push(10); min_pq.push(50); min_pq.push(20);

    std::cout << "Min-heap order: ";
    while (!min_pq.empty()) {
        std::cout << min_pq.top() << " ";
        min_pq.pop();
    }
    std::cout << std::endl;  // 10 20 30 50

    // =========================================================================
    // SECTION 7: std::deque
    // =========================================================================

    std::cout << "\n=== std::deque ===" << std::endl;
    // Double-ended queue: O(1) push/pop at BOTH ends

    std::deque<int> dq = {3, 4, 5};
    dq.push_front(2);  // add to front
    dq.push_front(1);
    dq.push_back(6);   // add to back
    dq.push_back(7);

    std::cout << "Deque: ";
    for (int x : dq) std::cout << x << " ";
    std::cout << std::endl;

    dq.pop_front();  // remove from front
    dq.pop_back();   // remove from back

    std::cout << "After pop front/back: ";
    for (int x : dq) std::cout << x << " ";
    std::cout << std::endl;

    // =========================================================================
    // SECTION 8: std::list
    // =========================================================================

    std::cout << "\n=== std::list ===" << std::endl;
    // Doubly-linked list: O(1) insert/erase anywhere (with iterator), O(n) access

    std::list<int> lst = {1, 2, 3, 4, 5};

    // Insert in middle:
    auto lit = lst.begin();
    std::advance(lit, 2);  // move to index 2
    lst.insert(lit, 99);   // insert before index 2

    std::cout << "After insert: ";
    for (int x : lst) std::cout << x << " ";
    std::cout << std::endl;

    // Remove all occurrences of a value:
    lst.push_back(3);
    lst.remove(3);  // removes ALL 3s
    std::cout << "After remove(3): ";
    for (int x : lst) std::cout << x << " ";
    std::cout << std::endl;

    // Sort and unique:
    std::list<int> lst2 = {5, 3, 1, 4, 1, 5, 2};
    lst2.sort();
    lst2.unique();  // remove consecutive duplicates
    std::cout << "Sorted unique: ";
    for (int x : lst2) std::cout << x << " ";
    std::cout << std::endl;

    return 0;
}
