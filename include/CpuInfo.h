#include <iostream>
#include <string>

class Person {
private:
    std::string name;
    int age;

public:
    // Constructor
    Person(const std::string& personName, int personAge) {
        name = personName;
        age = personAge;
    }

    // Method to print a greeting
    void greet() const {
        std::cout << "Hi, I'm " << name << " and I'm " << age << " years old.\n";
    }

    // Getter for age
    int getAge() const {
        return age;
    }

    // Setter for age
    void setAge(int newAge) {
        if (newAge >= 0)
            age = newAge;
    }
};

int main() {
    Person p("Alice", 30);
    p.greet();
    p.setAge(31);
    std::cout << "Next year, I'll be " << p.getAge() << ".\n";
    return 0;
}
