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

std::array<int, 5> STARTING_HANDS = {1,1,1,1,0};

std::vector<std::vector<std::pair<int, int>>> SPLIT_COMBINATIONS =
        {{{1,1},{2,0}},{{1,2},{3,0}},{{1,3},{2,2},{4,0}},
         {{1,4},{3,2}},{{2,4},{3,3}}};

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
    updatedHands[targetHand] = hands[targetHand] + hands[attackHand];

    // Check for any dead hands
    for (int i=0; i<updatedHands.size(); i++) {
        if (updatedHands[i] > 4) {
            updatedHands[i] = 0;
        }
    }
    updatedHands[4] = 0; // Reset split counter to 0
    return updatedHands;
}

std::vector<std::pair<int,int>> getSplitMoves(int leftHand, int rightHand) {
    std::vector<std::pair<int,int>> validSplitCombos = {};

    for (auto splitCombo: SPLIT_COMBINATIONS) {
        for (auto hands: splitCombo) {
            if ((leftHand == hands.first && rightHand == hands.second) ||
            (rightHand==hands.first && leftHand == hands.second)) {
                for (auto i: splitCombo) {
                    if (!((leftHand == i.first && rightHand == i.second) ||
                        (rightHand==i.first && leftHand == i.second))) {
                        validSplitCombos.push_back(i);
                    }
                }
                return validSplitCombos;
            }
        }
    }
    return validSplitCombos;
}

