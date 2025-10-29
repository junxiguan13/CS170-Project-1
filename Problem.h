#ifndef PROBLEM_H
#define PROBLEM_H

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>

using namespace std;

struct Node {
    vector<int> state;//the board configuration in a sequential order of numbers
    int g;//cost from the initial to the current state
    int h;//cost from the current state to the goal state (heuristic value)
    int blank;//the index of the blank

    int f() const {//overall cost; use this to determine the priority of expanding
        return g + h;
    }
};

class Problem {
    public:
        enum Heuristic {ZERO, MISPLACED, EUCLIDEAN};

        void setHeuristic(Heuristic h);//user will input a type of algortihm and we should use this function to determine the type
        int computeH(const vector<int>& s) const;

    private:
        vector<int> initial;//initial state
        vector<int> goal;//final state
        int size;//size of the problem(e.g. 3*3 means size = 3)

        //helper function
        string find_key(const vector<int>& curr_state) const;//convert states from number form to string form; use for hash table to check repeated state
        int find_blank(const vector<int>& curr_state) const;//get the position of the blank
        int find_h(const vector<int>& new_state, const vector<int>& goal_state, const int size);//finding h depends on which algorithm we want to use

        //operators
        bool moveUp(const vector<int>& curr, int curr_blank, vector<int>& next, int& next_blank) const;
        //get state representation in num and index of blank from the Node. also pass a new state if this is a legal move
        //the next step should be checking for repeated states
        //then creating a node by using the next and next_blank
        bool moveDown(const vector<int>& curr_state, int curr_blank, vector<int>& next_state, int& next_blank) const;
        bool moveLeft(const vector<int>& curr_state, int curr_blank, vector<int>& next_state, int& next_blank) const;
        bool moveRight(const vector<int>& curr_state, int curr_blank, vector<int>& next_state, int& next_blank) const;

        int find_g(const Node& prev_state);//IMPORTANT: NOT NEEDED. This function is just a reminder and shuold be deleted later.
        //we can do this by simply add one when creating node

        Node create_node(const vector<int>& new_state, const int& new_blank, const int size);//IMPORTANT: NOT NEEDED. This function is just a reminder and shuold be deleted later.
        //create a new valid node; must be already checked for legal operations and repeated states
        //Node nxt;
        //nxt.state = new_state;
        //nxt.g     = curr_state.g + 1
        //nxt.h = computeH(ns);
        //nxt.blank = next_blank;
        
        //find_h will call the following functions for each specific method
        static int h_misplaced_distance(const vector<int>& curr_state, const vector<int>& goal_state, int size);
        static int h_euclidean_distance(const vector<int>& curr_state, const vector<int>& goal_state, int size);

    public:
        //default constructor
        Problem();

        void run_UniformCost() const;
        void run_AStarMisplaced() const; //setHeuristic(MISPLACED)
        void run_AStarEuclidean() const;
};

#endif