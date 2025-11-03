#include "Problem.h"

Problem::Problem() : initial{1, 0, 3, 4, 2, 6, 7, 5, 8}, goal{1, 2, 3, 4, 5, 6, 7, 8, 0}, size(3) {}//default example (103 426 758)

Problem::Problem(const vector<int>& user_init, const vector<int>& user_goal, const int user_size) : initial(user_init), goal(user_goal), size(user_size) {}

void Problem::run_UniformCost() const {
    int init_g = 0;
    int init_h = 0;
    int init_blank = find_blank(this->initial);
    Node init_node = Node(this->initial, init_g, init_h, init_blank, '?', nullptr);
    //build the initial node 

    priority_queue<pair<int, int>> smallestF_queue;
    //pair<int, int>: the first int refers to the f value; the second int refers to the order that enters the queue(index simply)
    //this is to determine which state the algorithm chooses due to smallest f
    smallestF_queue.push(make_pair(- init_node.f(), - 0));
    //first: adding minus so we can get the largest value(smallest f) in the queue
    //second: 0 means the first index

    unordered_map<string, int> best_g;//this best_g is to check repeated
    best_g.reserve(30000);//to avoid reallocation or data overwritten since we will have plenty of states while running the algorithm
    string init_key = find_key(this->initial);
    best_g[init_key] = 0;//the initial state should have g equals to 0

    vector<Node> all_nodes;//this pool is to store all the nodes
    all_nodes.reserve(30000);
    all_nodes.push_back(init_node);

    //counter for print functions
    int total_node = 0;
    int max_queue = smallestF_queue.size();

    Tree tree;//this stores the final solution of reverse pathway

    while (!smallestF_queue.empty()) {
        pair<int, int> smallest_pair = smallestF_queue.top();
        smallestF_queue.pop();
        int index = - smallest_pair.second;//get the specific index of the state with smallest f

        const Node& smallest_node = all_nodes.at(index);
        const vector<int>& smallest_state = smallest_node.state;
        int smallest_blank = smallest_node.blank;
        //get the state representation and the index of blank with smallest f

        //we only want to expand on the state which has the smallest f; to avoid repeated expanding, use the following
        //NOTE: consider including <it_smallest == best_g.end()> in the if statement if something unexpected happened
        string smallest_key = find_key(smallest_state);
        unordered_map<string,int>::iterator it_smallest = best_g.find(smallest_key);
        if (it_smallest == best_g.end() || smallest_node.g != it_smallest->second) {
            continue;
        }

        //check if this is already the goal state; if so just return
        if (smallest_state == this->goal) {
            printGoal(total_node, max_queue, smallest_node.g);

            cout << "Type \'Y\' to show solution path. Type others to skip." << endl;//ask if the user want to print the path in case the path is too long
            char sol_choice;
            cin >> sol_choice;
            if (sol_choice == 'Y') {
                tree.final_solution = &all_nodes[index];
                printSolution(tree.final_solution);
            }

            return;
        }

        printExpand(smallest_node);//print each step
        total_node++;

        vector<int> up_state;
        int up_blank;
        if (moveUp(smallest_state, smallest_blank, up_state, up_blank)) {//if this is a feasible move, check repeated
            moveCore(up_state, up_blank, smallest_node, index, 'U', ZERO, best_g, all_nodes, smallestF_queue);
        }

        vector<int> down_state;
        int down_blank;
        if (moveDown(smallest_state, smallest_blank, down_state, down_blank)) {
            moveCore(down_state, down_blank, smallest_node, index, 'D', ZERO, best_g, all_nodes, smallestF_queue);
        }

        vector<int> left_state;
        int left_blank;
        if (moveLeft(smallest_state, smallest_blank, left_state, left_blank)) {
            moveCore(left_state, left_blank, smallest_node, index, 'L', ZERO, best_g, all_nodes, smallestF_queue);
        }

        vector<int> right_state;
        int right_blank;
        if (moveRight(smallest_state, smallest_blank, right_state, right_blank)) {
            moveCore(right_state, right_blank, smallest_node, index, 'R', ZERO, best_g, all_nodes, smallestF_queue);
        }

        //update the max in queue
        if (smallestF_queue.size() > max_queue) {
            max_queue = smallestF_queue.size();
        }
    }//end while loop

    cout << "Goal not found!" << endl;
}

