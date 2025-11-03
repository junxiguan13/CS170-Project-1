#include <iostream>
#include "Problem.h"

using namespace std;

void runAlgor(const Problem& Prob) {
    bool repeat = true;

    while (repeat) {
        cout << "Enter your choice of algorithm: " << endl;
        cout << "1. Uniform Cost Search" << endl;
        cout << "2. A* with the Misplaced Tile heuristic" << endl;
        cout << "3. A* with the Euclidean distance heuristic" << endl;
        cout << "4. Try a different puzzle" << endl;

        char algor_choice;
        cin >> algor_choice;
        if (algor_choice == '1') {
            Prob.run_UniformCost();
        }
        else if (algor_choice == '2') {
            Prob.run_AStarMisplaced();
        }
        else if (algor_choice == '3') {
            Prob.run_AStarEuclidean();
        }
        else if (algor_choice == '4') {
            break;
        }
        else {
            cout << "Undefined algorithm choice. Please try again!" << endl;
        }
    }

    return;
}

int main() {

    cout << "Welcome to <862293376> 8 puzzle solver." << endl;
    bool repeat = true;

    while (repeat) {
        cout << "Type \"1\" to use a default puzzle, or \"2\" to enter your own puzzle." << endl;
        cout << "Type \"3\" to exit the solver." << endl;

        char puzz_choice;
        cin >> puzz_choice;

        if (puzz_choice == '1') {
            Problem Prob;
            runAlgor(Prob);
        }
        else if (puzz_choice == '2') {
            cout << "Enter your puzzle, use a zero to represent the blank" << endl;
            vector<int> initial;
            vector<int> goal = {1, 2, 3, 4, 5, 6, 7, 8, 0};
            const int size = 3;

            cout << "Enter the first row, use space or tabs between numbers: ";
            for (int i = 0; i < 3; ++i) {
                int puzz_input;
                cin >> puzz_input;
                initial.push_back(puzz_input);
            }

            cout << "Enter the second row, use space or tabs between numbers: " << endl;
            for (int i = 0; i < 3; ++i) {
                int puzz_input;
                cin >> puzz_input;
                initial.push_back(puzz_input);
            }

            cout << "Enter the third row, use space or tabs between numbers: " << endl;
            for (int i = 0; i < 3; ++i) {
                int puzz_input;
                cin >> puzz_input;
                initial.push_back(puzz_input);
            }

            cout << "Your puzzle is: " << endl;
            for (int i = 0; i < 9; ++i) {
                cout << initial.at(i) << " ";
                if (i % 3 == 2) {
                    cout << endl;
                }
            }

            Problem Prob(initial, goal, size);
            runAlgor(Prob);
        }
        else if (puzz_choice == '3') {
            break;
        }
        else {
            cout << "Undefined puzzle choice. Please try again!" << endl;
            continue;
        }
    }//end while loop

    cout << "Solver exited!" << endl;
    return 0;
}