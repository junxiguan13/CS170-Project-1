#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
using namespace std;


int main(int argc, char *argv[]) {


    vector<vector<int>> startState(3, vector<int>(3));

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
                rowinputcounter++;
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

    //Problem Puzzle;

    return 0;
}

