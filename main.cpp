#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <thread>

using namespace std;

// Function prototypes
void help();
bool playGame(int roomNumber);
int generateSanityBoost();
string getSanityBoostItem();
void printSlow(string text, int delay);

class Exorcist {
public:
    Exorcist(int sanity) : sanity(sanity) {}

    int getSanity() { return sanity; }
    void increaseSanity(int value) { sanity += value; }
    void decreaseSanity(int value) { sanity -= value; }
    

private:
    int sanity;
};


int main() {
    srand(time(0));

    const int maxSanity(100);
    const int sanityLossPerFailure(20);
    const int numRooms(5);
    
    Exorcist player(maxSanity);
    int currentRoom = 1;
    string currentRoom2;
    string roomDescriptions[5] = {
    "You enter a dark and cold room. The air is heavy, and you can feel an evil presence nearby.",
    "As you proceed to the next room, the walls seem to be closing in. A sense of dread fills your heart.",
    "You find yourself in a room filled with broken mirrors. The distorted reflections send chills down your spine.",
    "The stench of decay fills this room. Shadows dance on the walls as if they have a life of their own.",
    "You finally reach the last room, where the possessed person lies. Their body is twisted in an unnatural way, and their eyes are pitch black."
    };

    printSlow("::::::::::: :::    ::: ::::::::::      :::::::::: :::    :::  ::::::::  :::::::::   :::::::: ::::::::::: :::::::: ::::::::::: \n", 1);
    printSlow("    :+:     :+:    :+: :+:             :+:        :+:    :+: :+:    :+: :+:    :+: :+:    :+:    :+:    :+:    :+:    :+:      \n", 1);
    printSlow("    +:+     +:+    +:+ +:+             +:+         +:+  +:+  +:+    +:+ +:+    +:+ +:+           +:+    +:+           +:+           \n", 1);
    printSlow("    +#+     +#++:++#++ +#++:++#        +#++:++#     +#++:+   +#+    +:+ +#++:++#:  +#+           +#+    +#++:++#++    +#+           \n", 1);
    printSlow("    +#+     +#+    +#+ +#+             +#+         +#+  +#+  +#+    +#+ +#+    +#+ +#+           +#+           +#+    +#+       \n", 1);
    printSlow("    #+#     #+#    #+# #+#             #+#        #+#    #+# #+#    #+# #+#    #+# #+#    #+#    #+#    #+#    #+#    #+#       \n", 1);
    printSlow("    ###     ###    ### ##########      ########## ###    ###  ########  ###    ###  ######## ########### ########     ###  \n", 1);
    printSlow("\n\n\n\n\n                                                   The Cursed House                             \n\n", 15);

    cout << "Type 'help' for instructions." << endl << endl;

    while (player.getSanity() > 0 && currentRoom <= numRooms) {
        string userInput;
        cout << roomDescriptions[currentRoom - 1] << endl;
        if (currentRoom == 1) {currentRoom2 = "first";}
        else if (currentRoom == 2) {currentRoom2 = "second";}
        else if (currentRoom == 3) {currentRoom2 = "third";}
        else if (currentRoom == 4) {currentRoom2 = "fourth";}
        else if (currentRoom == 5) {currentRoom2 = "last";}

        cout << "You are in the " << currentRoom2 << "room " << ". Type 'play' to start the game, 'help' for instructions, or 'quit' to exit." << endl;
        cin >> userInput;
        cout << endl;

        if (userInput == "play") {
            bool success = playGame(currentRoom);

            if (success) {
                cout << "Congratulations! You have successfully completed room " << currentRoom << "." << endl;
                if (currentRoom < numRooms) {
                    int sanityBoost = generateSanityBoost();
                    string item = getSanityBoostItem();
                    cout << "You found a " << item << "! Your sanity increased by " << sanityBoost << "." << endl;
                    player.increaseSanity(sanityBoost);
                    cout << "\nYour current sanity: " << player.getSanity() << ".\n" << endl;
                }
                currentRoom++;
            } else {
                player.decreaseSanity(sanityLossPerFailure);
                cout << "You failed! Your sanity decreased by " << sanityLossPerFailure << ". Your current sanity: " << player.getSanity() << "." << endl;
            }
        } else if (userInput == "help") {
            help();
        } else if (userInput == "quit") {
            break;
        } else {
            cout << "Invalid command." << endl;
        }
    }

    if (player.getSanity() <= 0) {
        cout << "You have lost all your sanity! Game over." << endl;
    } else if (currentRoom > numRooms) {
        cout << "Congratulations! You have saved the possessed person and won the game !" << endl;
    } else {
        cout << "Goodbye!" << endl;
    }
    return 0;
}


// Function definitions
void help() {
    cout << "\nIn this game, you are an exorcist attempting to rescue a possessed person from a cursed house with 5 rooms." << endl;
    cout << "You must complete a game in each room to proceed to the next room." << endl;
    cout << "If you fail a game, you lose 20 sanity. If your sanity reaches 0, you lose the game." << endl;
    cout << "Randomly, you may find a sacred item that will restore some sanity." << endl;
    cout << "If you complete all 5 rooms, you save the possessed person and win the game!\n" << endl;
}
bool playGame(int roomNumber) {
    // Implement the game for each room here.
    // This is just a simple example. You can replace it with more complex games.
    int randomNumber = rand() % 100 + 1;
    int userGuess;
    int maxAttempts = 5;
    cout << "Room " << roomNumber << " game: Guess a number between 1 and 100. You have " << maxAttempts << " attempts." << endl;

    for (int attempt = 1; attempt <= maxAttempts; attempt++) {
        cout << "Attempt " << attempt << ": ";
        cin >> userGuess;

        if (userGuess == randomNumber) {
            return true;
        } else if (userGuess < randomNumber) {
            cout << "The voice whispers, 'Higher...' " << endl;
        } else {
            cout << "The voice whispers, 'Lower...' " << endl;
        }
    }
    return false;
}
int generateSanityBoost() {
    int randomValue = rand() % 3;
    return 10;
}
string getSanityBoostItem() {
    vector<string> items = {"holy water", "necklace", "blessed candle"};
    return items[rand() % items.size()];
}

void printSlow(string text, int delay) {
    for (char c : text) {
        cout << c;
        this_thread::sleep_for(chrono::milliseconds(delay));
        cout.flush();
    }
}

