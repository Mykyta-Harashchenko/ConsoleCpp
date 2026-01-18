#include <iostream>
#include <string>
#include <vector>

class Character {
public:
    std::string name;
    int health;
    int x;
    int y;
    int speed;

    void Move(int dx, int dy) {
        x += dx * speed;
        y += dy * speed;
    }
};

int main() {
    int n;
    std::cout << "Enter number of characters: ";
    std::cin >> n;

    std::vector<Character> characters;
    characters.reserve(n);

    for (int i = 0; i < n; i++) {
        Character c;
        std::cout << "Enter name: ";
        std::cin >> c.name;
        std::cout << "Enter health: ";
        std::cin >> c.health;
        std::cout << "Enter position x: ";
        std::cin >> c.x;
        std::cout << "Enter position y: ";
        std::cin >> c.y;
        std::cout << "Enter speed: ";
        std::cin >> c.speed;

        characters.push_back(c);
    }

    int commands;
    std::cout << "Enter number of commands: ";
    std::cin >> commands;

    for (int i = 0; i < commands; i++) {
        int index, dx, dy;
        std::cout << "Enter command (index dx dy): ";
        std::cin >> index >> dx >> dy;

        if (index >= 0 && index < (int)characters.size()) {
            characters[index].Move(dx, dy);
        }
        else {
            std::cout << "Invalid character index\n";
            i--;
        }
    }

    std::cout << "\nFinal positions:\n";
    for (int i = 0; i < characters.size(); i++) {
        std::cout << characters[i].name
            << " -> (" << characters[i].x
            << ", " << characters[i].y << ")\n";
    }

    return 0;
}