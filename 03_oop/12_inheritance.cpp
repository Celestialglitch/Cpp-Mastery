/*
================================================================================
  LESSON 12: Inheritance, Polymorphism, and Abstraction
================================================================================

  THEORY — INHERITANCE:
  ----------------------
  Inheritance lets a class REUSE and EXTEND another class.
  "Is-a" relationship: Dog IS-A Animal. Car IS-A Vehicle.

  Base class (parent) → Derived class (child)
  Child inherits all non-private members of parent.

  INHERITANCE TYPES:
  ------------------
  public    inheritance: public→public, protected→protected (most common)
  protected inheritance: public→protected, protected→protected
  private   inheritance: public→private, protected→private

  THEORY — POLYMORPHISM:
  -----------------------
  "Many forms" — same function call behaves differently based on actual type.

  TWO TYPES:
  1. Compile-time (static):  function overloading, templates
  2. Runtime (dynamic):      virtual functions, base class pointers

  VIRTUAL FUNCTIONS:
  ------------------
  When you call a virtual function through a base class pointer/reference,
  C++ looks up the ACTUAL type at runtime and calls the right version.
  This is done via the VTABLE (virtual function table).

  VTABLE:
  -------
  Each class with virtual functions has a hidden vtable — an array of
  function pointers. Each object has a hidden vptr pointing to its class's vtable.
  Virtual call = follow vptr → look up function in vtable → call it.

  ABSTRACT CLASS:
  ---------------
  A class with at least one PURE VIRTUAL function (= 0).
  Cannot be instantiated — only used as a base.
  Forces derived classes to implement the pure virtual functions.

================================================================================
*/

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cmath>

// =========================================================================
// ABSTRACT BASE CLASS — Shape
// =========================================================================

class Shape {
protected:
    std::string color;
    std::string name;

public:
    Shape(std::string n, std::string c = "white") : name(n), color(c) {
        std::cout << "[Shape] Created: " << name << " (" << color << ")" << std::endl;
    }

    // PURE VIRTUAL — must be overridden in derived classes
    // Makes Shape an abstract class — cannot instantiate Shape directly
    virtual double area()      const = 0;
    virtual double perimeter() const = 0;

    // VIRTUAL — can be overridden, but has a default implementation
    virtual void describe() const {
        std::cout << name << " [" << color << "]"
                  << " | Area: " << area()
                  << " | Perimeter: " << perimeter() << std::endl;
    }

    // Non-virtual — same for all shapes
    std::string get_name()  const { return name; }
    std::string get_color() const { return color; }
    void set_color(std::string c) { color = c; }

    // VIRTUAL DESTRUCTOR — CRITICAL for polymorphism!
    // Without this, deleting a derived object through base pointer
    // would only call base destructor → resource leak!
    virtual ~Shape() {
        std::cout << "[Shape] Destroyed: " << name << std::endl;
    }
};

// Shape s;  // COMPILE ERROR: Shape is abstract (has pure virtual functions)

// =========================================================================
// DERIVED CLASS — Circle
// =========================================================================

class Circle : public Shape {
private:
    double radius;
    static const double PI;

public:
    Circle(double r, std::string color = "red")
        : Shape("Circle", color), radius(r) {
        std::cout << "[Circle] Created with radius " << r << std::endl;
    }

    ~Circle() override {
        std::cout << "[Circle] Destroyed (radius=" << radius << ")" << std::endl;
    }

    // OVERRIDE pure virtual functions — 'override' keyword is optional but recommended
    // It tells compiler "I intend to override" — catches typos at compile time
    double area()      const override { return PI * radius * radius; }
    double perimeter() const override { return 2 * PI * radius; }

    // Override describe to add extra info:
    void describe() const override {
        Shape::describe();  // call parent's version first
        std::cout << "  Radius: " << radius << std::endl;
    }

    double get_radius() const { return radius; }
};

const double Circle::PI = 3.14159265358979;

// =========================================================================
// DERIVED CLASS — Rectangle
// =========================================================================

class Rectangle : public Shape {
protected:
    double width, height;

public:
    Rectangle(double w, double h, std::string color = "blue")
        : Shape("Rectangle", color), width(w), height(h) {
        std::cout << "[Rectangle] Created " << w << "x" << h << std::endl;
    }

    ~Rectangle() override {
        std::cout << "[Rectangle] Destroyed" << std::endl;
    }

    double area()      const override { return width * height; }
    double perimeter() const override { return 2 * (width + height); }

    double get_width()  const { return width; }
    double get_height() const { return height; }
};

// =========================================================================
// DERIVED FROM DERIVED — Square IS-A Rectangle IS-A Shape
// =========================================================================

