#include "Problem.h"
#include <iostream>
#include <queue>
#include <set>
#include <map>
#include <cmath>
#include <unordered_set>
#include <functional>


Problem::Problem(const vector<vector<int> >& initialparam){
    initial = initialparam;
    goal = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 0}
    };
}

pair<int,int> findBlank(const vector<vector<int> >& state){
    for(int i = 0; i < 3; ++i){
        for(int j = 0; j < 3; ++j){
            if(state[i][j] == 0){
                return {i, j};
            }
        }
    }
    return {-123, -123};
}

vector<vector<vector<int>>> generateSuccessors(const vector<vector<int>>& state){
    vector<vector<vector<int>>> successors;
    pair<int,int> blank = findBlank(state);
    int row = blank.first;
    int col = blank.second;

    vector<pair<int,int>> moves = {{-1,0},{1,0},{0,-1},{0,1}};

    for (auto move : moves) {
        int newRow = row + move.first;
        int newCol = col + move.second;

        if (newRow >= 0 && newRow < 3 && newCol >= 0 && newCol < 3){
            vector<vector<int>> newState = state;
            swap(newState[row][col], newState[newRow][newCol]);
            successors.push_back(newState);
        }
    }

    return successors;
}

Answer Problem::uniformCostSearch(){
    auto compare = [](shared_ptr<Node> a, shared_ptr<Node> b){
        return a->totalCost > b->totalCost;
    };
    priority_queue<shared_ptr<Node>, vector<shared_ptr<Node>>, decltype(compare)> frontier(compare);

    shared_ptr<Node> startNode = make_shared<Node>(this->initial, nullptr, 0, 0);
    frontier.push(startNode);

    set<vector<vector<int>>> explored;
    Answer result;
    result.totalExpanded = 0;
    result.maxFrontierSize = 1;
    result.goalDepth = 0;

    while (!frontier.empty()){
        if (frontier.size() > result.maxFrontierSize)
            result.maxFrontierSize = frontier.size();

        shared_ptr<Node> currentNode = frontier.top();
        frontier.pop();

        if(currentNode->puzzleState == this->goal){
            result.goalNode = currentNode;
            result.goalDepth = currentNode->costSoFar;
            return result;
        }

        result.expandedNodes.push_back(currentNode);
        result.totalExpanded++;


        explored.insert(currentNode->puzzleState);

        vector<vector<vector<int>>> successors = generateSuccessors(currentNode->puzzleState);
        for (auto& succState : successors){
            if (explored.find(succState) == explored.end()){
                shared_ptr<Node> succNode = make_shared<Node>(succState, currentNode, currentNode->costSoFar + 1, 0);
                frontier.push(succNode);
            }
        }
    }

    //Solution not found
    result.goalNode = nullptr;
    return result;
}



int misplacedTileHeuristic(const vector<vector<int>>& state, const vector<vector<int>>& goal) {
    int misplaced = 0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (state[i][j] != 0 && state[i][j] != goal[i][j]) {
                misplaced++;
            }
        }
    }
    return misplaced;
}

Answer Problem::MisplacedTile() {
    auto compare = [](shared_ptr<Node> a, shared_ptr<Node> b) {
        return a->totalCost > b->totalCost;
    };
    priority_queue<shared_ptr<Node>, vector<shared_ptr<Node>>, decltype(compare)> frontier(compare);

    int h = misplacedTileHeuristic(this->initial, this->goal);
    shared_ptr<Node> startNode = make_shared<Node>(this->initial, nullptr, 0, h);
    frontier.push(startNode);

    set<vector<vector<int>>> explored;
    Answer result;
    result.totalExpanded = 0;
    result.maxFrontierSize = 1;
    result.goalDepth = 0;

    while (!frontier.empty()) {
        if (frontier.size() > result.maxFrontierSize)
            result.maxFrontierSize = frontier.size();

        shared_ptr<Node> currentNode = frontier.top();
        frontier.pop();

        if (currentNode->puzzleState == this->goal) {
            result.goalNode = currentNode;
            result.goalDepth = currentNode->costSoFar;
            return result;
        }

        result.expandedNodes.push_back(currentNode);
        result.totalExpanded++;

        


        explored.insert(currentNode->puzzleState);

        vector<vector<vector<int>>> successors = generateSuccessors(currentNode->puzzleState);
        for (auto& succState : successors) {
            if (explored.find(succState) == explored.end()) {
                int succH = misplacedTileHeuristic(succState, this->goal);
                shared_ptr<Node> succNode = make_shared<Node>(succState, currentNode, currentNode->costSoFar + 1, succH);
                frontier.push(succNode);
            }
        }
    }

    result.goalNode = nullptr;
    return result;
}

