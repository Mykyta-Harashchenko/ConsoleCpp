#include <iostream>
#include <string>
#include <utility>
#include <vector>

struct Character {
    std::string name;
    int health;
    std::pair<int, int> position;
};

struct Meteor {
    std::pair<int, int> met_position;
    int damage;
    int diameter;
};

int num_of_characters() {
    int character_number;
    std::cout << "Enter number of characters: ";
    std::cin >> character_number;
    return character_number;
}

Character create_character() {
    Character c;

    std::cout << "Enter character name: ";
    std::cin >> c.name;

    std::cout << "Enter character health: ";
    std::cin >> c.health;

    std::cout << "Enter character position x: ";
    std::cin >> c.position.first;

    std::cout << "Enter character position y: ";
    std::cin >> c.position.second;

    return c;
}

Meteor create_meteor() {
    Meteor m;

    std::cout << "Enter meteor position x: ";
    std::cin >> m.met_position.first;

    std::cout << "Enter meteor position y: ";
    std::cin >> m.met_position.second;

    std::cout << "Enter meteor damage: ";
    std::cin >> m.damage;

    std::cout << "Enter meteor diameter: ";
    std::cin >> m.diameter;

    return m;
}

std::pair<std::vector<Character>, std::vector<Character>>
list_of_characters(const std::vector<Character>& characters, const Meteor& meteor) {
    std::vector<Character> killed_characters;
    std::vector<Character> harmed_characters;
    const auto met_fall = meteor.met_position;
    const int damage = meteor.damage;
    const int diameter = meteor.diameter;
    const int range_x = met_fall.first + diameter / 2;
    const int range_y = met_fall.second + diameter / 2;
    for (const Character& c : characters) {
        const auto char_pos = c.position;
        const bool in_square =
            (char_pos.first <= range_x) && (char_pos.first >= (range_x - diameter)) &&
            (char_pos.second <= range_y) && (char_pos.second >= (range_y - diameter));

        if (!in_square) {
            continue;
        }
        Character updated = c;
        updated.health -= damage;
        if (updated.health <= 0) {
            killed_characters.push_back(updated);
        }
        else {
            harmed_characters.push_back(updated);
        }
    }

    return { killed_characters, harmed_characters };
}

int main() {
    const int num = num_of_characters();
    std::vector<Character> characters;
    characters.reserve(num);
    for (int i = 0; i < num; i++) {
        characters.push_back(create_character());
    }
    const Meteor meteor = create_meteor();
    const auto result = list_of_characters(characters, meteor);
    const auto& killed_characters = result.first;
    const auto& harmed_characters = result.second;
    std::cout << "\nHarmed characters:\n";
    for (const Character& c : harmed_characters) {
        std::cout << "Name: " << c.name
            << ", Health: " << c.health
            << ", Position: (" << c.position.first << ", " << c.position.second << ")\n";
    }
    std::cout << "\nKilled characters:\n";
    for (const Character& c : killed_characters) {
        std::cout << "Name: " << c.name
            << ", Health: " << c.health
            << ", Position: (" << c.position.first << ", " << c.position.second << ")\n";
    }

    return 0;
}
