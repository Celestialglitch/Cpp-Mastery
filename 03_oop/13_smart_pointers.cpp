/*
================================================================================
  LESSON 13: Smart Pointers — Modern Memory Management
================================================================================

  THEORY:
  -------
  Raw pointers (new/delete) are error-prone:
  - Easy to forget delete → memory leak
  - Easy to delete twice → crash
  - Easy to use after delete → undefined behavior

  Smart pointers are WRAPPER CLASSES around raw pointers.
  They use RAII: destructor automatically calls delete when pointer goes out of scope.

  THREE SMART POINTERS:
  ----------------------
  unique_ptr<T>  — EXCLUSIVE ownership. One owner. Zero overhead.
  shared_ptr<T>  — SHARED ownership. Reference counted. Deleted when count=0.
  weak_ptr<T>    — NON-OWNING observer of a shared_ptr. Breaks cycles.

  OWNERSHIP MODEL:
  ----------------
  unique_ptr:  [A] owns resource. A destroyed → resource freed.
  shared_ptr:  [A] and [B] share resource. Both destroyed → resource freed.
  weak_ptr:    [A] owns, [W] observes. A destroyed → resource freed (W becomes expired).

  WHEN TO USE WHICH:
  ------------------
  unique_ptr → default choice (90% of cases). Single owner, no overhead.
  shared_ptr → when multiple objects need to share ownership.
  weak_ptr   → to break circular references, or non-owning observers.

================================================================================
*/

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// =========================================================================
// HELPER CLASS
// =========================================================================

class Widget {
public:
    std::string name;
    int value;

    Widget(std::string n, int v) : name(n), value(v) {
        std::cout << "  [+] Widget '" << name << "' created (value=" << value << ")" << std::endl;
    }

    ~Widget() {
        std::cout << "  [-] Widget '" << name << "' destroyed" << std::endl;
    }

    void use() const {
        std::cout << "  Using Widget '" << name << "' (value=" << value << ")" << std::endl;
    }
};

// =========================================================================
// SECTION 1: unique_ptr
// =========================================================================

void demo_unique_ptr() {
    std::cout << "\n=== unique_ptr ===" << std::endl;

    // Create:
    std::unique_ptr<Widget> p1 = std::make_unique<Widget>("Alpha", 1);
    // make_unique is preferred over: unique_ptr<Widget> p1(new Widget(...))
    // because make_unique is exception-safe

    p1->use();                          // use -> like raw pointer
    std::cout << "p1 name: " << p1->name << std::endl;
    std::cout << "p1 value: " << (*p1).value << std::endl;  // dereference with *

    // Check if valid:
    if (p1) {
        std::cout << "p1 is valid" << std::endl;
    }

    // CANNOT copy unique_ptr (exclusive ownership):
    // std::unique_ptr<Widget> p2 = p1;  // COMPILE ERROR

    // CAN move (transfer ownership):
    std::unique_ptr<Widget> p2 = std::move(p1);  // p1 gives up ownership
    std::cout << "After move:" << std::endl;
    std::cout << "p1 is " << (p1 ? "valid" : "null") << std::endl;  // null
    std::cout << "p2 is " << (p2 ? "valid" : "null") << std::endl;  // valid
    p2->use();

    // Release: give up ownership without deleting
    Widget* raw = p2.release();  // p2 is now null, raw is a raw pointer
    std::cout << "After release: p2 is " << (p2 ? "valid" : "null") << std::endl;
    delete raw;  // must manually delete now!

    // Reset: delete current resource, optionally assign new one
    std::unique_ptr<Widget> p3 = std::make_unique<Widget>("Beta", 2);
    p3.reset(new Widget("Gamma", 3));  // Beta destroyed, p3 now owns Gamma
    p3.reset();  // Gamma destroyed, p3 is now null

    // Scope-based destruction:
    {
        std::unique_ptr<Widget> scoped = std::make_unique<Widget>("Scoped", 99);
        scoped->use();
        std::cout << "  (leaving scope)" << std::endl;
    }  // Scoped destroyed here automatically
    std::cout << "After scope" << std::endl;

    // unique_ptr with array:
    std::unique_ptr<int[]> arr = std::make_unique<int[]>(5);
    for (int i = 0; i < 5; i++) arr[i] = i * i;
    std::cout << "Array: ";
    for (int i = 0; i < 5; i++) std::cout << arr[i] << " ";
    std::cout << std::endl;
    // delete[] called automatically
}

// =========================================================================
// SECTION 2: shared_ptr
// =========================================================================

void demo_shared_ptr() {
    std::cout << "\n=== shared_ptr ===" << std::endl;

    // Create:
    std::shared_ptr<Widget> sp1 = std::make_shared<Widget>("Shared", 42);
    std::cout << "ref count: " << sp1.use_count() << std::endl;  // 1

    // Copy — BOTH own the resource:
    std::shared_ptr<Widget> sp2 = sp1;
    std::cout << "ref count after copy: " << sp1.use_count() << std::endl;  // 2

    {
        std::shared_ptr<Widget> sp3 = sp1;
        std::cout << "ref count inside block: " << sp1.use_count() << std::endl;  // 3
        sp3->use();
    }  // sp3 destroyed, count drops to 2
    std::cout << "ref count after block: " << sp1.use_count() << std::endl;  // 2

    sp2.reset();  // sp2 releases ownership, count drops to 1
    std::cout << "ref count after reset: " << sp1.use_count() << std::endl;  // 1

    // sp1 goes out of scope at end of function → count=0 → Widget destroyed

    // Sharing in a vector:
    std::vector<std::shared_ptr<Widget>> owners;
    auto shared = std::make_shared<Widget>("Shared2", 100);
    owners.push_back(shared);
    owners.push_back(shared);
    owners.push_back(shared);
    std::cout << "Shared by " << shared.use_count() << " owners" << std::endl;  // 4 (shared + 3 in vector)
}

