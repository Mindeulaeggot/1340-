#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <random>
using namespace std;

// Define constants for game parameters
const int MAX_SANITY = 100;
const int MIN_SANITY = 0;
const int NUM_ROOMS = 5;
const int NUM_ITEMS = 3;
const int NUM_GHOSTS = 3;

// Define struct for game objects
struct Item {
    string name;
    string description;
    bool isKey;
};

struct Room {
    string name;
    string description;
    vector<Item> items;
    int ghostChance;
    bool isLocked;
};

// Define functions for game mechanics
void printIntro();
void printRoom(Room currentRoom);
void printInventory(vector<Item> inventory);
void addItem(vector<Item>& inventory, Item newItem);
void removeItem(vector<Item>& inventory, int itemIndex);
bool hasItem(vector<Item> inventory, string itemName);
void printSanity(int sanity);
void adjustSanity(int& sanity, int adjustment);
Room generateRandomRoom();
void generateItems(vector<Item>& items);
void generateGhosts(int& numGhosts);
void saveGame(Room currentRoom, vector<Item> inventory, int sanity);
void loadGame(Room& currentRoom, vector<Item>& inventory, int& sanity);

int main() {
    // Seed random number generator
    srand(time(0));

    // Initialize game variables
    Room currentRoom = generateRandomRoom();
    vector<Item> inventory;
    generateItems(inventory);
    int sanity = MAX_SANITY;
    bool gameRunning = true;

    // Print game intro
    printIntro();

    // Game loop
    while (gameRunning) {
        // Print current room and inventory
        printRoom(currentRoom);
        printInventory(inventory);

        // Print sanity level
        printSanity(sanity);

        // Prompt player for action
        cout << "What do you want to do? ";
        string input;
        getline(cin, input);

        // Handle player action
        if (input == "help") {
            cout << "\nHere is a list of available commands:\n" << endl;
            cout << " - look: Look around the room and check for any hidden dangers or items." << endl;
            cout << " - take [item name]: Pick up an item found in the room and add it to your inventory." << endl;
            cout << " - use [item name]: Use an item from your inventory. Usage varies depending on the item." << endl;
            cout << " - unlock: Attempt to unlock the door if you have the key in your inventory." << endl;
            cout << " - quit: Exit the game at any time." << endl;
            cout << "\nRemember to explore carefully and manage your sanity while trying to escape the haunted mansion.\n" << endl;
        }
        else if (input == "look") {
            // Generate random ghost encounter
            if (rand() % 100 < currentRoom.ghostChance) {
                cout << "You hear a strange noise and feel a chill run down your spine." << endl;
                int numGhosts = 0;
                generateGhosts(numGhosts);
                cout << "You turn around and see " << numGhosts << " ghostly figures floating toward you!" << endl;
                adjustSanity(sanity, -20);
            }
            else {
                cout << currentRoom.description << endl;
            }
        }
        else if (input.substr(0, 4) == "take") {
            string itemName = input.substr(5);
            if (hasItem(currentRoom.items, itemName)) {
                int itemIndex = -1;
                for (int i = 0; i < currentRoom.items.size(); i++) {
                    if (currentRoom.items[i].name == itemName) {
                        itemIndex = i;
                        break;
                    }
                }
                addItem(inventory, currentRoom.items[itemIndex]);
                currentRoom.items.erase(currentRoom.items.begin() + itemIndex);
                cout << "You pick up the " << itemName << "." << endl;
            }
            else { cout << "There's no " << itemName << " here." << endl;
            }
        }
        else if (input.substr(0, 3) == "use") {
            string itemName = input.substr(4);
            if (hasItem(inventory, itemName)) {
                if (itemName == "key" && currentRoom.isLocked) {
                    currentRoom.isLocked = false;
                    cout << "You use the key to unlock the door." << endl;
                }
                else if (itemName == "flashlight") {
                    cout << "You turn on the flashlight and look around the room." << endl;
                    adjustSanity(sanity, -10);
                }
                else {
                    cout << "You can't use the " << itemName << " here." << endl;
                }
            }
            else {
                cout << "You don't have a " << itemName << "." << endl;
            }
        }
        else if (input == "unlock") {
            if (currentRoom.isLocked) {
                if (hasItem(inventory, "key")) {
                    currentRoom.isLocked = false;
                    cout << "You use the key to unlock the door." << endl;
                }
                else {
                    cout << "The door is locked and you don't have a key." << endl;
                }
            }
            else {
                cout << "The door is already unlocked." << endl;
            }
        }
        else if (input == "quit") {
            gameRunning = false;
            cout << "Thanks for playing!" << endl;
        }
        else {
            cout << "\nI didn't understand that command. Type 'help' for a list of available commands.\n" << endl;
        }

        // Check if player has won
        if (!currentRoom.isLocked && currentRoom.items.empty()) {
            gameRunning = false;
            cout << "Congratulations! You have escaped the haunted mansion!" << endl;
        }

        // Check if player has lost
        if (sanity <= MIN_SANITY) {
            gameRunning = false;
            cout << "You have gone insane and cannot escape the haunted mansion. Game over." << endl;
        }
    }

    // Save game
    saveGame(currentRoom, inventory, sanity);

    return 0;
}