class Square : public Rectangle {
public:
    Square(double side, std::string color = "green")
        : Rectangle(side, side, color) {
        name = "Square";  // override name (protected in Shape)
        std::cout << "[Square] Created with side " << side << std::endl;
    }

    ~Square() override {
        std::cout << "[Square] Destroyed" << std::endl;
    }

    // Inherits area() and perimeter() from Rectangle — no need to override
    // But we can add Square-specific methods:
    double get_side() const { return width; }
};

// =========================================================================
// MULTIPLE INHERITANCE
// =========================================================================

class Flyable {
public:
    virtual void fly() const {
        std::cout << "Flying!" << std::endl;
    }
    virtual ~Flyable() {}
};

class Swimmable {
public:
    virtual void swim() const {
        std::cout << "Swimming!" << std::endl;
    }
    virtual ~Swimmable() {}
};

// Duck inherits from BOTH Flyable and Swimmable:
class Duck : public Flyable, public Swimmable {
public:
    std::string name;
    Duck(std::string n) : name(n) {}

    void fly()  const override { std::cout << name << " is flying!" << std::endl; }
    void swim() const override { std::cout << name << " is swimming!" << std::endl; }
    void quack() const { std::cout << name << " says: Quack!" << std::endl; }
};

// =========================================================================
// POLYMORPHISM DEMO
// =========================================================================

// This function works with ANY Shape — current or future!
void print_shape_info(const Shape& s) {
    s.describe();  // calls the RIGHT describe() based on actual type
}

double total_area(const std::vector<Shape*>& shapes) {
    double total = 0;
    for (const Shape* s : shapes) {
        total += s->area();  // virtual dispatch — calls correct area()
    }
    return total;
}

// =========================================================================
// MAIN
// =========================================================================

int main() {

    std::cout << "=== CREATING SHAPES ===" << std::endl;
    Circle    c(5.0, "red");
    Rectangle r(4.0, 6.0, "blue");
    Square    s(3.0, "green");

    std::cout << "\n=== DIRECT CALLS ===" << std::endl;
    c.describe();
    r.describe();
    s.describe();

    std::cout << "\n=== POLYMORPHISM via base reference ===" << std::endl;
    // Base class reference can refer to any derived object:
    print_shape_info(c);
    print_shape_info(r);
    print_shape_info(s);

    std::cout << "\n=== POLYMORPHISM via base pointer ===" << std::endl;
    // Base class pointer can point to any derived object:
    Shape* shapes[] = {&c, &r, &s};
    for (Shape* shape : shapes) {
        std::cout << shape->get_name() << ": area=" << shape->area() << std::endl;
    }

    std::cout << "\n=== VECTOR OF SHAPES ===" << std::endl;
    std::vector<Shape*> shape_list = {&c, &r, &s};
    std::cout << "Total area: " << total_area(shape_list) << std::endl;

    std::cout << "\n=== HEAP POLYMORPHISM ===" << std::endl;
    // Using smart pointers for safe polymorphism:
    std::vector<std::unique_ptr<Shape>> owned_shapes;
    owned_shapes.push_back(std::make_unique<Circle>(3.0));
    owned_shapes.push_back(std::make_unique<Rectangle>(5.0, 2.0));
    owned_shapes.push_back(std::make_unique<Square>(4.0));

    for (const auto& shape : owned_shapes) {
        shape->describe();
    }
    // All shapes automatically deleted when vector goes out of scope

    std::cout << "\n=== MULTIPLE INHERITANCE ===" << std::endl;
    Duck duck("Donald");
    duck.fly();
    duck.swim();
    duck.quack();

    // Can use as Flyable or Swimmable:
    Flyable* flyer = &duck;
    flyer->fly();

    Swimmable* swimmer = &duck;
    swimmer->swim();

    std::cout << "\n=== DYNAMIC CAST ===" << std::endl;
    // dynamic_cast: safely downcast from base to derived pointer
    Shape* shape_ptr = new Circle(7.0);

    // Try to cast to Circle:
    Circle* circle_ptr = dynamic_cast<Circle*>(shape_ptr);
    if (circle_ptr) {
        std::cout << "Successfully cast to Circle, radius=" << circle_ptr->get_radius() << std::endl;
    }

    // Try to cast to Rectangle (will fail — it's actually a Circle):
    Rectangle* rect_ptr = dynamic_cast<Rectangle*>(shape_ptr);
    if (!rect_ptr) {
        std::cout << "Cast to Rectangle failed (as expected)" << std::endl;
    }

    delete shape_ptr;  // virtual destructor ensures Circle's destructor is called!

    std::cout << "\n=== END OF MAIN ===" << std::endl;
    return 0;
}
