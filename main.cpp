#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include "Problem.h"
using namespace std;

void printPuzzleState(const vector<vector<int>>& state) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (state[i][j] == 0)
                cout << "b ";
            else
                cout << state[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

int main(int argc, char *argv[]) {


    vector<vector<int> > startState(3, vector<int>(3));

    string input;
    

    cout << "Welcome to 8 puzzle solver.\n " << endl;
    int rowinputcounter = 1;
    bool repeat = true;
    while(repeat){
        cout << "Type “1” to use a default puzzle, or “2” to enter your own puzzle.\n" << endl;

        cin >> input;
        if (input == "1") {
            startState = {
                {1, 3, 0},
                {4, 2, 6},
                {7, 5, 8}
            };
            break;
        }
        else if(input == "2"){
            cout << "Enter your puzzle, use a zero to represent the blank\n" << endl;
            for (int i = 0; i < 3; ++i) {
                if(rowinputcounter == 1){
                    cout << "Enter the first row, use space or tabs between number: \n";
                }else if(rowinputcounter == 2){
                    cout << "Enter the second row, use space or tabs between number: \n";
                }else if(rowinputcounter == 3){
                    cout << "Enter the third row, use space or tabs between number: \n";
                }
                for (int j = 0; j < 3; ++j) {
                    cin >> startState[rowinputcounter - 1][j];
                }
                rowinputcounter++;//adsfa
            }
            repeat = false;
        }
        rowinputcounter = 1;
    }
    
    cout << "\nEnter your choice of algorithm:\n" << endl;
    cout << "1. Uniform Cost Search\n"  << endl;
    cout << "2. A* with the Misplaced Tile heuristic\n" << endl;
    cout << "3. A* with the Euclidean distance heuristic\n" << endl;

    string algorithm;
    cin >> algorithm;

    Problem puzzle(startState);

    Answer result;
    if (algorithm == "1") {
        result = puzzle.uniformCostSearch();
    }
    else if(algorithm == "2"){
        //result = puzzle.
    }
    else if(algorithm == "3"){
        //result = puzzle.
    }


    for (auto& node : result.expandedNodes) {
        cout << "Expanding state\n";
        printPuzzleState(node->puzzleState);

        if (node->parentNode) {
            cout << "The best state to expand with g(n) = " 
                 << node->costSoFar 
                 << " and h(n) = " 
                 << node->heuristic 
                 << " is...\n";
            printPuzzleState(node->puzzleState);
            cout << "Expanding this node...\n\n";
        }
    }

    if (result.goalNode) {
        cout << "Goal!!!\n";
        cout << "To solve this problem the search algorithm expanded a total of "
             << result.totalExpanded << " nodes.\n";
        cout << "The maximum number of nodes in the queue at any one time: "
             << result.maxFrontierSize << ".\n";
        cout << "The depth of the goal node was " << result.goalDepth << ".\n";
    } else {
        cout << "No solution found.\n";
    }

    return 0;
}

