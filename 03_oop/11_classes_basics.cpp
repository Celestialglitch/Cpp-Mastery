/*
================================================================================
  LESSON 11: Classes and Objects — Complete OOP Guide
================================================================================

  THEORY:
  -------
  OOP (Object-Oriented Programming) models the world as OBJECTS.
  An object has:
    - STATE (data/attributes) — what it IS
    - BEHAVIOR (methods)      — what it DOES

  CLASS vs OBJECT:
  ----------------
  Class  = blueprint/template (like a cookie cutter)
  Object = instance created from the class (like the actual cookie)

  You can create MANY objects from ONE class.

  THE 4 PILLARS OF OOP:
  ----------------------
  1. ENCAPSULATION  — bundle data + methods, hide internals (this lesson)
  2. INHERITANCE    — derive new classes from existing ones (lesson 12)
  3. POLYMORPHISM   — same interface, different behavior (lesson 12)
  4. ABSTRACTION    — expose only what's necessary (lesson 12)

  ACCESS SPECIFIERS:
  ------------------
  private:   only accessible inside the class (default for class)
  protected: accessible inside class AND derived classes
  public:    accessible from anywhere

  MEMORY LAYOUT:
  --------------
  An object in memory is just its data members laid out sequentially.
  Methods are NOT stored per-object — they're shared code in the text segment.
  'this' pointer is passed implicitly to every non-static method.

================================================================================
*/

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

// =========================================================================
// EXAMPLE 1: BankAccount — demonstrates encapsulation
// =========================================================================

class BankAccount {
private:
    // Private data — cannot be accessed directly from outside
    std::string owner;
    double balance;
    std::string account_number;
    std::vector<std::string> transaction_history;

    // Private helper method — internal use only
    void log_transaction(const std::string& description) {
        transaction_history.push_back(description);
    }

public:
    // -----------------------------------------------------------------------
    // CONSTRUCTORS
    // -----------------------------------------------------------------------

    // Default constructor:
    BankAccount() : owner("Unknown"), balance(0.0), account_number("000") {
        std::cout << "[BankAccount] Default constructor called" << std::endl;
    }

    // Parameterized constructor with initializer list:
    // Initializer list (: owner(n), ...) is MORE EFFICIENT than assignment in body
    // because it initializes directly instead of default-init then assign
    BankAccount(std::string n, double initial_balance, std::string acc_num)
        : owner(n), balance(initial_balance), account_number(acc_num) {
        if (initial_balance < 0) {
            balance = 0;
            std::cout << "[Warning] Negative initial balance set to 0" << std::endl;
        }
        log_transaction("Account opened with $" + std::to_string(initial_balance));
        std::cout << "[BankAccount] Created account for " << owner << std::endl;
    }

    // Copy constructor — called when object is copied
    BankAccount(const BankAccount& other)
        : owner(other.owner + "_copy"),
          balance(other.balance),
          account_number(other.account_number + "_copy"),
          transaction_history(other.transaction_history) {
        std::cout << "[BankAccount] Copy constructor called" << std::endl;
    }

    // Destructor — called when object is destroyed
    ~BankAccount() {
        std::cout << "[BankAccount] Account for " << owner << " closed." << std::endl;
    }

    // -----------------------------------------------------------------------
    // PUBLIC METHODS (interface)
    // -----------------------------------------------------------------------

    bool deposit(double amount) {
        if (amount <= 0) {
            std::cout << "Invalid deposit amount." << std::endl;
            return false;
        }
        balance += amount;
        log_transaction("Deposit: +$" + std::to_string(amount));
        std::cout << "Deposited $" << amount << ". New balance: $" << balance << std::endl;
        return true;
    }

    bool withdraw(double amount) {
        if (amount <= 0) {
            std::cout << "Invalid withdrawal amount." << std::endl;
            return false;
        }
        if (amount > balance) {
            std::cout << "Insufficient funds! Balance: $" << balance << std::endl;
            return false;
        }
        balance -= amount;
        log_transaction("Withdrawal: -$" + std::to_string(amount));
        std::cout << "Withdrew $" << amount << ". New balance: $" << balance << std::endl;
        return true;
    }

    bool transfer(BankAccount& target, double amount) {
        if (withdraw(amount)) {
            target.deposit(amount);
            log_transaction("Transfer to " + target.owner + ": -$" + std::to_string(amount));
            return true;
        }
        return false;
    }

    // GETTERS (read-only access to private data):
    std::string get_owner()          const { return owner; }
    double      get_balance()        const { return balance; }
    std::string get_account_number() const { return account_number; }

    // SETTER with validation:
    void set_owner(const std::string& new_name) {
        if (!new_name.empty()) {
            owner = new_name;
        }
    }

    void print_history() const {
        std::cout << "\nTransaction history for " << owner << ":" << std::endl;
        for (const std::string& t : transaction_history) {
            std::cout << "  - " << t << std::endl;
        }
    }