void printIntro() {
    cout << "Welcome to the Haunted Mansion game!" << endl;
    cout << "You wake up in a haunted mansion with no memory of how you got there." << endl;
    cout << "You notice that the front door is locked, and you must find the key to escape." << endl;
    cout << "You also see that there are some items lying around that could be useful." << endl;
    cout << "Be careful, as there are ghosts lurking in the shadows!" << endl;
    cout << endl;
}

void printRoom(Room currentRoom) {
    cout << "You are in the " << currentRoom.name << "." << endl;
    cout << currentRoom.description << endl;
    if (currentRoom.name == "bathroom") {
        cout <<       "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⠀⢀⣀⣀⣀⡈⠹⢿⣿⠓⣿⡿⣿⣿⡶⣿⣿⣽⣿⡇⠀⠀⠀⠀⠀⠀⣿⣇⠀⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
        cout <<       "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣦⣾⣿⣯⣿⣯⣭⣽⣿⣿⣿⣗⣺⣿⣷⣿⣿⣾⣿⡇⠀⠀⠀⠀⠀⠀⣿⣿⢰⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
        cout <<       "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⠁⠀⠀⠀⠀⠀⠀⣿⣷⢸⣿⣿⣿⡿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
        cout <<       "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⡟⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡏⠩⠇⠀⠀⠀⠀⠀⠀⠀⣿⣿⠀⠙⢿⣏⣤⡿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
        cout <<       "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠏⡇⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⡿⠀⢘⣉⣩⣤⣶⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
        cout <<       "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢸⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣷⣿⡿⠿⠿⠛⠋⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
        cout <<       "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢸⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⢹⣀⣀⣤⠤⠄⠐⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
        cout <<       "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢸⡇⠀⣤⡄⠀⠀⠀⠀⠀⠀⠀⠈⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
        cout <<       "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠤⠾⠿⠳⠄⠤⠀⠄⠠⠄⠤⠤⠤⠚⠿⠧⠤⠍⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
        cout <<       "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
        cout <<       "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡆⢸⣿⡿⣿⣿⣿⣻⣿⣷⠀⠀⣠⣴⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀\n";
        cout <<       "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣷⣼⣿⣿⣿⣾⣿⣿⣟⣿⠀⠀⠀⠉⠻⣿⣿⠀⠀⠀⠀⠀⠀⠀\n";
        cout <<       "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⠻⣿⣿⣀⡀⠀⠀⠀⠈⣿⠇⠀⠀⠀⠀⠀⠀\n";
        cout <<       "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⠀⠀⠀⠀⢀⣀⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠿⣿⣿⣿⣿⣿⣽⣿⣿⣿⣤⣿⣿⣷⣶⣴⣦⣼⣿⡄⠀⠀⠀⠀⠀⠀\n";
        cout <<       "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢨⡉⠉⠉⠉⠉⠉⠉⠉⠀⢠⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⡿⣦⣄⠀⠈⣿⣿⣟⣿⣿⣿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀\n";
        cout <<       "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡇⠀⠀⠀⠀⠀⠀⠀⠀⠸⡇⠀⠀⠀⠀⠀⠀⠀⠀⣰⣶⣿⣿⣷⣦⣄⠀⠛⠛⠛⠻⠃⠉⠉⠉⣭⣿⣿⣿⣿⣿⣿⣿⣿⣿⣀⠀⠀⠀⠀⠀\n";
        cout <<       "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠻⢿⣿⣧⡟⢿⣿⣤⠀⠶⣷⣶⣿⣿⣷⣶⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀\n";
        cout <<       "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡀⠀⠀⠀⠀⠀⢀⡀⣷⣟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡇⡇⠀⣿⡟⠛⠾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀\n";
        cout <<       "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣟⣷⠞⠃⠀⠀⠉⠉⠙⠛⢿⣤⠀⠀⠀⠀⠀⠀⠀⠀⠀⢐⢢⡇⠀⠀⠀⠀⠀⢿⣿⣿⣿⣿⡟⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡧⠀⠀⠀⠀\n";
        cout <<       "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣿⡃⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⡇⠀⠀⠀⠀⠀⠀⠀⠀⠘⢹⠁⠀⢀⠀⠀⣄⣻⣿⣿⣿⣿⣿⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⠀⠀⠀⠀\n";
        cout <<       "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣦⣤⣤⣀⣀⣤⣤⣤⣶⣿⠁⠀⠀⠀⠀⠀⠀⢀⡀⠈⠀⠀⠀⠀⠀⠀⠈⠙⠛⠿⣿⣿⡏⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀\n";
        cout <<       "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣷⣄⣉⣀⣰⣦⣤⣼⣿⣧⣤⣼⣦⣴⣶⠦⠿⣿⣿⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀\n";
        cout <<       "⢠⣤⢠⣀⡠⢤⣀⠐⢲⣦⠘⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣭⣭⣥⣤⣶⣶⣶⣾⣿⣿⣿⣿⣷⣄⠀⢰⠀⠀⠀⠀⠀⠀⢸⣿⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀\n";
        cout <<       "⢈⣉⣭⣭⣭⣴⣶⣶⣶⣶⣶⣿⣿⣿⣏⡟⢉⣉⡉⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣼⠀⠀⠀⠀⠀⠀⣾⣿⠀⠸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡆⠀⠀⠀\n";
        cout <<       "⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠃⠈⠉⠁⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡆⠀⠀⠀⠀⠀⣿⣿⠀⠀⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀\n";
        cout <<       "⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣄⠀⠀⣀⣸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⣿⣿⠀⠀⠈⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀\n";
        cout <<       "⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠟⣟⡿⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡄⠀⠀⠀⠀⠀⣿⣿⠀⠀⠀⠈⢿⣿⣿⣿⠛⡿⣿⣿⣿⡏⠀⠀⠀\n";
        cout <<       "⣸⣿⣿⣿⣿⣿⠰⣾⠺⢿⣿⣿⣏⡉⣿⣿⠿⢿⡟⠋⢩⢉⣅⠀⠯⠥⢰⣘⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀⠀⠉⠉⠁⠀⠀⠈⠙⢟⡇⠀⠀⠀\n";
        cout <<       "⣿⣿⣿⣿⡿⣉⢰⣿⠠⢼⣿⣿⣿⣿⣿⣿⠂⠀⣿⠏⠹⣬⢈⣋⡷⢼⣨⣿⣼⢿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⢐⣿⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⠃⠀⠀⠀\n";
        cout <<       "⣿⣿⣿⣻⠄⠁⣸⡏⠉⠃⣄⣻⣿⣟⣿⣿⣴⣆⣿⣶⡠⣧⣿⣿⢄⡾⢟⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⣰⣶⣿⣿⣿⣦⡀⠀⠀⠀⠀⣦⣀⣀⣀⣠⣿⣀⣀⣀⣿\n";
        cout <<       "⢻⣿⢹⣗⠃⠄⣿⣿⣿⣾⣿⣿⣯⣿⣿⣿⣿⣿⣽⣿⣶⣾⣭⣿⣾⣟⣬⣿⣾⡿⣿⣿⣿⣿⣿⣿⣿⣷⣄⣀⣸⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⢹⣿⣿⣿⣿⣿⣿⣿⣿⣿" << endl;
    } else if (currentRoom.name == "bedroom") {
        cout << "  __|__  {____}{____}  __|__  |__*__|" << endl;
        cout << "__|_*_|__%%%%%%%%%%%%__|_*_|__|__*__|__" << endl;
        cout << "  |   | %%%%%%%%%%%%%% |   |  |/   \\|" << endl;
        cout << "       %%%%%%%%%%%%%%%%" << endl;
        cout << "      %%%%%%%%%%%%%%%%%%" << endl;
        cout << "     %%%%%%%%%%%%%%%%%%%%" << endl;
        cout << "    /||||||||||||||||||||\\" << endl;
        cout << "    ||||||||||||||||||||||" << endl;
    } else if (currentRoom.name == "kitchen") {
        cout << "   ____________________________________________________________________    \n";
    cout << " /|    |__I__I__I__I__I__I__I__I__I_|       _-       %       %         |\\\n";
    cout << "  | _- |_I__I__I__I__I__I__I__I__I__|-_              %       %     _-  | \n";
    cout << "  |    |__I__I__I__I__I__I__I__I__I_|                %       %         | \n";
    cout << "  |  - |_I__I__I__I__I__I__I__I__I__|               ,j,      %w ,      | \n";
    cout << "  | -  |__I__I__I__I__I__I__I__I__I_|  -_ -        / ) \\    /%mMmMm.   | \n";
    cout << "  |    |_I__I__I__I__I__I__I__I__I__|             //|  |   ;  `.,,'    | \n";
    cout << "  |-_- /                            \\             w |  |   `.,;`       | \n";
    cout << "  |   /                              \\    -_       / ( |    ||         | \n";
    cout << "  |  /                                \\           //\\_'/    (.\\"<< char(92)<<"    -_  | \n";
    cout << "  | /__________________________________\\          w  \\/   -  ``'       | \n";
    cout << "  | |__________________________________|                               | \n";
    cout << "  |    |   _______________________   |     _-            -             | \n";
    cout << "  |_-  |  |                       |  |                        _-       | \n";
    cout << "  |    |  |                     _ |  |  T  T  T  T  T                  | \n";
    cout << "  | _-_|  |    __.'`'`'`''`;__ /  |  |  |  |  |  |  |        _-     -  | \n";
    cout << "  |    |  | _/U  `'.'.,.," << char(46) << "'  U   |  |  | (_) |  |  |                  | \n";
    cout << "  |    |  |   |               |   |  | / " << char(92) <<"    @ [_]d b    _@_     |    |   \n";
    cout << "  |    |  |   |      `', `,   |   |  | |_|   ____         [ ]     |    |   \n";
    cout << "  |_-  |  |   |   `') ( )'    |   |  | ______" << char(92) << "__/"<< char(92)<<"__________[_]__   |    | \n";
    cout << "  |    |  |   |____(,`)(,(____|   |  |/________________________    |    | \n";
    cout << "  |    |  |  /|   `@@(@@)@)'  |"<< char(92) <<"  |  | ||            _____   ||   |    | \n";
    cout << "  |    |  | //!  @@)@@)@@@( /!    |  | ||   _--          /   ||  /|    |\n";
    cout << "  |__lc|__|/_____________________ |__|_||____________/### ___||_|||||__|" << endl;
    } else if (currentRoom.name == "attic") {
        cout << "        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠀⠀⠀⣸⣿⣿⣷⠀⠀⠀⠀⠀⠀⠀⠀⠈⢿⡈⢹⣷⣦⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀" << endl;
        cout << "        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠳⣦⣰⡟⠉⠙⢻⡆⠀⠀⠀⠀⢻⣆⠀⠀⠸⣇⠘⣿⣿⣿⡟⠷⣦⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀" << endl;
        cout << "        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠻⣿⣦⣤⣾⠇⠀⠀⠀⠀⠀⢻⡆⠀⠀⢻⡄⢽⣿⣿⡇⢠⢀⣙⡻⢶⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀" << endl;
        cout << "        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠻⢧⣄⠀⠀⠀⠀⠀⠀⠀⢻⡆⠀⠸⣇⢀⣿⣿⣷⡀⠀⠉⠙⠛⠛⠿⠿⣦⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀" << endl;
        cout << "        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢷⣄⠀⠀⠀⠀⠀⠀⢻⡆⠀⢻⣾⢻⣿⣿⣿⣿⣶⣾⣶⣤⣤⣀⠀⠉⠛⠷⣦⣀⠀⠀⠀⠀⠀⠀⠀" << endl;
        cout << "        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠷⣦⠀⠀⠀⠀⠀⢻⡆⠀⠙⠿⣧⣉⠛⢿⣿⣿⣿⣿⣿⣿⣿⣶⣦⣀⠈⠙⢻⣶⣄⡀⠀⠀⠀" << endl;
        cout << "        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠀⠀⠀⠀⠀⠈⢻⣆⠀⠀⠈⠙⠷⣦⣉⠻⢿⣿⣿⣿⣮⣛⠛⢿⣧⡾⣟⣽⣿⠿⣷⣦⠀" << endl;
        cout << "        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣠⡄⠠⠤⠤⣿⣷⣦⡀⠀⠀⠈⠙⠷⣦⣈⠻⢿⣿⣿⣿⣿⣿⣿⣻⣿⠶⠟⠋⠁⠀" << endl;
        cout << "        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣴⣿⣿⣿⣟⣂⣛⣛⣛⣿⣿⣿⣷⣄⡀⠀⠀⠈⠙⠳⣦⣈⢻⣿⣿⠌⠈⣉⣤⣤⡴⠶⠀⠀" << endl;
        cout << "        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣾⣿⠟⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⠈⠻⢷⣅⣤⣄⠀⠀⠀⠈⠙⠻⠿⠟⠛⠛⠉⠁⠀⠀⠀⠀⠀" << endl;
        cout << "        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣤⣾⡿⠟⠁⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠙⠻⣿⣿⣦⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀" << endl;
        cout << "        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⣀⣠⣤⣤⣿⣶⣶⣶⣤⣠⣄⣽⣿⡏⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣧⣿⣿⠀⠀⠀⠀⠀⠀⠙⡏⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠐⠆⠀⠐⠶" << endl;
        cout << "        ⠀⠀⠀⢀⣀⣀⣀⣀⣀⣰⣿⣿⣿⣯⣤⠶⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⣄⣀⣀⣀⣸⣿⣿⣯⣍⣉⣙⣿⣹⣿⠀⠀⠀⠀⠀⠀⠀⣧⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀" << endl;
        cout << "        ⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢉⣻⣿⣿⣦⣀⣽⣿⠿⠿⠿⠿⠛⠋⠉⠉⠉⠉⠉⠉⠉⠉⠉⢙⣷⣦⣤⣬⣭⣍⣀⣀⣀⣀⣠⣤⣤⣿⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀" << endl;
        cout << "        ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣾⣿⣿⣿⣿⣿⣷⣠⢠⣴⠆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠁⣹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀" << endl;
        cout << "        ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣾⡟⢾⣿⣿⡼⢯⡄⣠⡄⣤⠀⠀⠀⠀⠘⣧⢀⡀⠀⢀⣠⣿⡿⠃⠀⠀⠀⠿⣿⣿⣿⣿⡿⠿⣿⣿⣷⣦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀" << endl;
        cout << "        ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣦⣿⣿⣿⣿⣧⣿⣃⡀⢠⡄⠀⠀⣀⢹⡎⠇⠀⣰⣿⡟⠛⠛⠳⠶⣤⣤⣀⣀⠀⠀⠀⠀⠀⠘⢿⣟⢷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀" << endl;
        cout << "        ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡋⠛⠛⠛⠿⠿⠿⣿⣿⣿⣵⣾⣿⣿⣿⣿⣿⣶⣤⣿⣼⣇⢠⣾⣿⡟⠀⠀⠀⠀⠀⠀⠀⠉⠙⠻⣆⠀⠀⠀⠀⠀⠹⣯⡹⢧⡄⠀⣀⣀⣀⣀⣀⣀" << endl;
        cout << "        ⣷⣿⣿⣿⣿⣿⣿⣿⡿⠛⠁⠀⠀⠀⠀⠀⠀⣀⣈⣹⢉⡉⠉⢉⣉⣛⡛⢿⣿⣿⡿⣇⣼⡿⠏⠙⠻⠶⢶⣤⣀⡀⠀⠀⠀⠀⢻⡆⠀⠀⠀⠀⠀⠈⢻⣮⡻⣶⣿⣿⣿⣿⣿⣿" << endl;
    } else if (currentRoom.name == "basement") {
        cout << "⠀⢠⢤⣤⣤⣄⣀⣀⢀⣀⣀⣀⣀⡀⣀⣀⣀⣀⣀⣀⣀⡀⡀⣀⣀⣈⡉⡉⠒⠀⠀⠀⠀⠀⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠠⠶⢶⣒⣉⡓⠲⣶⣤⣄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀" << endl;
        cout << "⠀⢸⢼⣿⣙⣿⣛⣛⣻⣻⣿⣿⣻⣿⣛⣛⣛⣛⡛⠛⠛⠃⠛⠙⠛⠛⠛⠋⠀⠀⠀⠀⠀⠀⠀⠈⠉⠙⠓⠒⠂⠠⡤⠄⠒⡖⢶⡤⢬⣭⣍⣋⡉⠉⠀⠛⠣⠄⣐⣒⣀⣤⠀⠀⠀" << endl;
        cout << "⠀⢸⢨⣭⣭⣭⣭⣭⣭⣥⣭⣭⣭⣭⣭⠉⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⢹⠉⠛⠒⠛⢿⣿⣶⣶⣦⣤⣤⣤⡇⠀⠀⡇⢸⣇⢸⢰⣿⣿⠉⣉⣿⠉⠉⣀⣀⣀⡀⠀⠀⠀⠀" << endl;
        cout << "⠀⠘⢾⡿⠿⠿⠿⠿⠿⠿⠿⣿⣿⠿⠛⠲⠶⣶⠖⠲⢶⣶⠶⢶⡶⠲⢶⡖⠚⢸⠀⠀⠀⠀⢸⡿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠛⠚⠻⢾⢼⣿⣿⣴⣿⣿⣿⣿⣿⣿⣿⡇⠀⢀⡀⠀" << endl;
        cout << "⠀⠀⢸⡇⠀⠀⠀⠆⠀⠀⠀⢹⣿⣠⣤⣤⣤⣿⣀⣀⣸⣿⣀⢀⡇⢀⣸⣿⠀⢠⠀⠀⣶⠀⢸⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠘⠀⡏⠉⢻⠋⢁⣿⣿⣻⡇⣿⡇⠰⣟⣿⠀" << endl;
        cout << "⠀⠀⢸⡇⠀⠀⠀⡄⠀⠀⠀⢸⣿⣿⡀⠀⠀⠀⠉⠉⠉⠉⠉⠛⠛⠛⠛⠛⠒⠛⣿⣿⣿⠷⢿⣷⡯⠿⠿⣿⣿⣿⡇⠀⠀⠀⠀⢲⡄⠀⡇⠀⢸⣀⢸⣿⣿⢻⡇⣿⡇⠀⠀⠀⠀" << endl;
        cout << "⠀⠀⢸⣧⠀⠀⠀⡇⠀⠀⡀⢸⣿⢻⣷⡄⠀⠀⠀⠀⢲⡊⢽⣧⡄⠀⠀⠀⣄⠀⣿⣿⣿⡶⠿⠿⠿⣿⠶⠿⢿⣿⡿⠀⠀⠀⠀⠈⡇⢠⠇⣷⣿⠉⢿⣿⡟⠀⠃⣿⡇⠀⠀⠀⠀" << endl;
        cout << "⠀⠀⢸⡿⠀⠀⠀⠃⠀⠀⢧⣼⣿⠿⣿⣷⣤⣤⣤⣤⣿⣷⣯⣥⣤⣤⣤⣾⣾⣿⣿⣿⣿⣇⣀⣀⠀⣾⠃⠀⠀⣿⠀⠀⠀⠀⠀⠀⡇⢸⠀⠋⣿⠀⢸⡏⡇⠀⠀⢹⠃⠀⠀⠀⠀" << endl;
        cout << "⠀⠀⢸⡇⠀⠒⠀⡆⠒⠰⡎⢿⡇⠀⠀⣿⣿⣿⣿⣟⣛⣿⣿⣿⠉⢿⠛⢿⣿⡿⢹⡇⠀⣿⡇⢹⠀⣿⠀⠀⠀⢻⠀⠀⠀⠀⠀⠀⠆⢸⠀⠀⣿⠀⢸⡇⠀⠀⠀⣸⠀⠀⠀⠀⠀" << endl;
        cout << "⠀⠀⢸⡇⠀⠀⠀⠇⠀⠀⠁⢸⡇⠀⠀⣇⣁⣈⣽⡇⠀⣬⣭⡥⠀⢼⠠⢤⣤⡤⢸⡇⠀⠿⠀⣾⠀⣿⠀⠀⠀⢾⠀⠀⠀⠀⠀⠀⠤⠼⠒⢒⡟⠀⠘⠋⠛⠻⠷⢿⠀⠀⠀⠀⠀" << endl;
        cout << "⠀⠀⢸⣵⠀⣀⣀⡆⣦⣤⣄⢸⡇⠀⠀⣧⣭⣭⣽⡧⠤⢤⣤⡴⠀⣺⠐⠲⠶⢆⣺⣇⣀⣠⣤⣿⠀⠈⠉⠧⠤⠿⠶⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠀⠀⠀⠀⠀⠀⠈⠷⢶⣤⣤⣤" << endl;
        cout << "⠀⠀⢸⣿⠀⠀⠀⠀⣈⣀⣀⣸⣷⣦⣄⣧⣭⣭⣿⣷⣶⣶⣶⣶⣶⣿⠿⠿⠿⠿⠿⠟⠛⠛⠛⠛⠀⠀⠀⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀" << endl;
        cout << "⠿⠿⠿⠿⣿⣿⣿⣿⣿⣿⠿⠿⠿⠿⠿⠛⠋⠉⠉⠉⠉⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀" << endl;
        cout << "⠀⠀⠀⠀⠀⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀" << endl;
        cout << "⠀⢀⣤⣤⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀" << endl;
        cout << "⠀⣿⣿⣿⣿⡧⠀⠀⠀⠀⠀⠀⠀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀" << endl;
    }
    if (!currentRoom.items.empty()) {
        cout << "You see the following items in the room:" << endl;
        for (int i = 0; i < currentRoom.items.size(); i++) {
            cout << " - " << currentRoom.items[i].name;
            if (currentRoom.items[i].isKey) {
                cout << " (key)";
            }
            cout << endl;
        }
    }
}

