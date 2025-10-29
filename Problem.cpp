#include "Problem.h"

Problem::Problem() {
    size = 3;
    //case1: initial state = goal state
    goal    = vector<int>();
    goal.push_back(1); goal.push_back(2); goal.push_back(3);
    goal.push_back(4); goal.push_back(5); goal.push_back(6);
    goal.push_back(7); goal.push_back(8); goal.push_back(0);

    //case2: discussion presentation example(103 426 758)
    initial = vector<int>();
    initial.push_back(1); initial.push_back(0); initial.push_back(3);
    initial.push_back(4); initial.push_back(2); initial.push_back(6);
    initial.push_back(7); initial.push_back(5); initial.push_back(8);
}

void Problem::setHeuristic(Heuristic h) {
    htype = h;
}

int Problem::computeH(const std::vector<int>& s) const {
    if (htype == ZERO) {
        return 0;
    }
    if (htype == MISPLACED) {
        return hMisplaced(s, goal, size);
    }
    return hEuclidean(s, goal, size);
}