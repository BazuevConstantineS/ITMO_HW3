#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <cstdlib>
#include <ctime>
#include <fstream>

// Функция для очистки экрана
void clearScreen() {
    std::system("cls");
}

// Класс артефактов
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

// Абстрактный класс Раскопки
class Excavation {
protected:
    std::string name;         // Название раскопок
    std::string terrainType;  // Тип местности
    int cost;                 // Стоимость экспедиции
    std::map<std::string, std::vector<std::string>> lootTable; // Таблица лута

public:
    Excavation(const std::string& name, const std::string& terrainType, int cost)
        : name(name), terrainType(terrainType), cost(cost) {}

    virtual ~Excavation() {}

    virtual void excavate(const std::string& tool, Player& player) = 0; // Логика раскопок зависит от инструмента
    virtual void printText() = 0;    // Смешной текст при раскопках

    int getCost() const { return cost; }
    const std::string& getTerrainType() const { return terrainType; }

protected:
    void findLoot(const std::string& tool, Player& player);
};

// Конкретные классы Раскопок
class ExcavationUnderWater : public Excavation {
public:
    ExcavationUnderWater() : Excavation("Подводные раскопки", "Подводная местность", 100) {
        lootTable["Кисточка"] = { "Аммонит", "Окаменелая_рыба" };
        lootTable["Лопата"] = { "Раковина", "Фрагмент_коралла" };
        lootTable["Кирка"] = { "Кусок_мрамора", "Ничего" };
    }

    void excavate(const std::string& tool, Player& player) override {
        std::cout << "Вы исследуете подводные глубины.\n";
        findLoot(tool, player);
    }

    void printText() override {
        std::cout << "Вы чувствуете давление глубин, но азарт держит вас на плаву!\n";
    }
};

class ExcavationVulkano : public Excavation {
public:
    ExcavationVulkano() : Excavation("Вулканические раскопки", "Вулканическая местность", 200) {
        lootTable["Кисточка"] = { "Окаменелое_растение" };
        lootTable["Лопата"] = { "Кусок_лавы", "Обсидиан" };
        lootTable["Кирка"] = { "Минералы", "Ничего" };
    }

    void excavate(const std::string& tool, Player& player) override {
        std::cout << "Вы осторожно отбиваете куски вулканической породы.\n";
        findLoot(tool, player);
    }

    void printText() override {
        std::cout << "Жара невыносима, но ваше упорство не знает границ!\n";
    }
};

class ExcavationMoon : public Excavation {
public:
    ExcavationMoon() : Excavation("Лунные раскопки", "Лунная поверхность", 300) {
        lootTable["Кисточка"] = { "Пыльные_микрофоссилии" };
        lootTable["Лопата"] = { "Фрагмент_лунного грунта" };
        lootTable["Кирка"] = { "Метеоритный_обломок" };
    }

    void excavate(const std::string& tool, Player& player) override {
        std::cout << "Вы бурите лунный грунт в поисках окаменелостей.\n";
        findLoot(tool, player);
    }

    void printText() override {
        std::cout << "Космическая пустота окружает вас, но вы сосредоточены на задаче!\n";
    }
};

