#include <iostream>
#include <vector>
#include <conio.h>
#include "User.h" 
#include <thread>
#include "Enemy.h"    
#include <cstdlib>
#include <ctime>

using namespace std;  

// @ Declarations
struct ISlot {
    int slotId;
    int row;
    int column;
    bool discovered;
};

int tableSize = 0;
int scapeDoorSlot = 0; 

User player("Xavi", 100);
vector<ISlot> Slots; 
vector<Enemy> Enemies; 

// @ Functions
bool randomFactor() {
    int random = rand() % 30; 
    return random <= 0;
}

float calculateDiscoveredPercentage(const vector<ISlot>& Slots) {
    int totalSlots = Slots.size();
    int discoveredSlots = 0;

    for (const ISlot& slot : Slots) {
        if (slot.discovered) {
            discoveredSlots++;
        }
    }

    float percentage = (static_cast<float>(discoveredSlots) / totalSlots) * 100.0;
    return percentage;
}

string getHealthProgress(int health) {
    string progress = ""; 
    int toDecimal = health / 10; 
    string whitespace = "\x1B[47m \x1B[0m";

    for (int index = 0; index < toDecimal; index++)
    {
        progress += whitespace;
    }

    
    return progress; 
}

void displayTable(vector<ISlot> Slots, int tableSize, int characterSlot) { 
    system("CLS"); 
    int slotId = 0;

    for (int rowIndex = 0; rowIndex < (tableSize / 2); rowIndex++) {
        string row = "";

        for (int columnIndex = 0; columnIndex < tableSize; columnIndex++) {
            if (slotId == characterSlot) row = row + "0";
            else if (Slots[slotId].discovered) row = row + " ";
            else row = row + "*";

            slotId++;
        }

        cout << row << endl;
    }

    cout << "\nSlot actual: " << characterSlot << endl;
    cout << "Salida: " << scapeDoorSlot << endl;
    cout << "Porcentaje descubierto: " << round(calculateDiscoveredPercentage(Slots)) << "%" << endl;
    cout << "Vida: " << getHealthProgress(player.getHealth()) << endl;
}

vector<int> renderDefaultValues(vector<ISlot>& Slots, int tableSize) {
    int slotId = 0;
    vector<int> enemySlots;  

    for (int rowIndex = 0; rowIndex < (tableSize / 2); rowIndex++) {
        for (int columnIndex = 0; columnIndex < tableSize; columnIndex++) {
            ISlot slot = { slotId, rowIndex, columnIndex, false };
            Slots.push_back(slot); 

            bool createEnemy = randomFactor(); 

            if (createEnemy && slotId != round(tableSize / 2)) {
                enemySlots.push_back(slotId); 
                Enemy enemy("Enemigo", 20, false, slotId);
                Enemies.push_back(enemy);
            } 

            slotId++;

            if (slotId >= ((tableSize / 2) * tableSize)) {
                return enemySlots; 
            };
        }
    } 
}

void initFight(Enemy &target) {
    system("CLS");
    cout << "¡Acabas de encontrarte con " << target.getName() << "!" << endl; 
    bool isEnemyTurn = false; 

    while (target.getHealth() > 0 && player.getHealth() > 0) {
        int random = (rand() % 20) + 1;
        
        if (!isEnemyTurn) {
            target.setHealth(target.getHealth() - random);
            
            if(target.getHealth() > 0) cout << "Acabas de asestar un golpe a " << target.getName() << " y le has quitado " << random << " de vida" << endl;
            else cout << "¡Acabas de asestar un golpe mortal a " << target.getName() << " y le has dejado KO!" << endl;
        }
        else {
            player.setHealth(player.getHealth() - random);

            if (player.getHealth() > 0) cout << target.getName() << " te acaba de asestar un golpe y te ha quitado " << random << " de vida" << endl;
            else cout << target.getName() << " te acaba de asestar un golpe mortal y te ha dejado KO!" << endl;
        }

        isEnemyTurn = !isEnemyTurn; 
        std::this_thread::sleep_for(std::chrono::seconds(3));
    } 
}

void setCharacterSlot(int &characterSlot, int slotId, vector<int> enemySlots) {
    characterSlot = slotId;
    
    for (int index = 0; index < enemySlots.size(); index++) {
        if (enemySlots[index] == slotId) { 
            for (int enemyIndex = 0; enemyIndex < Enemies.size(); enemyIndex++) { 
                if (Enemies[enemyIndex].getSlot() == slotId) { 
                    if (Enemies[enemyIndex].getHealth() <= 0) return;

                    initFight(Enemies[enemyIndex]);
                }
            }
        };
    } 
}

// @ Main
int main() {  
    srand(time(0));

    do {
        cout << "Introduce tamaño de tablero: "; 
        cin >> tableSize; 
    } while (tableSize <= 0); 

    int characterSlot = round(tableSize / 2); 
    
    vector<int> enemySlots = renderDefaultValues(Slots, tableSize);
    displayTable(Slots, tableSize, characterSlot);

    scapeDoorSlot = Slots.size() - tableSize + characterSlot;
    cout << scapeDoorSlot; 
    Enemy boss("Enemigo Final", 50, true, scapeDoorSlot);
    enemySlots.push_back(scapeDoorSlot);
    Enemies.push_back(boss);

    while (player.getHealth() > 0 && characterSlot != scapeDoorSlot) {
        ISlot current = Slots[characterSlot];

        if (!current.discovered) {
            Slots[characterSlot].discovered = true;
        };

        switch (_getch()) {
        case 72:
            // @ Up 
            if ((current.row - 1) >= 0) {
                setCharacterSlot(characterSlot, current.slotId - tableSize, enemySlots);
            }

            break;
        case 80:
            // @ Down 
            if ((current.row + 1) < (tableSize / 2)) {
                setCharacterSlot(characterSlot, current.slotId + tableSize, enemySlots);
            }

            break;
        case 75:
            // @ Left
            if ((current.column - 1) >= 0) {
                setCharacterSlot(characterSlot, current.slotId - 1, enemySlots);
            }

            break;
        case 77:
            // @ Right
            if ((current.column + 1) < tableSize) {
                setCharacterSlot(characterSlot, current.slotId + 1, enemySlots);
            }

            break;
        default:
            break;
        }

        displayTable(Slots, tableSize, characterSlot);
    };  

    system("CLS"); 

    if (player.getHealth() > 0) cout << "Felicidades! Acabas de encontrar la salida!"; 
    else cout << "Ohh! Parece que no has llegado vivo a la final..."; 

    return 0; 
}
