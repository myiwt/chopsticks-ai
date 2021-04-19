#ifndef CHOPSTICKS_AI_FUNCTIONS_H
#define CHOPSTICKS_AI_FUNCTIONS_H

#endif //CHOPSTICKS_AI_FUNCTIONS_H

#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <array>
#include<stdio.h>
#include<stdlib.h>

using namespace std;
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::stoi;
using std::sscanf;

//std::pair<std::pair<int, int>,std::pair<int, int>>  OLD_STARTING_HANDS = {{1, 1},{1,1}};

std::array<int, 5> STARTING_HANDS = {1,1,1,1,0};

std::vector<std::vector<string>> SPLIT_COMBINATIONS = {{"11","20"},{"12","30"},{"13","22","40"},
                                                       {"14","32"}, {"23","14"},{"24","33"},{"33","42"}};

void printHands(std::array<int, 5> hands) {
    cout << endl << "             Player 1    ||   Player 2" << endl;
    cout << " Left hand       "<< hands[0] << "       ||      " << hands[2] << endl;
    cout << "Right hand       "<< hands[1]<< "       ||      " << hands[3] << endl << endl;
}

// User chooses who goes first.
bool setHumanPlayerOne() {
    string goFirst;
    do {
        cout << "Would you like to go first? (y/n)" << endl;
        cin >> goFirst;
    }

    while( !cin.fail() && goFirst != "y" && goFirst!= "n" && goFirst != "Y" && goFirst!= "N");

    if (goFirst == "y" || goFirst == "Y") {
        return true;
    } else {
        return false;
    }
}

// User chooses the depth level search for the AI player that they will play against
int setAIAlgorithm() {
    string depthStr;
    do {
        cout << "What depth search level should the AI algorithm play?" << endl;
        cout << "A depth of 9 represents a full depth search. (Choose a number from 1 to 9)" << endl;
        cin >> depthStr;
    }

    while( !cin.fail() && depthStr != "1" && depthStr!= "2" && depthStr != "3" && depthStr!= "4"
           && depthStr != "5" && depthStr!= "6" && depthStr != "7" && depthStr!= "8" && depthStr!= "9");
    int depthInt = std::stoi(depthStr);
    return depthInt;
}

bool gameWon(std::array<int, 5> hands) {
    if (((hands[0] < 1 || hands[0] > 4) && (hands[1] < 1 || hands[1] > 4)) ||
            ((hands[2] < 1 || hands[2] > 4) && (hands[3] < 1 || hands[3] > 4))) {
        return true;
    } else {
        return false;
    }
}

std::array<int, 5> attackMove(std::array<int, 5> hands, int attackHand, int targetHand) {
    std::array<int, 5> updatedHands = hands;
    updatedHands[targetHand] = updatedHands[targetHand] + attackHand;

    // Check for any dead hands
    for (int i=0; i<updatedHands.size(); i++) {
        if (updatedHands[i] > 4) {
            updatedHands[i] = 0;
        }
    }
    updatedHands[4] = 0; // Reset split counter to 0
    return updatedHands;
}

std::vector<string> getSplitCombinations(int leftHand, int rightHand) {
    string yourHandsStr = to_string(leftHand) + to_string(rightHand);
    string yourHandsStrOpp = to_string(rightHand) + to_string(leftHand);
    std::vector<string> validSplitCombos = {};

    for (auto splitCombo: SPLIT_COMBINATIONS) {
        for (auto hands: splitCombo) {
            if (yourHandsStr == hands) {
                for (auto i: splitCombo) {
                    if (i != yourHandsStr && i != yourHandsStrOpp) {
                        validSplitCombos.push_back(i);
                    }
                }
                return validSplitCombos;
            } else if (yourHandsStrOpp == hands) {
                for (auto i: splitCombo) {
                    if (i != yourHandsStr && i != yourHandsStrOpp) {
                        validSplitCombos.push_back(i);
                    }
                }
                return validSplitCombos;
            }
        }
    }
    return validSplitCombos;
}

