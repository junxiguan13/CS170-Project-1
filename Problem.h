#ifndef PROBLEM_H
#define PROBLEM_H
#include <iostream>
#include <queue>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <sstream>

using namespace std;

struct Node {
    vector<vector<int> > puzzleState ;
    shared_ptr<Node> parentNode;
    int costSoFar;
    int heuristic;
    int totalCost;

    Node(vector<vector<int> > state, shared_ptr<Node> parent = nullptr, int cost = 0, int h = 0){
        puzzleState = state;
        parentNode = parent;
        costSoFar = cost; //g
        heuristic = h;
        totalCost = costSoFar + heuristic;
    }

    bool operator>(const Node& NodeComparedTo) const {
        return totalCost > NodeComparedTo.totalCost;
    }
};

struct Answer {
    vector<shared_ptr<Node>> expandedNodes;
    int totalExpanded;
    int maxFrontierSize;
    int goalDepth;
    shared_ptr<Node> goalNode;
};


class Problem{

    private:
        vector<vector<int> > initial; //initial state
        vector<vector<int> > goal;    //goal state

    public:
        Problem(const vector<vector<int> >& initial);
        double euclideanDistance(const vector<vector<int>>& state) const;
        vector<vector<int> > getInitialState() const;
        vector<vector<int> > getGoalState() const;
        Answer uniformCostSearch();
        Answer MisplacedTile();
        //Answer aStarEuclidean();

    
};

#endif 