void Problem::run_AStarMisplaced() const {
    int init_g = 0;
    int init_h = computeH(this->initial, MISPLACED);
    int init_blank = find_blank(this->initial);
    Node init_node = Node(this->initial, init_g, init_h, init_blank, '?', nullptr);
    //build the initial node 

    priority_queue<pair<int, int>> smallestF_queue;
    //pair<int, int>: the first int refers to the f value; the second int refers to the order that enters the queue(index simply)
    //this is to determine which state the algorithm chooses due to smallest f
    smallestF_queue.push(make_pair(- init_node.f(), - 0));
    //first: adding minus so we can get the largest value(smallest f) in the queue
    //second: 0 means the first index

    unordered_map<string, int> best_g;//this best_g is to check repeated
    best_g.reserve(30000);//to avoid reallocation or data overwritten since we will have plenty of states while running the algorithm
    string init_key = find_key(this->initial);
    best_g[init_key] = 0;//the initial state should have g equals to 0

    vector<Node> all_nodes;//this pool is to store all the nodes
    all_nodes.reserve(30000);
    all_nodes.push_back(init_node);

    //counter for print functions
    int total_node = 0;
    int max_queue = smallestF_queue.size();

    Tree tree;
    
    while (!smallestF_queue.empty()) {
        pair<int, int> smallest_pair = smallestF_queue.top();
        smallestF_queue.pop();
        int index = - smallest_pair.second;//get the specific index of the state with smallest f

        const Node& smallest_node = all_nodes.at(index);
        const vector<int>& smallest_state = smallest_node.state;
        int smallest_blank = smallest_node.blank;
        //get the state representation and the index of blank with smallest f

        //we only want to expand on the state which has the smallest f; to avoid repeated expanding, use the following
        //NOTE: consider including <it_smallest == best_g.end()> in the if statement if something unexpected happened
        string smallest_key = find_key(smallest_state);
        unordered_map<string,int>::iterator it_smallest = best_g.find(smallest_key);
        if (it_smallest == best_g.end() || smallest_node.g != it_smallest->second) {
            continue;
        }

        //check if this is already the goal state; if so just return
        if (smallest_state == this->goal) {
            printGoal(total_node, max_queue, smallest_node.g);

            cout << "Type \'Y\' to show solution path. Type others to skip." << endl;//ask if the user want to print the path in case the path is too long
            char sol_choice;
            cin >> sol_choice;
            if (sol_choice == 'Y') {
                tree.final_solution = &all_nodes[index];
                printSolution(tree.final_solution);
            }

            return;
        }

        printExpand(smallest_node);//print each step
        total_node++;

        vector<int> up_state;
        int up_blank;
        if (moveUp(smallest_state, smallest_blank, up_state, up_blank)) {//if this is a feasible move, check repeated
            moveCore(up_state, up_blank, smallest_node, index, 'U', MISPLACED, best_g, all_nodes, smallestF_queue);
        }

        vector<int> down_state;
        int down_blank;
        if (moveDown(smallest_state, smallest_blank, down_state, down_blank)) {
            moveCore(down_state, down_blank, smallest_node, index, 'D', MISPLACED, best_g, all_nodes, smallestF_queue);
        }

        vector<int> left_state;
        int left_blank;
        if (moveLeft(smallest_state, smallest_blank, left_state, left_blank)) {
            moveCore(left_state, left_blank, smallest_node, index, 'L', MISPLACED, best_g, all_nodes, smallestF_queue);
        }

        vector<int> right_state;
        int right_blank;
        if (moveRight(smallest_state, smallest_blank, right_state, right_blank)) {
            moveCore(right_state, right_blank, smallest_node, index, 'R', MISPLACED, best_g, all_nodes, smallestF_queue);
        }

        //update the max in queue
        if (smallestF_queue.size() > max_queue) {
            max_queue = smallestF_queue.size();
        }
    }//end while loop

    cout << "Goal not found!" << endl;
}

