#include <iostream>
#include <string>

int main() {
	std::string name;
	int health;
	std::string unit_class;
	std::cout << "Enter player name:";
	std::cin >> name;
	std::cout << "\n";
	std::cout << "Enter player health:";
	std::cin >> health;
	std::cout << "\n";
	std::cout << "Enter player class (wizard, warrior, archer):";
	std::cin >> unit_class;
	std::cout << "\n";
	std::cout << "Name:" << name << "\n" << "Health:" << health << "\n" << "Class:" << unit_class << "\n";
}