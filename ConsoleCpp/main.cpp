#include <iostream>
#include <string>
#include <vector>

struct Player {
    std::string name;
    int health;
};

void readPlayers(std::vector<Player>& players, int count) {
    for (int i = 0; i < count; i++) {
        Player p;
        std::cout << "Enter player name: ";
        std::cin >> p.name;
        std::cout << "Enter player health: ";
        std::cin >> p.health;
        players.push_back(p);
    }
}

void applyDamage(std::vector<Player>& players, int damage) {
    for (Player& p : players) {
        p.health -= damage;
    }
}

int findMaxHealthIndex(const std::vector<Player>& players) {
    int maxIndex = 0;
    for (int i = 1; i < players.size(); i++) {
        if (players[i].health > players[maxIndex].health) {
            maxIndex = i;
        }
    }
    return maxIndex;
}

int findMinHealthIndex(const std::vector<Player>& players) {
    int minIndex = 0;
    for (int i = 1; i < players.size(); i++) {
        if (players[i].health < players[minIndex].health) {
            minIndex = i;
        }
    }
    return minIndex;
}


void printDeadPlayers(const std::vector<Player>& players) {
    bool found = false;
    for (const Player& p : players) {
        if (p.health <= 0) {
            std::cout << p.name << " is dead.\n";
            found = true;
        }
    }
    if (!found) {
        std::cout << "No dead players.\n";
    }
}

int main() {
    int playersCount;
    std::cout << "Enter number of players: ";
    std::cin >> playersCount;

    std::vector<Player> players;
    readPlayers(players, playersCount);

    int damage;
    std::cout << "Enter damage to apply: ";
    std::cin >> damage;
    applyDamage(players, damage);

    int request;
    std::cout << "\nChoose request:\n";
    std::cout << "1 - Player with max health\n";
    std::cout << "2 - Player with min health\n";
    std::cout << "3 - Dead players\n";
    std::cin >> request;

    if (request == 1) {
        int idx = findMaxHealthIndex(players);
        std::cout << "Player with max health: "
                  << players[idx].name
                  << " (" << players[idx].health << " HP)\n";
    }
    else if (request == 2) {
        int idx = findMinHealthIndex(players);
        std::cout << "Player with min health: "
                  << players[idx].name
                  << " (" << players[idx].health << " HP)\n";
    }
    else if (request == 3) {
        printDeadPlayers(players);
    }
    else {
        std::cout << "Invalid request.\n";
    }

    return 0;
}
