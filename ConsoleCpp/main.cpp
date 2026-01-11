#include <iostream>
#include <string>
#include <utility>
#include <unordered_map>

int main() {
	int n;
	int x, y;
	std::string command;
	std::pair<int, int> movement;
	std::unordered_map<std::string, std::pair<int, int>> movements;
	movements["right"] = std::make_pair(1, 0);
	movements["left"] = std::make_pair(-1, 0);
	movements["forward"] = std::make_pair(0, 1);
	movements["backward"] = std::make_pair(0, -1);
	int dxTotal = 0, dyTotal = 0;

	std::cout << "Enter number of iterations:"<< "\n";
	std::cin >> n;
	int i = 0;
	
	while (i < n) {
		std::cout << "Enter command (right, left, forward, backward):" << "\n";
		std::cin >> command;
		if (movements.find(command) != movements.end()) {
			movement = movements[command];
			dxTotal += movement.first;
			dyTotal += movement.second;
			i++;
		}
		else {
			std::cout << "Invalid command. Please try again." << "\n";
		}
	}
	std::cout << "Tell me starting coordinates of x:" << "\n";
	std::cin >> x;
	std::cout << "Tell me starting coordinates of y:" << "\n";
	std::cin >> y;
	x = x + dxTotal;
	y = y + dyTotal;
	std::cout << "Final coordinates are: (" << x << ", " << y << ")" << "\n";
	return 0;
}