std::array<int, 5> splitMove(std::array<int, 5> hands, std::pair<int,int> splitCombo, bool isPlayerOne) {
    int yourLeft, yourRight, opponentLeft, opponentRight;
    std::array<int, 5> updatedHands = hands;
    if (isPlayerOne) { // set reference indices for the hands array
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
    updatedHands[yourLeft] = splitCombo.first;
    updatedHands[yourRight] = splitCombo.second;
    updatedHands[5] = updatedHands[5] + 1;
    return updatedHands;
}

std::array<int, 5> makeHumanMove(std::array<int, 5> hands, bool isPlayerOne) {
    string moveRequest;
    int attackHand, targetHand;
    std::array<int, 5> updatedHands = hands;
    std::vector<std::pair<int,int>> splitCombos;
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
                        attackHand = yourLeft;
                        validAttackHand = true;
                    } else {
                        cout << "Cannot attack with that hand - try the other hand" << endl;
                    }
                } else if (moveRequest == "2") {
                    if (hands[yourRight] >= 1 && hands[yourRight] <= 4) { // Check attack hand is not dead
                        attackHand = yourRight;
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
                int nLeft = hands[yourLeft];
                int nRight = hands[yourRight];
                splitCombos = getSplitMoves(nLeft, nRight);
                if (splitCombos.size() == 1) { // checks if you are able to split your hand
                    updatedHands = splitMove(hands, splitCombos.front(), isPlayerOne);
                    isValidMove = true;
                } else if (splitCombos.size() == 2) {
                    do {
                        cout << "How would you like to split your hand?" << endl;
                        for (int i=0; i<splitCombos.size(); i++) {
                            cout << "Option " << i+1 << ". " << splitCombos[i].first << "-" << splitCombos[i].second << endl;
                        }
                        cin >> moveRequest;
                    }
                    while (!cin.fail() && moveRequest != "1" && moveRequest != "2");
                    int leftHand, rightHand;
                    if (moveRequest == "1") {
                        updatedHands = splitMove(hands, splitCombos.front(), isPlayerOne);
                    } else if (moveRequest == "2") {
                        updatedHands = splitMove(hands, splitCombos.back(), isPlayerOne);
                    }
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

int calculateUtility(std::array<int, 5> hands) {
    int utility = hands[0]+hands[1]-hands[2]-hands[3];
    return utility;
}

// Return all possible game states for the next player's turn, to be used for the AI minimax algorithm
std::vector<std::array<int, 5>> getPossibleMoves(std::array<int, 5> hands, bool isPlayerOne) {
    int yourLeft, yourRight, opponentLeft, opponentRight;
    std::vector<std::array<int, 5>> possibleMoves;
    std::vector<std::pair<int,int>> possibleSplitMoves;

    if (isPlayerOne) { // set reference indices for the hands array
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
    // Add all valid attack moves
    if (hands[yourLeft] != 0 && hands[opponentLeft] != 0) {
        possibleMoves.push_back(attackMove(hands, yourLeft, opponentLeft));
        if (hands[opponentRight] != 0) {
            possibleMoves.push_back(attackMove(hands, yourLeft, opponentRight));
        }
    }
    if (hands[yourRight] != 0 && hands[opponentLeft] != 0) {
        possibleMoves.push_back(attackMove(hands, yourRight, opponentLeft));
        if (hands[opponentRight != 0]) {
            possibleMoves.push_back(attackMove(hands, yourRight, opponentRight));
        }
    }

    // Add all valid split moves
    int nLeft = hands[yourLeft];
    int nRight = hands[yourRight];
    possibleSplitMoves = getSplitMoves(nLeft, nRight);
    std::array<int, 5> updatedMoves;
    for (auto possibleSplitMove: possibleSplitMoves) {
        updatedMoves = splitMove(hands,possibleSplitMove,isPlayerOne);
        possibleMoves.push_back(updatedMoves);
    }
    return possibleMoves;
}

// Returns an array of 6 integers: 1) player 1 left hand, 2) player 1 right hand, 3) player 2 left hand,
// 4) player 2 right hand, 5) split counter and 6) utility score.
std::array<int, 6> minimax(std::array<int, 5> hands, bool isPlayerOne, int depth, int alpha, int beta) {
    int utility = 0;
    int bestUtility = 0;
    std::array<int, 6> bestMove;
    std::array<int, 5> updatedHands, bestHands;
    std::vector<std::array<int, 5>> possibleMoves;

    // If terminal state or depth search limit is reached, return the utility of the current game state.
    if (depth==0 || gameWon(hands)) {
        bestUtility = calculateUtility(hands);
        for (int i=0; i<hands.size(); i++) {
            bestMove[i] = hands[i];
        }
        bestMove.back() = bestUtility;
        return bestMove;
    }

    possibleMoves = getPossibleMoves(hands, isPlayerOne);

    if (isPlayerOne) { // Player One is the maximising player
        bestUtility = -10;
        for (int i=0; i<possibleMoves.size(); i++) {
            updatedHands = possibleMoves[i];
            std::array<int, 6> minimaxResult = minimax(updatedHands, !isPlayerOne, depth-1, alpha, beta);
            utility = minimaxResult.back();
            if (bestUtility < utility) {
                for (int i=0; i<updatedHands.size(); i++) {
                    bestMove.at(i) = updatedHands.at(i);
                }
                bestMove.back() = minimaxResult.back();
                alpha = std::max(alpha, bestUtility);
                if (beta <= alpha) {
                    break;
                }
            }
            updatedHands = hands; // undo move
        }
    } else { // Player Two is the minimising player
        bestUtility = 10;
        for (int i=0; i<possibleMoves.size(); i++) {
            updatedHands = possibleMoves[i];
            std::array<int, 6> minimaxResult = minimax(updatedHands, !isPlayerOne, depth-1, alpha, beta);
            utility = minimaxResult.back();
            if (bestUtility > utility) {
                for (int i=0; i<updatedHands.size(); i++) {
                    bestMove.at(i) = updatedHands.at(i);
                }
                bestMove.back() = minimaxResult.back();
                bestUtility = minimaxResult.back();
                beta = std::min(beta, bestUtility);
                if (beta <= alpha) {
                    break;
                }
            }
            updatedHands = hands; // undo move
        }
    }
    if (bestMove.empty()) {
        for (int i=0; i<updatedHands.size(); i++) {
            bestMove[i] = updatedHands.at(i);
        }
        bestMove.back() = calculateUtility(updatedHands);
    }
    return bestMove;
}

int humanVSAIGame() {
    string moveRequest;
    bool isPlayerOneTurn;
    std::array<int, 5> currentHands;
    std::array<int, 5> updatedHands;
    std::array<int, 6> aiMove;

    currentHands = STARTING_HANDS;

    // User chooses who goes first
    isPlayerOneTurn = setHumanPlayerOne();
    bool isHumanTurn;

    if (isPlayerOneTurn) {
        isHumanTurn = true;
    } else {
        isHumanTurn = false;
    }

    int depth = setAIAlgorithm();

    printHands(currentHands);

    while (!gameWon(currentHands)) {
        if (isHumanTurn) {
            cout << "--- Human's turn " << (isPlayerOneTurn ? "(Player One)" : "(Player Two)") << "---" << endl;
        } else {
            cout << "--- AI's turn " << (isPlayerOneTurn ? "(Player One)" : "(Player Two)") << " ---" << endl;
        }
        if (isHumanTurn) {
             updatedHands = makeHumanMove(currentHands, isPlayerOneTurn);
             printHands(updatedHands);
             currentHands = updatedHands;
        }
        else if (!isHumanTurn) {
            aiMove = minimax(currentHands, isPlayerOneTurn, depth, INT16_MIN, INT16_MAX);
            for (int i=0; i<updatedHands.size(); i++) {
                updatedHands.at(i) = aiMove.at(i);
            }
            printHands(updatedHands);
            currentHands = updatedHands;
        }
        isHumanTurn = !isHumanTurn;
        isPlayerOneTurn = !isPlayerOneTurn;
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