    void print_summary() const {
        std::cout << "Account: " << account_number
                  << " | Owner: " << owner
                  << " | Balance: $" << balance << std::endl;
    }
};

// =========================================================================
// EXAMPLE 2: Vector2D — demonstrates operator overloading
// =========================================================================

class Vector2D {
public:
    double x, y;

    // Constructor with default values:
    Vector2D(double x = 0, double y = 0) : x(x), y(y) {}

    // Operator overloading — define what +, -, *, == mean for this class:
    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }

    Vector2D operator-(const Vector2D& other) const {
        return Vector2D(x - other.x, y - other.y);
    }

    Vector2D operator*(double scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }

    bool operator==(const Vector2D& other) const {
        return x == other.x && y == other.y;
    }

    double magnitude() const {
        return std::sqrt(x*x + y*y);
    }

    Vector2D normalize() const {
        double mag = magnitude();
        if (mag == 0) return Vector2D(0, 0);
        return Vector2D(x/mag, y/mag);
    }

    double dot(const Vector2D& other) const {
        return x * other.x + y * other.y;
    }

    // Overload << for easy printing:
    friend std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
        os << "(" << v.x << ", " << v.y << ")";
        return os;
    }
};

// =========================================================================
// EXAMPLE 3: Static members
// =========================================================================

class Counter {
private:
    int id;
    static int total_count;  // shared across ALL instances

public:
    Counter() {
        total_count++;
        id = total_count;
        std::cout << "Counter #" << id << " created. Total: " << total_count << std::endl;
    }

    ~Counter() {
        total_count--;
        std::cout << "Counter #" << id << " destroyed. Total: " << total_count << std::endl;
    }

    int get_id() const { return id; }

    // Static method — can be called without an object
    static int get_total() { return total_count; }
};

// Static member must be defined outside the class:
int Counter::total_count = 0;

// =========================================================================
// MAIN
// =========================================================================

int main() {

    std::cout << "=== BANK ACCOUNT ===" << std::endl;

    // Creating objects:
    BankAccount acc1("Alice", 1000.0, "ACC001");
    BankAccount acc2("Bob", 500.0, "ACC002");

    std::cout << std::endl;

    // Using methods:
    acc1.deposit(250.0);
    acc1.withdraw(100.0);
    acc1.withdraw(2000.0);  // should fail

    std::cout << std::endl;
    acc1.transfer(acc2, 300.0);

    std::cout << std::endl;
    acc1.print_summary();
    acc2.print_summary();

    acc1.print_history();
    acc2.print_history();

    // Accessing private data — only through getters:
    std::cout << "\nOwner: " << acc1.get_owner() << std::endl;
    std::cout << "Balance: $" << acc1.get_balance() << std::endl;
    // acc1.balance = 999999;  // COMPILE ERROR! balance is private

    // Copy constructor:
    std::cout << "\n--- Copy ---" << std::endl;
    BankAccount acc3 = acc1;  // copy constructor called
    acc3.print_summary();

    // Object on heap:
    std::cout << "\n--- Heap object ---" << std::endl;
    BankAccount* heap_acc = new BankAccount("Charlie", 2000.0, "ACC003");
    heap_acc->deposit(500.0);   // use -> for pointer to object
    heap_acc->print_summary();
    delete heap_acc;  // destructor called here

    // =========================================================================
    std::cout << "\n=== VECTOR2D ===" << std::endl;

    Vector2D v1(3, 4);
    Vector2D v2(1, 2);

    std::cout << "v1 = " << v1 << std::endl;
    std::cout << "v2 = " << v2 << std::endl;
    std::cout << "v1 + v2 = " << (v1 + v2) << std::endl;
    std::cout << "v1 - v2 = " << (v1 - v2) << std::endl;
    std::cout << "v1 * 2  = " << (v1 * 2)  << std::endl;
    std::cout << "|v1|    = " << v1.magnitude() << std::endl;  // 5
    std::cout << "v1 norm = " << v1.normalize() << std::endl;
    std::cout << "v1.v2   = " << v1.dot(v2) << std::endl;
    std::cout << "v1==v2  = " << (v1 == v2) << std::endl;

    // =========================================================================
    std::cout << "\n=== STATIC MEMBERS ===" << std::endl;

    std::cout << "Total before: " << Counter::get_total() << std::endl;  // 0
    {
        Counter c1, c2, c3;
        std::cout << "Total inside: " << Counter::get_total() << std::endl;  // 3
    }  // c1, c2, c3 destroyed here
    std::cout << "Total after:  " << Counter::get_total() << std::endl;  // 0

    std::cout << "\n=== END OF MAIN ===" << std::endl;
    // acc1, acc2, acc3 destroyed here (stack order: reverse of creation)
    return 0;
}
