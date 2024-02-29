#include "simulator.h"
#include <limits.h>
#include <map>

Simulator::Simulator(){
    getMatrix();
    thread_groups = DSU{number_of_threads};
    run();
}

void Simulator::getMatrix()
{
    std::ifstream  data("test.csv");
    std::string line;
    std::vector<std::vector<double> > parsedCsv;
    while(std::getline(data,line))
    {
        std::stringstream lineStream(line);
        std::string cell;
        std::vector<double> parsedRow;
        while(std::getline(lineStream,cell,','))
        {
            parsedRow.push_back(std::stod(cell));
        }

        parsedCsv.push_back(parsedRow);
    }
    memory_overlap_matrix = parsedCsv;
    number_of_threads = parsedCsv.size();
};

void Simulator::printMatrix(){
    for(auto row: memory_overlap_matrix){
        for(auto cell: row){
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
}

void Simulator::run(){
    while(!stoppingCondition()) step();
    printSets();
}

void Simulator::step(){
    static int stp = 0;
    std::cout << "STEP:\t" << ++stp << std::endl;
    int best_i {-1}, best_j {-1};
    int min_val = INT_MAX;
    for(auto i: thread_groups.representative_elements){
        for(auto j: thread_groups.representative_elements){
            if(i>=j || (!eligible(i)) || (!eligible(j))) continue;
            int dij = distance(i,j);
            if(dij<min_val) min_val = dij, best_i = i, best_j = j;
        }
    }
    thread_groups.merge(best_i,best_j);
}

bool Simulator::stoppingCondition(){
    return (thread_groups.n_sets <= n_CN);
}

bool Simulator::eligible(int i){
    return thread_groups.sz[i] <= max_thread_group_size;
}

double Simulator::distance(int i, int j){
    if(thread_groups.representative_elements.find(i)== thread_groups.representative_elements.end() || thread_groups.representative_elements.find(j)== thread_groups.representative_elements.end()) {
        std::cout << "Error, invalid indexes to find distance, indexes given were " << i << " " << j << std::endl;   
        system("pause");
        return -1;
    }
    printf("D[%d][%d] = %f \t Fs = %f \t Fo = %f \n", i, j, similarity(i,j) - overlap(i,j), similarity(i,j), overlap(i,j));
    return similarity(i,j) - overlap(i,j);
}

double Simulator::overlap(int i, int j){
    double overlap_value = 0;
    for(int _i = 1; _i <= number_of_threads; ++_i){
        for(int _j = 1; _j <= number_of_threads; ++_j){
            if((thread_groups.find(_i)==i) && (thread_groups.find(_j)==j)) overlap_value += memory_overlap_matrix[_i-1][_j-1];
        }
    }
    overlap_value /= thread_groups.sz[i];
    overlap_value /= thread_groups.sz[j];
    return overlap_value;
}

double Simulator::similarity(int i, int j){
    if(thread_groups.n_sets<3) return 1;
    double similarity_value = 0;
    for(auto t_group: thread_groups.representative_elements){
        if(t_group == i || t_group == j) continue;
        similarity_value += abs(overlap(t_group,i) - overlap(t_group,j));
    }
    similarity_value /= (thread_groups.n_sets-2);
    return similarity_value;
}

void Simulator::printSets(){
    std::map<int, std::vector<int>> thread_placement;
    for(int i = 1; i <= number_of_threads; ++i){
        thread_placement[thread_groups.find(i)].push_back(i);
    }
    std::cout << "Current Thread Placement:\n";
    for(int i = 1; i <= number_of_threads; ++i){
        if(thread_placement[i].size()){
            for(auto c: thread_placement[i]){
                std::cout << c << " ,";
            }
            std::cout << std::endl;
        }
    }
}

int main(){
    Simulator sim1{};
    system("pause"); //Windows only
    return 0;
}