// Класс Игрок
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
            std::cout << "У вас пока нет находок.\n";
        }
        else {
            std::cout << "Ваши находки: \n";
            for (int i = 0; i < loot.size(); ++i) {
                std::cout << i + 1 << ". " << loot[i].getName() << " Цена: " << loot[i].getPrice() << "\n";
            }
        }
    }

    void sellLoot() {
        showLoot();
        if (loot.empty()) return;

        std::cout << "Выберите номер артефакта для продажи (0 для выхода): ";
        int choice;
        std::cin >> choice;

        if (choice > 0 && choice <= static_cast<int>(loot.size())) {
            Artifact& artifact = loot[choice - 1];
            money += artifact.getPrice();
            loot.erase(loot.begin() + choice - 1);
            std::cout << "Вы продали " << artifact.getName() << " за " << artifact.getPrice() << " монет.\n";
        }
        else if (choice != 0) {
            std::cout << "Некорректный выбор.\n";
        }
    }

    void buyEnergy() {

        while (true)
        {
            clearScreen();
            showInventory();

            std::cout << "\nВыберите номер еды для покупки (0 для выхода) \n";
            std::cout << "1. Яблоко (1) Цена: 10\n2. Булка с маком (3) Цена: 25\n3. Сытный супчик (5) Цена: 45\n4. Божественная похлёбка (10) Цена: 80\nВаш выбор: ";
            int choice;
            std::cin >> choice;

            switch (choice) {
            case 1:
                if (getMoney() < 10) {
                    std::cout << "У вас недостаточно денег для этой еды.\n";
                    break;
                }
                food += 1;
                money -= 10;
                break;
            case 2:
                if (getMoney() < 25) {
                    std::cout << "У вас недостаточно денег для этой еды.\n";
                    break;
                }
                food += 3;
                money -= 25;
                break;
            case 3:
                if (getMoney() < 45) {
                    std::cout << "У вас недостаточно денег для этой еды.\n";
                    break;
                }
                food += 5;
                money -= 45;
                break;
            case 4:
                if (getMoney() < 80) {
                    std::cout << "У вас недостаточно денег для этой еды.\n";
                    break;
                }
                food += 10;
                money -= 80;
                break;
            case 0:
                return;
            default:
                std::cout << "Некорректный выбор. Попробуйте снова.\n";
                break;
            }
        }
        
    }

    void showStatus() const {
        std::cout << "Деньги: " << money << "\nЕда: " << food << "\n";
    }

    void showInventory() const {
        showStatus();
        showLoot();
    }

    int getMoney() const { return money; }

    // Сохранение состояния игрока в файл
    void save(const std::string& filename) const {
        std::ofstream outFile(filename);
        if (!outFile) {
            std::cout << "Не удалось открыть файл для сохранения.\n";
            return;
        }

        outFile << money << " " << food << "\n";
        outFile << loot.size() << "\n";  // Сначала записываем количество артефактов
        for (const auto& artifact : loot) {
            outFile << artifact.getName() << " " << artifact.getPrice() << "\n";  // Записываем название и цену артефакта
        }

        outFile.close();
    }

    // Загрузка состояния игрока из файла
    void load(const std::string& filename) {
        std::ifstream inFile(filename);
        if (!inFile) {
            std::cout << "Не удалось открыть файл для загрузки. Начинаем с нового состояния.\n";
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
            Artifact artifact(lootList[index], std::rand() % 100 + 50); // Генерация случайной цены
            std::cout << "Вы нашли: " << artifact.getName() << "!\n";
            player.addLoot(artifact);
        }
        else {
            std::cout << "Этот инструмент оказался бесполезным здесь.\n";
        }
    }
    else {
        std::cout << "Такой инструмент не подходит для этой местности.\n";
    }
}

// Функции игрового цикла
void expedition(Player& player, std::unique_ptr<Excavation> excavation) {
    if (player.getMoney() < excavation->getCost()) {
        std::cout << "У вас недостаточно денег для этой экспедиции.\n";
        return;
    }

    player.deductMoney(excavation->getCost());
    std::cout << "Начинается экспедиция: " << excavation->getTerrainType() << "\n";

    while (player.hasFood()) {
        clearScreen();
        player.showInventory();
        excavation->printText();

        std::cout << "Выберите инструмент: \n1. Кисточка \n2. Лопата \n3. Кирка\nВаш выбор: ";
        int toolChoice;
        std::cin >> toolChoice;

        std::string tool;
        switch (toolChoice) {
        case 1:
            tool = "Кисточка";
            break;
        case 2:
            tool = "Лопата";
            break;
        case 3:
            tool = "Кирка";
            break;
        default:
            std::cout << "Некорректный выбор. Попробуйте снова.\n";
            continue;
        }

        excavation->excavate(tool, player);
        player.consumeFood();

        char choice;
        std::cout << "Хотите продолжить? (Y (default)/n): ";
        std::cin >> choice;
        if (choice == 'n') break;
    }

    if (!player.hasFood()) {
        std::cout << "У вас закончилась еда, и вы вынуждены вернуться на базу.\n";
    }
}

void shop(Player& player) {
    clearScreen();
    std::cout << "Добро пожаловать в магазин!\n";
    std::cout << "Выберите опцию: \n1. Купить \n2. Продать\nВаш выбор: ";
    int shopChoice;
    std::cin >> shopChoice;
    switch (shopChoice) {
    case 1:
        clearScreen();
        player.buyEnergy();
    case 2:
        player.sellLoot();
    default:
        std::cout << "Некорректный выбор. Попробуйте снова.\n";
        return;
    }
    system("pause");
}

int main() {

    // Установка русского языка в консоли
    system("chcp 1251");
    clearScreen();

    Player player(500, 10);

    player.load("savegame.txt");

    while (!player.isBankrupt()) {
        player.save("savegame.txt");
        clearScreen();
        player.showInventory();
        std::cout << "\nВаши действия: \n1. Отправиться в экспедицию\n2. Зайти в магазин\n3. Выйти\nВаш выбор: ";
        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1: {
            clearScreen();
            std::cout << "Выберите экспедицию: \n1. Подводная (100)\n2. Вулканическая (200)\n3. Лунная(300)\nВаш выбор: ";
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
                std::cout << "Некорректный выбор экспедиции.\n";
            }
            break;
        }
        case 2:
            clearScreen();
            shop(player);
        case 3:
            std::cout << "Выход из игры. Спасибо за игру!\n";
            return 0;
        default:
            std::cout << "Некорректный выбор. Попробуйте снова.\n";
        }
    }

    std::cout << "Вы обанкротились. Игра окончена.\n";
    return 0;
}
