#ifndef PROBLEM_H
#define PROBLEM_H

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>

using namespace std;

struct Node {
    vector<int> state;
    int g;
    int h;
    int blank;

    int f() const{
        return g + h;
    }
};

class Problem {
    public:
        explicit Problem(const vector<int>& , const vector<int>& , const int );
        void run_UniformCost() const;
        void run_AStarMisplaced() const;
        void run_AStarEuclidean() const;

    public:
        enum Heuristic {ZERO, MISPLACED, EUCLIDEAN};
        int computeH(const vector<int>& , Heuristic) const;

    private:
        vector<int> initial;
        vector<int> goal;
        const int size;

        int h_misplaced_distance(const vector<int>& );
        int h_euclidean_distance(const vector<int>& );

        //helper function
        string find_key(const vector<int>& ) const;
        int find_blank(const vector<int>& ) const;

        //operators
        bool moveUp(const vector<int>& , int , vector<int>& , int& ) const;
        bool moveDown(const vector<int>& , int , vector<int>& , int& ) const;
        bool moveLeft(const vector<int>& , int , vector<int>& , int& ) const;
        bool moveRight(const vector<int>& , int , vector<int>& , int& ) const;
};

#endif