void Problem::run_AStarEuclidean() const {
    int init_g = 0;
    int init_h = computeH(this->initial, EUCLIDEAN);
    int init_blank = find_blank(this->initial);
    Node init_node = Node(this->initial, init_g, init_h, init_blank, '?', nullptr);
    //build the initial node 

    priority_queue<pair<int, int>> smallestF_queue;
    //pair<int, int>: the first int refers to the f value; the second int refers to the order that enters the queue(index simply)
    //this is to determine which state the algorithm chooses due to smallest f
    smallestF_queue.push(make_pair(- init_node.f(), - 0));
    //first: adding minus so we can get the largest value(smallest f) in the queue
    //second: 0 means the first index

    unordered_map<string, int> best_g;//this best_g is to check repeated
    best_g.reserve(30000);//to avoid reallocation or data overwritten since we will have plenty of states while running the algorithm
    string init_key = find_key(this->initial);
    best_g[init_key] = 0;//the initial state should have g equals to 0

    vector<Node> all_nodes;//this pool is to store all the nodes
    all_nodes.reserve(30000);
    all_nodes.push_back(init_node);

    //counter for print functions
    int total_node = 0;
    int max_queue = smallestF_queue.size();

    Tree tree;

    while (!smallestF_queue.empty()) {
        pair<int, int> smallest_pair = smallestF_queue.top();
        smallestF_queue.pop();
        int index = - smallest_pair.second;//get the specific index of the state with smallest f

        const Node& smallest_node = all_nodes.at(index);
        const vector<int>& smallest_state = smallest_node.state;
        int smallest_blank = smallest_node.blank;
        //get the state representation and the index of blank with smallest f

        //we only want to expand on the state which has the smallest f; to avoid repeated expanding, use the following
        //NOTE: consider including <it_smallest == best_g.end()> in the if statement if something unexpected happened
        string smallest_key = find_key(smallest_state);
        unordered_map<string,int>::iterator it_smallest = best_g.find(smallest_key);
        if (it_smallest == best_g.end() || smallest_node.g != it_smallest->second) {
            continue;
        }

        //check if this is already the goal state; if so just return
        if (smallest_state == this->goal) {
            printGoal(total_node, max_queue, smallest_node.g);

            cout << "Type \'Y\' to show solution path. Type others to skip." << endl;//ask if the user want to print the path in case the path is too long
            char sol_choice;
            cin >> sol_choice;
            if (sol_choice == 'Y') {
                tree.final_solution = &all_nodes[index];
                printSolution(tree.final_solution);
            }

            return;
        }

        printExpand(smallest_node);//print each step
        total_node++;

        vector<int> up_state;
        int up_blank;
        if (moveUp(smallest_state, smallest_blank, up_state, up_blank)) {//if this is a feasible move, check repeated
            moveCore(up_state, up_blank, smallest_node, index, 'U', EUCLIDEAN, best_g, all_nodes, smallestF_queue);
        }

        vector<int> down_state;
        int down_blank;
        if (moveDown(smallest_state, smallest_blank, down_state, down_blank)) {
            moveCore(down_state, down_blank, smallest_node, index, 'D', EUCLIDEAN, best_g, all_nodes, smallestF_queue);
        }

        vector<int> left_state;
        int left_blank;
        if (moveLeft(smallest_state, smallest_blank, left_state, left_blank)) {
            moveCore(left_state, left_blank, smallest_node, index, 'L', EUCLIDEAN, best_g, all_nodes, smallestF_queue);
        }

        vector<int> right_state;
        int right_blank;
        if (moveRight(smallest_state, smallest_blank, right_state, right_blank)) {
            moveCore(right_state, right_blank, smallest_node, index, 'R', EUCLIDEAN, best_g, all_nodes, smallestF_queue);
        }

        //update the max in queue
        if (smallestF_queue.size() > max_queue) {
            max_queue = smallestF_queue.size();
        }
    }//end while loop

    cout << "Goal not found!" << endl;
}

