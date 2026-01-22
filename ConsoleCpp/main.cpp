#include <iostream>
#include <string>
#include <vector>
#include <memory>

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

    Weapon() : name(""), damage(0), cooldown(0), icon(""), lastUseTime(-1000000) {}

    Weapon(string n, int d, int cd, string ic)
        : name(n), damage(d), cooldown(cd), icon(ic), lastUseTime(-1000000) {
    }

    bool isReady(int timeNow) const {
        return timeNow - lastUseTime >= cooldown;
    }

    int remaining(int timeNow) const {
        int left = cooldown - (timeNow - lastUseTime);
        return left < 0 ? 0 : left;
    }

    bool use(int timeNow) {
        if (!isReady(timeNow)) return false;
        lastUseTime = timeNow;
        return true;
    }
};

class InventoryItem {
public:
    virtual ~InventoryItem() = default;
    virtual string getName() const = 0;

    virtual Stats statBonus() const { return { 0, 0, 0 }; }
    virtual int speedBonus() const { return 0; }
};

class PassiveItem : public InventoryItem {
    string name;
    Stats bonus;
    string icon;

public:
    PassiveItem(string n, Stats b, string ic) : name(n), bonus(b), icon(ic) {}

    string getName() const override { return name; }
    Stats statBonus() const override { return bonus; }
};

class SpeedItem : public InventoryItem {
    string name;
    int bonusSpeed;
    string icon;

public:
    SpeedItem(string n, int s, string ic) : name(n), bonusSpeed(s), icon(ic) {}

    string getName() const override { return name; }
    int speedBonus() const override { return bonusSpeed; }
};

class Character {
    vector<unique_ptr<InventoryItem>> inventory;
    unique_ptr<Weapon> weapon;

public:
    string name;
    int health;

    Stats baseStats;
    Stats stats;

    int baseSpeed;
    int currentSpeed;

    Character(string n, int hp, Stats s, int spd)
        : name(n), health(hp), baseStats(s), stats(s), baseSpeed(spd), currentSpeed(spd) {
    }

    bool addItem(unique_ptr<InventoryItem> item) {
        if (!item) return false;
        inventory.push_back(move(item));
        return true;
    }

    const vector<unique_ptr<InventoryItem>>& getInventory() const {
        return inventory;
    }

    int getBaseSpeed() const { return baseSpeed; }

    void setSpeed(int spd) { currentSpeed = spd; }

    void recalcStatsFromInventory() {
        stats = baseStats;
        for (const auto& it : inventory) {
            Stats b = it->statBonus();
            stats.str += b.str;
            stats.intel += b.intel;
            stats.agi += b.agi;
        }
    }

    void pickWeapon(unique_ptr<Weapon> w) {
        if (!w) return;

        if (!weapon) {
            weapon = move(w);
            cout << "Picked weapon: " << weapon->name << "\n";
            return;
        }

        cout << "Swapped weapon: " << weapon->name << " -> " << w->name << "\n";
        weapon = move(w);
    }

    void takeDamage(int dmg) {
        health -= dmg;
        if (health < 0) health = 0;
    }

    void attack(Character& target, int timeNow) {
        if (!weapon) {
            cout << "No weapon\n";
            return;
        }

        if (!weapon->use(timeNow)) {
            cout << "Weapon cooldown: " << weapon->remaining(timeNow) << "\n";
            return;
        }

        int dmg = weapon->damage + stats.str;
        target.takeDamage(dmg);

        cout << name << " attacked " << target.name
            << " for " << dmg
            << ", target HP: " << target.health << "\n";
    }
};

void updateCharacterSpeed(Character& c) {
    int spd = c.getBaseSpeed();
    for (const auto& it : c.getInventory()) {
        spd += it->speedBonus();
    }
    if (spd < 0) spd = 0;
    c.setSpeed(spd);
}

int main() {
    Character hero("Hero", 100, { 5, 2, 3 }, 10);
    Character enemy("Enemy", 80, { 2, 1, 2 }, 8);

    hero.addItem(make_unique<PassiveItem>("Ring", Stats{ 2, 0, 1 }, "ring.png"));
    hero.addItem(make_unique<PassiveItem>("Book", Stats{ 0, 3, 0 }, "book.png"));
    hero.addItem(make_unique<SpeedItem>("Boots", 5, "boots.png"));

    hero.recalcStatsFromInventory();
    updateCharacterSpeed(hero);

    cout << "Hero stats: STR=" << hero.stats.str
        << " INT=" << hero.stats.intel
        << " AGI=" << hero.stats.agi << "\n";

    cout << "Hero speed: " << hero.currentSpeed << "\n";

    hero.pickWeapon(make_unique<Weapon>("Sword", 10, 3, "sword.png"));

    int t = 0;
    hero.attack(enemy, t);

    t = 1;
    hero.attack(enemy, t);

    t = 3;
    hero.attack(enemy, t);

    hero.pickWeapon(make_unique<Weapon>("Axe", 14, 5, "axe.png"));

    t = 4;
    hero.attack(enemy, t);

    t = 9;
    hero.attack(enemy, t);

    return 0;
}
