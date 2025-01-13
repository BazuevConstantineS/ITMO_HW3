#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <cstdlib>
#include <ctime>
#include <fstream>

// ������� ��� ������� ������
void clearScreen() {
    std::system("cls");
}

// ����� ����������
class Artifact {
private:
    std::string name;
    int price;

public:
    Artifact(const std::string& name, int price) : name(name), price(price) {}

    const std::string& getName() const { return name; }
    int getPrice() const { return price; }
};

class Player;

// ����������� ����� ��������
class Excavation {
protected:
    std::string name;         // �������� ��������
    std::string terrainType;  // ��� ���������
    int cost;                 // ��������� ����������
    std::map<std::string, std::vector<std::string>> lootTable; // ������� ����

public:
    Excavation(const std::string& name, const std::string& terrainType, int cost)
        : name(name), terrainType(terrainType), cost(cost) {}

    virtual ~Excavation() {}

    virtual void excavate(const std::string& tool, Player& player) = 0; // ������ �������� ������� �� �����������
    virtual void printText() = 0;    // ������� ����� ��� ���������

    int getCost() const { return cost; }
    const std::string& getTerrainType() const { return terrainType; }

protected:
    void findLoot(const std::string& tool, Player& player);
};

// ���������� ������ ��������
class ExcavationUnderWater : public Excavation {
public:
    ExcavationUnderWater() : Excavation("��������� ��������", "��������� ���������", 100) {
        lootTable["��������"] = { "�������", "����������_����" };
        lootTable["������"] = { "��������", "��������_�������" };
        lootTable["�����"] = { "�����_�������", "������" };
    }

    void excavate(const std::string& tool, Player& player) override {
        std::cout << "�� ���������� ��������� �������.\n";
        findLoot(tool, player);
    }

    void printText() override {
        std::cout << "�� ���������� �������� ������, �� ����� ������ ��� �� �����!\n";
    }
};

class ExcavationVulkano : public Excavation {
public:
    ExcavationVulkano() : Excavation("������������� ��������", "������������� ���������", 200) {
        lootTable["��������"] = { "����������_��������" };
        lootTable["������"] = { "�����_����", "��������" };
        lootTable["�����"] = { "��������", "������" };
    }

    void excavate(const std::string& tool, Player& player) override {
        std::cout << "�� ��������� ��������� ����� ������������� ������.\n";
        findLoot(tool, player);
    }

    void printText() override {
        std::cout << "���� ����������, �� ���� �������� �� ����� ������!\n";
    }
};

class ExcavationMoon : public Excavation {
public:
    ExcavationMoon() : Excavation("������ ��������", "������ �����������", 300) {
        lootTable["��������"] = { "�������_�������������" };
        lootTable["������"] = { "��������_������� ������" };
        lootTable["�����"] = { "�����������_�������" };
    }

    void excavate(const std::string& tool, Player& player) override {
        std::cout << "�� ������ ������ ����� � ������� �������������.\n";
        findLoot(tool, player);
    }

    void printText() override {
        std::cout << "����������� ������� �������� ���, �� �� ������������� �� ������!\n";
    }
};

// ����� �����
class Player {
private:
    int money;
    int food;
    std::vector<Artifact> loot;

public:
    Player(int startingMoney, int startingFood) : money(startingMoney), food(startingFood) {}

    void addMoney(int amount) { money += amount; }
    void deductMoney(int amount) { money -= amount; }
    void addFood(int amount) { food += amount; }
    void consumeFood() { food--; }
    bool hasFood() const { return food > 0; }
    bool isBankrupt() const { return money <= 0; }

    void addLoot(const Artifact& artifact) { loot.push_back(artifact); }
    void showLoot() const {
        if (loot.empty()) {
            std::cout << "� ��� ���� ��� �������.\n";
        }
        else {
            std::cout << "���� �������: \n";
            for (int i = 0; i < loot.size(); ++i) {
                std::cout << i + 1 << ". " << loot[i].getName() << " ����: " << loot[i].getPrice() << "\n";
            }
        }
    }

    void sellLoot() {
        showLoot();
        if (loot.empty()) return;

        std::cout << "�������� ����� ��������� ��� ������� (0 ��� ������): ";
        int choice;
        std::cin >> choice;

        if (choice > 0 && choice <= static_cast<int>(loot.size())) {
            Artifact& artifact = loot[choice - 1];
            money += artifact.getPrice();
            loot.erase(loot.begin() + choice - 1);
            std::cout << "�� ������� " << artifact.getName() << " �� " << artifact.getPrice() << " �����.\n";
        }
        else if (choice != 0) {
            std::cout << "������������ �����.\n";
        }
    }

    void buyEnergy() {

        while (true)
        {
            clearScreen();
            showInventory();

            std::cout << "\n�������� ����� ��� ��� ������� (0 ��� ������) \n";
            std::cout << "1. ������ (1) ����: 10\n2. ����� � ����� (3) ����: 25\n3. ������ ������ (5) ����: 45\n4. ������������ ������� (10) ����: 80\n��� �����: ";
            int choice;
            std::cin >> choice;

            switch (choice) {
            case 1:
                if (getMoney() < 10) {
                    std::cout << "� ��� ������������ ����� ��� ���� ���.\n";
                    break;
                }
                food += 1;
                money -= 10;
                break;
            case 2:
                if (getMoney() < 25) {
                    std::cout << "� ��� ������������ ����� ��� ���� ���.\n";
                    break;
                }
                food += 3;
                money -= 25;
                break;
            case 3:
                if (getMoney() < 45) {
                    std::cout << "� ��� ������������ ����� ��� ���� ���.\n";
                    break;
                }
                food += 5;
                money -= 45;
                break;
            case 4:
                if (getMoney() < 80) {
                    std::cout << "� ��� ������������ ����� ��� ���� ���.\n";
                    break;
                }
                food += 10;
                money -= 80;
                break;
            case 0:
                return;
            default:
                std::cout << "������������ �����. ���������� �����.\n";
                break;
            }
        }
        
    }