int Problem::computeH(const vector<int>& curr_state, Heuristic h_type) const {
    if (h_type == ZERO) {
        return 0;
    }
    if (h_type == MISPLACED) {
        return h_misplaced_distance(curr_state);
    }
    return h_euclidean_distance(curr_state);
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
int Problem::h_euclidean_distance(const vector<int>& curr_state) const{
    const int num_elem = this->size * this->size;
    unordered_map <int, int> goalIndexOf;
    goalIndexOf.reserve(num_elem);
    //we want to make this work for general cases; considering goal state is not in consecutive form and numbers are randomly inputted

    for (int i = 0; i < num_elem; ++i) {//storing each goal index in the hash table goalIndexOf
        int val = this->goal.at(i);
        goalIndexOf[val] = i;
    }

    //next to run a loop and compute the total euclidean distance
    double eu_total = 0;
    for (int i = 0; i < num_elem; ++i) {
        int val = curr_state.at(i);
        int goalElem_index = goalIndexOf[val];
        int currElem_index = i;

        int x_dist = 0;
        int y_dist = 0;

        if (val != 0 && goalElem_index != currElem_index) {//we do not count the element zero
            x_dist = abs(currElem_index % this->size - goalElem_index % this->size);
            y_dist = abs(currElem_index / this->size - goalElem_index / this->size);

        }

        double eu_dist = sqrt(x_dist * x_dist + y_dist * y_dist);
        eu_total += eu_dist;
    }

    return ceil(eu_total);
}

//convert states from numbers to a string; use for hash table to check repeated state
string Problem::find_key(const vector<int>& curr_state) const {
    const int num_elem = this->size * this->size;
    string key = "";
    key.reserve(num_elem * 2);
    //curr_state gets the string form of state numeric representation, each number is separated by a comma.
    //so (size * size * 2 - 1) times of comma in total, we reserve a (size * 2) space

    for (int i = 0; i < num_elem; ++i) {
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
    const int num_elem = this->size * this->size;
    for (int i = 0; i < num_elem; ++i) {
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

void Problem::moveCore(const vector<int>& next_state, 
              const int next_blank, 
              const Node& smallest_node, 
              int prev_id,
              char move, 
              Heuristic h_type, 
              unordered_map<string, int>& best_g, 
              vector<Node>& all_nodes, 
              priority_queue<pair<int, int>>& smallestF_queue) const{
    string next_key = find_key(next_state);

    int next_g = smallest_node.g + 1;
    unordered_map<string,int>::iterator itBestg = best_g.find(next_key);
    if (itBestg == best_g.end() || next_g < itBestg->second) {
        //if not found in the hash table, then it is a new state and build a new node
        //or if it has a smaller g, then we take that
        int next_h = computeH(next_state, h_type);
        Node next_node = Node(next_state, next_g, next_h, next_blank, move, nullptr);

        best_g[next_key] = next_g;//update the next state in best_g since this is not a repeated state
        Node* prev_node = &all_nodes.at(prev_id);
        next_node.prev = prev_node;//update parent
        all_nodes.push_back(next_node);//update the node in the vector since this move is feasible
        int next_order = all_nodes.size() - 1;
        smallestF_queue.push(make_pair(- next_node.f(), - next_order));//update it in the queue to compare f with other states
    }

    return;
}

void Problem::printExpand(const Node& curr) const {
    const int num_elem = this->size * this->size;

    cout << "The best state to expand with g(n) = " << curr.g << " and h(n) = " << curr.h << " is..." << endl;
    for (int i = 0; i < num_elem; ++i) {
        if (curr.state.at(i) == 0) {//change 0 to b(blank)
            cout << 'b' << " ";
        }
        else {
            cout << curr.state.at(i) << " ";
        }
        
        if (i % this->size == this->size - 1) {
            cout << endl;
        }
    }

    cout << "Expanding this node..." << endl;
}

void Problem::printGoal(int total_node, int max_queue, int goal_depth) const {
    cout << "Goal!!!" << endl;
    cout << "To solve this problem the search algorithm expanded a total of " << total_node << " nodes." << endl;
    cout << "The maximum number of nodes in the queue at any one time: " << max_queue << "." << endl;
    cout << "The depth of the goal node was " << goal_depth << "." << endl;
}

void Problem::printSolution(const Node* goal_ptr) const {
    if (goal_ptr == nullptr) {
        return;
    }

    cout << "The solution path is: " << endl;
    Node* curr_ptr = goal_ptr;

    while (curr_ptr != nullptr) {
        vector<int> sol_state = curr_ptr->state;

        for (int i = 0; i < this->size; ++i) {//print the solution state
            if (sol_state.at(i) == 0) {//change 0 to b(blank)
                cout << 'b' << " ";
            }
            else {
                cout << sol_state.at(i) << " ";
            }

            if (i % this->size == this->size - 1) {
                cout << endl;
            }
        }

        if (curr_ptr->move != '?') {//if this is not the root, then print the following statement
            cout << "To achieve this state, we need: " << curr_ptr->move << endl << endl;
        }
    }
}