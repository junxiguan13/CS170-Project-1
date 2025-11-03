#ifndef PROBLEM_H
#define PROBLEM_H

#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <unordered_map>
#include <string>
#include <cmath>

using namespace std;

struct Node {
    vector<int> state;
    int g;
    int h;
    int blank;

    int f() const{
        return g + h;
    }

    Node() : g(0), h(0), blank(-1) {}
    Node(const vector<int>& curr_state, int curr_g, int curr_h, int curr_blank) : state(curr_state), g(curr_g), h(curr_h), blank(curr_blank) {}
};

class Problem {
    public:
        enum Heuristic {ZERO, MISPLACED, EUCLIDEAN};
        int computeH(const vector<int>& , Heuristic) const;

    private:
        vector<int> initial;
        vector<int> goal;
        const int size;

        int h_misplaced_distance(const vector<int>& ) const;
        int h_euclidean_distance(const vector<int>& ) const;

        //helper function
        string find_key(const vector<int>& ) const;
        int find_blank(const vector<int>& ) const;

        //operators
        bool moveUp(const vector<int>& , int , vector<int>& , int& ) const;
        bool moveDown(const vector<int>& , int , vector<int>& , int& ) const;
        bool moveLeft(const vector<int>& , int , vector<int>& , int& ) const;
        bool moveRight(const vector<int>& , int , vector<int>& , int& ) const;
        void moveCore(const vector<int>& next_state, 
              const int next_blank, 
              const Node& smallest_node, 
              Heuristic h_type, 
              unordered_map<string, int>& best_g, 
              vector<Node>& all_nodes, 
              priority_queue<pair<int, int>>& smallestF_queue) const;
        
        //print function
        void printExpand(const Node& ) const;
        void printGoal(int , int , int ) const;

    public:
        Problem();
        explicit Problem(const vector<int>& , const vector<int>& , const int );
        void run_UniformCost() const;
        void run_AStarMisplaced() const;
        void run_AStarEuclidean() const;
};

#endif