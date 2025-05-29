#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

class Character { //เป็นคลาสแม่ หรือ base class ที่จะเก็บค่าไว้ให้ player เอาไว้ใช้ต่อ
protected: //ใช้ protected เพราะว่าต้องการให้ subclass อย่าง player เอาข้อมูลตรงนี้ไปใช้
    string name;
    int hp;
    int attackPower;
public:
    Character(string n, int h, int a) : name(n), hp(h), attackPower(a) {} //n = name
    virtual ~Character() {}

    string getName() { return name; }
    int getHP() { return hp; }
    void takeDamage(int dmg) { hp -= dmg; if (hp < 0) hp = 0; }
    bool isAlive() { return hp > 0; } //ตรวจสอบว่าตัวละครยังมีชีวิตอยู่หรือไม่ (HP > 0) true false
    virtual int attack() { return attackPower; } 
};

class Player : public Character {
private: //ใช้ private ไปเลยเพราะว่าไม่ได้มีใครนอก player เอาไปใช้ต่อแล้ว 
    int potions;
    int mana;
public:
    Player(string n) : Character(n, 100, 20), potions(5), mana(100) {}

    void usePotion() { // เพิ่ม HP และลดโพชั่น ใช้ void เพราะว่าไม่ต้องการให้คืนค่ากลับแต่ +30 เข้าไปใน character  ใ
        if (potions > 0) {
            hp += 30;
            if (hp > 100) hp = 100;
            potions--;
            cout << name << " uses a potion! (+30 HP, Potions left: " << potions << ")\n";
        } else {
            cout << "No potions left!\n";
        }
    }

    void gainMana(int amount) { //amount คือ ค่าที่เราต้องการเพิ่มเข้าไปในมานาของผู้เล่น
        mana += amount;
        if (mana > 100) mana = 100;
    }

    int getMana() { return mana; } //ทำหน้าที่คืนค่าเพื่อบอกถึงมานาที่เหลืออยู่

    int useFireBolt() {
        if (mana >= 10) {
            mana -= 10;
            cout << name << " casts Fire Bolt! (-10 Mana)\n";
            return 25; //ส่งค่าคงที่ 25 ออกไปเลย โดยไม่ได้ขึ้นกับตัวแปรอื่นที่สร้างมาก่อนหน้าในฟังก์ชันนี้
        } else {
            cout << "Not enough mana!\n";
            return 0;
        }
    }

    int useExcaliburr() {
        if (mana >= 70) {
            mana -= 70;
            cout << name << " uses EXCALIBURR! Massive damage! (-70 Mana)\n";
            return 90;
        } else {
            cout << "Not enough mana!\n";
            return 0;
        }
    }

    void showStatus() { //ใช้ void เพราะว่า ฟังก์ชันนี้แค่แสดงผลข้อมูล (print) ออกทางหน้าจอเท่านั้น
        cout << "[" << name << "] HP: " << hp << " | Mana: " << mana << " | Potions: " << potions << "\n";
    }
};

class Monster : public Character {
public:
    Monster(string n, int h, int a) : Character(n, h, a) {}
};

class Game {
private:
    Player* player;
    int score;
    int difficulty;
public:
    Game(string playerName) { //การสร้างออบเจกต์ของคลาส Player ขึ้นมาใหม่ โดยใช้ชื่อที่รับมาจากผู้เล่น (playerName) เพื่อเก็บไว้ใช้งานในเกม
        player = new Player(playerName);
        score = 0;
    }

    ~Game() {
        delete player;
    }

    void chooseDifficulty() {
        cout << "Choose difficulty level:\n1. Easy\n2. Medium\n3. Hard\n> ";
        cin >> difficulty;
        if (difficulty < 1 || difficulty > 3) difficulty = 2; //กันกรอกเลขเกินหรือน้อยกว่า จับเท่ากับ 2 เลย
    }

    Monster getRandomMonster() {
        vector<string> names = {"Slime", "Goblin", "Orc", "Wolf"};
        string mName = names[rand() % names.size()]; //random ตั้งแต่ 0-3[0 = slime , 1 =  goblin bla bla]
        int mHP = 40 + (rand() % 20) + difficulty * 5; 
        int mAtk = 6 + difficulty * 2;
        return Monster(mName, mHP, mAtk);
    }

    Monster createBoss() {
        return Monster("Dragon King", 400, 20);
    }

    void fight(Monster& enemy) {
        cout << "A wild " << enemy.getName() << " appears! (HP: " << enemy.getHP() << ")\n";

        while (enemy.isAlive() && player->isAlive()) { //ใช้ while loop เพราะว่า  หากเป็น true กับ true ก็จะทำงานต่อไปเรื่อยๆ
            player->showStatus(); //สั่งให้แสดงข้อมูลสถานะของผู้เล่นบนหน้าจอ
            cout << "Enemy HP: " << enemy.getHP() << "\n";
            cout << "\n=== Choose an action ===\n";
            cout << "1. Normal Attack (0 Mana, 20 dmg)\n";
            cout << "2. Fire Bolt (10 Mana, 25 dmg)\n";
            cout << "3. EXCALIBURR (70 Mana, 90 dmg)\n";
            cout << "4. Use Potion (+30 HP)\n> ";

            int choice;
            cin >> choice;
            int dmg = 0;
            switch (choice) {
                case 1:
                    dmg = player->attack();
                    cout << "You attacked for " << dmg << " damage!\n";
                    break;
                case 2:
                    dmg = player->useFireBolt();
                    break;
                case 3:
                    dmg = player->useExcaliburr();
                    break;
                case 4:
                    player->usePotion();
                    break;
                default:
                    cout << "Invalid action!\n";
            }

            if (dmg > 0) enemy.takeDamage(dmg);

            if (enemy.isAlive()) {
                int enemyDmg = enemy.attack();
                player->takeDamage(enemyDmg);
                cout << enemy.getName() << " attacks you for " << enemyDmg << " damage!\n";
            }

            player->gainMana(20);
            cout << "You regained 20 Mana.\n";
            cout << "--------------------------\n";
        }

        if (player->isAlive()) {
            cout << "You defeated " << enemy.getName() << " \n";
            if (enemy.getName() == "Dragon King") {
                cout << "\n=== VICTORY! ===\n";
                cout << "You have defeated the Final Boss!\n";
            }
            score += (enemy.getName() == "Dragon King") ? 100 : 20;
        } else {
            cout << "You were defeated... Game Over.\n";
        }
    }

    void start() {
        chooseDifficulty();

        for (int i = 0; i < 3 && player->isAlive(); i++) {
            Monster m = getRandomMonster();
            fight(m);
        }

        if (player->isAlive()) {
            cout << " Final Boss Appears! \n";
            Monster boss = createBoss();
            fight(boss);
        }

        cout << "Final Score: " << score << "\n";
    }
};

int main() {
    srand(time(0));
    string name;
    char playAgain;

    do {
        cout << "Enter your character's name: ";
        cin >> name;

        Game g(name);
        g.start();

        cout << "Do you want to play again? (y/n): ";
        cin >> playAgain;
        cout << "----------------------------------\n";
    } while (playAgain == 'y' || playAgain == 'Y');

    cout << "Thanks for playing!\n";
    return 0;
}
