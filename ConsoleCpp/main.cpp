#include <iostream>
#include <string>
using namespace std;

struct Stats {
    int str;
    int intel;
    int agi;
};

struct Weapon {
    string name;
    int damage;
    int cooldown;
    string icon;
    int lastUseTime;

    Weapon() {
        name = "";
        damage = 0;
        cooldown = 0;
        icon = "";
        lastUseTime = -1000000;
    }

    Weapon(string n, int d, int cd, string ic) {
        name = n;
        damage = d;
        cooldown = cd;
        icon = ic;
        lastUseTime = -1000000;
    }

    bool isReady(int timeNow) {
        return timeNow - lastUseTime >= cooldown;
    }

    int remaining(int timeNow) {
        int left = cooldown - (timeNow - lastUseTime);
        if (left < 0) return 0;
        return left;
    }

    bool use(int timeNow) {
        if (!isReady(timeNow)) return false;
        lastUseTime = timeNow;
        return true;
    }
};

struct PassiveItem {
    string name;
    Stats bonus;
    string icon;

    PassiveItem() {
        name = "";
        bonus = {0, 0, 0};
        icon = "";
    }

    PassiveItem(string n, Stats b, string ic) {
        name = n;
        bonus = b;
        icon = ic;
    }
};

struct Character {
    string name;
    int health;

    Stats baseStats;
    Stats stats;

    bool hasWeapon;
    Weapon weapon;

    PassiveItem items[3];
    int itemsCount;

    Character(string n, int hp, Stats s) {
        name = n;
        health = hp;
        baseStats = s;
        stats = s;
        hasWeapon = false;
        weapon = Weapon();
        itemsCount = 0;
    }

    void applyPassive(PassiveItem it) {
        if (itemsCount == 3) {
            cout << "No passive slots\n";
            return;
        }
        items[itemsCount] = it;
        itemsCount++;

        stats.str += it.bonus.str;
        stats.intel += it.bonus.intel;
        stats.agi += it.bonus.agi;
    }

    void pickWeapon(Weapon w) {
        if (!hasWeapon) {
            weapon = w;
            hasWeapon = true;
            cout << "Picked weapon: " << weapon.name << "\n";
            return;
        }

        cout << "Swapped weapon: " << weapon.name << " -> " << w.name << "\n";
        weapon = w;
    }

    void takeDamage(int dmg) {
        health -= dmg;
        if (health < 0) health = 0;
    }

    void attack(Character &target, int timeNow) {
        if (!hasWeapon) {
            cout << "No weapon\n";
            return;
        }

        if (!weapon.use(timeNow)) {
            cout << "Weapon cooldown: " << weapon.remaining(timeNow) << "\n";
            return;
        }

        int dmg = weapon.damage + stats.str;
        target.takeDamage(dmg);

        cout << name << " attacked " << target.name
             << " for " << dmg
             << ", target HP: " << target.health << "\n";
    }
};

int main() {
    Character hero("Hero", 100, {5, 2, 3});
    Character enemy("Enemy", 80, {2, 1, 2});

    PassiveItem ring("Ring", {2, 0, 1}, "ring.png");
    PassiveItem book("Book", {0, 3, 0}, "book.png");

    hero.applyPassive(ring);
    hero.applyPassive(book);

    Weapon sword("Sword", 10, 3, "sword.png");
    Weapon axe("Axe", 14, 5, "axe.png");

    hero.pickWeapon(sword);

    int t = 0;
    hero.attack(enemy, t);

    t = 1;
    hero.attack(enemy, t);

    t = 3;
    hero.attack(enemy, t);

    hero.pickWeapon(axe);

    t = 4;
    hero.attack(enemy, t);

    t = 9;
    hero.attack(enemy, t);

    return 0;
}