// =========================================================================
// SECTION 3: weak_ptr
// =========================================================================

// CIRCULAR REFERENCE PROBLEM:
// If A has shared_ptr to B, and B has shared_ptr to A,
// neither will ever be destroyed (ref count never reaches 0)!

struct Node {
    std::string name;
    std::shared_ptr<Node> next;   // strong reference
    std::weak_ptr<Node>   prev;   // weak reference (breaks cycle)

    Node(std::string n) : name(n) {
        std::cout << "  [+] Node '" << name << "' created" << std::endl;
    }
    ~Node() {
        std::cout << "  [-] Node '" << name << "' destroyed" << std::endl;
    }
};

void demo_weak_ptr() {
    std::cout << "\n=== weak_ptr ===" << std::endl;

    // Basic weak_ptr usage:
    std::shared_ptr<Widget> owner = std::make_shared<Widget>("Observed", 7);
    std::weak_ptr<Widget> observer = owner;  // doesn't increase ref count

    std::cout << "owner ref count: " << owner.use_count() << std::endl;  // 1 (not 2!)

    // To USE a weak_ptr, you must LOCK it (get a temporary shared_ptr):
    if (auto temp = observer.lock()) {  // lock() returns shared_ptr or nullptr
        temp->use();
        std::cout << "Resource is alive, ref count during lock: " << owner.use_count() << std::endl;  // 2
    }
    // temp goes out of scope, count back to 1

    // Check if expired:
    std::cout << "observer expired? " << observer.expired() << std::endl;  // 0 (false)

    owner.reset();  // release ownership
    std::cout << "After owner.reset():" << std::endl;
    std::cout << "observer expired? " << observer.expired() << std::endl;  // 1 (true)

    if (observer.lock() == nullptr) {
        std::cout << "Resource is gone — weak_ptr correctly shows expired" << std::endl;
    }

    // Doubly-linked list without circular reference leak:
    std::cout << "\n--- Doubly-linked list ---" << std::endl;
    auto n1 = std::make_shared<Node>("Node1");
    auto n2 = std::make_shared<Node>("Node2");
    auto n3 = std::make_shared<Node>("Node3");

    n1->next = n2;  // n1 → n2 (strong)
    n2->next = n3;  // n2 → n3 (strong)
    n2->prev = n1;  // n2 ← n1 (weak — no cycle!)
    n3->prev = n2;  // n3 ← n2 (weak — no cycle!)

    // Traverse forward:
    std::cout << "Forward: ";
    auto current = n1;
    while (current) {
        std::cout << current->name << " ";
        current = current->next;
    }
    std::cout << std::endl;

    // Traverse backward from n3:
    std::cout << "Backward from n3: ";
    auto cur = n3;
    while (cur) {
        std::cout << cur->name << " ";
        if (auto p = cur->prev.lock()) cur = p;
        else break;
    }
    std::cout << std::endl;

    std::cout << "--- Nodes going out of scope ---" << std::endl;
    // n1, n2, n3 destroyed here — no leak because weak_ptr breaks cycle
}

// =========================================================================
// SECTION 4: Custom Deleter
// =========================================================================

void demo_custom_deleter() {
    std::cout << "\n=== Custom Deleter ===" << std::endl;

    // Sometimes you need custom cleanup logic:
    auto custom_delete = [](Widget* w) {
        std::cout << "  Custom deleter called for: " << w->name << std::endl;
        delete w;
    };

    std::unique_ptr<Widget, decltype(custom_delete)> p(
        new Widget("Custom", 55), custom_delete
    );
    p->use();
    // custom_delete called when p goes out of scope

    // Useful for C-style resources (FILE*, etc.):
    auto file_deleter = [](FILE* f) {
        if (f) { fclose(f); std::cout << "  File closed." << std::endl; }
    };
    std::unique_ptr<FILE, decltype(file_deleter)> file(
        fopen("test_smart.txt", "w"), file_deleter
    );
    if (file) {
        fputs("Hello from smart pointer!", file.get());
    }
    // file automatically closed when unique_ptr goes out of scope
}

int main() {
    demo_unique_ptr();
    demo_shared_ptr();
    demo_weak_ptr();
    demo_custom_deleter();

    std::cout << "\n=== SUMMARY ===" << std::endl;
    std::cout << "unique_ptr: single owner, zero overhead, use by default" << std::endl;
    std::cout << "shared_ptr: shared ownership, ref-counted, slight overhead" << std::endl;
    std::cout << "weak_ptr:   non-owning observer, breaks cycles" << std::endl;

    return 0;
}
