#include "functions.h"


int main() {
    string gameSelection;
    cout << ".............................." << endl;
    cout << ".  The Chopsticks Hand Game  ." << endl;
    cout << ".............................." << endl;

    do {
        cout << "Which game would you like to run? (select 1 or 2)" << endl;
        cout << "1: Start a 1 player game, human vs. AI" << endl;
        cout << "2: Run 2 AI player game simulations" << endl;
        cin >> gameSelection;
    }

    while(!cin.fail() && gameSelection != "1" && gameSelection != "2");

    if (gameSelection == "1") {
        humanVSAIGame();
    } else if (gameSelection == "2") {
        AIGameSimulations();
    }

    return 0;
}
