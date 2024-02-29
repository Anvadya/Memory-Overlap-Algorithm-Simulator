#ifndef DSU_H
#define DSU_H

#include <vector>
#include <set>
#include <iostream>
#include <algorithm>

struct DSU{
    std::vector<int> parent,sz;
    std::set<int> representative_elements;
    int n_sets;
    DSU(int n): parent(n+1), sz(n+1,1), n_sets(n){
        for(int i = 1; i <= n; ++i) parent[i] = i, representative_elements.insert(i);
    }
    int find(int i){
        return ((i==parent[i])?(i):(i = find(parent[i])));
    }
    int merge(int i, int j){
        std::cout << "Merging " << i << " " << j << std::endl;
        if((i=find(i)) == (j=find(j))) return -1;
        --n_sets;
        if(sz[i]>sz[j]) std::swap(i,j);
        representative_elements.erase(i);
        parent[i] = j;
        sz[j] += sz[i];
        return j;
    }
    
};

#endif