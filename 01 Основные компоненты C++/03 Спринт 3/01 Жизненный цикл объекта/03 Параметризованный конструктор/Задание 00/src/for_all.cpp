#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

enum class AnimalType {
    Cat,
    Dog,
    Mouse,
};

class Animal {
public:
    Animal(AnimalType type_, string name_, string owner_name_) {
        this->type_ = type_;
        this->name_ = name_;
        this->owner_name_ = owner_name_;
    }
    AnimalType GetType() const {
        return type_;
    }

    const string& GetName() const {
        return name_;
    }

    const string& GetOwnerName() const {
        return owner_name_;
    }

private:
    AnimalType type_;
    string name_;
    string owner_name_;
};

int main() {
    Animal animal(AnimalType::Dog, "Berta"s, "Artem"s);
    string type;
    switch (animal.GetType()) {
    case AnimalType::Cat:
        type = "Cat";
        break;
    case AnimalType::Dog:
        type = "Dog";
        break;
    case AnimalType::Mouse:
        type = "Mouse";
        break;

    }

    cout << type << "; " << animal.GetName() << "; " << animal.GetOwnerName() << endl;
}