void printInventory(vector<Item> inventory) {
    if (inventory.empty()) {
        cout << "You don't have any items." << endl;
    }
    else {
        cout << "You have the following items in your inventory:" << endl;
        for (int i = 0; i < inventory.size(); i++) {
            cout << " - " << inventory[i].name;
            if (inventory[i].isKey) {
                cout << " (key)";
            }
            cout << endl;
        }
    }
}

void addItem(vector<Item>& inventory, Item newItem) {
    inventory.push_back(newItem);
}

void removeItem(vector<Item>& inventory, int itemIndex) {
    inventory.erase(inventory.begin() + itemIndex);
}

bool hasItem(vector<Item> inventory, string itemName) {
    for (int i = 0; i < inventory.size(); i++) {
        if (inventory[i].name == itemName) {
            return true;
        }
    }
    return false;
}

void printSanity(int sanity) {
    cout << "Your sanity level is " << sanity << "/100." << endl;
}

void adjustSanity(int& sanity, int adjustment) {
    sanity += adjustment;
    if (sanity > MAX_SANITY) {
        sanity = MAX_SANITY;
    }
    else if (sanity < MIN_SANITY) {
        sanity = MIN_SANITY;
        cout << "You have gone insane and cannot continue!" << endl;
    }
}

Room generateRandomRoom() {
    // Define possible rooms
    string names[NUM_ROOMS] = {"kitchen", "bedroom", "bathroom", "attic", "basement"};
    string descriptions[NUM_ROOMS] = {"a spacious room with a large counter and lots of cabinets", 
                                      "a cozy room with a large bed and a nightstand", 
                                      "a small room with a toilet and a shower", 
                                      "a dusty room with old furniture and cobwebs", 
                                      "a dimly lit room with storage shelves and a workbench"};
    int ghostChances[NUM_ROOMS] = {10, 5, 2, 20, 30};
    bool isLockeds[NUM_ROOMS] = {false, false, true, true, true};

    // Generate random room index
    int roomIndex = rand() % NUM_ROOMS;

    // Create room object
    Room newRoom;
    newRoom.name = names[roomIndex];
    newRoom.description = descriptions[roomIndex];
    newRoom.ghostChance = ghostChances[roomIndex];
    newRoom.isLocked = isLockeds[roomIndex];

    // Return new room object
    return newRoom;
}