    void showStatus() const {
        std::cout << "������: " << money << "\n���: " << food << "\n";
    }

    void showInventory() const {
        showStatus();
        showLoot();
    }

    int getMoney() const { return money; }

    // ���������� ��������� ������ � ����
    void save(const std::string& filename) const {
        std::ofstream outFile(filename);
        if (!outFile) {
            std::cout << "�� ������� ������� ���� ��� ����������.\n";
            return;
        }

        outFile << money << " " << food << "\n";
        outFile << loot.size() << "\n";  // ������� ���������� ���������� ����������
        for (const auto& artifact : loot) {
            outFile << artifact.getName() << " " << artifact.getPrice() << "\n";  // ���������� �������� � ���� ���������
        }

        outFile.close();
    }

    // �������� ��������� ������ �� �����
    void load(const std::string& filename) {
        std::ifstream inFile(filename);
        if (!inFile) {
            std::cout << "�� ������� ������� ���� ��� ��������. �������� � ������ ���������.\n";
            return;
        }

        inFile >> money >> food;
        int lootCount;
        inFile >> lootCount;

        loot.clear();
        for (int i = 0; i < lootCount; ++i) {
            std::string name;
            int price;
            inFile >> name >> price;
            loot.push_back(Artifact(name, price));
        }

        inFile.close();
    }
};

void Excavation::findLoot(const std::string& tool, Player& player) {
    if (lootTable.find(tool) != lootTable.end()) {
        const auto& lootList = lootTable[tool];
        if (!lootList.empty()) {
            int index = std::rand() % lootList.size();
            Artifact artifact(lootList[index], std::rand() % 100 + 50); // ��������� ��������� ����
            std::cout << "�� �����: " << artifact.getName() << "!\n";
            player.addLoot(artifact);
        }
        else {
            std::cout << "���� ���������� �������� ����������� �����.\n";
        }
    }
    else {
        std::cout << "����� ���������� �� �������� ��� ���� ���������.\n";
    }
}

// ������� �������� �����
void expedition(Player& player, std::unique_ptr<Excavation> excavation) {
    if (player.getMoney() < excavation->getCost()) {
        std::cout << "� ��� ������������ ����� ��� ���� ����������.\n";
        return;
    }

    player.deductMoney(excavation->getCost());
    std::cout << "���������� ����������: " << excavation->getTerrainType() << "\n";

    while (player.hasFood()) {
        clearScreen();
        player.showInventory();
        excavation->printText();

        std::cout << "�������� ����������: \n1. �������� \n2. ������ \n3. �����\n��� �����: ";
        int toolChoice;
        std::cin >> toolChoice;

        std::string tool;
        switch (toolChoice) {
        case 1:
            tool = "��������";
            break;
        case 2:
            tool = "������";
            break;
        case 3:
            tool = "�����";
            break;
        default:
            std::cout << "������������ �����. ���������� �����.\n";
            continue;
        }

        excavation->excavate(tool, player);
        player.consumeFood();

        char choice;
        std::cout << "������ ����������? (Y (default)/n): ";
        std::cin >> choice;
        if (choice == 'n') break;
    }

    if (!player.hasFood()) {
        std::cout << "� ��� ����������� ���, � �� ��������� ��������� �� ����.\n";
    }
}

void shop(Player& player) {
    clearScreen();
    std::cout << "����� ���������� � �������!\n";
    std::cout << "�������� �����: \n1. ������ \n2. �������\n��� �����: ";
    int shopChoice;
    std::cin >> shopChoice;
    switch (shopChoice) {
    case 1:
        clearScreen();
        player.buyEnergy();
    case 2:
        player.sellLoot();
    default:
        std::cout << "������������ �����. ���������� �����.\n";
        return;
    }
    system("pause");
}

int main() {

    // ��������� �������� ����� � �������
    system("chcp 1251");
    clearScreen();

    Player player(500, 10);

    player.load("savegame.txt");

    while (!player.isBankrupt()) {
        player.save("savegame.txt");
        clearScreen();
        player.showInventory();
        std::cout << "\n���� ��������: \n1. ����������� � ����������\n2. ����� � �������\n3. �����\n��� �����: ";
        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1: {
            clearScreen();
            std::cout << "�������� ����������: \n1. ��������� (100)\n2. ������������� (200)\n3. ������(300)\n��� �����: ";
            int expeditionChoice;
            std::cin >> expeditionChoice;

            switch (expeditionChoice) {
            case 1:
                expedition(player, std::make_unique<ExcavationUnderWater>());
                break;
            case 2:
                expedition(player, std::make_unique<ExcavationVulkano>());
                break;
            case 3:
                expedition(player, std::make_unique<ExcavationMoon>());
                break;
            default:
                std::cout << "������������ ����� ����������.\n";
            }
            break;
        }
        case 2:
            clearScreen();
            shop(player);
        case 3:
            std::cout << "����� �� ����. ������� �� ����!\n";
            return 0;
        default:
            std::cout << "������������ �����. ���������� �����.\n";
        }
    }

    std::cout << "�� �������������. ���� ��������.\n";
    return 0;
}