std::array<int, 5> makeHumanMove(std::array<int, 5> hands, bool isPlayerOne) {
    string moveRequest;
    int attackHand, targetHand;
    std::array<int, 5> updatedHands = hands;
    std::vector<string> splitCombos;
    // If you do a split, and then the opponent splits, on your next turn you cannot split again. This avoids the game getting stuck in a loop
    bool isValidMove = false;
    bool validAttackHand = false;
    bool validTargetHand = false;
    int yourLeft, yourRight, opponentLeft, opponentRight;
    if (isPlayerOne) {
        yourLeft = 0;
        yourRight = 1;
        opponentLeft = 2;
        opponentRight = 3;
    } else {
        yourLeft = 2;
        yourRight = 3;
        opponentLeft = 0;
        opponentRight = 1;
    }
    while (!isValidMove) {
        do {
            cout << "Enter 1 to attack, or 2 to split" << endl;
            cin >> moveRequest;
        }
        while (!cin.fail() && moveRequest != "1" && moveRequest != "2");

        if (moveRequest == "1") {
            while (!validAttackHand) {
                do {
                    cout << "Would you like to attack with your left or right hand? (Enter 1 for left, 2 for right)"
                         << endl;
                    cin >> moveRequest;
                } while (!cin.fail() && moveRequest != "1" && moveRequest != "2");

                if (moveRequest == "1") {
                    if (hands[yourLeft] >= 1 && hands[yourLeft] <= 4) { // Check attack hand is not dead
                        attackHand = hands[yourLeft];
                        validAttackHand = true;
                    } else {
                        cout << "Cannot attack with that hand - try the other hand" << endl;
                    }
                } else if (moveRequest == "2") {
                    if (hands[yourRight] >= 1 && hands[yourRight] <= 4) { // Check attack hand is not dead
                        attackHand = hands[yourRight];
                        validAttackHand = true;
                    } else {
                        cout << "Cannot attack with that hand - try the other hand" << endl;
                    }
                }
            }
            while (!validTargetHand) {
                do {
                    cout << "Which of your opponent's hands would you like to attack? (Enter 1 for left, 2 for right)"
                         << endl;
                    cin >> moveRequest;
                } while (!cin.fail() && moveRequest != "1" && moveRequest != "2");
                // 1 = Player 1 LH, 2 = Player 1 RH, 3 = Player 2 LH, 4 = Player 2 RH
                if (moveRequest == "1") {
                    if (hands[opponentLeft] >= 1 && hands[opponentLeft] <= 4) { // Check target hand is not dead
                        targetHand = opponentLeft;
                        validTargetHand = true;
                    } else {
                        cout << "You cannot attack a dead hand - try the other hand" << endl;
                    }
                } else if (moveRequest == "2") {
                    if (hands[opponentRight] >= 1 && hands[opponentRight] <= 4) { // Check target hand is not dead
                        targetHand = opponentRight;
                        validTargetHand = true;
                    } else {
                        cout << "You cannot attack a dead hand - try the other hand" << endl;
                    }
                }
            }
            hands[4] = 0;
            updatedHands = attackMove(hands, attackHand, targetHand);
            isValidMove = true;
        } else if (moveRequest == "2") {
            if (hands[4] <= 3) {
                splitCombos = getSplitCombinations(hands[yourLeft], hands[yourRight]);
                if (splitCombos.size() == 1) { // checks if you are able to split your hand
                    int leftHand = splitCombos.front().at(0) - 48; // convert ascii char to integer
                    int rightHand = splitCombos.front().at(1) - 48;
                    updatedHands[yourLeft] = leftHand;
                    updatedHands[yourRight] = rightHand;
                    updatedHands[4] = updatedHands[4]+1;
                    isValidMove = true;
                } else if (splitCombos.size() == 2) {
                    do {
                        cout << "How would you like to split your hand?" << endl;
                        for (int i=0; i<splitCombos.size(); i++) {
                            cout << "Option " << i+1 << ". " << splitCombos[i].at(0) << "-" << splitCombos[i].at(1) << endl;
                        }
                        cin >> moveRequest;
                    }
                    while (!cin.fail() && moveRequest != "1" && moveRequest != "2");
                    int leftHand, rightHand;
                    if (moveRequest == "1") {
                        leftHand = splitCombos.front().at(0) - 48;
                        rightHand = splitCombos.front().at(1) - 48;
                    } else if (moveRequest == "2") {
                        leftHand = splitCombos.at(1).at(0) - 48;
                        rightHand = splitCombos.at(1).at(1) - 48;
                    }
                    updatedHands[yourLeft] = leftHand;
                    updatedHands[yourRight] = rightHand;
                    updatedHands[4] = updatedHands[4]+1;
                    isValidMove = true;
                } else if (splitCombos.size() == 0) {
                    cout << "Your hand cannot be split - try attacking instead" << endl;
                }
            } else {
                cout << "Too many splits already made - try attacking instead" << endl;
            }
        }
    }
    return updatedHands;
}

int humanVSAIGame() {
    string moveRequest;
    bool isHumanPlayerOne, moveMade;
    std::array<int, 5> currentHands;
    std::array<int, 5> updatedHands;

    currentHands = STARTING_HANDS;

    // User chooses who goes first
    isHumanPlayerOne = setHumanPlayerOne();
    bool isHumanTurn;
    char currentMarker;

    if (isHumanPlayerOne) {
        isHumanTurn = true;
    } else {
        isHumanTurn = false;
    }

    int depth = setAIAlgorithm();

    printHands(currentHands);

    while (!gameWon(currentHands)) {
        moveMade = false;
        if (isHumanTurn) {
            cout << "--- Human's turn " << (isHumanPlayerOne ? "(Player One)" : "(Player Two)") << "---" << endl;
        } else {
            cout << "--- AI's turn " << (isHumanPlayerOne ? "(Player Two)" : "(Player One)") << " ---" << endl;
        }
         if (isHumanTurn) {
             updatedHands = makeHumanMove(currentHands, isHumanPlayerOne);
             printHands(updatedHands);
             currentHands = updatedHands;
         }
         else if (!isHumanTurn) {
             //std::pair<string, int> aiMinimax = minimax(board, depth, currentMarker, INT16_MIN, INT16_MAX);
             updatedHands = makeHumanMove(currentHands, !isHumanPlayerOne);
             printHands(updatedHands);
             currentHands = updatedHands;
         }
        isHumanTurn = !isHumanTurn;
     }

     // Game has ended - print end message
     cout << endl;
    if (gameWon(currentHands)) {
         string winMessage = isHumanTurn ? "AI wins!": "Human wins!";
         cout << winMessage << endl;
    }
        return 0;
}

int AIGameSimulations() {
    return 0;
}