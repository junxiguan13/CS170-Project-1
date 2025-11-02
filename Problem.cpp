#include "Problem.h"

explicit Problem::Problem(const vector<int>& user_init, const vector<int>& user_goal, const int user_size) : initial(user_init), goal(user_goal), size(user_size) {}

void run_UniformCost() const;
void run_AStarMisplaced() const;
void run_AStarEuclidean() const;

//After the operation steps:
//get state representation in num and index of blank from the Node. also pass a new state if this is a legal move
//the next step should be checking for repeated states
//then creating a node by using the next and next_blank
int find_g(const Node& prev_state);//IMPORTANT: NOT NEEDED. This function is just a reminder and shuold be deleted later.
//we can do this by simply add one when creating node
Node create_node(const vector<int>& new_state, const int& new_blank, const int size);//IMPORTANT: NOT NEEDED. This function is just a reminder and shuold be deleted later.
//create a new valid node; must be already checked for legal operations and repeated states
//Node nxt;
//nxt.state = new_state;
//nxt.g     = curr_state.g + 1
//nxt.h = computeH(ns);
//nxt.blank = next_blank;

int Problem::computeH(const vector<int>& curr_state, Heuristic h_type) const {
    if (htype == ZERO) {
        return 0;
    }
    if (htype == MISPLACED) {
        return hMisplaced(curr_state);
    }
    return hEuclidean(curr_state);
}

//find the h for misplaced algor; called by the funtion <computeH>
int Problem::h_misplaced_distance(const vector<int>& curr_state) const {
    int misplaced_counter = 0;
    const int num_elem = this->size * this->size;

    for (int i = 0; i < num_elem; ++i) {
        int check_elem = curr_state.at(i);
        int goal_elem = this->goal.at(i);

        if (check_elem != 0 && check_elem != goal_elem) {
            ++misplaced_counter;
        }
    }

    return misplaced_counter;
}

//find the h for enclidean algor; called by the function <computeH>
int Problem::h_euclidean_distance(const vector<int>& curr_state) {
    return 0;
}

//convert states from numbers to a string; use for hash table to check repeated state
string Problem::find_key(const vector<int>& curr_state) const {
    string key = "";
    key.reserve(this->size * 2);
    //curr_state gets the string form of state numeric representation, each number is separated by a comma.
    //so (size * 2 - 1) times of comma in total, we reserve a (size * 2) space

    for (int i = 0; i < this->size; ++i) {
        if (i != 0) {
            key += ',';
        }

        string element_holder = to_string(curr_state.at(i));
        key += element_holder;
    }

    return key;
}

//get the index of the blank; use to determine feasible movements(up, down, left, right)
int Problem::find_blank(const vector<int>& curr_state) const {

    for (int i = 0; i < this->size; ++i) {
        if (curr_state.at(i) == 0) {
            return i;
        }
    }

    return -1;
}

bool Problem::moveUp(const vector<int>& curr_state, int curr_blank, vector<int>& next_state, int& next_blank) const {
    //if the blank is in the first row, then not eligible to move up and return false
    if (curr_blank < this->size) {
        return false;
    }

    next_blank = curr_blank - this->size;
    next_state = curr_state;//copy from curr, swap blank and the element
    swap(next_state.at(next_blank), next_state.at(curr_blank));
    return true;
}

bool Problem::moveDown(const vector<int>& curr_state, int curr_blank, vector<int>& next_state, int& next_blank) const {
    //if the blank is in the last row, then not eligible to move down and return false
    int lastRow_index = this->size * (this->size - 1);
    if (curr_blank >= lastRow_index) {
        return false;
    }

    next_blank = curr_blank + this->size;
    next_state = curr_state;//copy from curr, swap blank and the element
    swap(next_state.at(next_blank), next_state.at(curr_blank));
    return true;
}

bool Problem::moveLeft(const vector<int>& curr_state, int curr_blank, vector<int>& next_state, int& next_blank) const {
    //if the blank is in the first col, then not eligible to move left and return false
    int check_firstCol = curr_blank % this->size;
    if (check_firstCol == 0) {
        return false;
    }

    next_blank = curr_blank - 1;
    next_state = curr_state;//copy from curr, swap blank and the element
    swap(next_state.at(next_blank), next_state.at(curr_blank));
    return true;
}

bool Problem::moveRight(const vector<int>& curr_state, int curr_blank, vector<int>& next_state, int& next_blank) const {
    //if the blank is in the last col, then not eligible to move right and return false
    int check_lastCol = curr_blank % this->size;
    if (check_lastCol == this->size - 1) {
        return false;
    }

    next_blank = curr_blank + 1;
    next_state = curr_state;//copy from curr, swap blank and the element
    swap(next_state.at(next_blank), next_state.at(curr_blank));
    return true;
}