void generateItems(vector<Item>& items) {
    // Define possible items
    string names[NUM_ITEMS] = {"key", "flashlight", "book"};
    string descriptions[NUM_ITEMS] = {"a shiny metal key", 
                                      "a small flashlight with a working battery", 
                                      "a thick book with strange symbols on the cover"};
    bool isKeys[NUM_ITEMS] = {true, false, false};

    // Clear input vector
    items.clear();

    // Generate random item indices
    vector<int> itemIndices;
    for (int i = 0; i < NUM_ITEMS; i++) {
        itemIndices.push_back(i);
    }
    shuffle(itemIndices.begin(), itemIndices.end(), mt19937{random_device{}()});

    // Add random items to input vector
    for (int i = 0; i < NUM_ITEMS; i++) {
        Item newItem;
        newItem.name = names[itemIndices[i]];
        newItem.description = descriptions[itemIndices[i]];
        newItem.isKey = isKeys[itemIndices[i]];
        items.push_back(newItem);
    }
}

void generateGhosts(int& numGhosts) {
    // Generate random number of ghosts
    numGhosts = rand() % 5 + 1;
}

void saveGame(Room currentRoom, vector<Item> inventory, int sanity) {
    // Open file for writing
    ofstream saveFile("save.txt");

    // Write game state to file
    saveFile << currentRoom.name << endl;
    saveFile << currentRoom.description << endl;
    saveFile << currentRoom.ghostChance << endl;
    saveFile << currentRoom.isLocked << endl;
    saveFile << inventory.size() << endl;
    for (int i = 0; i < inventory.size(); i++) {
        saveFile << inventory[i].name << endl;
        saveFile << inventory[i].description << endl;
        saveFile << inventory[i].isKey << endl;
    }
    saveFile << sanity << endl;

    // Close file
    saveFile.close();
}

void loadGame(Room& currentRoom, vector<Item>& inventory, int& sanity) {
    // Open the file for reading
    ifstream file("saved_game.txt");
    if (!file.is_open()) {
        cout << "Error: Could not open file." << endl;
        return;
    }

    // Read the data from the file
    string roomName, roomDescription;
    int ghostChance, isLocked, numItems, numGhosts;
    file >> roomName >> ws;
    getline(file, roomDescription);
    file >> ghostChance >> isLocked >> numItems >> numGhosts;

    // Set the values of the current room
    currentRoom.name = roomName;
    currentRoom.description = roomDescription;
    currentRoom.ghostChance = ghostChance;
    currentRoom.isLocked = (bool)isLocked;

    // Clear the items vector and read in the new items
    inventory.clear();
    for (int i = 0; i < numItems; i++) {
        string itemName, itemDescription;
        bool isKey;
        file >> itemName >> ws;
        getline(file, itemDescription);
        file >> isKey;
        Item newItem = { itemName, itemDescription, isKey };
        inventory.push_back(newItem);
    }

    // Read in the sanity level
    file >> sanity;

    // Close the file
    file.close();
}

