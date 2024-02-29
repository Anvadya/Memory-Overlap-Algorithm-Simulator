#ifndef SIM_H
#define SIM_H
//<----------------------->

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#include "dsu.h"

class Simulator{
public:
    Simulator();
    void getMatrix();
    void printMatrix();
    void run();
    void step();
    bool stoppingCondition();
    bool eligible(int i);
private:
    const int n_CN = 2;
    const int max_thread_group_size = 100;
    std::vector<std::vector<double>> memory_overlap_matrix;
    int number_of_threads;
    double distance(int i, int j);
    double overlap(int i, int j);
    double similarity(int i, int j);
    void printSets();
    DSU thread_groups{0};
};

//<----------------------->
#endif